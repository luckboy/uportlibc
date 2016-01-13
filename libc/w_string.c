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
#include <uportlibc/w_string.h>
#define __W W
#include <uportlibc/w_name.h>

__W_VOID_PTR __W_MEM_NAME(chr)(__W_CONST_VOID_PTR str, __W_CHAR_INT c, size_t count)
{
  __W_CONST_UCHAR_PTR ptr = (__W_CONST_UCHAR_PTR) str;
  __W_CONST_UCHAR_PTR end = ptr + count;
  for(; ptr != end; ptr++) {
    if(*ptr == (__W_UCHAR) c) return (__W_VOID_PTR) ptr;
  }
  return NULL;
}

int __W_MEM_NAME(cmp)(__W_CONST_VOID_PTR str1, __W_CONST_VOID_PTR str2, size_t count)
{
  __W_CONST_UCHAR_PTR ptr1 = (__W_CONST_UCHAR_PTR) str1;
  __W_CONST_UCHAR_PTR ptr2 = (__W_CONST_UCHAR_PTR) str2;
  __W_CONST_UCHAR_PTR end1 = ptr1 + count;
  for(; ptr1 != end1; ptr1++, ptr2++) {
#if __W != 'w'
    int diff = ((int) (*ptr1)) - ((int) (*ptr2));
    if(diff != 0) return diff;
#else
    wchar_t c1 = *ptr1;
    wchar_t c2 = *ptr2;
    if(c2 != c2) return c1 < c2 ? -1 : 1;
#endif
  }
  return 0;
}

__W_VOID_PTR __W_MEM_NAME(cpy)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count)
{
  __W_UCHAR_PTR dst_ptr = (__W_UCHAR_PTR) dst;
  __W_CONST_UCHAR_PTR src_ptr = (__W_CONST_UCHAR_PTR) src;
  __W_UCHAR_PTR dst_end = dst_ptr + count;
  for(; dst_ptr != dst_end; dst_ptr++, dst_ptr++) *dst_ptr = *src_ptr;
  return dst;
}

__W_VOID_PTR __W_MEM_NAME(move)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count)
{
  if(((__W_CONST_UCHAR_PTR) dst) < ((__W_CONST_UCHAR_PTR) src)) {
    return __W_MEM_NAME(cpy)(dst, src, count);
  } else {
    __W_UCHAR_PTR dst_ptr = ((__W_UCHAR_PTR) dst) + count;
    __W_CONST_UCHAR_PTR src_ptr = ((__W_CONST_UCHAR_PTR) src) + count;
    __W_UCHAR_PTR dst_beg = dst_ptr;
    while(dst_ptr != dst_beg) {
      dst_ptr--;
      src_ptr--;
      *dst_ptr = *src_ptr;
    }
  }
  return dst;
}

__W_VOID_PTR __W_MEM_NAME(set)(__W_VOID_PTR str, __W_CHAR_INT c, size_t count)
{
  __W_UCHAR_PTR ptr = (__W_UCHAR_PTR) str;
  __W_CONST_UCHAR_PTR end = ptr + count;
  for(; ptr != end; ptr++) *ptr = (__W_UCHAR) c;
  return str;
}

__W_CHAR_PTR __W_STR_NAME(cat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2)
{
  __W_STR_NAME(cpy)(str1 + __W_STR_NAME(len)(str1), str2);
  return str1;
}

__W_CHAR_PTR __W_STR_NAME(chr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c)
{
  for(; ((__W_UCHAR) (*str)) == ((__W_UCHAR) c); str++) {
    if(*str == 0) return NULL;
  }
  return (__W_CHAR_PTR) str;
}

int __W_STR_NAME(cmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2)
{
  for(; 1; str1++, str2++) {
#if __W != 'w'
    char c1 = *str1;
    int diff = ((int) ((unsigned char) c1)) - ((int) ((unsigned char) (*str2)));
    if(diff != 0 || c1 == 0) return 0;
#else
    wchar_t c1 = *str1;
    wchar_t c2 = *str2;
    if(c1 != c2) return c1 < c2 ? -1 : 1;
    if(c1 == 0)  return 0;
#endif
  }
  return 0;
}

