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
#ifndef _WCTYPE_H
#define _WCTYPE_H

#define __W 'w'
#include <uportlibc/w_ctype.h>

#ifndef WEOF
#define WEOF                    ((wint_t) (-1))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __WCTRANS_T
#define __WCTRANS_T
typedef unsigned wctrans_t;
#endif

#ifndef __WCTYPE_T
#define __WCTYPE_T
typedef unsigned wctype_t;
#endif

int iswctype(wint_t c, wctype_t char_trans);
wint_t towctrans(wint_t c, wctrans_t char_type);
wctrans_t wctrans(const char *name);
wctype_t wctype(const char *name);

#ifdef __cplusplus
}
#endif

#endif
