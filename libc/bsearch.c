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
#include <stdlib.h>
#include "array.h"

void *bsearch(const void *key, const void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *))
{
  size_t l, r;
  if(elem_count <= 0) return NULL;
  l = 0;
  r = elem_count - 1;
  while(1) {
    size_t m = (l + r) >> 1;
    const void *elem = const_array_elem(elems, m, elem_size);
    int res = cmp(key, elem);
    if(res == 0) {
      return (void *) elem;
    } else if(res < 0) {
      if(l >= r) break;
      r = m - 1;
    } else {
      if(l >= r) break;
      l = m + 1;
    }
  }
  return NULL;
}