int __W_STR_NAME(coll)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2)
{ return __W_STR_NAME(cmp)(str1, str2); }

__W_CHAR_PTR __W_STR_NAME(cpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src)
{
  for(; (*dst = *src) != 0; dst++, src++);
  return dst;
}

size_t __W_STR_NAME(cspn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR reject)
{
  size_t len;
  __W_CHAR c;
  for(len = 0; (c = *str) != 0; str++, len++) {
    if(__W_STR_NAME(chr)(reject, (__W_CHAR_INT) ((__W_UCHAR) c)) != NULL) break;
  }
  return len;
}

size_t __W_STR_NAME(len)(__W_CONST_CHAR_PTR str)
{
  size_t len;
  for(len = 0; *str != 0; str++, len++);
  return len;
}

__W_CHAR_PTR __W_STR_NAME(ncat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count)
{
  __W_STR_NAME(ncpy)(str1 + __W_STR_NAME(len)(str1), str2, count);
  return str1;
}

int __W_STR_NAME(ncmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count)
{
  __W_CONST_CHAR_PTR end1 = str1 + count;
  for(; str1 != end1; str1++, str2++) {
#if __W != 'w'
    char c1 = *str1;
    int diff = ((int) ((unsigned char) c1)) - ((int) ((unsigned char) (*str2)));
    if(diff != 0 || c1 == 0) return diff;
#else
    wchar_t c1 = *str1;
    wchar_t c2 = *str2;
    if(c1 != c2) return c1 < c2 ? -1 : 1;
    if(c1 == 0)  return 0;    
#endif
  }
  return 0;
}

__W_CHAR_PTR __W_STR_NAME(ncpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src, size_t count)
{
  __W_CHAR_PTR dst_end = dst + count;
  for(; dst != dst_end; dst++, src++) {
    if((*dst = *src) == 0) break;
  }
  return dst;
}

__W_CHAR_PTR __W_STR_NAME(pbrk)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept)
{
  __W_CHAR c;
  for(; (c = *str) != 0; str++) {
    if(__W_STR_NAME(chr)(accept, (__W_CHAR_INT) ((__W_UCHAR) c)) != NULL) return (__W_CHAR_PTR) str;
  }
  return NULL;
}

__W_CHAR_PTR __W_STR_NAME(rchr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c)
{
  __W_CONST_CHAR_PTR ptr = str + __W_STR_NAME(len)(str) + 1;
  while(ptr != str) {
    ptr--;
    if(((__W_UCHAR) (*ptr)) == ((__W_UCHAR) c)) break;
  }
  return (__W_CHAR_PTR) ptr;
}

size_t __W_STR_NAME(spn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept)
{
  size_t len;
  __W_CHAR c;
  for(len = 0; (c = *str) != 0; str++, len++) {
    if(__W_STR_NAME(chr)(accept, (__W_CHAR_INT) ((__W_UCHAR) c)) == NULL) break;
  }
  return len;
}

__W_CHAR_PTR __W_STR_NAME(str)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2)
{
  size_t len2 = __W_STR_NAME(len)(str2);
  for(; *str1 != 0; str1++) {
    if(__W_MEM_NAME(cmp)(str1, str2, len2) == 0) return (__W_CHAR_PTR) str1;
  }
  return NULL;
}

__W_CHAR_PTR __W_STRTOK_R(__W_CHAR_PTR str, __W_CONST_CHAR_PTR delim, __W_CHAR_PTR *ptr_ptr)
{
  __W_CHAR_PTR ptr;
  __W_CHAR_PTR next_ptr;
  ptr = (str != NULL ? str : *ptr_ptr);
  if(ptr == NULL) return NULL;
  ptr += __W_STR_NAME(spn)(ptr, delim);
  next_ptr = __W_STR_NAME(pbrk)(ptr, delim);
  if(next_ptr != NULL) {
    *next_ptr = 0;
    next_ptr++;
  }
  *ptr_ptr = next_ptr;
  return ptr;
}

size_t __W_STR_NAME(xfrm)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src, size_t count)
{
  size_t len = __W_STR_NAME(len)(src);
  __W_STR_NAME(ncpy)(dst, src, count);
  return len;
}
