// file yarefpersys/main_yrps.c
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

const char main_yrps_id[] = YRPS_ID;


int yrps_argc;
char *const *yrps_argv;
void *yrps_proghdl;
char yrps_dirpath[YRPS_PATHMAX] = YRPS_SRCDIR;


bool
yrps_readable_directory (const char *dn)
{
  struct stat dst;
  memset (&dst, 0, sizeof (dst));
  if (!dn)
    return false;
  if (stat (dn, &dst))
    return false;
  if ((dst.st_mode & S_IFMT) == S_IFDIR)
    return ((dst.st_mode & 0500) != 0);	//0500 is r-x------
  return false;
}				/* end yrps_readable_directory */

static void
handle_two_arguments_yrps (const char **argv)
{
  assert (argv);
  assert (argv[0]);
  assert (argv[1]);
  if (!strcmp (argv[1], "--version"))
    {
      printf ("%s version id %s\n", argv[0], main_yrps_id);
      printf ("\t curl version is %s\n", curl_version ());
      printf ("\t see github.com/bstarynk/yarefpersys\n");
      exit (EXIT_SUCCESS);
    }
  else if (!strcmp (argv[1], "--help"))
    {
      printf ("%s usage (from %s:%d):\n", argv[0], __FILE__, __LINE__);
      printf ("\t --version              # gives version information\n");
      printf ("\t --help                 # gives this help\n");
      printf ("\t see refpersys.org and github.com/RefPerSys\n");
      printf ("\t this %s:%d is in github.com/bstarynk/yarefpersys\n",
	      __FILE__, __LINE__ - 1);
      printf ("\t °yrps_prefix_st size %d align %d\n",
	      (int) sizeof (struct yrps_prefix_st),
	      (int) alignof (struct yrps_prefix_st));
      printf ("\t °yrps_value_st size %d align %d\n",
	      (int) sizeof (struct yrps_value_st),
	      (int) alignof (struct yrps_value_st));
      printf ("\t °yrps_pairvect_st size %d align %d\n",
	      (int) sizeof (struct yrps_pairvect_st),
	      (int) alignof (struct yrps_pairvect_st));
      printf ("\t °yrps_dictvect_st size %d align %d\n",
	      (int) sizeof (struct yrps_dictvect_st),
	      (int) alignof (struct yrps_dictvect_st));
      printf ("\t °yrps_object_st size %d align %d\n",
	      (int) sizeof (struct yrps_object_st),
	      (int) alignof (struct yrps_object_st));
#warning incomplete --help code
      exit (EXIT_SUCCESS);
    };
  if (argv[1][0] == '/' || argv[1][0] == '.' || isalpha (argv[1][0]))
    {
      if (yrps_readable_directory (argv[1])
	  && strlen (argv[1]) < YRPS_PATHMAX)
	strcpy (yrps_dirpath, argv[1]);
    }
}				/* end handle_two_arguments_yrps */

void
yrps_fail_at (const char *fil, int lin)
{
  fprintf (stderr, "%s failing at %s:%d (git %s)\n",
	   yrps_argv[0], fil, lin, YRPS_ID);
  fflush (NULL);
  abort ();
}				/* end yrps_fail_at */


void *
yrps_calloc_at (long nbelem, unsigned size, const char *fil, int lin)
{
  if (nbelem < 0)
    return NULL;
  if (size == 0)
    return NULL;
  void *p = calloc (nbelem, size);
  if (!p)
    {
      fprintf (stderr, "%s calloc failed at %s:%d (%s)\n",
	       yrps_argv[0], fil, lin, strerror (errno));
      YRPS_FAIL ();
    };
  return p;
}				/* end yrps_calloc_at */

void *
yrps_malloc_at (unsigned size, const char *fil, int lin)
{
  if (size == 0)
    return NULL;
  void *p = calloc (1, size);
  if (!p)
    {
      fprintf (stderr, "%s malloc failed at %s:%d (%s)\n",
	       yrps_argv[0], fil, lin, strerror (errno));
      YRPS_FAIL ();
    };
  return p;
}				/* end yrps_malloc_at */

int
main (int argc, char **argv)
{
  yrps_argc = argc;
  yrps_argv = argv;
  yrps_proghdl = dlopen (NULL, RTLD_NOW);
  assert (yrps_proghdl != NULL);
  assert (argc > 0);
  if (argc == 2)
    handle_two_arguments_yrps ((const char **) argv);
  yrps_load_state_from_directory (yrps_dirpath);
  return 0;
#warning nearly empty main
}				/* end of main */
