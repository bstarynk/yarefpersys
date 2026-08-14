// file yarefpersys/readline_yrps.c
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

const char readline_yrps_id[] = YRPS_ID;

static char *readline_completion_yrps (const char *, int);

void
yrps_init_readline (void)
{
#warning unimplemented yrps_init_readline
  YRPS_UNIQUE_BREAKPOINT ();
  fprintf (stderr, "yrps_init_readline [%s:%d] unimplemented\n",
	   __FILE__, __LINE__ - 1);
  rl_completion_entry_function = readline_completion_yrps;
}				/* end yrps_init_readline */


char *
readline_completion_yrps (const char *str, int key)
{
  YRPS_UNIQUE_BREAKPOINT ();
  YRPS_PRINTFAIL ("readline_completion_yrps str=%s key=%d unimplemented\n",
		  str, key);
}				/* end readline_completion_yrps */


void
yrps_readline_loop (void)
{
  YRPS_PRINTFAIL ("unimplemented yrps_readline_loop git %s\n",
		  readline_yrps_id);
#warning unimplemented yrps_readline_loop
}				/* end yrps_readline_loop */

///// eof readline_yrps.c [€fin du fichier]
