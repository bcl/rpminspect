/*
 * Copyright (C) 2019-2020  Red Hat, Inc.
 * Author(s):  David Cantrell <dcantrell@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * This header includes the API definition for librpminspect.
 */

#include <stdbool.h>
#include <sys/types.h>
#include <sys/capability.h>
#include <regex.h>
#include <rpm/header.h>

#include "constants.h"
#include "types.h"
#include "inspect.h"
#include "results.h"
#include "output.h"
#include "readelf.h"

#ifndef _LIBRPMINSPECT_RPMINSPECT_H
#define _LIBRPMINSPECT_RPMINSPECT_H

/* Debugging mode toggle */
extern bool debug_mode;

/* List of all inspections (inspect.c) */
extern struct inspect inspections[];

/* List of all output format types (output.c) */
extern struct format formats[];

/* Macros */
#ifdef NDEBUG
/* Don't create unused variables if not using assert() */
#define xasprintf(dest, ...) {                   \
    *(dest) = NULL;                              \
    asprintf((dest), __VA_ARGS__);               \
}
#else
#define xasprintf(dest, ...) {                   \
    int _xasprintf_result;                       \
    *(dest) = NULL;                              \
    _xasprintf_result = asprintf((dest), __VA_ARGS__);\
    assert(_xasprintf_result != -1);             \
}
#endif

#ifdef GETTEXT_DOMAIN
#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)
#define N_(STRING) ngettext(STRING)
#else
#define _(STRING) STRING
#define N_(STRING) STRING
#endif

/*
 * Simple debugging printf.  Sends output to stderr if debugging
 * mode is enabled at runtime.
 */
