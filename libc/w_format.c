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
#include <uportlibc/w_stdlib.h>
#else
#define __W W
#include "w_uportlibc.h"
#endif
#include <errno.h>
#include <limits.h>
#define __W W
#include "w_format.h"
#ifndef TEST
#define __W W
#include <uportlibc/w_name.h>
#else
#define __W W
#include "w_uportlibc_name.h"
#endif

int __W_UPORTLIBC_NAME(, parse_conv_spec_num)(__W_CONST_CHAR_PTR *format_ptr)
{
  unsigned long res;
  int saved_errno = errno;
  errno = 0;
  res = __W_STR_NAME(toul)(*format_ptr, (__W_CHAR_PTR *) format_ptr, 10);
  if(res == ULONG_MAX && errno != 0) {
    errno = EOVERFLOW;
    return -1;
  }
  if(res > INT_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  errno = saved_errno;
  return res;
}

int __W_UPORTLIBC_NAME(, parse_arg_pos)(__W_CONST_CHAR_PTR *format_ptr, unsigned *curr_arg_idx_ptr, unsigned *arg_count_ptr)
{
  __W_CONST_CHAR_PTR ptr;
  unsigned arg_idx;
  for(ptr = *format_ptr; *ptr >= '0' && *ptr <= '9'; ptr++);
  if(*ptr == '$') {
    __W_CONST_CHAR_PTR format = *format_ptr;
    int res = __W_UPORTLIBC_NAME(, parse_conv_spec_num)(&format);
    if(res == -1) return -1;
    if(res == 0) {
      errno = EINVAL;
      return -1;
    }
    arg_idx = res - 1;
    *arg_count_ptr = (arg_idx + 1 < *arg_count_ptr ? *arg_count_ptr : arg_idx + 1);
    *format_ptr = ptr + 1;
  } else {
    arg_idx = *arg_count_ptr;
    (*arg_count_ptr)++;
    *curr_arg_idx_ptr = arg_idx;
  }
  if(arg_idx >= NL_ARGMAX) {
    errno = EINVAL;
    return -1;
  }
  return (int) arg_idx;
}
