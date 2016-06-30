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
#include <errno.h>
#include <float.h>
#include <math.h>
#ifndef TEST
#include "conv.h"
#else
#define UPORTLIBC_CONV
#include "uportlibc.h"
#endif
#include "float_priv.h"

long double __uportlibc_log_pow_div_for_conv(long double x, long *exp, int is_hex)
{
  long double base;
  long min_exp;
  if(is_hex) {
    base = 2.0;
    min_exp = LDBL_MIN_HEX_STRTOLD_EXP;
  } else {
    base = 10.0;
    min_exp = LDBL_MIN_DEC_STRTOLD_EXP;
  }
  *exp = floorl(logl(x) / logl(base));
  if(*exp >= min_exp) {
    x /= powl(base, *exp);
  } else {
    x /= powl(base, *exp + 2);
    x *= base * base;
  }
  return x;
}

long double __uportlibc_pow_mul_for_conv(long double x, long exp, long fract_digit_exp, int is_hex)
{
  long double base;
  long max_exp, min_exp;
  int is_overflow, is_underflow;
  if(is_hex) {
    base = 2.0;
    max_exp = LDBL_MAX_HEX_STRTOLD_EXP;
    min_exp = LDBL_MIN_HEX_STRTOLD_EXP;
  } else {
    base = 10.0;
    max_exp = LDBL_MAX_DEC_STRTOLD_EXP;
    min_exp = LDBL_MIN_DEC_STRTOLD_EXP;
  }
  is_overflow = is_underflow = 0;
  if(exp > 0) {
    if(exp <= max_exp) {
      x *= powl(base, exp - 1);
      if(x <= LDBL_MAX / base)
        x *= base;
      else
        is_overflow = 1;
    } else
      is_overflow = 1;
  } else if(exp < 0) {
    if(exp >= min_exp) {
      x *= powl(base, exp + 1);
      if(x >= LDBL_MIN * base)
        x /= base;
      else
        is_underflow = 1;
    } else {
      if(exp + fract_digit_exp >= min_exp) {
        x *= powl(base, -(fract_digit_exp + 1));
        x *= powl(base, exp + fract_digit_exp + 2);
        if(x >= LDBL_MIN * base)
          x /= base;
        else
          is_underflow = 1;
      } else
        is_underflow = 1;
    }
  }
  if(is_overflow) {
    errno = ERANGE;
    return HUGE_VALL;
  }
  if(is_underflow) {
    errno = ERANGE;
    return 0.0;
  }
  return x;
}
