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

#define RL_PROMPTSIZE_YRPS 32
static char readline_prompt_yrps[RL_PROMPTSIZE_YRPS] = "?°:";

static pthread_mutex_t readline_mtxpr_yrps =
  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static char *readline_completion_yrps (const char *, int);
static char **readline_attempted_completion_yrps (const char *, int, int);
static void readline_replace_yrps(const char*init, int start, int end, const char*replac);

void
yrps_init_readline (void)
{
#warning unimplemented yrps_init_readline
  YRPS_UNIQUE_BREAKPOINT ();
  rl_readline_name = "yarefpersys";
  fprintf (stderr, "yrps_init_readline [%s:%d] unimplemented\n",
	   __FILE__, __LINE__ - 1);
  rl_completion_entry_function = readline_completion_yrps;
  rl_attempted_completion_function = readline_attempted_completion_yrps;
}				/* end yrps_init_readline */


char *
readline_completion_yrps (const char *str, int key)
{
  if (!strcmp (str, "euro"))
    {
      YRPS_UNIQUE_BREAKPOINT ();
    }
  fputc ('\n', stderr);		/// temporary
  YRPS_UNIQUE_BREAKPOINT ();
  YRPS_PRINTFAIL
    ("readline_completion_yrps str=%s key=%d='%c' linbuf='%s' unimplemented\n",
     str, key, (char) key, rl_line_buffer);
}				/* end readline_completion_yrps */


char **
readline_attempted_completion_yrps (const char *text, int start, int end)
{
  char **res = NULL;
  YRPS_UNIQUE_BREAKPOINT ();
  if (start > 0 && rl_line_buffer[start - 1] == '\\')
    {
      if (!strcmp (text, "euro"))
	{
	  YRPS_UNIQUE_BREAKPOINT();
	  YRPS_PRINTFAIL ("readline_attempted_completion_yrps € text=%s "
			  "start=%d end=%d rlbuf=%s\n", text, start, end,
			  rl_line_buffer);
	  readline_replace_yrps("\euro", start-1, end, "€");
	  /// TODO we probably want to complete \eu as €
	}
    }
  YRPS_PRINTFAIL ("readline_attempted_completion_yrps text=%s "
		  "start=%d end=%d rlbuf=%s\n", text, start, end,
		  rl_line_buffer);
  return res;
}				/* end readline_attempted_completion_yrps */

void
yrps_set_readline_prompt (const char *ps)
{
  pthread_mutex_lock (&readline_mtxpr_yrps);
  memset (readline_prompt_yrps, 0, sizeof (readline_prompt_yrps));
  if (ps)
    {
      assert (strlen (ps) < sizeof (readline_prompt_yrps));
      strncpy (readline_prompt_yrps, ps, sizeof (readline_prompt_yrps) - 1);
    }
  pthread_mutex_unlock (&readline_mtxpr_yrps);
}

void
yrps_readline_loop (void)
{
  char prbuf[RL_PROMPTSIZE_YRPS];
  char *r = NULL;
  memset (prbuf, 0, sizeof (prbuf));
  bool again = false;
  YRPS_UNIQUE_BREAKPOINT ();
  do
    {
      r = NULL;
      pthread_mutex_lock (&readline_mtxpr_yrps);
      memcpy (prbuf, readline_prompt_yrps, RL_PROMPTSIZE_YRPS - 1);
      pthread_mutex_unlock (&readline_mtxpr_yrps);
      if (!prbuf[0])
	break;
      r = readline (prbuf);
      if (!r)
	break;
#warning should parse somehow the read line
      fprintf (stderr, "\n..readline got %s [%s:%d]\n", r, __FILE__,
	       __LINE__ - 1);
      free (r);
      r = NULL;
    }
  while (again);
  YRPS_PRINTFAIL ("unimplemented yrps_readline_loop git %s\n",
		  readline_yrps_id);
#warning unimplemented yrps_readline_loop
}				/* end yrps_readline_loop */


void
readline_replace_yrps(const char*init, int start, int end, const char*replac)
{
  char*oldrlbuf = NULL;
  char*newrlbuf = NULL;
  assert(init != NULL);
  assert(replac != NULL);
  assert(start < end);
  pthread_mutex_lock (&readline_mtxpr_yrps);
  YRPS_PRINTFAIL("unimplemented readline_replace_yrps init=%s start=%d\n"
		 " end=%d replac=%s rl_line_buffer=%s\n",
		 init, start, end, replac, rl_line_buffer);
#warning unimplemented readline_replace_yrps
  pthread_mutex_unlock (&readline_mtxpr_yrps);
} /* end readline_replace_yrps */
///// eof readline_yrps.c [€fin du fichier]
