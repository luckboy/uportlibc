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
  (__W == 'c' && !defined(_C_W_CUNIT_H)) || \
  (__W == 'w' && !defined(_W_W_CUNIT_H)))
#if __W == 'c'
#define _C_W_CUNIT_H
#endif
#if __W == 'w'
#define _W_W_CUNIT_H
#endif

#ifdef __W
#if __W == 'c'

#define __W_CU_ASSERT_STRING_EQUAL(actual, expected)                        \
CU_ASSERT_STRING_EQUAL(actual, expected)

#define __W_CU_ASSERT_STRING_NOT_EQUAL(actual, expected)                    \
CU_ASSERT_STRING_NOT_EQUAL(actual, expected)

#define __W_CU_ASSERT_NSTRING_EQUAL(actual, expected, count)                \
CU_ASSERT_NSTRING_EQUAL(actual, expected, count)

#define __W_CU_ASSERT_NSTRING_NOT_EQUAL(actual, expected, count)            \
CU_ASSERT_NSTRING_NOT_EQUAL(actual, expected, count)

#else
#if __W == 'w'

#include <wchar.h>

#define __W_CU_ASSERT_STRING_EQUAL(actual, expected)                        \
CU_ASSERT(wcscmp(actual, expected) == 0)

#define __W_CU_ASSERT_STRING_NOT_EQUAL(actual, expected)                    \
CU_ASSERT(wcscmp(actual, expected) != 0)

#define __W_CU_ASSERT_NSTRING_EQUAL(actual, expected, count)                \
CU_ASSERT(wcsncmp(actual, expected, count) == 0)

#define __W_CU_ASSERT_NSTRING_NOT_EQUAL(actual, expected, count)            \
CU_ASSERT(wcsncmp(actual, expected, count) != 0)

#else
#error "Incorrect macro __W."
#endif
#endif
#else
#error "Undefined macro __W."
#endif

#undef __W

#else
#undef __W
#endif
#endif
