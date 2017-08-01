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
#include <stddef.h>
#ifdef TEST
#define UPORTLIBC_ULLTOSTR
#include "uportlibc.h"
#endif

char *__uportlibc_ulltostr(unsigned long long x, int base, char *str, int is_uppercase)
{
  size_t i, len; 
  if(x != 0) {
    char a_c = (is_uppercase ? 'A' : 'a');
    for(i = 0; x != 0; i++) {
      unsigned digit = x % base;
      x /= base;
      str[i] = (digit < 10 ? digit + '0' : digit + a_c - 10);
    }
    len = i;
  } else {
    str[0] = '0';
    len = 1;
  }
  for(i = 0; i < len >> 1; i++) {
    char tmp_c = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp_c;
  }
  str[len] = 0;
  return str;
}
