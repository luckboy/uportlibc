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

long double uportlibc_log_pow_div_for_conv(long double x, long *exp, int is_hex);
long double uportlibc_pow_mul_for_conv(long double x, long exp, long fract_digit_exp, int is_hex);
void *uportlibc_bsearch(const void *key, const void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));
void uportlibc_qsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));

#ifdef UPORTLIBC_LOG_POW_DIV_FOR_CONV
#define __uportlibc_log_pow_div_for_conv uportlibc_log_pow_div_for_conv
#endif
#ifdef UPORTLIBC_POW_MUL_FOR_CONV
#define __uportlibc_pow_mul_for_conv uportlibc_pow_mul_for_conv
#endif
#ifdef UPORTLIBC_BSEARCH
#define bsearch uportlibc_bsearch
#endif
#ifdef UPORTLIBC_QSORT
#define qsort uportlibc_qsort
#endif

#endif
