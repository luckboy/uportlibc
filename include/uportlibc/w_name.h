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
#ifndef __W_UNDEF
#ifdef __W
#if __W == 'c'
#define __W_EOF                             (-1)
#define __W_CHAR                            char
#define __W_UCHAR                           unsigned char
#define __W_CHAR_INT                        int
#define __W_INT                             int
#define __W_CHAR_PTR                        char *
#define __W_UCHAR_PTR                       unsigned char *
#define __W_VOID_PTR                        void *
#define __W_CONST_CHAR_PTR                  const char *
#define __W_CONST_UCHAR_PTR                 const unsigned char *
#define __W_CONST_VOID_PTR                  const void *
#define __W_NAME(prefix, suffix)            prefix##suffix
#define __W_STR_NAME(suffix)                str##suffix
#define __W_MEM_NAME(suffix)                mem##suffix
#define __W_UPORTLIBC_NAME(prefix, suffix)  __uportlibc_##prefix##suffix
#define __W_STRTOK_R                        strtok_r
#define __W_SNPRINTF                        snprintf
#define __W_VSNPRINTF                       vsnprintf
#else
#if __W == 'w'
#define __W_EOF                             ((wint_t) (-1))
#define __W_CHAR                            wchar_t
#define __W_UCHAR                           wchar_t
#define __W_CHAR_INT                        wchar_t
#define __W_INT                             wint_t
#define __W_CHAR_PTR                        wchar_t *
#define __W_UCHAR_PTR                       wchar_t *
#define __W_VOID_PTR                        wchar_t *
#define __W_CONST_CHAR_PTR                  const wchar_t *
#define __W_CONST_UCHAR_PTR                 const wchar_t *
#define __W_CONST_VOID_PTR                  const wchar_t *
#define __W_NAME(prefix, suffix)            prefix##w##suffix
#define __W_STR_NAME(suffix)                wcs##suffix
#define __W_MEM_NAME(suffix)                wmem##suffix
#define __W_UPORTLIBC_NAME(prefix, suffix)  __uportlibc_##prefix##w##suffix
#define __W_STRTOK_R                        wcstok
#define __W_SNPRINTF                        swprintf
#define __W_VSNPRINTF                       vswprintf
#else
#error "Incorrect macro __W."
#endif
#endif
#else
#error "Undefined macro __W."
#endif
#else
#undef __W
#undef __W_UNDEF
#undef __W_EOF
#undef __W_CHAR
#undef __W_UCHAR
#undef __W_CHAR_INT
#undef __W_INT
#undef __W_CHAR_PTR
#undef __W_UCHAR_PTR
#undef __W_VOID_PTR
#undef __W_CONST_CHAR_PTR
#undef __W_CONST_UCHAR_PTR
#undef __W_CONST_VOID_PTR
#undef __W_NAME
#undef __W_STR_NAME
#undef __W_MEM_NAME
#undef __W_UPORTLIBC_NAME
#undef __W_STRTOK_R
#undef __W_SNPRINTF
#undef __W_VSNPRINTF
#endif
