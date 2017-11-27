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
#define __W W
#include <uportlibc/w_stdio.h>
#include <stdio.h>
#define __W W
#include "w_vxscanf.h"
#define __W W
#include <uportlibc/w_name.h>

__W_INT __W_NAME(__uportlibc_unget, c_unlocked)(__W_CHAR_INT c, FILE *stream);

static __W_INT vxscanf_get_char(void *data)
{ return __W_NAME(get, c_unlocked)((FILE *) data); }

static void vxscanf_unget_char(void *data, __W_CHAR_INT c)
{ __W_NAME(unget, c_unlocked)(c, (FILE *) data); }

int __W_NAME(, scanf)(__W_CONST_CHAR_PTR format, ...)
{
  va_list ap;
  int res;
  va_start(ap, format);
  res = __W_NAME(v, scanf)(format, ap);
  va_end(ap);
  return res;
}

int __W_NAME(f, scanf)(FILE *stream, __W_CONST_CHAR_PTR format, ...)
{
  va_list ap;
  int res;
  va_start(ap, format);
  res = __W_NAME(vf, scanf)(stream, format, ap);
  va_end(ap);
  return res;
}

int __W_NAME(v, scanf)(__W_CONST_CHAR_PTR format, va_list ap)
{ return __W_NAME(vf, scanf)(stdin, format, ap); }

int __W_NAME(vf, scanf)(FILE *stream, __W_CONST_CHAR_PTR format, va_list ap)
{
  struct __W_NAME(vx, scanf_stream) tmp_stream;
  int res;
  tmp_stream.get_char = vxscanf_get_char;
  tmp_stream.unget_char = vxscanf_unget_char;
  tmp_stream.data = (void *) stream;
  tmp_stream.pushed_c_count = 0;
  tmp_stream.has_error = 0;
  flockfile(stream);
  res = __W_UPORTLIBC_NAME(vx, scanf)(&tmp_stream, format, ap);
  funlockfile(stream);
  return res;
}
