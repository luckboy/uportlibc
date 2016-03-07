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
#ifndef TEST
#include <stdlib.h>
#else
#define UPORTLIBC_QSORT
#include "uportlibc.h"
#endif
#include "array.h"

#ifndef TEST
#define STATIC                  static
#else
#define STATIC
#endif

static void swap(void *str1, void *str2, size_t count)
{
  unsigned char *ptr1 = (unsigned char *) str1;
  unsigned char *ptr2 = (unsigned char *) str2;
  unsigned char *end1 = ptr1 + count;
  for(; ptr1 != end1; ptr1++, ptr2++) {
    unsigned char tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
  }
}

static void shift_down(void *elems, size_t elem_count, size_t i, size_t elem_size, int (*cmp)(const void *, const void *))
{
  while(1) {
    size_t l = ((i + 1) << 1) - 1;
    size_t r = l + 1;
    size_t greatest = i;
    void *left_elem, *right_elem;
    void *parent_elem = array_elem(elems, i, elem_size);
    void *greatest_elem = parent_elem;
    if(l < elem_count) {
      left_elem = array_elem(elems, l, elem_size);
      if(cmp(left_elem, parent_elem) > 0) {
        greatest_elem = left_elem;
        greatest = l;
      }
    }
    if(r < elem_count) {
      right_elem = array_elem(elems, r, elem_size);
      if(cmp(right_elem, greatest_elem) > 0) {
        greatest_elem = right_elem;
        greatest = r;
      }
    }
    if(i == greatest) return;
    swap(parent_elem, greatest_elem, elem_size);
    i = greatest;
  }
}

static void heapify(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *))
{
  size_t i = elem_count >> 1;
  while(i != 0) {
    i--;
    shift_down(elems, elem_count, i, elem_size, cmp);
  }
}

STATIC void heapsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *))
{
  heapify(elems, elem_count, elem_size, cmp);
  for(; elem_count != 0; elem_count--) {
    swap(array_elem(elems, 0, elem_size), array_elem(elems, elem_count - 1, elem_size), elem_size);
    shift_down(elems, elem_count - 1, 0, elem_size, cmp);
  }
}

STATIC void introsort(void *elems, size_t l, size_t r, size_t elem_size, int (*cmp)(const void *, const void *), unsigned depth)
{
  if(l >= r) return;
  if(depth == 0) {
    heapsort(array_elem(elems, l, elem_size), r - l + 1, elem_size, cmp);
  } else {
    void *elem = array_elem(elems, l, elem_size);
    size_t i = l, j = r;
    while(1) {
      while(cmp(elem, array_elem(elems, i, elem_size)) > 0) i++;
      while(cmp(elem, array_elem(elems, j, elem_size)) < 0) j--;
      if(i >= j) break;
      if(i != j) swap(array_elem(elems, i, elem_size), array_elem(elems, j, elem_size), elem_size);
      i++;
      j--;
    }
    introsort(elems, l, j, elem_size, cmp, depth - 1);
    introsort(elems, j + 1, r, elem_size, cmp, depth - 1);
  }
}

void qsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *))
{
  unsigned depth, bit;
  size_t mask;
  if(elem_count == 0) return;
  depth = 0;
  for(bit = 0, mask = 1; bit < sizeof(size_t) * 8; bit++, mask <<= 1) {
    if((elem_count & mask) != 0) depth = bit;
  }
  depth <<= 1;
  return introsort(elems, 0, elem_count - 1, elem_size, cmp, depth);
}
