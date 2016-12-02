/*
 * Copyright (c) 2016 Łukasz Szpakowski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _UPORTLIBC_H
#define _UPORTLIBC_H

#include <sys/types.h>
#include <stddef.h>
#include <wchar.h>
#include "lock.h"

/* Structures, types, variables and macros. */

#ifndef _UPORTLIBC_FILE
#define _UPORTLIBC_FILE

struct uportlibc_FILE;

typedef struct uportlibc_FILE uportlibc_FILE;

#if defined(UPORTLIBC_STDIO) || defined(UPORTLIBC_WCHAR)
#define FILE uportlibc_FILE
#define __uportlibc_FILE uportlibc_FILE
#endif

#endif

/* Functions and macros for the bsearch.c file. */

void *uportlibc_bsearch(const void *key, const void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));

#ifdef UPORTLIBC_BSEARCH
#define bsearch uportlibc_bsearch
#endif

/* Functions and macros for the conv.c file. */

long double uportlibc_log_pow_div_for_conv(long double x, long *exp, int is_hex);
long double uportlibc_pow_mul_for_conv(long double x, long exp, long fract_digit_exp, int is_hex);

#ifdef UPORTLIBC_CONV
#define __uportlibc_log_pow_div_for_conv uportlibc_log_pow_div_for_conv
#define __uportlibc_pow_mul_for_conv uportlibc_pow_mul_for_conv
#endif

/* Functions and macros for the exit.c file. */

extern lock_t uportlibc_exit_lock;
extern void (*uportlibc_atexit_exit_fun)(void);
extern void (*uportlibc_environ_exit_fun)(void);
extern void (*uportlibc_stdio_exit_fun)(void);

void uportlibc_exit(int status);

#ifdef UPORTLIBC_EXIT
#define __uportlibc_exit_lock uportlibc_exit_lock
#define __uportlibc_atexit_exit_fun uportlibc_atexit_exit_fun
#define __uportlibc_environ_exit_fun uportlibc_environ_exit_fun
#define __uportlibc_stdio_exit_fun uportlibc_stdio_exit_fun
#define exit uportlibc_exit
#endif

/* Functions and macros for the qsort.c file. */

void uportlibc_qsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));

#ifdef UPORTLIBC_QSORT
#define qsort uportlibc_qsort
#endif

/* Types, variables, functions and macros for the fopen.c file, the init_stdio.c file, the stdio.c file and the stdio_priv.c file. */

#define UPORTLIBC_BUFSIZ        4096
#ifndef UPORTLIBC_EOF
#define UPORTLIBC_EOF           (-1)
#endif
#ifndef UPORTLIBC_WEOF
#define UPORTLIBC_WEOF          ((wint_t) (-1))
#endif

#define UPORTLIBC__IONBF        0
#define UPORTLIBC__IOFBF        1
#define UPORTLIBC__IOLBF        2

typedef struct
{
  off_t offset;
  mbstate_t mb_state;
} uportlibc_fpos_t;

extern uportlibc_FILE *uportlibc_stdin;
extern uportlibc_FILE *uportlibc_stdout;
extern uportlibc_FILE *uportlibc_stderr;

