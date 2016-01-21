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
#include <uportlibc/w_ctype.h>
#define __W W
#include <uportlibc/w_name.h>
#if __W == 'w'
#include "wctype_table.h"
#endif

#if __W == 'w'
static struct wctype_table_entry *find_wctype_table_entry(wint_t c)
{
  size_t l, r;
  wint_t i = c >> 16;
  if(i < 0 || i >= __uportlibc_wctype_table_index_pair_count) return NULL;
  l = __uportlibc_wctype_table_index_pairs[i].first_entry_idx;
  r = __uportlibc_wctype_table_index_pairs[i].last_entry_idx;
  while(1) {
    size_t m = (l + r) >> 1;
    if(c < __uportlibc_wctype_table[m].min_c) {
      if(l >= r) break;
      r = m - 1;
    } else if(c > __uportlibc_wctype_table[m].max_c) {
      if(l >= r) break;
      l = m + 1;
    } else {
      return &(__uportlibc_wctype_table[m]);
    }
  }
  return NULL;
}
#endif

int __W_NAME(is, alnum)(__W_INT c)
{ return __W_NAME(is, alpha)(c) || __W_NAME(is, digit)(c); }

int __W_NAME(is, alpha)(__W_INT c)
{
  if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) return (entry->flags & WCTYPE_TABLE_FLAG_ALPHA) != 0;
  }
#endif
  return 0;
}

int __W_NAME(is, cntrl)(__W_INT c)
{
  if((c >= 0 && c <= 0x1f) || c == 0x7f) return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) return (entry->flags & WCTYPE_TABLE_FLAG_CNTRL) != 0;
  }
#endif
  return 0;
}

int __W_NAME(is, digit)(__W_INT c)
{ return c >= '0' && c <= '9'; }

int __W_NAME(is, graph)(__W_INT c)
{ return __W_NAME(is, print)(c) && !__W_NAME(is, space)(c); }

int __W_NAME(is, lower)(__W_INT c)
{
  if(c >= 'a' && c <= 'z') return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) {
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_N) != 0) return 1;
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_UPPER_1) != 0) return (c & 1) == 0;
      if((entry->flags & WCTYPE_TABLE_FLAG_UPPER_LOWER_1) != 0) return (c & 1) == 1;
    }
  }
#endif
  return 0;
}

int __W_NAME(is, print)(__W_INT c)
{
  if(c >= ' ' && c <= '~') return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) return (entry->flags & WCTYPE_TABLE_FLAG_PRINT) != 0;
  }
#endif
  return 0;
}

int __W_NAME(is, punct)(__W_INT c)
{
  if((c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) return (entry->flags & WCTYPE_TABLE_FLAG_PUNCT) != 0;
  }
#endif
  return 0;  
}

int __W_NAME(is, space)(__W_INT c)
{
  if((c >= '\t' && c <= '\r') || c == ' ') return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) return (entry->flags & WCTYPE_TABLE_FLAG_SPACE) != 0;
  }
#endif
  return 0;  
}

int __W_NAME(is, upper)(__W_INT c)
{
  if(c >= 'A' && c <= 'Z') return 1;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) {
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_N) != 0) return 1;
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_UPPER_1) != 0) return (c & 1) == 1;
      if((entry->flags & WCTYPE_TABLE_FLAG_UPPER_LOWER_1) != 0) return (c & 1) == 0;
    }
  }
#endif
  return 0;
}

int __W_NAME(is, xdigit)(__W_INT c)
{ return (c >= '0' && c <= '9') || ((c & ~0x20) >= 'A' && (c & ~0x20) <= 'F'); }

__W_INT __W_NAME(to, lower)(__W_INT c)
{
  if(c >= 'A' && c <= 'Z') return c | 0x20;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) {
      if((entry->flags & WCTYPE_TABLE_FLAG_UPPER_N) != 0) return c + entry->offset;
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_UPPER_1) != 0 && (c & 1) == 1) return c + 1;
      if((entry->flags & WCTYPE_TABLE_FLAG_UPPER_LOWER_1) != 0 && (c & 1) == 0) return c + 1;
    }
  }
#endif
  return c;
}

__W_INT __W_NAME(to, upper)(__W_INT c)
{
  if(c >= 'a' && c <= 'z') return c & ~0x20;
#if __W == 'w'
  if(c >= 0x80) {
    struct wctype_table_entry *entry = find_wctype_table_entry(c);
    if(entry != NULL) {
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_N) != 0) return c + entry->offset;
      if((entry->flags & WCTYPE_TABLE_FLAG_LOWER_UPPER_1) != 0 && (c & 1) == 0) return c - 1;
      if((entry->flags & WCTYPE_TABLE_FLAG_UPPER_LOWER_1) != 0 && (c & 1) == 1) return c - 1;
    }
  }
#endif
  return c;
}
