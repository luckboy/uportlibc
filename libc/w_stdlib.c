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
#include <uportlibc/w_ctype.h>
#define __W W
#include <uportlibc/w_stdlib.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#define __W W
#include <uportlibc/w_name.h>

float __W_STR_NAME(tof)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr)
{
  long double res = __W_STR_NAME(told)(str, end_ptr);
  if(res < -FLT_MAX) {
    errno = ERANGE;
    return -HUGE_VALF;
  }
  if(res > FLT_MAX) {
    errno = ERANGE;
    return HUGE_VALF;
  }
  return (float) res;
}

double __W_STR_NAME(tod)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr)
{
  long double res = __W_STR_NAME(told)(str, end_ptr);
  if(res < -DBL_MAX) {
    errno = ERANGE;
    return -HUGE_VAL;
  }
  if(res > DBL_MAX) {
    errno = ERANGE;
    return HUGE_VAL;
  }
  return (double) res;
}

long __W_STR_NAME(tol)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base)
{
  long res = __W_STR_NAME(toll)(str, end_ptr, base);
  if(res < LONG_MIN) {
    errno = ERANGE;
    return LONG_MIN;
  }
  if(res > LONG_MAX) {
    errno = ERANGE;
    return LONG_MAX;
  }
  return res;
}

long double __W_STR_NAME(told)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr)
{
  long double res, sign, base, exp_base;
  long exp, max_exp, min_exp;
  int is_hex, is_first, is_dot, is_overflow, is_underflow;
  size_t max_digits, digits, int_digits, significant_digits;
  int saved_errno;
  if(end_ptr != NULL) *end_ptr = (__W_CHAR_PTR) str;
  for(; __W_NAME(is, space)((__W_CHAR_INT) ((__W_UCHAR) (*str))); str++);
  switch(*str) {
  case '+':
    sign = 1.0;
    str++;
    break;
  case '-':
    sign = -1.0;
    str++;
    break;
  }
  if(str[0] == '0' && (str[1] == 'X' || str[1] == 'x')) {
#if FLT_RADIX == 2
    double logr_log2 = 1.0;
#else
    double logr_log2 = log(FLT_RADIX) / log(2.0);
#endif
    base = 16.0;
    exp_base = 2.0;
    max_exp = floor(LDBL_MAX_EXP * logr_log2);
    min_exp = ceil(LDBL_MIN_EXP * logr_log2);
    max_digits = ceil(LDBL_MANT_DIG * logr_log2 / 16.0);
    is_hex = 1;
    str += 2;
  } else {
#if FLT_RADIX == 2
    double logr_log10 = 0.30102999566398119521;
#else
    double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
    base = 10.0;
    exp_base = 10.0;
    max_exp = floor(LDBL_MAX_EXP * logr_log10);
    min_exp = ceil(LDBL_MIN_EXP * logr_log10);
    max_digits = ceil(LDBL_MANT_DIG * logr_log10);
    is_hex = 0;
  }
  res = 0.0;
  is_first = 1;
  is_dot = 0;
  digits = 0;
  significant_digits = 0;
  for(; *str != 0; str++) {
    __W_CHAR_INT c = (__W_CHAR_INT) ((__W_UCHAR) (*str));
    long double digit;
    if(c != '.') {
      if(c >= '0' && c <= '9')
        digit = c - '0';
      else if(is_hex && (c & 0x20) >= 'A' && (c & 0x20) <= 'F')
        digit = (c & 0x20) + 10 - 'A';
      else
        break;
      if(digits <= max_digits + 1) {
        res = res * base + digit;
        if(res >= 1.0) {
          digits++;
          significant_digits++;
        }
      } else
        digits++;
    } else {
      if(is_dot) break;
      is_dot = 1;
      int_digits = digits; 
    }
    is_first = 0;
  }
  if(is_first) return 0;
  if(!is_dot) int_digits = digits;
  saved_errno = errno;
  if(is_hex ? (*str == 'P' || *str == 'p') : (*str == 'E' || *str == 'e')) {
    str++;
    if(*str == '+' || *str == '-' || (*str >= '0' && *str <= '9'))
      exp = __W_STR_NAME(tol)(str, (__W_CHAR_PTR *) (&str), 10);
    else
      exp = 0;
  } else
    exp = 0;
  is_overflow = 0;
  is_underflow = 0;
  if((digits << (is_hex ? 2 : 0)) <= ((unsigned long) LONG_MAX)) {
    if(errno == 0) {
      exp -= ((long) (digits - int_digits)) << (is_hex ? 2 : 0);
      exp += ((long) (digits - significant_digits)) << (is_hex ? 2 : 0);
      if(exp > 0) {
        if(exp <= max_exp) {
          res *= powl(exp_base, exp - 1);
          if(res <= LDBL_MAX / exp_base)
            res *= exp_base;
          else
            is_overflow = 1;
        } else
          is_overflow = 1;
      } else if(exp < 0) {
        res *= powl(exp_base, exp);
        if(exp >= min_exp) {
          res *= powl(exp_base, exp + 1);
          if(res >= LDBL_MIN * exp_base)
            res /= exp_base;
          else
            is_underflow = 1;
        }
      }
    } else {
      if(exp >= LONG_MAX)
        is_overflow = 1;
      else
        is_underflow = 1;
    }
  } else
    is_overflow = 1;
  errno = saved_errno;
  if(end_ptr != NULL) *end_ptr = (__W_CHAR_PTR) str;
  if(is_overflow) {
    errno = ERANGE;
    res = HUGE_VALL;
  } else if(is_underflow) {
    errno = ERANGE;
    return 0.0;
  }
  res *= sign;
  return res;
}

