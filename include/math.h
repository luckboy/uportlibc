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
#ifndef _MATH_H
#define _MATH_H

#define __F 'f'
#include <uportlibc/f_math.h>
#undef _UPORTLIBC_F_MATH_H
#define __F 'd'
#include <uportlibc/f_math.h>
#undef _UPORTLIBC_F_MATH_H
#define __F 'l'
#include <uportlibc/f_math.h>
#undef _UPORTLIBC_F_MATH_H

#define M_1_PI                  0.31830988618379067154
#define M_2_PI                  0.63661977236758134308
#define M_2_SQRT                1.12837916709551257390
#define M_E                     2.7182818284590452354
#define M_LN2                   0.69314718055994530942     
#define M_LN10                  2.30258509299404568402
#define M_LOG2E                 1.4426950408889634074
#define M_LOG10E                0.43429448190325182765
#define M_PI                    3.14159265358979323846
#define M_PI_2                  1.57079632679489661923
#define M_PI_4                  0.78539816339744830962
#define M_SQRT2                 1.41421356237309504880
#define M_SQRT1_2               0.70710678118654752440

#define MAXFLOAT                3.40282346638528859812e38f

#define HUGE_VALF               (__builtin_inff())        
#define HUGE_VAL                (__builtin_inf())
#define HUGE_VALL               (__builtin_infl())

#endif
