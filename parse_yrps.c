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

static void load_initial_module_yrps (const char *dirpath,
				      const char *entnam);
static void parse_generated_c_file_yrps (const char *dirpath,
					 const char *entnam);

static void parse_textual_data_file_yrps (const char *dirpath,
					  const char *entnam);

static struct yrps_value_st **valvec_yrps;
static int sizvalvec_yrps, lenvalvec_yrps;

static int add_value_yrps (struct yrps_value_st *);

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
    {
      reason = "fopen failed";
      errline = __LINE__ - 2;
      goto failure;
    }
  do
    {
      linecnt++;
      if (linecnt > YRPS_LINECOUNTMAX)
	{
	  reason = "too many lines";
	  errline = __LINE__ - 2;
	  goto failure;
	};
      memset (linebuf, 0, sizeof (linebuf));
      if (!fgets (linebuf, (int) sizeof (linebuf), f))
	{
	  if (feof (f))
	    break;
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
  assert (linecnt > 0);
  return true;
failure:
  if (errline > 0)
    fprintf (stderr, "%s: %s failed on %s line %s:%d (%s @ %s:%d)\n",
	     yrps_argv[0], __FUNCTION__, path, __FILE__, errline, reason,
	     path, linecnt);
  fclose (f);
  fflush (NULL);
  return false;
}				/* end yrps_check_valid_textual_file */

void
yrps_load_state_from_directory (const char *dirpath)
{
  assert (yrps_readable_directory (dirpath));
  char *lastselfslash = strrchr (yrps_argv[0], '/');
  const char *selfname = lastselfslash ? (lastselfslash + 1) : yrps_argv[0];
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
      if (!strcmp (dent->d_name, selfname))
	continue;
      int namlen = (int) strlen (dent->d_name);
      if (namlen > 3 && dent->d_name[namlen - 1] == 'o'
	  && dent->d_name[namlen - 2] == '.')
	continue;
      // A loadable module like abc.so or _xy*.so is dlopened and its
      // initialization function (e.g. abc_inityrps) is run if it
      // exists.
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
	  load_initial_module_yrps (dirpath, dent->d_name);
	}
      else if (namlen > 4 && dent->d_name[0] == '_'
	       && dent->d_name[namlen - 2] == '.'
	       && dent->d_name[namlen - 1] == 'c')
	{
	  parse_generated_c_file_yrps (dirpath, dent->d_name);
	}
      else if (namlen > 8
	       && (dent->d_name[0] == '_' || isalpha (dent->d_name[0]))
	       && !strcmp (dent->d_name + namlen - 5, ".yrps"))
	{
	  parse_textual_data_file_yrps (dirpath, dent->d_name);
	}
    }
  while (dent);
}				/* end yrps_load_state_from_directory */

void
load_initial_module_yrps (const char *dirpath, const char *entnam)
{
  assert (dirpath);
  assert (entnam);
  char bufpath[YRPS_PATHMAX];
  memset (bufpath, 0, sizeof (bufpath));
  snprintf (bufpath, sizeof (bufpath), "%s/%s", dirpath, entnam);
  void *dlh = dlopen (bufpath, RTLD_NOW);
  if (!dlh)
    {
      fprintf (stderr, "%s: %s dlopen %s failed line %s:%d (%s)\n",
	       yrps_argv[0], __FUNCTION__, bufpath,
	       __FILE__, __LINE__ - 2, dlerror ());
      return;
    }
  char symbuf[YRPS_SYMLENMAX];
  memset (symbuf, 0, sizeof (symbuf));
  const char *endrps = strstr (entnam, "_yrps");
  if (endrps)
    {
      strncpy (symbuf, entnam, endrps - entnam);
      assert (strlen (symbuf) + sizeof ("_inityrps") < sizeof (symbuf));
      strcat (symbuf, "_inityrps");
      assert (strlen (symbuf) < sizeof (symbuf));
    }
  else
    snprintf (symbuf, sizeof (symbuf), "%.20s_inityrps", entnam);
  void *ad = dlsym (dlh, symbuf);
  if (ad)
    {
      yrps_initfun_t *inif = (yrps_initfun_t *) ad;
      (*inif) ();
    };
}				/* end load_initial_module_yrps */

