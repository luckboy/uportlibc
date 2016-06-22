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
  (__W == 'c' && !defined(_C_VXSCANF_H)) || \
  (__W == 'w' && !defined(_W_VXSCANF_H)))
#if __W == 'c'
#define _C_VXSCANF_H
#endif
#if __W == 'w'
#define _W_VXSCANF_H
#endif

#include <stdarg.h>
#undef __W_UNDEF
#include <uportlibc/w_name.h>

struct __W_NAME(vx, scanf_stream)
{
  __W_INT (*get_char)(void *data);
  void (*unget_char)(void *data, __W_CHAR_INT c);
  void *data;
  __W_CHAR pushed_cs[2];
  unsigned pushed_c_count;
  int has_error;
};

int __W_NAME(__uportlibc_vx, scanf)(struct __W_NAME(vx, scanf_stream) *stream, const __W_CHAR_PTR format, va_list ap);

#define __W_UNDEF
#include <uportlibc/w_name.h>

#endif
#endif
