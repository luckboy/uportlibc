/*
 * Copyright (c) 2016-2017 Łukasz Szpakowski
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
  (__W == 'c' && !defined(_C_W_UPORTLIBC_H)) || \
  (__W == 'w' && !defined(_W_W_UPORTLIBC_H)))
#if __W == 'c'
#define _C_W_UPORTLIBC_H
#endif
#if __W == 'w'
#define _W_W_UPORTLIBC_H
#endif

#if __W == 'c'
#define __SAVED_W 'c'
#endif
#if __W == 'w'
#define __SAVED_W 'w'
#endif

#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>
#undef __W
#define __W __SAVED_W
#undef __W_UNDEF
#include "w_uportlibc_name.h"

/* Structures, types and macros. */

#ifndef _UPORTLIBC_FILE
#define _UPORTLIBC_FILE

struct uportlibc_FILE;

typedef struct uportlibc_FILE uportlibc_FILE;

#if defined(UPORTLIBC_STDIO) || defined(UPORTLIBC_WCHAR)
#define FILE uportlibc_FILE
#define __uportlibc_FILE uportlibc_FILE
#endif

#endif

/* Variables, functions and macros for the stdio.c file. */

#ifndef UPORTLIBC_EOF
#define UPORTLIBC_EOF           (-1)
#endif
#ifndef UPORTLIBC_WEOF
#define UPORTLIBC_WEOF          ((wint_t) (-1))
#endif

#define UPORTLIBC__IONBF        0
#define UPORTLIBC__IOFBF        1
#define UPORTLIBC__IOLBF        2

extern uportlibc_FILE *uportlibc_stdin;
extern uportlibc_FILE *uportlibc_stdout;

int uportlibc_unsafely_flush_stream(uportlibc_FILE *stream);
int uportlibc_unsafely_prepare_stream_to_read(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_prepare_stream_to_write(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_prepare_stream_to_unread(uportlibc_FILE *stream, int wide_mode);
int uportlibc_unsafely_get_char(uportlibc_FILE *stream);
int uportlibc_unsafely_put_char(int c, uportlibc_FILE *stream);
int uportlibc_unsafely_unget_char(int c, uportlibc_FILE *stream);

#ifdef UPORTLIBC_STDIO
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
#undef stdin
#define stdin uportlibc_stdin
#undef stdout
#define stdout uportlibc_stdout
#define __uportlibc_unsafely_flush_stream uportlibc_unsafely_flush_stream
#define __uportlibc_unsafely_prepare_stream_to_read uportlibc_unsafely_prepare_stream_to_read
#define __uportlibc_unsafely_prepare_stream_to_write uportlibc_unsafely_prepare_stream_to_write
#define __uportlibc_unsafely_prepare_stream_to_unread uportlibc_unsafely_prepare_stream_to_unread
#define __uportlibc_unsafely_get_char uportlibc_unsafely_get_char
#define __uportlibc_unsafely_put_char uportlibc_unsafely_put_char
#define __uportlibc_unsafely_unget_char uportlibc_unsafely_unget_char
#endif

/* Functions for the w_stdio.c file. */

__W_INT __W_NAME(fget, c)(uportlibc_FILE *stream);
__W_CHAR_PTR __W_NAME(fget, s)(__W_CHAR_PTR str, int count, uportlibc_FILE *stream);
__W_CHAR_PTR __W_NAME(fget, s_unlocked)(__W_CHAR_PTR str, int count, uportlibc_FILE *stream);
__W_INT __W_NAME(fput, c)(__W_CHAR_INT c, uportlibc_FILE *stream);
int __W_NAME(fput, s)(__W_CONST_CHAR_PTR str, uportlibc_FILE *stream);
int __W_NAME(fput, s_unlocked)(__W_CONST_CHAR_PTR str, uportlibc_FILE *stream);
__W_INT __W_NAME(get, c)(uportlibc_FILE *stream);
__W_INT __W_NAME(get, c_unlocked)(uportlibc_FILE *stream);
__W_INT __W_NAME(get, char)(void);
__W_INT __W_NAME(get, char_unlocked)(void);
__W_INT __W_NAME(put, c)(__W_CHAR_INT c, uportlibc_FILE *stream);
__W_INT __W_NAME(put, c_unlocked)(__W_CHAR_INT c, uportlibc_FILE *stream);
__W_INT __W_NAME(put, char)(__W_CHAR_INT c);
__W_INT __W_NAME(put, char_unlocked)(__W_CHAR_INT c);
__W_INT __W_NAME(unget, c)(__W_INT c, uportlibc_FILE *stream);
__W_INT __W_NAME(unget, c_unlocked)(__W_INT c, uportlibc_FILE *stream);

/* Functions for the w_stdlib.c file. */

float __W_STR_NAME(tof)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
double __W_STR_NAME(tod)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
long __W_STR_NAME(tol)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
long double __W_STR_NAME(told)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr);
long long __W_STR_NAME(toll)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
unsigned long __W_STR_NAME(toul)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);
unsigned long long __W_STR_NAME(toull)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base);

/* Functions for the w_snprintf.c file. */

int __W_SNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, ...);
int __W_VSNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, va_list ap);

/* Functions for the w_snprintf.c file. */

int __W_NAME(s, scanf)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR format, ...);
int __W_NAME(vs, scanf)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR format, va_list ap);

/* Functions for the w_string.c file. */

__W_VOID_PTR __W_MEM_NAME(chr)(__W_CONST_VOID_PTR str, __W_CHAR_INT c, size_t count);
int __W_MEM_NAME(cmp)(__W_CONST_VOID_PTR str1, __W_CONST_VOID_PTR str2, size_t count);
__W_VOID_PTR __W_MEM_NAME(cpy)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count);
__W_VOID_PTR __W_MEM_NAME(move)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count);
__W_VOID_PTR __W_MEM_NAME(set)(__W_VOID_PTR str, __W_CHAR_INT c, size_t count);
__W_CHAR_PTR __W_STR_NAME(cat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STR_NAME(chr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c);
int __W_STR_NAME(cmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
int __W_STR_NAME(coll)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STR_NAME(cpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src);
size_t __W_STR_NAME(cspn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR reject);
size_t __W_STR_NAME(len)(__W_CONST_CHAR_PTR str);
__W_CHAR_PTR __W_STR_NAME(ncat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);
int __W_STR_NAME(ncmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);
__W_CHAR_PTR __W_STR_NAME(ncpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src, size_t count);
__W_CHAR_PTR __W_STR_NAME(pbrk)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept);
__W_CHAR_PTR __W_STR_NAME(rchr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c);
size_t __W_STR_NAME(spn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept);
__W_CHAR_PTR __W_STR_NAME(str)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STRTOK_R(__W_CHAR_PTR str, __W_CONST_CHAR_PTR delim, __W_CHAR_PTR *ptr_ptr);
size_t __W_STR_NAME(xfrm)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);

#define __W_UNDEF
#include "w_uportlibc_name.h"

#else
#undef __W
#endif
#endif
