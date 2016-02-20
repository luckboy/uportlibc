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
#ifndef _FLOAT_H
#define _FLOAT_H

#include <uportsys/float_limits.h>

#define FLT_RADIX               2
#define FLT_ROUNDS              1

#define FLT_EPSILON             1.19209289550781250000e-7f
#define FLT_MANT_DIG            24
#define FLT_MAX_10_EXP          38
#define FLT_MAX_EXP             128
#define FLT_MIN                 1.17549435082228750797e-38f
#define FLT_MIN_10_EXP          (-37)
#define FLT_MIN_EXP             (-125)

#if __SIZEOF_DOUBLE__ == __SIZEOF_FLOAT__
#define FLT_EPSILON             1.19209289550781250000e-7
#define DBL_MANT_DIG            FLT_MANT_DIG
#define DBL_MAX_10_EXP          FLT_MAX_10_EXP
#define DBL_MAX_EXP             FLT_MAX_EXP
#define FLT_MIN                 1.17549435082228750797e-38
#define DBL_MIN_10_EXP          FLT_MIN_10_EXP
#define DBL_MIN_EXP             FLT_MIN_EXP
#else
#define DBL_EPSILON             2.22044604925031308085e-16
#define DBL_MANT_DIG            53
#define DBL_MAX_10_EXP          304
#define DBL_MAX_EXP             1024
#define DBL_MIN                 2.22507385850720138309e-308
#define DBL_MIN_10_EXP          (-307)
#define DBL_MIN_EXP             (-1021)
#endif

#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_FLOAT__
#define LDBL_EPSILON            1.19209289550781250000e-7L
#define LDBL_MANT_DIG           FLT_MANT_DIG
#define LDBL_MAX_10_EXP         FLT_MAX_10_EXP
#define LDBL_MAX_EXP            FLT_MAX_EXP
#define LDBL_MIN                1.17549435082228750797e-38fL
#define LDBL_MIN_10_EXP         FLT_MIN_10_EXP
#define LDBL_MIN_EXP            FLT_MIN_EXP
#else
#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_DOUBLE__
#define LDBL_EPSILON            2.22044604925031308085e-16L
#define LDBL_MANT_DIG           DBL_MANT_DIG
#define LDBL_MAX_10_EXP         DBL_MAX_10_EXP
#define LDBL_MAX_EXP            DBL_MAX_EXP
#define LDBL_MIN                2.22507385850720138309e-308L
#define LDBL_MIN_10_EXP         DBL_MIN_10_EXP
#define LDBL_MIN_EXP            DBL_MIN_EXP
#else
#define LDBL_EPSILON            1.08420217248550443401e-19L
#define LDBL_MANT_DIG           64
#define LDBL_MAX_10_EXP         4932
#define LDBL_MAX_EXP            16384
#define LDBL_MIN                3.36210314311209350626e-4932L
#define LDBL_MIN_10_EXP         (-4931)
#define LDBL_MIN_EXP            (-16381)
#endif
#endif

#endif
