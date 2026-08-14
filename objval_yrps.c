// file yarefpersys/objval_yrps.c
// SPDX-License-Identifier: GPL-3.0-or-later

/***
 * License:
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****/
#include "yrps.h"

const char objval_yrps_id[] = YRPS_ID;

struct yrps_objbucket_st
{
  YRPS_PREFIX_FIELDS;
  unsigned b_buckcount;
  struct yrps_object_st *b_objvec[];
};


static struct yrps_objbucket_st **buckobarr_yrps;
static int64_t sizbuckobarr_yrps;
static int64_t cntob_yrps;
static pthread_mutex_t mtxob_yrps = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static struct yrps_value_st **valvec_yrps;
static int sizvalvec_yrps, lenvalvec_yrps;
static pthread_mutex_t mtxval_yrps = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

struct yrps_object_st *
yrps_find_object (int64_t oid)
{
  struct yrps_object_st *pob = NULL;
  struct yrps_objbucket_st *pbucket = NULL;
  pthread_mutex_lock (&mtxob_yrps);
  if (!oid)
    goto end;
  if (!buckobarr_yrps)
    {
      int64_t newsiz = 31;	/* it is prime */
      buckobarr_yrps = YRPS_CALLOC (newsiz, sizeof (struct yrps_calloc *));
      sizbuckobarr_yrps = newsiz;
      assert (cntob_yrps == 0);
      goto end;
    };
  unsigned bucknum = (unsigned) (oid % sizbuckobarr_yrps);
  pbucket = buckobarr_yrps[bucknum];
  if (!pbucket)
    goto end;
end:
  pthread_mutex_unlock (&mtxob_yrps);
  return pob;
}				/* end yrps_find_object */

struct yrps_object_st *
yrps_make_object (int64_t oid)
{
  struct yrps_object_st *pob = NULL;
  struct yrps_objbucket_st *pbucket = NULL;
  if (oid)
    {
      pob = yrps_find_object (oid);
      if (pob)
	return pob;
      else
	oid = 0;
    };
  while (!oid)
    {
      pthread_mutex_lock (&mtxob_yrps);
      int64_t newoid = (random () << 24) ^ (random ());
      if (!yrps_find_object (newoid))
	oid = newoid;
      pthread_mutex_unlock (&mtxob_yrps);
    };
  pob = YRPS_MALLOC (sizeof (struct yrps_object_st));
  pob->vkind = Kyrps_object;
  pob->o_id = oid;
  pthread_mutex_init (&pob->o_mtx, NULL);
  pob->o_nbpair = 0;
  pob->o_nbval = 0;
  pob->o_flags = 0;
  pob->o_state = 0;
  pob->o_funad = NULL;
  pob->o_pairv = (struct yrps_pairvect_st *) NULL;
  pob->o_valseq = (struct yrps_value_st *) NULL;
  pob->vindex = yrps_register_object (pob);
  return pob;
}				/* end yrps_make_object */


int
yrps_register_object (struct yrps_object_st *pob)
{
  int oix = 0;
  struct yrps_objbucket_st *pbucket = NULL;
  assert (pob);
  assert (pob->vkind == Kyrps_object);
  assert (pob->o_id > 0);
  YRPS_UNIQUE_BREAKPOINT ();
  pthread_mutex_lock (&mtxob_yrps);
  if (!buckobarr_yrps)
    {
      int64_t newsiz = 31;	/* it is prime */
      buckobarr_yrps = YRPS_CALLOC (newsiz, sizeof (struct yrps_calloc *));
      sizbuckobarr_yrps = newsiz;
      assert (cntob_yrps == 0);
    };
  assert (sizbuckobarr_yrps > 0);
  assert (buckobarr_yrps != NULL);
  unsigned bucknum = (unsigned) (pob->o_id % sizbuckobarr_yrps);
  pbucket = buckobarr_yrps[bucknum];
  if (!pbucket)
    {
      unsigned bucksiz = 13;	/* a prime number */
      pbucket = YRPS_MALLOC (sizeof (struct yrps_objbucket_st)
			     + bucksiz * sizeof (void *));
      pbucket->vkind = Kyrps_objbucket;
      pbucket->vlen = bucksiz;
      pbucket->b_buckcount = 1;
      pbucket->b_objvec[0] = pob;
      for (unsigned bix = 1; bix < bucksiz; bix++)
	pbucket->b_objvec[bix] = NULL;
      buckobarr_yrps[bucknum] = pbucket;
    }
  else if (5 * pbucket->b_buckcount > 4 * pbucket->vlen)
    {
      unsigned newbucksiz = yrps_prime_above ((9 * pbucket->b_buckcount) / 8);
      if (!newbucksiz)
	YRPS_PRINTFAIL ("too many buckets for %d object buckets", newbucksiz);
      struct yrps_objbucket_st *pnewbucket =
	YRPS_MALLOC (sizeof (struct yrps_objbucket_st)
		     + newbucksiz * sizeof (void *));
      pnewbucket->vlen = newbucksiz;
      unsigned oldbucksize = pbucket->vlen;
      for (unsigned oldix = 0; oldix < oldbucksize; oldix++)
	if (pbucket->b_objvec[oldix])
	  {
	    pnewbucket->b_objvec[pnewbucket->b_buckcount++] =
	      pbucket->b_objvec[oldix];
	  };
    }
  pob->vindex = bucknum;
  goto end;
end:
  pthread_mutex_unlock (&mtxob_yrps);
  return oix;
}				/* end yrps_register_object */

int
yrps_register_value (struct yrps_value_st *v)
{
  int vix = 0;
  assert (v);
  assert (v->vkind > Kyrps__None && v->vkind < Kyrps__Internal);
  if (v->vkind == Kyrps_object)
    return yrps_register_object ((struct yrps_object_st *) v);
  YRPS_UNIQUE_BREAKPOINT ();
  pthread_mutex_lock (&mtxval_yrps);
  if (lenvalvec_yrps >= sizvalvec_yrps)
    {
      int newsiz = ((4 * lenvalvec_yrps / 3 + 10) & 0x1f) + 1;
      assert (newsiz > sizvalvec_yrps);
      assert (newsiz < (2 << 30));
      struct yrps_value_st **oldvec = valvec_yrps;
      valvec_yrps = YRPS_CALLOC (newsiz, sizeof (struct yrps_value_st *));
      if (oldvec)
	{
	  memcpy (valvec_yrps, oldvec,
		  lenvalvec_yrps * sizeof (struct yrps_value_st *));
	  free (oldvec);
	};
    };
  vix = ++lenvalvec_yrps;
  valvec_yrps[vix] = v;
  v->vindex = vix;
  assert (vix > 0 && vix < (2 << 30));
  goto end;
end:
  pthread_mutex_unlock (&mtxval_yrps);
  return vix;
}				/* end yrps_register_value */

#warning objval_yrps.c needs a lot of code
