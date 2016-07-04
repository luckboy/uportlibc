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

#include <stddef.h>
#include <wchar.h>

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

/* Functions and macros for the qsort.c file. */

void uportlibc_qsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));

#ifdef UPORTLIBC_QSORT
#define qsort uportlibc_qsort
#endif

/* Structures, types, functions and macros for the wchar.c file. */

#define UPORTLIBC_MB_LEN_MAX    4

typedef struct
{
  size_t count;
  wchar_t wc;
} uportlibc_mbstate_t;

struct uportlibc_FILE;

typedef struct uportlibc_FILE uportlibc_FILE;

wint_t uportlinbc_btowc(int c);
int uportlibc_fwide(uportlibc_FILE *stream, int mode);
size_t uportlibc_mbrlen(const char *str, size_t count, uportlibc_mbstate_t *state);
size_t uportlibc_mbrtowc(wchar_t *wc, const char *str, size_t count, uportlibc_mbstate_t *state);
int uportlibc_mbsinit(const uportlibc_mbstate_t *state);
size_t uportlibc_mbsrtowcs(wchar_t *wcs, const char **str, size_t count, uportlibc_mbstate_t *state);
size_t uportlibc_wcrtomb(char *str, wchar_t wc, uportlibc_mbstate_t *state);
size_t uportlibc_wcsrtombs(char *str, const wchar_t **wcs, size_t count, uportlibc_mbstate_t *state);

#ifdef UPORTLIBC_WCHAR
#undef MB_LEN_MAX
#define MB_LEN_MAX UPORTLIBC_MB_LEN_MAX
#define mbstate_t uportlibc_mbstate_t
#define FILE uportlibc_FILE
#define __uportlibc_FILE uportlibc_FILE
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
