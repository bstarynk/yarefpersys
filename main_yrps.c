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

static void
handle_two_arguments_yrps (const char **argv)
{
  if (!strcmp (argv[1], "--version"))
    {
      printf ("%s version id %s\n", argv[0], main_yrps_id);
      printf ("\t see github.com/bstarynk/yarefpersys\n");
      exit (EXIT_SUCCESS);
    }
}				/* end handle_two_arguments_yrps */

int
main (int argc, char **argv)
{
  yrps_argc = argc;
  yrps_argv = argv;
  yrps_proghdl = dlopen (NULL, RTLD_NOW);
  if (argc == 2)
    handle_two_arguments_yrps ((const char **) argv);
  return 0;
#warning nearly empty main
}				/* end of main */
