// file yarefpersys/scalar_yrps.c
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

const char scalar_yrps_id[] = YRPS_ID;


struct yrps_string_st *
yrps_make_string (const char *buf)
{
  struct yrps_string_st *pstr = NULL;
  if (!buf)
    buf = "";
  size_t blen = strlen (buf);
  if (u8_check ((uint8_t *) buf, blen))
    YRPS_PRINTFAIL ("invalid non-UTF8 string origin %s", buf);
  if (blen > 65536)
    YRPS_PRINTFAIL ("too long %ld byte string %.128s", (long) blen, buf);
  pstr = YRPS_MALLOC (sizeof (struct yrps_string_st) + blen + 2);
  pstr->vkind = Kyrps_string;
  pstr->vmark = 0;
  pstr->vlen = (int32_t) blen;
  strcpy ((char *) pstr->v_str, buf);
  yrps_register_value ((struct yrps_value_st *) pstr);
  return pstr;
}				/* end yrps_make_string */

struct yrps_string_st *
yrps_format_string (const char *fmt, ...)
{
  struct yrps_string_st *pstr = NULL;
  char *buf = NULL;
  char locbuf[256];
  va_list arglist;
  assert (fmt != NULL);
  va_start (arglist, fmt);
  if (strlen (fmt) < sizeof (locbuf) / 2)
    {
      memset (locbuf, 0, sizeof (locbuf));
      if (vsnprintf (locbuf, sizeof (locbuf) - 2, fmt, arglist) <
	  (int) sizeof (locbuf))
	{
	  pstr = yrps_make_string (locbuf);
	  goto end;
	}
    }
  else
    {
      int n = vasprintf (&buf, fmt, arglist);
      if (n < 0 || !buf)
	YRPS_PRINTFAIL ("vasprintf failed for format %s", fmt);
      pstr = yrps_make_string (buf);
    };
end:
  va_end (arglist);
  if (buf)
    free (buf);
  return pstr;
}				/* end yrps_format_string */

struct yrps_intvec_st *
yrps_make_intvec (unsigned nbint, int64_t *intarr)
{
  struct yrps_intvec_st *piv = NULL;
  if (!intarr)
    nbint = 0;
  if (sizeof (struct yrps_intvec_st) + nbint * sizeof (int64_t) >
      (size_t) YRPS_MAXDATASIZE)
    YRPS_PRINTFAIL ("too big intvect of %d int64_t", nbint);
  piv =
    YRPS_MALLOC (sizeof (struct yrps_intvec_st) +
		 (nbint + 1) * sizeof (int64_t));
  piv->vkind = Kyrps_intvect;
  piv->vmark = 0;
  piv->vlen = (int32_t) nbint;
  for (int ix = 0; ix < (int) nbint; ix++)
    ((int64_t *) piv->v_intvec)[ix] = intarr[ix];
  yrps_register_value ((struct yrps_value_st *) piv);
  return piv;
}				/* end yrps_make_intvec */

struct yrps_dblvec_st *
yrps_make_dblvec (unsigned nbdbl, int64_t *intarr)
{
  struct yrps_dblvec_st *pdv = NULL;
  if (!intarr)
    nbdbl = 0;
  if (sizeof (struct yrps_dblvec_st) + nbdbl * sizeof (int64_t) >
      (size_t) YRPS_MAXDATASIZE)
    YRPS_PRINTFAIL ("too big dblvect of %d int64_t", nbdbl);
  pdv =
    YRPS_MALLOC (sizeof (struct yrps_dblvec_st) +
		 (nbdbl + 1) * sizeof (int64_t));
  pdv->vkind = Kyrps_doublevect;
  pdv->vmark = 0;
  pdv->vlen = (int32_t) nbdbl;
  for (int ix = 0; ix < (int) nbdbl; ix++)
    ((double *) pdv->v_dblvec)[ix] = intarr[ix];
  yrps_register_value ((struct yrps_value_st *) pdv);
  return pdv;
}				/* end yrps_make_dblvec */

///// eof scalar_yrps.c [€fin du fichier]
