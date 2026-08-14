// file yarefpersys/obj_yrps.c
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

const char obj_yrps_id[] = YRPS_ID;

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
    };
  pthread_mutex_lock (&mtxob_yrps);
  pob = YRPS_MALLOC (sizeof (struct yrps_object_st));
  pob->vkind = Kyrps_object;
  pob->o_id = oid;
  unsigned bucknum = (unsigned) (oid % sizbuckobarr_yrps);
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
      goto end;
    }
  else if (5 * pbucket->b_buckcount > 4 * pbucket->vlen)
    {
      unsigned newbucksiz = yrps_prime_above ((9 * pbucket->b_buckcount) / 8);
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
#warning very incomplete  yrps_make_object
  fprintf (stderr, "incomplete yrps_make_object oid=%ld\n", (long) oid);
  YRPS_FAIL ();
end:
  pthread_mutex_unlock (&mtxob_yrps);
  return pob;
}

#warning obj_yrps.c needs a lot of code
