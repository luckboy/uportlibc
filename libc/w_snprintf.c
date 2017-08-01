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
#ifndef TEST
#define __W W
#include <uportlibc/w_stdio.h>
#define __W W
#include "w_vxprintf.h"
#define __W W
#include <uportlibc/w_name.h>
#else
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_vxprintf.h"
#define __W W
#include "w_uportlibc_name.h"
#endif

struct __W_NAME(vx, printf_data)
{
  __W_CHAR *ptr;
  __W_CHAR *end;
};

static int __W_NAME(vx, printf_put_char)(void *data, __W_CHAR_INT c)
{
  struct __W_NAME(vx, printf_data) *tmp_data = (struct __W_NAME(vx, printf_data) *) data;
  if(tmp_data->ptr == tmp_data->end) return 0;
  *(tmp_data->ptr) = c;
  tmp_data->ptr++;
  return 0;
}

int __W_SNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, ...)
{
  va_list ap;
  int res;
  va_start(ap, format);
  res = __W_VSNPRINTF(str, count, format, ap);
  va_end(ap);
  return res;
}

int __W_VSNPRINTF(__W_CHAR_PTR str, size_t count, __W_CONST_CHAR_PTR format, va_list ap)
{
  struct __W_NAME(vx, printf_stream) stream;
  struct __W_NAME(vx, printf_data) data;
  int res;
  data.ptr = str;
  data.end = (count != 0 ? str + (count - 1) : str);
  stream.put_char = __W_NAME(vx, printf_put_char);
  stream.data = &data;
  res = __W_NAME(__uportlibc_vx, printf)(&stream, format, ap);
  if(count != 0) *((__W_CHAR_PTR) (data.ptr)) = 0;
  return res;
}
