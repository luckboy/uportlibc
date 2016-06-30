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
#ifndef _WCHAR_H
#define _WCHAR_H

#include <uportlibc/mbstate.h>
#define __W 'w'
#include <uportlibc/w_ctype.h>
#define __W 'w'
#include <uportlibc/w_stdio.h>
#define __W 'w'
#include <uportlibc/w_stdlib.h>
#define __W 'w'
#include <uportlibc/w_string.h>

#ifndef WEOF
#define WEOF                    ((wint_t) (-1))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WCTYPE_T
#define _WCTYPE_T
typedef unsigned wctype_t;
#endif

typedef __uportlibc_mbstate_t mbstate_t;

/* Functions from the wctype.h header. */

int iswctype(wint_t c, wctype_t char_type);
wctype_t wctype(const char *name);

/* Other functions. */

wint_t btowc(int c);
int fwide(FILE *stream, int mode);
size_t mbrlen(const char *str, size_t count, mbstate_t *state);
size_t mbrtowc(wchar_t *wc, const char *str, size_t count, mbstate_t *state);
int mbsinit(const mbstate_t *state);
size_t mbsrtowcs(wchar_t *wcs, const char **str, size_t count, mbstate_t *state);
size_t wcrtomb(char *str, wchar_t wc, mbstate_t *state);
size_t wcsrtombs(char *str, const wchar_t **wcs, size_t count, mbstate_t *state);

#ifdef __cplusplus
}
#endif

#endif
