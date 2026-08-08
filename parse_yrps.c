// file yarefpersys/parse_yrps.c
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

const char parse_yrps_id[] = YRPS_ID;

bool
yrps_check_valid_textual_file (const char *path)
{
  char linebuf[256];
  int linecnt = 0;
  const char *reason = "?";
  memset (linebuf, 0, sizeof (linebuf));
  if (!path || !path[0])
    return false;
  FILE *f = fopen (path, "r");
  int errline = 0;
  if (!f)
    goto failure;
  do
    {
      linecnt++;
      if (linecnt > YRPS_LINECOUNTMAX)
	{
	  reason = "too many lines";
	  errline = __LINE__ - 2;
	  goto failure;
	};
      if (!fgets (linebuf, (int) sizeof (linebuf), f))
	{
	  reason = "fgets failed";
	  errline = __LINE__ - 2;
	  goto failure;
	};
      if (u8_check ((uint8_t *) linebuf, sizeof (linebuf)))
	{
	  reason = "bad utf8 unicode";
	  errline = __LINE__ - 2;
	  goto failure;
	};
    }
  while (!feof (f));
failure:
  if (errline > 0)
    fprintf (stderr, "%s: %s failed on %s line %s:%d (%s)\n",
	     yrps_argv[0], __FUNCTION__, path, __FILE__, errline, reason);
  fclose (f);
  return false;
}				/* end yrps_check_valid_textual_file */
