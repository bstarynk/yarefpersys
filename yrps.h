// file yarefpersys/yrps.h
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef YRPS_INCLUDED
#define YRPS_INCLUDED
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

#ifndef YRPS_ID
#error YRPS_ID should be defined by compilation command (using yrps-shortgit.sh)
#endif

#define _GNU_SOURCE 1
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistr.h> 
#include <readline/readline.h>
#include <curl/curl.h>
#include <dlfcn.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef YRPS_PATHMAX
#define YRPS_PATHMAX 384
#endif /*YRPS_PATHMAX */

#ifndef YRPS_LINEWIDTHMAX
#define YRPS_LINEWITHMAX 256
#endif

#ifndef YRPS_LINECOUNTMAX
#define YRPS_LINECOUNTMAX 4096
#endif

extern int yrps_argc;
extern char *const *yrps_argv;
extern void *yrps_proghdl;
extern char yrps_dirpath[YRPS_PATHMAX];

// check that a directory is readable
extern bool yrps_readable_directory (const char *dirpath);

// check that a file is textual, no more than YRPS_LINECOUNTMAX lines &
// YRPS_LINEWIDTHMAX bytes per line, each line being UTF8
// on error stderr gets a message
extern bool yrps_check_valid_textual_file(const char*path);
#endif //YRPS_INCLUDED
