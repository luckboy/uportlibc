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
  (__W == 'c' && !defined(_C_VXPRINTF_H)) || \
  (__W == 'w' && !defined(_W_VXPRINTF_H)))
#if __W == 'c'
#define _C_VXPRINTF_H
#endif
#if __W == 'w'
#define _W_VXPRINTF_H
#endif

#include <stdarg.h>
#undef __W_UNDEF
#ifndef TEST
#include <uportlibc/w_name.h>
#else
#include "w_uportlibc_name.h"
#endif

struct __W_NAME(vx, printf_stream)
{
  int (*put_char)(void *, __W_CHAR_INT);
  void *data;
};

int __W_UPORTLIBC_NAME(vx, printf)(struct __W_NAME(vx, printf_stream) *stream, __W_CONST_CHAR_PTR format, va_list ap);

#define __W_UNDEF
#ifndef TEST
#include <uportlibc/w_name.h>
#else
#include "w_uportlibc_name.h"
#endif

#endif
#endif