long long __W_STR_NAME(toll)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base)
{
  __W_CONST_CHAR_PTR tmp_str;
  unsigned long long res, tmp_res;
  int is_minus, saved_errno;
  for(tmp_str = str; __W_NAME(is, space)((__W_CHAR_INT) ((__W_UCHAR) (*tmp_str))); tmp_str++);
  is_minus = (*tmp_str == '-');
  saved_errno = errno;
  errno = 0;
  res = __W_STR_NAME(toull)(tmp_str, end_ptr, base);
  if(errno != 0) {
    if(res == ULLONG_MAX)
      return is_minus ? LLONG_MIN : LLONG_MAX;
    else
      return 0;
  }
  errno = saved_errno;
  if(end_ptr != NULL && *end_ptr == tmp_str) *end_ptr = (__W_CHAR_PTR) str;
  if(is_minus) tmp_res = (unsigned long long) (-((long long) res));
  if(is_minus) {
    if(tmp_res > ((unsigned long long) (-(LLONG_MIN + 1LL) + 1ULL))) {
      errno = ERANGE;
      return LLONG_MIN;
    }
  } else {
    if(tmp_res > ((unsigned long long) LLONG_MAX)) {
      errno = ERANGE;
      return LLONG_MAX;
    }
  }
  return (long long) res;
}

unsigned long __W_STR_NAME(toul)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base)
{
  unsigned long long res = __W_STR_NAME(toull)(str, end_ptr, base);
  if(res > ULONG_MAX) {
    errno = ERANGE;
    return ULONG_MAX;
  }
  return res;
}

unsigned long long __W_STR_NAME(toull)(__W_CONST_CHAR_PTR str, __W_CHAR_PTR *end_ptr, int base)
{
  unsigned long long res, max_mul_res;
  int sign, is_first, is_overflow;
  if(base < 2 || base > 36) {
    errno = EINVAL;
    return 0;
  }
  if(end_ptr != NULL) *end_ptr = (__W_CHAR_PTR) str;
  for(; __W_NAME(is, space)((__W_CHAR_INT) ((__W_UCHAR) (*str))); str++);
  switch(*str) {
  case '+':
    sign = 1;
    str++;
    break;
  case '-':
    sign = -1;
    str++;
    break;
  }
  if(base == 0) {
    if(str[0] == '0') {
      if(str[1] == 'X' || str[1] == 'x') {
        base = 16;
        str += 2;
      } else
        base = 8;
    } else
      base = 10;
  } else if(base == 16) {
    if(str[0] == '0' && (str[1] == 'X' || str[1] == 'x')) str += 2;
  }
  res = 0ULL;
  is_first = 1;
  is_overflow = 0;
  max_mul_res = ULLONG_MAX / base;
  for(; *str != 0; str++) {
    __W_CHAR_INT c = (__W_CHAR_INT) ((__W_UCHAR) (*str));
    unsigned digit;
    if(c >= '0' && c <= (base > 10 ? '9' : base - 1 + '0'))
      digit = c - '0';
    else if(base > 10 && (c & 0x20) >= 'A' && (c & 0x20) <= base - 11 + 'A')
      digit = (c & 0x20) - 'A' + 10;
    else
      break;
    if(!is_overflow) {
      if(res <= max_mul_res) {
        res *= base;
        if(res <= ULLONG_MAX - digit)
          res += digit;
        else
          is_overflow = 1;
      } else 
        is_overflow = 1;
    }
    is_first = 0;
  }
  if(is_first) return 0;
  if(end_ptr != NULL) *end_ptr = (__W_CHAR_PTR) str;
  if(is_overflow) {
    errno = ERANGE;
    return ULLONG_MAX;
  }
  res = (unsigned long long) (sign * ((long long) res));
  return res;
}