void
parse_generated_c_file_yrps (const char *dirpath, const char *entnam)
{
  assert (dirpath);
  assert (entnam);
  char bufpath[YRPS_PATHMAX];
  memset (bufpath, 0, sizeof (bufpath));
  snprintf (bufpath, sizeof (bufpath), "%s/%s", dirpath, entnam);
  if (yrps_check_valid_textual_file (bufpath))
    return;
  FILE *f = fopen (bufpath, "r");
  do
    {
      char linbuf[YRPS_LINEWIDTHMAX];
      memset (linbuf, 0, sizeof (linbuf));
      if (!fgets (linbuf, sizeof (linbuf), f))
	break;
      long int i = 0;
      int p = -1;
      char typbuf[16];
      memset (typbuf, 0, sizeof (typbuf));
      if (sscanf
	  (linbuf, " struct yrps_%10[a-z]_st yrps_v%ld =%n", typbuf, &i,
	   &p) > 2 && p > 0 && i > 0)
	{
	  char valnambuf[32];
	  memset (valnambuf, 0, sizeof (valnambuf));
	  snprintf (valnambuf, sizeof (valnambuf), "yrps_v%ld", i);
	  void *valad = dlsym (yrps_proghdl, valnambuf);
	  if (!valad)
	    {
	      fprintf (stderr, "%s failed to dlsym %s [%s:%d] : %s\n",
		       yrps_argv[0], valnambuf, __FILE__, __LINE__ - 1,
		       dlerror ());
	      YRPS_FAIL ();
	    };
	  if (!strcmp (typbuf, "string"))
	    {
	      struct yrps_string_st *valstr = valad;
	      assert (valstr->vkind == Kyrps_string);

#warning should have an hash table and call add_value_yrps
	      (void) add_value_yrps;
	    }
	}
      else if (sscanf
	       (linbuf, " struct yrps_object_st yrps_ob%ld =%n", &i,
		&p) > 2 && p > 0 && i > 0)
	{
#warning should call yrps_make_object
	  (void) yrps_make_object;
	}
    }
  while (!feof (f));
  fclose (f);
#warning incomplete parse_generated_c_path_yrps
}				/* end parse_generated_c_path_yrps */

void
parse_textual_data_file_yrps (const char *dirpath, const char *entnam)
{
  assert (dirpath);
  assert (entnam);
  char bufpath[YRPS_PATHMAX];
  memset (bufpath, 0, sizeof (bufpath));
  snprintf (bufpath, sizeof (bufpath), "%s/%s", dirpath, entnam);
  if (yrps_check_valid_textual_file (bufpath))
    return;
  FILE *f = fopen (bufpath, "r");
  assert (f);
  do
    {
      char linbuf[YRPS_LINEWIDTHMAX];
      memset (linbuf, 0, sizeof (linbuf));
      if (!fgets (linbuf, sizeof (linbuf), f))
	break;
      YRPS_FAIL ();
#warning unimplemented parse_textual_data_file_yrps
    }
  while (!feof (f));
  fclose (f);
}				/* end parse_textual_data_file_yrps */


int
add_value_yrps (struct yrps_value_st *v)
{
  assert (v);
  if (lenvalvec_yrps >= sizvalvec_yrps)
    {
      int newsiz = ((4 * lenvalvec_yrps / 3 + 10) & 0x1f) + 1;
      assert (newsiz > sizvalvec_yrps);
      struct yrps_value_st **oldvec = valvec_yrps;
      valvec_yrps = YRPS_CALLOC (newsiz, sizeof (struct yrps_value_st *));
      if (oldvec)
	{
	  memcpy (valvec_yrps, oldvec,
		  lenvalvec_yrps * sizeof (struct yrps_value_st *));
	  free (oldvec);
	};
    };
  valvec_yrps[lenvalvec_yrps++] = v;
}				/* end add_value_yrps */
