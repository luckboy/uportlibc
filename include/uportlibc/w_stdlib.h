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
#ifdef __W
#if ( \
  (__W == 'c' && !defined(_C_UPORTLIBC_W_STDLIB_H)) || \
  (__W == 'w' && !defined(_W_UPORTLIBC_W_STDLIB_H)))
#if __W == 'c'
#define _C_UPORTLIBC_W_STDLIB_H
#endif
#if __W == 'w'
#define _W_UPORTLIBC_W_STDLIB_H
#endif

#include <stddef.h>
#undef __W_UNDEF
#include <uportlibc/w_name.h>

#ifdef __cplusplus
extern "C" {
#endif

double __W_STR_NAME(tod)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
float __W_STR_NAME(tof)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
long __W_STR_NAME(tol)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
long double __W_STR_NAME(told)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
long long __W_STR_NAME(toll)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
unsigned long __W_STR_NAME(toul)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
unsigned long long __W_STR_NAME(toull)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);

#ifdef __cplusplus
}
#endif

#define __W_UNDEF
#include <uportlibc/w_name.h>

#else
#undef __W
#endif
#endif
