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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

extern int yrps_argc;
extern char** yrps_argv;

#endif //YRPS_INCLUDED
