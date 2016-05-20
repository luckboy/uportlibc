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
  (__W == 'c' && !defined(_C_UPORTLIBC_W_STDIO_H)) || \
  (__W == 'w' && !defined(_W_UPORTLIBC_W_STDIO_H)))
#if __W == 'c'
#define _C_UPORTLIBC_W_STDIO_H
#endif
#if __W == 'w'
#define _W_UPORTLIBC_W_STDIO_H
#endif

#include <stddef.h>
#undef __W_UNDEF
#include <uportlibc/w_name.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FILE
#define _FILE

struct __uportlibc_FILE;

typedef struct __uportlibc_FILE FILE;

#endif

#ifndef _VA_LIST
#define _VA_LIST
typedef __builtin_va_list va_list;
#endif

#if __W == 'w'
#ifndef _WINT_T
#define _WINT_T
typedef __WINT_TYPE__ wint_t;
#endif
#endif

int __W_NAME(, printf)(__W_CONST_CHAR_PTR format, ...); 
int __W_NAME(, scanf)(__W_CONST_CHAR_PTR format, ...);
int __W_NAME(f, printf)(FILE *stream, __W_CONST_CHAR_PTR format, ...); 
int __W_NAME(f, scanf)(FILE *stream, __W_CONST_CHAR_PTR format, ...);
__W_INT __W_NAME(fget, c)(FILE *stream);
__W_CHAR_PTR __W_NAME(fget, s)(__W_CHAR_PTR str, int count, FILE *stream);
__W_CHAR_PTR __W_NAME(fget, s_unlocked)(__W_CHAR_PTR str, int count, FILE *stream);
__W_INT __W_NAME(fput, c)(__W_CHAR_INT c, FILE *stream);
int __W_NAME(fput, s)(__W_CONST_CHAR_PTR str, FILE *stream);
int __W_NAME(fput, s_unlocked)(__W_CONST_CHAR_PTR str, FILE *stream);
__W_INT __W_NAME(get, c)(FILE *stream);
__W_INT __W_NAME(get, c_unlocked)(FILE *stream);
__W_INT __W_NAME(get, char)(void);
__W_INT __W_NAME(get, char_unlocked)(void);
__W_INT __W_NAME(put, c)(__W_CHAR_INT c, FILE *stream);
__W_INT __W_NAME(put, c_unlocked)(__W_CHAR_INT c, FILE *stream);
__W_INT __W_NAME(put, char)(__W_CHAR_INT c);
__W_INT __W_NAME(put, char_unlocked)(__W_CHAR_INT c);
int __W_SNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, ...);
int __W_NAME(s, scanf)(__W_CHAR_PTR str, __W_CONST_CHAR_PTR format, ...);
__W_INT __W_NAME(unget, c)(__W_CHAR_INT c, FILE *stream);
int __W_NAME(v, printf)(__W_CONST_CHAR_PTR format, va_list ap);
int __W_NAME(v, scanf)(__W_CONST_CHAR_PTR format, va_list ap);
int __W_NAME(vf, printf)(FILE *stream, __W_CONST_CHAR_PTR format, va_list ap);
int __W_NAME(vf, scanf)(FILE *stream, __W_CONST_CHAR_PTR format, va_list ap);
int __W_VSNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, va_list ap);
int __W_NAME(vs, scanf)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR format, va_list ap);

#ifdef __cplusplus
}
#endif

#define __W_UNDEF
#include <uportlibc/w_name.h>

#else
#undef __W
#endif
#endif
