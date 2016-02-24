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
#ifdef __W
#if ( \
  (__W == 'c' && !defined(_C_UPORTLIBC_W_STRING_H)) || \
  (__W == 'w' && !defined(_W_UPORTLIBC_W_STRING_H)))
#if __W == 'c'
#define _C_UPORTLIBC_W_STRING_H
#endif
#if __W == 'w'
#define _W_UPORTLIBC_W_STRING_H
#endif

#include <stddef.h>
#undef __W_UNDEF
#include <uportlibc/w_name.h>

#ifdef __cplusplus
extern "C" {
#endif

__W_VOID_PTR __W_MEM_NAME(chr)(__W_CONST_VOID_PTR str, __W_CHAR_INT c, size_t count);
int __W_MEM_NAME(cmp)(__W_CONST_VOID_PTR str1, __W_CONST_VOID_PTR str2, size_t count);
__W_VOID_PTR __W_MEM_NAME(cpy)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count);
__W_VOID_PTR __W_MEM_NAME(move)(__W_VOID_PTR dst, __W_CONST_VOID_PTR src, size_t count);
__W_VOID_PTR __W_MEM_NAME(set)(__W_VOID_PTR str, __W_CHAR_INT c, size_t count);
__W_CHAR_PTR __W_STR_NAME(cat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STR_NAME(chr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c);
int __W_STR_NAME(cmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
int __W_STR_NAME(coll)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STR_NAME(cpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src);
size_t __W_STR_NAME(cspn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR reject);
size_t __W_STR_NAME(len)(__W_CONST_CHAR_PTR str);
__W_CHAR_PTR __W_STR_NAME(ncat)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);
int __W_STR_NAME(ncmp)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);
__W_CHAR_PTR __W_STR_NAME(ncpy)(__W_CHAR_PTR dst, __W_CONST_CHAR_PTR src, size_t count);
__W_CHAR_PTR __W_STR_NAME(pbrk)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept);
__W_CHAR_PTR __W_STR_NAME(rchr)(__W_CONST_CHAR_PTR str, __W_CHAR_INT c);
size_t __W_STR_NAME(spn)(__W_CONST_CHAR_PTR str, __W_CONST_CHAR_PTR accept);
__W_CHAR_PTR __W_STR_NAME(str)(__W_CONST_CHAR_PTR str1, __W_CONST_CHAR_PTR str2);
__W_CHAR_PTR __W_STRTOK_R(__W_CHAR_PTR str, __W_CONST_CHAR_PTR delim, __W_CHAR_PTR *ptr_ptr);
size_t __W_STR_NAME(xfrm)(__W_CHAR_PTR str1, __W_CONST_CHAR_PTR str2, size_t count);

#ifdef __cplusplus
}
#endif

#define __W_UNDEF
#include <uportlibc/w_name.h>

#endif
#endif
