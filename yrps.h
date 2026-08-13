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
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistr.h>
#include <dirent.h>
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

/// maximal symbol name length in bytes
#ifndef YRPS_SYMLENMAX
/*€ longueur maximale des noms de fichiers */
#define YRPS_SYMLENMAX 32
#endif /*YRPS_SYMLENMAX */

// maximal width in bytes 
#ifndef YRPS_LINEWIDTHMAX
/*€ longueur max (octets) d'une ligne de fichier */
#define YRPS_LINEWIDTHMAX 256
#endif

// maximal number of lines in any textual files, including C files
// (either generated like _pseudo_yrps.c should be or hand-written
// like main_yrps.c)
#ifndef YRPS_LINECOUNTMAX
/*€ nombre maximum de lignes dans nos fichiers textuels, y compris en
  C (générés comme devrait l'être _pseudo_yrps.c ou écrits à la main
  comme main_yrps.c) */
#define YRPS_LINECOUNTMAX 8192
#endif


/// main arguments
/*€ argument du main et du programme */
extern int yrps_argc;
extern char *const *yrps_argv;

void *yrps_calloc_at (long nbelem, unsigned size, const char *fil, int lin);
#define YRPS_CALLOC(N,S) yrps_calloc_at((N),(S),__FILE__,__LINE__);
void *yrps_malloc_at (unsigned size, const char *fil, int lin);
#define YRPS_MALLOC(S) yrps_malloc_at((N),__FILE__,__LINE__);

void yrps_fail_at (const char *fil, int lin) __attribute__((noreturn));
#define YRPS_FAIL() yrps_fail_at(__FILE__,__LINE__);

enum yrps_kind_en
{
  Kyrps__None,
  Kyrps_string,
  Kyrps_intvect,
  Kyrps_doublevect,
  Kyrps_pairvect,
  Kyrps_dictvect,
  Kyrps_obseq,
  Kyrps_obref,
  Kyrps__Last
};

struct yrps_object_st;
struct yrps_value_st;
struct yrps_pairvect_st;

#define YRPS_PREFIX_FIELDS			\
  enum yrps_kind_en vkind;			\
  unsigned char vmark;				\
  short vlen

struct yrps_prefix_st
{
  YRPS_PREFIX_FIELDS;
};

struct yrps_string_st
{
  YRPS_PREFIX_FIELDS;
  const char v_str[];
};

struct yrps_intvec_st
{
  YRPS_PREFIX_FIELDS;
  const int64_t v_intvec[];
};

struct yrps_dblvec_st
{
  YRPS_PREFIX_FIELDS;
  const double v_dblvec[];
};
struct yrps_pairvect_st
{
  YRPS_PREFIX_FIELDS;
  /// the pairs should be ordered by the o_id of objects
  struct
  {
    struct yrps_object_st *p_ob;
    struct yrps_value_st *p_va;
  } v_pairvec[];
};

struct yrps_dictvect_st
{
  YRPS_PREFIX_FIELDS;
  /// the pairs should be ordered by the p_name
  struct
  {
    const char *p_name;		/* strduped */
    struct yrps_object_st *p_nmob;
  } v_dictvec[];
};

struct yrps_value_st
{
  YRPS_PREFIX_FIELDS;
  union
  {
    const char v_str[];
    const int64_t v_intvec[];
    const double v_dblvec[];
    const struct yrps_object_st *v_obvec[];
  };
};

struct yrps_object_st
{
  YRPS_PREFIX_FIELDS;
  const int64_t o_id;
  pthread_mutex_t o_mtx;
  int32_t o_nbpair;
  int32_t o_nbval;
  void *o_funad;
  struct yrps_pairvect_st *o_pairv;	// vector ordered by o_id
  struct yrps_value_st *o_valseq;	//
};

// self program handle (dlopen)
/*€ poignée vers le programme tout entier (dlopen) */
extern void *yrps_proghdl;

// self program base 
// self code and/or data directory path
/*€ le repertoire contenant le source et/ou les données textuelles */
extern char yrps_dirpath[YRPS_PATHMAX];

// check that a directory is readable and convenient
/*€ vérifie qu'un repertoire est lisible et convenable */
extern bool yrps_readable_directory (const char *dirpath);

// Check that a file is textual, no more than YRPS_LINECOUNTMAX lines &
// YRPS_LINEWIDTHMAX bytes per line, each line being UTF8
// on error stderr gets a message and returns false
/*€ Vérifie qu'un fichier textuel est correct: pas plus de
  YRPS_LINECOUNTMAX lignes, chacune encodée UTF8 et ayant au plus
  YRPS_LINEWIDTHMAX octets; en cas d'erreur la sort d'erreur affiche
  un message et renvoie faux */
extern bool yrps_check_valid_textual_file (const char *path);

// Signature of initialization functions named <NAME>_inityrps
/*€ signature des fonctions d'initialisation <NOM>_inityrps*/
typedef void yrps_initfun_t (void);
// Load the state from directory
/*€ charge l'état depuis le repertoire indiqué */
extern void yrps_load_state_from_directory (const char *dirpath);


// return 0 if not found
extern int64_t yrps_prime_above (int64_t l);
extern int64_t yrps_prime_below (int64_t l);
#endif //YRPS_INCLUDED
