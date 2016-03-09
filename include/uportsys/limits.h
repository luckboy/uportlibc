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
#ifndef _UPORTSYS_LIMITS_H
#define _UPORTSYS_LIMITS_H

#define __UPORTSYS_CHAR_BIT     __CHAR_BIT__
#define __UPORTSYS_WORD_BIT     (__SIZEOF_INT__ * 8)
#define __UPORTSYS_LONG_BIT     (__SIZEOF_LONG__ * 8)

#define __UPORTSYS_SCHAR_MAX    __SCHAR_MAX__
#define __UPORTSYS_SCHAR_MIN    (-__UPORTSYS_SCHAR_MAX - 1)
#define __UPORTSYS_SHRT_MAX     __SHRT_MAX__
#define __UPORTSYS_SHRT_MIN     (-__UPORTSYS_SHRT_MAX - 1)
#define __UPORTSYS_INT_MAX      __INT_MAX__
#define __UPORTSYS_INT_MIN      (-__UPORTSYS_INT_MAX - 1)
#define __UPORTSYS_LONG_MAX     __LONG_MAX__
#define __UPORTSYS_LONG_MIN     (-__UPORTSYS_LONG_MAX - 1L)
#define __UPORTSYS_LLONG_MAX    __LONG_LONG_MAX__
#define __UPORTSYS_LLONG_MIN    (-__UPORTSYS_LLONG_MAX - 1LL)

#define __UPORTSYS_UCHAR_MAX    (__UPORTSYS_SCHAR_MAX * 2U + 1U)
#define __UPORTSYS_USHRT_MAX    (__UPORTSYS_SHRT_MAX * 2U + 1U)
#define __UPORTSYS_UINT_MAX     (__UPORTSYS_INT_MAX * 2U + 1U)
#define __UPORTSYS_ULONG_MAX    (__UPORTSYS_LONG_MAX * 2UL + 1UL)
#define __UPORTSYS_ULLONG_MAX   (__UPORTSYS_LLONG_MAX * 2ULL + 1ULL)

#ifdef __CHAR_UNSIGNED__
#define __UPORTSYS_CHAR_MAX     __UPORTSYS_UCHAR_MAX
#define __UPORTSYS_CHAR_MIN     0U
#else
#define __UPORTSYS_CHAR_MAX     __UPORTSYS_SCHAR_MAX
#define __UPORTSYS_CHAR_MIN     __UPORTSYS_SCHAR_MIN
#endif

#endif
