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
  (__W == 'c' && !defined(_C_UPORTLIBC_W_CTYPE_H)) || \
  (__W == 'w' && !defined(_W_UPORTLIBC_W_CTYPE_H)))
#if __W == 'c'
#define _C_UPORTLIBC_W_CTYPE_H
#endif
#if __W == 'w'
#define _W_UPORTLIBC_W_CTYPE_H
#endif

#undef __W_UNDEF
#include <uportlibc/w_name.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __W == 'w'
typedef __WINT_TYPE__ wint_t;
#endif

int __W_NAME(is, alnum)(__W_INT c);
int __W_NAME(is, alpha)(__W_INT c);
int __W_NAME(is, cntrl)(__W_INT c);
int __W_NAME(is, digit)(__W_INT c);
int __W_NAME(is, graph)(__W_INT c);
int __W_NAME(is, lower)(__W_INT c);
int __W_NAME(is, print)(__W_INT c);
int __W_NAME(is, punct)(__W_INT c);
int __W_NAME(is, space)(__W_INT c);
int __W_NAME(is, upper)(__W_INT c);
int __W_NAME(is, xdigit)(__W_INT c);
__W_INT __W_NAME(to, lower)(__W_INT c);
__W_INT __W_NAME(to, upper)(__W_INT c);

#ifdef __cplusplus
}
#endif

#define __W_UNDEF
#include <uportlibc/w_name.h>

#endif
#endif
