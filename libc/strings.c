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
#include <ctype.h>
#include <string.h>
#include <strings.h>

int bcmp(const void *str1, const void *str2, size_t count)
{ return memcmp(str1, str2, count); }

void bcopy(const void *src, void *dst, size_t count)
{ memmove(dst, src, count); }

void bzero(void *str, size_t count)
{ memset(str, 0, count); }

int ffs(int x)
{
  int bit, mask;
  if(x == 0) return 0;
  for(bit = 1, mask = 1; (x & mask) == 0; bit++, mask <<= 1);
  return bit;
}

char *index(const char *str, int c)
{ return strchr(str, c); }

char *rindex(const char *str, int c)
{ return strrchr(str, c); }

int strcasecmp(const char *str1, const char *str2)
{
  for(; 1; str1++, str2++) {
    int c1 = tolower((int) ((unsigned char) (*str1)));
    int diff = c1 - tolower((int) ((unsigned char) (*str2)));
    if(diff != 0 || c1 == 0) return diff;
  }
  return 0;
}

int strncasecmp(const char *str1, const char *str2, size_t count)
{
  const char *end1 = str1 + count;
  for(; str1 != end1; str1++, str2++) {
    int c1 = tolower((int) ((unsigned char) (*str1)));
    int diff = c1 - tolower((int) ((unsigned char) (*str2)));
    if(diff != 0 || c1 == 0) return diff;
  }
  return 0;
}
