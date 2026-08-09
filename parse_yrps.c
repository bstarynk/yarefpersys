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

void
yrps_load_state_from_directory (const char *dirpath)
{
  assert (yrps_readable_directory (dirpath));
  size_t dirpathlen = strlen (dirpath);
  struct dirent *dent = NULL;
  DIR *dir = opendir (dirpath);
  do
    {
      dent = readdir (dir);
      if (!dent)
	break;
      if (dent->d_type != DT_REG)
	continue;
      if (dent->d_name[0] == '.')
	continue;
      int namlen = strlen (dent->d_name);
      // A loadable module like abc.so or _xy*.so is dlopened and its
      // initialization function run (e.g. abc_inityrps) if it exists.
      /*€ Un module chargeable tel que abc.so ou _xy.so est chargé
         dynamiquement par dlopen et sa fonction d'initialisation
         (e.g. abc_inityrps) executée */
      if (namlen > 4 && namlen + dirpathlen + 1 < YRPS_PATHMAX
	  && namlen < YRPS_SYMLENMAX
	  && (isalnum (dent->d_name[0]) || dent->d_name[0] == '_')
	  && dent->d_name[namlen - 3] == '.'
	  && dent->d_name[namlen - 2] == 's'
	  && dent->d_name[namlen - 1] == 'o')
	{
	  char bufpath[YRPS_PATHMAX];
	  memset (bufpath, 0, sizeof (bufpath));
	  snprintf (bufpath, sizeof (bufpath), "%s/%s",
		    dirpath, dent->d_name);
	  void *dlh = dlopen (bufpath, RTLD_NOW);
	  if (!dlh)
	    {
	      fprintf (stderr, "%s: %s dlopen %s failed line %s:%d (%s)\n",
		       yrps_argv[0], __FUNCTION__, bufpath,
		       __FILE__, __LINE__ - 2, dlerror ());
	      continue;
	    }
	  char symbuf[YRPS_SYMLENMAX];
	  memset (symbuf, 0, sizeof (symbuf));
	  snprintf (symbuf, sizeof (symbuf), "%.20s_inityrps", dent->d_name);
	  void *ad = dlsym (dlh, symbuf);
	  if (ad)
	    {
	      yrps_initfun_t *inif = (yrps_initfun_t *) ad;
	      (*inif) ();
	    };
	}
    }
  while (dent);
}				/* end yrps_load_state_from_directory */
