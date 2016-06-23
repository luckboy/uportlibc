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
#define __W W
#include <uportlibc/w_stdio.h>
#include <stdio.h>
#define __W W
#include "w_vxprintf.h"
#define __W W
#include <uportlibc/w_name.h>

static int __W_NAME(vx, printf_put_char)(void *data, __W_CHAR_INT c)
{ return __W_NAME(put, c_unlocked)(c, (FILE *) data) != __W_EOF ? 0 : -1; }

int __W_NAME(, printf)(__W_CONST_CHAR_PTR format, ...)
{
  va_list ap;
  int res;
  va_start(ap, format);
  res = __W_NAME(v, printf)(format, ap);
  va_end(ap);
  return res;
}

int __W_NAME(f, printf)(FILE *stream, __W_CONST_CHAR_PTR format, ...)
{
  va_list ap;
  int res;
  va_start(ap, format);
  res = __W_NAME(vf, printf)(stream, format, ap);
  va_end(ap);
  return res;
}

int __W_NAME(v, printf)(__W_CONST_CHAR_PTR format, va_list ap)
{ return __W_NAME(vf, printf)(stdout, format, ap); }

int __W_NAME(vf, printf)(FILE *stream, __W_CONST_CHAR_PTR format, va_list ap)
{
  struct __W_NAME(vx, printf_stream) tmp_stream;
  int res;
  tmp_stream.put_char = __W_NAME(vx, printf_put_char);
  tmp_stream.data = (void *) stream;
  flockfile(stream);
  res = __W_NAME(__uportlibc_vx, printf)(&tmp_stream, format, ap);
  funlockfile(stream);
  return res;
}
