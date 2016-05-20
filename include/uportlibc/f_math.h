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
#ifdef __F
#if ( \
  (__F == 'f' && !defined(_F_UPORTLIBC_F_MATH_H)) || \
  (__F == 'd' && !defined(_D_UPORTLIBC_F_MATH_H)) || \
  (__F == 'l' && !defined(_L_UPORTLIBC_F_MATH_H)))
#if __F == 'f'
#define _F_UPORTLIBC_F_MATH_H
#endif
#if __F == 'd'
#define _D_UPORTLIBC_F_MATH_H
#endif
#if __F == 'l'
#define _L_UPORTLIBC_F_MATH_H
#endif

#undef __F_UNDEF
#include <uportlibc/f_name.h>

#ifdef __cplusplus
extern "C" {
#endif

__F_DOUBLE __F_NAME(acos, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(acosh, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(asin, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(asinh, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(atan, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(atanh, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(atan2, )(__F_DOUBLE y, __F_DOUBLE x);
__F_DOUBLE __F_NAME(cbrt, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(ceil, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(cos, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(cosh, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(exp, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(fabs, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(floor, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(fmod, )(__F_DOUBLE x, __F_DOUBLE y);
__F_DOUBLE __F_NAME(frexp, )(__F_DOUBLE x, int *exp);
int __F_NAME(isnan, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(ldexp, )(__F_DOUBLE x, int exp);
__F_DOUBLE __F_NAME(log, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(log10, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(logb, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(modf, )(__F_DOUBLE x, __F_DOUBLE *i_ptr);
__F_DOUBLE __F_NAME(nextafter, )(__F_DOUBLE x, __F_DOUBLE y);
__F_DOUBLE __F_NAME(pow, )(__F_DOUBLE x, __F_DOUBLE y);
__F_DOUBLE __F_NAME(remainder, )(__F_DOUBLE x, __F_DOUBLE y);
__F_DOUBLE __F_NAME(rint, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(scalb, )(__F_DOUBLE x, __F_DOUBLE exp);
__F_DOUBLE __F_NAME(sin, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(sinh, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(sqrt, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(tan, )(__F_DOUBLE x);
__F_DOUBLE __F_NAME(tanh, )(__F_DOUBLE x);

#ifdef __cplusplus
}
#endif

#define __F_UNDEF
#include <uportlibc/f_name.h>

#else
#undef __F
#endif
#endif