#define DEBUG_PRINT(...)                                         \
    if (debug_mode) {                                            \
        fprintf(stderr, "debug: %s (%d): ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                            \
        fflush(stderr);                                          \
    }

/*
 * Types of exit codes from the program.
 */
enum {
    RI_INSPECTION_SUCCESS = 0,   /* inspections passed */
    RI_INSPECTION_FAILURE = 1,   /* inspections failed */
    RI_PROGRAM_ERROR = 2         /* program errored in some way */
};

/*
 * Build identifier strings (used in paths)
 * The enum values map to the build_desc array index.
 */
enum { BEFORE_BUILD, AFTER_BUILD };

/*
 * Supported checksum types.
 */
enum checksum { NULLSUM, MD5SUM, SHA1SUM, SHA256SUM };

/* Common functions */

/* init.c */
bool init_stat_whitelist(struct rpminspect *);
bool init_caps_whitelist(struct rpminspect *);
int init_rpminspect(struct rpminspect *, const char *, const char *);

/* free.c */
void free_regex(regex_t *);
void free_mapping(struct hsearch_data *, string_list_t *);
void free_pair(pair_list_t *);
void free_rpminspect(struct rpminspect *);

/* listfuncs.c */
char *list_to_string(const string_list_t *, const char *);
char **list_to_array(const string_list_t *);
struct hsearch_data * list_to_table(const string_list_t *);
string_list_t * list_difference(const string_list_t *, const string_list_t *);
string_list_t * list_intersection(const string_list_t *, const string_list_t *);
string_list_t * list_union(const string_list_t *, const string_list_t *);
string_list_t * list_symmetric_difference(const string_list_t *, const string_list_t *);
typedef void (*list_entry_data_free_func)(void *);
void list_free(string_list_t *, list_entry_data_free_func);
size_t list_len(const string_list_t *);
string_list_t * list_sort(const string_list_t *);
string_list_t * list_copy(const string_list_t *);
string_list_t *list_from_array(const char **);

/* local.c */
bool is_local_build(const char *);
bool is_local_rpm(struct rpminspect *, const char *);

/* koji.c */
koji_buildlist_t *init_koji_buildlist(void);
void free_koji_buildlist(koji_buildlist_t *);
koji_rpmlist_t *init_koji_rpmlist(void);
void free_koji_rpmlist(koji_rpmlist_t *);
void init_koji_build(struct koji_build *);
void free_koji_build(struct koji_build *);
void init_koji_descendent(koji_task_entry_t *);
void init_koji_task(struct koji_task *);
void free_koji_task_entry(koji_task_entry_t *);
void free_koji_task(struct koji_task *);
struct koji_build *get_koji_build(struct rpminspect *, const char *);
struct koji_task *get_koji_task(struct rpminspect *, const char *);
string_list_t *get_all_arches(const struct rpminspect *);
bool allowed_arch(const struct rpminspect *, const char *);

/* kmods.c */
bool compare_module_parameters(const struct kmod_list *, const struct kmod_list *, string_list_t **, string_list_t **);
bool compare_module_dependencies(const struct kmod_list *, const struct kmod_list *, string_list_t **, string_list_t **);
kernel_alias_data_t *gather_module_aliases(const char *module_name, const struct kmod_list *modinfo_list);
void free_module_aliases(kernel_alias_data_t *);
bool compare_module_aliases(kernel_alias_data_t *, kernel_alias_data_t *, module_alias_callback, void *);
string_list_t *get_kmod_values(const char *, const char *);

/* mkdirp.c */
int mkdirp(char *, mode_t);

/* rmtree.c */
int rmtree(const char *, const bool, const bool);

/* strfuncs.c */
bool strprefix(const char *, const char *);
bool strsuffix(const char *, const char *);
int printwrap(const char *, const size_t, const unsigned int, FILE *);
bool versioned_match(const char *, Header, const char *, Header);
char *strseverity(const severity_t);
severity_t getseverity(const char *);
char *strwaiverauth(const waiverauth_t);
char *strreplace(const char *, const char *, const char *);
char *strappend(char *, const char *);
string_list_t *strsplit(const char *, const char *);
const char *strtype(const mode_t mode);

/* badwords.c */
bool has_bad_word(const char *, const string_list_t *);

/* copyfile.c */
int copyfile(const char *, const char *, bool, bool);

/* rpm.c */
int init_librpm(void);
Header get_rpm_header(struct rpminspect *, const char *);
char *get_rpmtag_str(Header, rpmTagVal);
char *get_nevr(Header);
char *get_nevra(Header);
const char *get_rpm_header_arch(Header);

/* peers.c */
rpmpeer_t *init_rpmpeer(void);
void free_rpmpeer(rpmpeer_t *);
void add_peer(rpmpeer_t **, int, bool, const char *, Header);

/* files.c */
void free_files(rpmfile_t *files);
rpmfile_t * extract_rpm(const char *, Header, char **output_dir);
bool process_file_path(const rpmfile_entry_t *, regex_t *, regex_t *);
void find_file_peers(rpmfile_t *, rpmfile_t *);
cap_t get_cap(rpmfile_entry_t *);
bool is_debug_or_build_path(const char *);
bool is_payload_empty(rpmfile_t *);

/* tty.c */
size_t tty_width(void);

/* results.c */
void init_result_params(struct result_params *);
results_t *init_results(void);
void free_results(results_t *);
void add_result_entry(results_t **, struct result_params *);
void add_result(struct rpminspect *, struct result_params *);

/* output.c */
const char *format_desc(unsigned int);

/* output_text.c */
void output_text(const results_t *, const char *);

/* output_json.c */
void output_json(const results_t *, const char *);

/* unpack.c */
int unpack_archive(const char *, const char *, const bool);

/* magic.c */
char *get_mime_type(rpmfile_entry_t *);
bool is_text_file(rpmfile_entry_t *);

/* checksums.c */
char *compute_checksum(const char *, mode_t *, enum checksum);
char *checksum(rpmfile_entry_t *);

/* runcmd.c */
char *run_cmd(int *, const char *, ...);

/* whitelist.c */
bool on_stat_whitelist_mode(struct rpminspect *, const rpmfile_entry_t *, const char *, const char *);
bool on_stat_whitelist_owner(struct rpminspect *, const rpmfile_entry_t *, const char *, const char *, const char *);
bool on_stat_whitelist_group(struct rpminspect *, const rpmfile_entry_t *, const char *, const char *, const char *);
caps_filelist_entry_t *get_caps_whitelist_entry(struct rpminspect *, const char *, const char *);

/* flags.c */
bool process_inspection_flag(const char *, const bool, uint64_t *);

/* debug.c */
void set_debug_mode(bool);
void dump_cfg(const struct rpminspect *);

/* readfile.c */
string_list_t *read_file(const char *);

/* release.c */
char *read_release(const rpmfile_t *);
const char *get_before_rel(struct rpminspect *);
const char *get_after_rel(struct rpminspect *);

/* builds.c */
int gather_builds(struct rpminspect *, bool);

/* macros.c */
string_list_t *get_macros(const char *);
int get_specfile_macros(struct rpminspect *, const char *);

/* inspect_elf.c */
bool is_execstack_valid(Elf *elf, uint64_t flags);
bool is_stack_executable(Elf *elf, uint64_t flags);
bool is_pic_ok(Elf *elf);
bool has_bind_now(Elf *elf);
bool has_executable_program(Elf *elf);
bool has_relro(Elf *elf);
uint64_t get_execstack_flags(Elf *elf);
bool is_execstack_present(Elf *elf);
bool has_textrel(Elf *elf);
void free_elf_data(void);
void init_elf_data(void);

/* bytes.c */
/**
 * Given a byte array of a specified length, convert it to a NUL
 * terminated string and return a pointer to the string to the caller.
 * The caller is responsible for freeing the memory associated with
 * this string.
 *
 * @param array The byte array to convert.
 * @param len Number of elements in the byte array.
 * @return Newly allocated string representation of the byte array.
 */
char *bytes_to_str(unsigned char *array, size_t len);

/* ignore.c */
/**
 * @brief Given a path and struct rpminspect, determine if the path
 * should be ignored or not.
 *
 * @param ri The struct rpminspect for the program.  @param path The
 * relative path to check (i.e., localpath).  @param root The root
 * directory, optional (pass NULL to use '/').  @return True if path
 * should be ignored, false otherwise.
 */
bool ignore_path(const struct rpminspect *ri, const char *path, const char *root);

#endif
