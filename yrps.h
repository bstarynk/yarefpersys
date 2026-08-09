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

/// maximal path of files 
#ifndef YRPS_PATHMAX
/*€ longueur maximale des noms de fichiers */
#define YRPS_PATHMAX 384
#endif /*YRPS_PATHMAX */

// maximal width in bytes 
#ifndef YRPS_LINEWIDTHMAX
/*€ longueur max (octets) d'une ligne de fichier */
#define YRPS_LINEWITHMAX 256
#endif

// maximal number of lines in any textual files 
#ifndef YRPS_LINECOUNTMAX
/*€ nombre maximum de lignes dans nos fichiers textuels, y compris en C */
#define YRPS_LINECOUNTMAX 8192
#endif


/// main arguments
/*€ argument du main et du programme */
extern int yrps_argc;
extern char *const *yrps_argv;

// self program handle (dlopen)
/*€ poignée vers le programme tout entier (dlopen) */
extern void *yrps_proghdl;

// self code and/or data directory path
/*€ le repertoire contenant le source et/ou les données textuelles */
extern char yrps_dirpath[YRPS_PATHMAX];

// check that a directory is readable and convenient
/*€ vérifie qu'un repertoire est lisible et convenable */
extern bool yrps_readable_directory (const char *dirpath);

// Check that a file is textual, no more than YRPS_LINECOUNTMAX lines &
// YRPS_LINEWIDTHMAX bytes per line, each line being UTF8
// on error stderr gets a message
/*€ Vérifie qu'un fichier textuel est correct: pas plus de
  YRPS_LINECOUNTMAX lignes, chacune encodée UTF8 et ayant au plus
  YRPS_LINEWIDTHMAX octets; en cas d'erreur la sort d'erreur affiche
  un message */
extern bool yrps_check_valid_textual_file (const char *path);
#endif //YRPS_INCLUDED
