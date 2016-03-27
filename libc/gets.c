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
#include <stdio.h>
#include "stdio_priv.h"

char *gets(char *str)
{
  char *res = str;
  lock_lock(&(stdin->lock));
  if((stdin->flags & FILE_FLAG_EOF) == 0) {
    int is_first = 1;
    unsigned saved_error_flag = stdin->flags & FILE_FLAG_ERROR;
    stdin->flags &= ~FILE_FLAG_ERROR;
    while(1) {
      int c = getchar_unlocked();
      if(c == EOF) {
        if((stdin->flags & FILE_FLAG_ERROR) != 0 || is_first) res = NULL;
        break;
      }
      if(c == '\n') break;
      *str = c;
      str++;
      is_first = 0;
    }
    stdin->flags |= saved_error_flag;
  } else
    res = NULL;
  *str = 0;
  lock_unlock(&(stdin->lock));
  return res;
}
