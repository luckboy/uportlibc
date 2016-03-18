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
#ifndef _UPORTLIBC_FLOAT_LIMITS_H
#define _UPORTLIBC_FLOAT_LIMITS_H

#define FLT_DIG                 6
#define FLT_MAX                 3.40282346638528859812e38f

#define DBL_DIG                 10
#if __SIZEOF_DOUBLE__ == __SIZEOF_FLOAT__
#define DBL_MAX                 3.40282346638528859812e38
#else
#define DBL_MAX                 1.79769313486231570815e308
#endif

#define LDBL_DIG                10
#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_FLOAT__
#define LDBL_MAX                3.40282346638528859812e38L
#else
#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_DOUBLE__
#define LDBL_MAX                1.79769313486231570815e308L
#else
#define LDBL_MAX                1.18973149535723176502e4932L
#endif
#endif

#endif