void uportlibc_init_stdio(void);
void uportlibc_add_stream(uportlibc_FILE *stream);
void uportlibc_delete_stream(uportlibc_FILE *stream);
int uportlibc_for_each_stream(int (*fun)(uportlibc_FILE *));
int uportlibc_unsafely_flush_stream(uportlibc_FILE *stream);
int uportlibc_unsafely_prepare_stream_to_read(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_prepare_stream_to_write(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_prepare_stream_to_unread(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_get_char(uportlibc_FILE *stream);
int uportlibc_unsafely_put_char(int c, uportlibc_FILE *stream);
int uportlibc_unsafely_unget_char(int c, uportlibc_FILE *stream);
void uportlibc_clearerr(uportlibc_FILE *stream);
int uportlibc_fclose(uportlibc_FILE *stream);
uportlibc_FILE *uportlibc_fdopen(int fd, const char *mode);
int uportlibc_feof(uportlibc_FILE *stream);
int uportlibc_ferror(uportlibc_FILE *stream);
int uportlibc_fflush(uportlibc_FILE *stream);
int uportlibc_fgetpos(uportlibc_FILE *stream, uportlibc_fpos_t *pos);
int uportlibc_fileno(uportlibc_FILE *stream);
void uportlibc_flockfile(uportlibc_FILE *stream);
uportlibc_FILE *uportlibc_fopen(const char *file_name, const char *mode);
size_t uportlibc_fread(void *ptr, size_t elem_size, size_t elem_count, uportlibc_FILE *stream);
uportlibc_FILE *uportlibc_freopen(const char *file_name, const char *mode, uportlibc_FILE *stream);
int uportlibc_fseek(uportlibc_FILE *stream, long offset, int whence);
int uportlibc_fseeko(uportlibc_FILE *stream, off_t offset, int whence);
int uportlibc_fsetpos(uportlibc_FILE *stream, const uportlibc_fpos_t *pos);
long uportlibc_ftell(uportlibc_FILE *stream);
off_t uportlibc_ftello(uportlibc_FILE *stream);
int uportlibc_ftrylockfile(uportlibc_FILE *stream);
void uportlibc_funlockfile(uportlibc_FILE *stream);
size_t uportlibc_fwrite(const void *ptr, size_t elem_size, size_t elem_count, uportlibc_FILE *stream);
int uportlibc_getw(uportlibc_FILE *stream);
char *uportlibc_gets(char *str);
int uportlibc_pclose(uportlibc_FILE *stream);
void uportlibc_perror(const char *str);
uportlibc_FILE *uportlibc_popen(const char *command, const char *mode);
int uportlibc_putw(int w, uportlibc_FILE *stream);
int uportlibc_puts(const char *str);
int uportlibc_remove(const char *path_name);
void uportlibc_rewind(uportlibc_FILE *stream);
void uportlibc_setbuf(uportlibc_FILE *stream, char *buf);
int uportlibc_setvbuf(uportlibc_FILE *stream, char *buf, int type, size_t size);

#ifdef UPORTLIBC_STDIO
#undef BUFSIZ
#define BUFSIZ UPORTLIBC_BUFSIZ
#undef EOF
#define EOF UPORTLIBC_EOF
#undef WEOF
#define WEOF UPORTLIBC_WEOF
#undef _IONBF
#define _IONBF UPORTLIBC__IONBF
#undef _IOFBF
#define _IOFBF UPORTLIBC__IOFBF
#undef _IOLBF
#define _IOLBF UPORTLIBC__IOLBF
#define fpos_t uportlibc_fpos_t
#undef stdin
#define stdin uportlibc_stdin
#undef stdout
#define stdout uportlibc_stdout
#undef stderr
#define stderr uportlibc_stderr
#define __uportlibc_init_stdio uportlibc_init_stdio
#define __uportlibc_add_stream uportlibc_add_stream
#define __uportlibc_delete_stream uportlibc_delete_stream
#define __uportlibc_for_each_stream uportlibc_for_each_stream
#define __uportlibc_unsafely_flush_stream uportlibc_unsafely_flush_stream
#define __uportlibc_unsafely_prepare_stream_to_read uportlibc_unsafely_prepare_stream_to_read
#define __uportlibc_unsafely_prepare_stream_to_write uportlibc_unsafely_prepare_stream_to_write
#define __uportlibc_unsafely_prepare_stream_to_unread uportlibc_unsafely_prepare_stream_to_unread
#define __uportlibc_unsafely_get_char uportlibc_unsafely_get_char
#define __uportlibc_unsafely_put_char uportlibc_unsafely_put_char
#define __uportlibc_unsafely_unget_char uportlibc_unsafely_unget_char
#define clearerr uportlibc_clearerr
#define fclose uportlibc_fclose
#define fdopen uportlibc_fdopen
#define feof uportlibc_feof
#define ferror uportlibc_ferror
#define fflush uportlibc_fflush
#define fgetpos uportlibc_fgetpos
#define fileno uportlibc_fileno
#define flockfile uportlibc_flockfile
#define fopen uportlibc_fopen
#define fread uportlibc_fread
#define freopen uportlibc_freopen
#define fseek uportlibc_fseek
#define fseeko uportlibc_fseeko
#define fsetpos uportlibc_fsetpos
#define ftell uportlibc_ftell
#define ftello uportlibc_ftello
#define ftrylockfile uportlibc_ftrylockfile
#define funlockfile uportlibc_funlockfile
#define fwrite uportlibc_fwrite
#define getw uportlibc_getw
#define gets uportlibc_gets
#define pclose uportlibc_pclose
#define perror uportlibc_perror
#define popen uportlibc_popen
#define putw uportlibc_putw
#define puts uportlibc_puts
#define remove uportlibc_remove
#define rewind uportlibc_rewind
#define setbuf uportlibc_setbuf
#define setvbuf uportlibc_setvbuf
#endif

/* Functions and macros for the w_stdio.c file. */

int uportlibc_fgetc(uportlibc_FILE *stream);
char *uportlibc_fgets(char *str, int count, uportlibc_FILE *stream);
int uportlibc_fputc(int c, uportlibc_FILE *stream);
int uportlibc_fputs(const char *str, uportlibc_FILE *stream);
int uportlibc_fputs_unlocked(const char *str, uportlibc_FILE *stream);
int uportlibc_getc_unlocked(uportlibc_FILE *stream);
int uportlibc_getchar_unlocked(void);
int uportlibc_putc_unlocked(int c, uportlibc_FILE *stream);
int uportlibc_putchar_unlocked(int c);
wint_t uportlibc_fgetwc(uportlibc_FILE *stream);
wchar_t *uportlibc_fgetws(wchar_t *str, int count, uportlibc_FILE *stream);
int uportlibc_ungetc(int c, uportlibc_FILE *stream);
wint_t uportlibc_ungetwc(wint_t c, uportlibc_FILE *stream);

#ifdef UPORTLIBC_W_STDIO
#define fgetc uportlibc_fgetc
#define fgets uportlibc_fgets
#define fputc uportlibc_fputc
#define fputs uportlibc_fputs
#define fputs_unlocked uportlibc_fputs_unlocked
#define getc_unlocked uportlibc_getc_unlocked
#define getchar_unlocked uportlibc_getchar_unlocked
#define putc_unlocked uportlibc_putc_unlocked
#undef putchar_unlocked
#define putchar_unlocked uportlibc_putchar_unlocked
#define fgetwc uportlibc_fgetwc
#define fgetws uportlibc_fgetws
#define ungetc unportlibc_ungetc
#define ungetwc unportlibc_ungetwc
#endif

/* Types, functions and macros for the wchar.c file. */

#define UPORTLIBC_MB_LEN_MAX    4

typedef struct
{
  size_t count;
  wchar_t wc;
} uportlibc_mbstate_t;

wint_t uportlinbc_btowc(int c);
int uportlibc_fwide(uportlibc_FILE *stream, int mode);
size_t uportlibc_mbrlen(const char *str, size_t count, uportlibc_mbstate_t *state);
size_t uportlibc_mbrtowc(wchar_t *wc, const char *str, size_t count, uportlibc_mbstate_t *state);
int uportlibc_mbsinit(const uportlibc_mbstate_t *state);
size_t uportlibc_mbsrtowcs(wchar_t *wcs, const char **str, size_t count, uportlibc_mbstate_t *state);
size_t uportlibc_wcrtomb(char *str, wchar_t wc, uportlibc_mbstate_t *state);
size_t uportlibc_wcsrtombs(char *str, const wchar_t **wcs, size_t count, uportlibc_mbstate_t *state);

#ifdef UPORTLIBC_WCHAR
#define btowc uportlinbc_btowc
#define fwide uportlibc_fwide
#define mbrlen uportlibc_mbrlen
#define mbrtowc uportlibc_mbrtowc
#define mbsinit uportlibc_mbsinit
#define mbsrtowcs uportlibc_mbsrtowcs
#define wcrtomb uportlibc_wcrtomb
#define wcsrtombs uportlibc_wcsrtombs
#endif

#endif
