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
#ifndef _WCTYPE_TABLE_H
#define _WCTYPE_TABLE_H

#include <stddef.h>

#define WCTYPE_TABLE_FLAG_ALPHA         (1 << 0)
#define WCTYPE_TABLE_FLAG_CNTRL         (1 << 1)
#define WCTYPE_TABLE_FLAG_LOWER_N       (1 << 2)
#define WCTYPE_TABLE_FLAG_PRINT         (1 << 3)
#define WCTYPE_TABLE_FLAG_PUNCT         (1 << 4)
#define WCTYPE_TABLE_FLAG_SPACE         (1 << 5)
#define WCTYPE_TABLE_FLAG_UPPER_N       (1 << 6)
#define WCTYPE_TABLE_FLAG_LOWER_UPPER_1 (1 << 7)
#define WCTYPE_TABLE_FLAG_UPPER_LOWER_1 (1 << 8)

struct wctype_table_entry
{
  unsigned min_c;
  unsigned max_c;
  unsigned short flags;
  short offset;
};

extern struct wctype_table_entry *__uportlibc_wctype_table;
extern size_t __uportlibc_wctype_table_length;

#endif
