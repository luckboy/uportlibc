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
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#define __W W
#include "w_cunit.h"
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_uportlibc_name.h"
#include "w_test_name.h"

#define FLOAT_EPS               FLT_EPSILON
#define DOUBLE_EPS              DBL_EPSILON
#define LONG_DOUBLE_EPS         DBL_EPSILON

#define CU_ASSERT_LONG_DOUBLE_EQUAL(actual, expected, granularity)          \
CU_ASSERT(fabsl(actual - expected) <= fabsl(granularity))

int __W_INITIALIZE_NAME(stdlib)(void) { return 0; }

int __W_CLEANUP_NAME(stdlib)(void) { return 0; }

void __W_TEST_STR_NAME(tod, _returns_number)(void)
{
  __W_CHAR_PTR str1 = __W_L("123.456");
  __W_CHAR_PTR end1;
  double res1;
  __W_CHAR_PTR str2 = __W_L("-987.654");
  __W_CHAR_PTR end2;
  double res2;
  errno = 0;
  res1 = __W_STR_NAME(tod)(str1, &end1);
  CU_ASSERT_DOUBLE_EQUAL(res1, 123.456, DOUBLE_EPS * 100.0);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(tod)(str2, &end2);
  CU_ASSERT_DOUBLE_EQUAL(res2, -987.654, DOUBLE_EPS * 100.0);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(tod, _returns_huge_val_and_sets_errno_for_overflow)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  double res;
#if __W == 'c'
  sprintf(str, "1e%d", (int) (ceil(DBL_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"1e%d", (int) (ceil(DBL_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(tod)(str, &end);
  CU_ASSERT_EQUAL(res, HUGE_VAL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(tod, _returns_minus_huge_val_and_sets_errno_for_overflow)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  double res;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) (ceil(DBL_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) (ceil(DBL_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(tod)(str, &end);
  CU_ASSERT_EQUAL(res, -HUGE_VAL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(tof, _returns_number)(void)
{
  __W_CHAR_PTR str1 = __W_L("123.456");
  __W_CHAR_PTR end1;
  double res1;
  __W_CHAR_PTR str2 = __W_L("-987.654");
  __W_CHAR_PTR end2;
  double res2;
  errno = 0;
  res1 = __W_STR_NAME(tof)(str1, &end1);
  CU_ASSERT_DOUBLE_EQUAL(res1, 123.456, FLOAT_EPS * 100.0);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(tof)(str2, &end2);
  CU_ASSERT_DOUBLE_EQUAL(res2, -987.654, FLOAT_EPS * 100.0);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(tof, _returns_huge_val_and_sets_errno_for_overflow)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  float res;
#if __W == 'c'
  sprintf(str, "1e%d", (int) (ceil(FLT_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"1e%d", (int) (ceil(FLT_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(tof)(str, &end);
  CU_ASSERT_EQUAL(res, HUGE_VALF);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(tof, _returns_minus_huge_val_and_sets_errno_for_overflow)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  float res;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) (ceil(FLT_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) (ceil(FLT_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(tof)(str, &end);
  CU_ASSERT_EQUAL(res, -HUGE_VALF);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(tol, _returns_number)(void)
{
  __W_CHAR_PTR str1 = __W_L("469456");
  __W_CHAR_PTR end1;
  long res1;
  __W_CHAR_PTR str2 = __W_L("-1096854");
  __W_CHAR_PTR end2;
  long res2;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 469456L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, -1096854L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(tol, _returns_long_max_and_sets_errno_for_overflow)(void)
{
  __W_CHAR str[1000];
  __W_CHAR_PTR end;
  long res;
#if __W == 'c'
  sprintf(str, "%ld0", LONG_MAX);
#else
  swprintf(str, 1000, L"%ld0", LONG_MAX);
#endif
  errno = 0;
  res = __W_STR_NAME(tol)(str, &end, 10);
  CU_ASSERT_EQUAL(res, LONG_MAX);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(tol, _returns_long_min_and_sets_errno_for_overflow)(void)
{
  __W_CHAR str[1000];
  __W_CHAR_PTR end;
  long res;
#if __W == 'c'
  sprintf(str, "%ld0", LONG_MIN);
#else
  swprintf(str, 1000, L"%ld0", LONG_MIN);
#endif
  errno = 0;
  res = __W_STR_NAME(tol)(str, &end, 10);
  CU_ASSERT_EQUAL(res, LONG_MIN);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(told, _returns_number_for_decimal_system)(void)
{
  __W_CHAR_PTR str1 = __W_L("123.4567");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("987045.123");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("368365");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("5670");
  __W_CHAR_PTR end4;
  long double res4;
  __W_CHAR_PTR str5 = __W_L("0.0234");
  __W_CHAR_PTR end5;
  long double res5;
  __W_CHAR_PTR str6 = __W_L("0.001055");
  __W_CHAR_PTR end6;
  long double res6;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 123.4567L, LONG_DOUBLE_EPS * 100.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 987045.123L, LONG_DOUBLE_EPS * 100000.0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res3, 368365L, LONG_DOUBLE_EPS * 100000.0L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res4, 5670L, LONG_DOUBLE_EPS * 1000.0L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(told)(str5, &end5);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res5, 0.0234L, LONG_DOUBLE_EPS * 0.01L);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(told)(str6, &end6);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res6, 0.001055L, LONG_DOUBLE_EPS * 0.001L);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_with_exponent_for_decimal_system)(void)
{
  __W_CHAR_PTR str1 = __W_L("2.45678e20");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("1023405e+15");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("5.66780127e-10");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("6E32");
  __W_CHAR_PTR end4;
  long double res4;
  __W_CHAR_PTR str5 = __W_L("0.2346e-10");
  __W_CHAR_PTR end5;
  long double res5;
  __W_CHAR_PTR str6 = __W_L("0.00016e+19");
  __W_CHAR_PTR end6;
  long double res6;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 2.45678e20L, LONG_DOUBLE_EPS);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 1023405e15L, LONG_DOUBLE_EPS * 1000000e15L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 11);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res3, 5.66780127e-10L, LONG_DOUBLE_EPS * 1e-10L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 14);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res4, 6e32L, LONG_DOUBLE_EPS * 1e32L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(told)(str5, &end5);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res5, 0.2346e-10L, LONG_DOUBLE_EPS * 0.1e-10L);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(told)(str6, &end6);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res6, 0.00016e19L, LONG_DOUBLE_EPS * 0.0001e19L);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 11);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_hexadecimal_system)(void)
{
  __W_CHAR_PTR str1 = __W_L("0x2a1f.5ac");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0XA1fCa.1Bd");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("0x1f01904A");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("0xabcdef");
  __W_CHAR_PTR end4;
  long double res4;
  __W_CHAR_PTR str5 = __W_L("0x0.abcd");
  __W_CHAR_PTR end5;
  long double res5;
  __W_CHAR_PTR str6 = __W_L("0x0.0005A3B");
  __W_CHAR_PTR end6;
  long double res6;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 0x2a1f.5acp0L, LONG_DOUBLE_EPS * 0x1000.0p0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0xa1fca.1bdp0L, LONG_DOUBLE_EPS * 0x10000.0p0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 11);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res3, 0x1f01904A.0p0L, LONG_DOUBLE_EPS * 0x10000000.0p0L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res4, 0xabcdef.0p0L, LONG_DOUBLE_EPS * 0x10000.0p0L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(told)(str5, &end5);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res5, 0x0.abcdp0L, LONG_DOUBLE_EPS * 0x0.1p0L);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(told)(str6, &end6);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res6, 0x0.0005a3bp0L, LONG_DOUBLE_EPS * 0x0.0001p0L);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 11);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_with_exponent_for_hexadecimal_system)(void)
{
  __W_CHAR_PTR str1 = __W_L("0x24a.bdef2p50");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0XABCDEFp+6");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("0x90402p-20");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("0X1B2P35");
  __W_CHAR_PTR end4;
  long double res4;
  __W_CHAR_PTR str5 = __W_L("0x0.0B1B2P18");
  __W_CHAR_PTR end5;
  long double res5;
  __W_CHAR_PTR str6 = __W_L("0x0.124678p-26");
  __W_CHAR_PTR end6;
  long double res6;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 0x24a.bdef2p50L, LONG_DOUBLE_EPS * 0x100.0p50);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 14);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0xabcdefp6L, LONG_DOUBLE_EPS * 0x100000.0p6L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 11);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res3, 0x90402p-20L, LONG_DOUBLE_EPS * 0x10000.0p-20L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 11);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res4, 0x1b2p35L, LONG_DOUBLE_EPS * 0x100e35L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(told)(str5, &end5);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res5, 0x0.0b1b2p18L, LONG_DOUBLE_EPS * 0x0.01p18L);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 12);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(told)(str6, &end6);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res6, 0x0.124678p-26L, LONG_DOUBLE_EPS * 0x0.1p-26L);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 14);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_zero_for_zero_string)(void)
{
  __W_CHAR_PTR str1 = __W_L("0");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0e15");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("0x0");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("0x0p20");
  __W_CHAR_PTR end4;
  long double res4;
  __W_CHAR_PTR str5 = __W_L("0e-10");
  __W_CHAR_PTR end5;
  long double res5;
  __W_CHAR_PTR str6 = __W_L("0x0p-15");
  __W_CHAR_PTR end6;
  long double res6;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_EQUAL(res1, 0.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_EQUAL(res2, 0.0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_EQUAL(res3, 0.0L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_EQUAL(res4, 0.0L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 6);
  CU_ASSERT_EQUAL(errno, 0);  
  errno = 0;
  res5 = __W_STR_NAME(told)(str5, &end5);
  CU_ASSERT_EQUAL(res5, 0.0L);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 5);
  CU_ASSERT_EQUAL(errno, 0);  
  errno = 0;
  res6 = __W_STR_NAME(told)(str6, &end6);
  CU_ASSERT_EQUAL(res6, 0.0L);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 7);
  CU_ASSERT_EQUAL(errno, 0);  
}

void __W_TEST_STR_NAME(told, _returns_ldbl_max_for_ldbl_max_string)(void)
{
#if FLT_RADIX == 2
  double logr_log2 = 1.0;
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log2 = log(FLT_RADIX) / log(2.0);
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR fmt1[1000], str1[1000];
  static __W_CHAR fmt2[1000], str2[1000];
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR end2;
  long double res2;
#if __W == 'c'
  sprintf(fmt1, "%%.%dLe", (int) (ceil(LDBL_MANT_DIG * logr_log10) + 1.0));
  sprintf(str1, fmt1, LDBL_MAX);
  sprintf(fmt2, "%%.%dLa", (int) (ceil(LDBL_MANT_DIG * logr_log2 / 4.0)));
  sprintf(str2, fmt2, LDBL_MAX);
#else
  swprintf(fmt1, 1000, L"%%.%dLe", (int) (ceil(LDBL_MANT_DIG * logr_log10) + 1.0));
  swprintf(str1, 1000, fmt1, LDBL_MAX);
  swprintf(fmt2, 1000, L"%%.%dLa", (int) (ceil(LDBL_MANT_DIG * logr_log2 / 4.0)));
  swprintf(str2, 1000, fmt2, LDBL_MAX);
#endif
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, LDBL_MAX, (LONG_DOUBLE_EPS * powl(10.0, floor(LDBL_MAX_EXP * logr_log10) - 1.0)) * 10.0);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end1, str1 + strlen(str1));
#else
  CU_ASSERT_PTR_EQUAL(end1, str1 + wcslen(str1));
#endif
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, LDBL_MAX, (LONG_DOUBLE_EPS * powl(2.0, floor(LDBL_MAX_EXP * logr_log2) - 1.0)) * 2);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end2, str2 + strlen(str2));
#else
  CU_ASSERT_PTR_EQUAL(end2, str2 + wcslen(str2));
#endif
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_ldbl_min_for_ldbl_min_string)(void)
{
#if FLT_RADIX == 2
  double logr_log2 = 1.0;
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log2 = log(FLT_RADIX) / log(2.0);
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR fmt1[1000], str1[1000];
  static __W_CHAR fmt2[1000], str2[1000];
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR end2;
  long double res2;
#if __W == 'c'
  sprintf(fmt1, "%%.%dLe", (int) (ceil(LDBL_MANT_DIG * logr_log10) + 1.0));
  sprintf(str1, fmt1, LDBL_MIN);
  sprintf(fmt2, "%%.%dLa", (int) (ceil(LDBL_MANT_DIG * logr_log2 / 4.0)));
  sprintf(str2, fmt2, LDBL_MIN);
#else
  swprintf(fmt1, 1000, L"%%.%dLe", (int) (ceil(LDBL_MANT_DIG * logr_log10) + 1.0));
  swprintf(str1, 1000, fmt1, LDBL_MIN);
  swprintf(fmt2, 1000, L"%%.%dLa", (int) (ceil(LDBL_MANT_DIG * logr_log2 / 4.0)));
  swprintf(str2, 1000, fmt2, LDBL_MIN);
#endif
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, LDBL_MIN, LDBL_MIN);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end1, str1 + strlen(str1));
#else
  CU_ASSERT_PTR_EQUAL(end1, str1 + wcslen(str1));
#endif
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, LDBL_MIN, LDBL_MIN);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end2, str2 + strlen(str2));
#else
  CU_ASSERT_PTR_EQUAL(end2, str2 + wcslen(str2));
#endif
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_huge_vall_and_sets_errno_for_overflow)(void)
{
#if FLT_RADIX == 2
  double logr_log2 = 1.0;
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log2 = log(FLT_RADIX) / log(2.0);
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str1[1000];
  static __W_CHAR str2[1000];
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("1e1000000");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("0x1p1000000");
  __W_CHAR_PTR end4;
  long double res4;
#if __W == 'c'
  sprintf(str1, "1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
  sprintf(str2, "0x1p%d", (int) (ceil(LDBL_MAX_EXP * logr_log2) + 1.0));
#else
  swprintf(str1, 1000, L"1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
  swprintf(str2, 1000, L"0x1p%d", (int) (ceil(LDBL_MAX_EXP * logr_log2) + 1.0));
#endif
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_EQUAL(res1, HUGE_VALL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end1, str1 + strlen(str1));
#else
  CU_ASSERT_PTR_EQUAL(end1, str1 + wcslen(str1));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_EQUAL(res2, HUGE_VALL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end2, str2 + strlen(str2));
#else
  CU_ASSERT_PTR_EQUAL(end2, str2 + wcslen(str2));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_EQUAL(res3, HUGE_VALL);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 9);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_EQUAL(res4, HUGE_VALL);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 11);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(told, _returns_zero_and_sets_errno_for_underflow)(void)
{
#if FLT_RADIX == 2
  double logr_log2 = 1.0;
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log2 = log(FLT_RADIX) / log(2.0);
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str1[1000];
  static __W_CHAR str2[1000];
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("1e-1000000");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("0x1p-1000000");
  __W_CHAR_PTR end4;
  long double res4;
#if __W == 'c'
  sprintf(str1, "1e%d", (int) (ceil((LDBL_MIN_EXP - 1.0) * logr_log10) - 1.0));
  sprintf(str2, "0x1p%d", (int) (ceil((LDBL_MIN_EXP - 1.0) * logr_log2) - 1.0));
#else
  swprintf(str1, 1000, L"1e%d", (int) (ceil((LDBL_MIN_EXP - 1.0) * logr_log10) - 1.0));
  swprintf(str2, 1000, L"0x1p%d", (int) (ceil((LDBL_MIN_EXP - 1.0) * logr_log2) - 1.0));
#endif
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_EQUAL(res1, 0.0);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end1, str1 + strlen(str1));
#else
  CU_ASSERT_PTR_EQUAL(end1, str1 + wcslen(str1));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_EQUAL(res2, 0.0);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end2, str2 + strlen(str2));
#else
  CU_ASSERT_PTR_EQUAL(end2, str2 + wcslen(str2));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_EQUAL(res3, 0.0L);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 10);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_EQUAL(res4, 0.0L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 12);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(told, _sets_end_pointer_for_incorrect_character_in_significand_part)(void)
{
  __W_CHAR_PTR str1 = __W_L("156.78&9e10");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0x1e4a5$%6p20");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 156.78L, LONG_DOUBLE_EPS * 100.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0x1e4a5.0p0L, LONG_DOUBLE_EPS * 0x10000.0p0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 7);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _sets_end_pointer_for_incorrect_character_in_exponent_part)(void)
{
  __W_CHAR_PTR str1 = __W_L("2.506e2&5");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0xabcdp6*#2");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 2.506e2L, LONG_DOUBLE_EPS * 1.0e2);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0xabcdp6L, LONG_DOUBLE_EPS * 0x1000.0p6);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_string_with_spaces)(void)
{
  __W_CHAR_PTR str1 = __W_L(" \r\n 2.678");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L(" 46578");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 2.678L, LONG_DOUBLE_EPS);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 9);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 46578.0L, LONG_DOUBLE_EPS * 10000.0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_string_with_sign)(void)
{
  __W_CHAR_PTR str1 = __W_L("+876.245");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("-124678");
  __W_CHAR_PTR end2;
  long double res2;
  __W_CHAR_PTR str3 = __W_L("-1.23578");
  __W_CHAR_PTR end3;
  long double res3;
  __W_CHAR_PTR str4 = __W_L("-0xfff45abc");
  __W_CHAR_PTR end4;
  long double res4;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, +876.245L, LONG_DOUBLE_EPS * 100.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, -124678.0L, LONG_DOUBLE_EPS * 100000.0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res3, -1.23578L, LONG_DOUBLE_EPS);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res4, -0xfff45abc.0p0L, LONG_DOUBLE_EPS * 0x10000000.0p0L);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 11);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_string_with_spaces_and_sign)(void)
{
  __W_CHAR_PTR str1 = __W_L(" \t +1.2");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L(" -243");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 1.2, LONG_DOUBLE_EPS);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, -243.0L, LONG_DOUBLE_EPS * 100L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 5);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_huge_vall_and_sets_errno_for_overflow_and_string_with_sign)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  long double res;
#if __W == 'c'
  sprintf(str, "+1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"+1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(told)(str, &end);
  CU_ASSERT_EQUAL(res, HUGE_VALL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(told, _returns_minus_huge_vall_and_sets_errno_for_overflow_and_string_with_sign)(void)
{
#if FLT_RADIX == 2
  double logr_log10 = 0.30102999566398119521;
#else
  double logr_log10 = log(FLT_RADIX) / log(10.0);
#endif
  static __W_CHAR str[1000];
  __W_CHAR_PTR end;
  long double res;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) (ceil(LDBL_MAX_EXP * logr_log10) + 1.0));
#endif
  errno = 0;
  res = __W_STR_NAME(told)(str, &end);
  CU_ASSERT_EQUAL(res, -HUGE_VALL);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end, str + strlen(str));
#else
  CU_ASSERT_PTR_EQUAL(end, str + wcslen(str));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(told, _returns_zero_for_string_without_digits)(void)
{
  __W_CHAR_PTR str1 = __W_L("");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \r\t ");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L(" !@#$%^&*()");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("&$&%");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_EQUAL(res1, 0.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_EQUAL(res2, 0.0L);
  CU_ASSERT_PTR_EQUAL(end2, str2);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(told)(str3, &end3);
  CU_ASSERT_EQUAL(res3, 0.0L);
  CU_ASSERT_PTR_EQUAL(end3, str3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(told)(str4, &end4);
  CU_ASSERT_EQUAL(res4, 0.0L);
  CU_ASSERT_PTR_EQUAL(end4, str4);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_end_pointer_that_is_null_pointer)(void)
{
  __W_CHAR_PTR str1 = __W_L("123.678");
  long double res1;
  __W_CHAR_PTR str2 = __W_L(" \t 1.23e10");
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, NULL);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 123.678L, LONG_DOUBLE_EPS * 100.0L);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, NULL);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 1.23e10L, LONG_DOUBLE_EPS * 1.0e10L);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_first_dot_character_in_significand_part)(void)
{
  __W_CHAR_PTR str1 = __W_L(".12346");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0x.abcde");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 0.12346L, LONG_DOUBLE_EPS * 0.1L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0x.abcdep0L, LONG_DOUBLE_EPS * 0x0.1p0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(told, _returns_number_for_last_dot_character_in_significand_part)(void)
{
  __W_CHAR_PTR str1 = __W_L("12346.e10");
  __W_CHAR_PTR end1;
  long double res1;
  __W_CHAR_PTR str2 = __W_L("0xabcde.p20");
  __W_CHAR_PTR end2;
  long double res2;
  errno = 0;
  res1 = __W_STR_NAME(told)(str1, &end1);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res1, 12346.0e10L, LONG_DOUBLE_EPS * 100000.0L);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 9);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(told)(str2, &end2);
  CU_ASSERT_LONG_DOUBLE_EQUAL(res2, 0xabcde.0p20L, LONG_DOUBLE_EPS * 0x10000.0p0L);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 11);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toll, _returns_number)(void)
{
  __W_CHAR_PTR str1 = __W_L("1234567890");
  __W_CHAR_PTR end1;
  long long res1;
  __W_CHAR_PTR str2 = __W_L("23456789");
  __W_CHAR_PTR end2;
  long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 1234567890LL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 23456789LL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toll, _returns_llong_max_for_llong_max_string)(void)
{
  __W_CHAR_PTR str = __W_L("9223372036854775807");
  __W_CHAR_PTR end;
  long long res;
  errno = 0;
  res = __W_STR_NAME(toll)(str, &end, 10);
  CU_ASSERT_EQUAL(res, LLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end, str + 19);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toll, _returns_llong_min_for_llong_min_string)(void)
{
  __W_CHAR_PTR str = __W_L("-9223372036854775808");
  __W_CHAR_PTR end;
  long long res;
  errno = 0;
  res = __W_STR_NAME(toll)(str, &end, 10);
  CU_ASSERT_EQUAL(res, LLONG_MIN);
  CU_ASSERT_PTR_EQUAL(end, str + 20);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toll, _returns_llong_max_and_sets_errno_for_overflow)(void)
{
  __W_CHAR_PTR str1 = __W_L("9223372036854775808");
  __W_CHAR_PTR end1;
  long long res1;
  __W_CHAR_PTR str2 = __W_L("18446744073709551616");
  __W_CHAR_PTR end2;
  long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, LLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 19);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, LLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 20);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(toll, _returns_llong_min_and_sets_errno_for_overflow)(void)
{
  __W_CHAR_PTR str1 = __W_L("-9223372036854775809");
  __W_CHAR_PTR end1;
  long long res1;
  __W_CHAR_PTR str2 = __W_L("-18446744073709551616");
  __W_CHAR_PTR end2;
  long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, LLONG_MIN);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 20);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, LLONG_MIN);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 21);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(toll, _returns_zero_for_string_without_digits)(void)
{
  __W_CHAR_PTR str1 = __W_L("");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \t\n ");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L(" )(*&^%$#@!");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("^|^");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 0ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 0ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toll)(str3, &end3, 10);
  CU_ASSERT_EQUAL(res3, 0ULL);
  CU_ASSERT_PTR_EQUAL(end3, str3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toll)(str4, &end4, 10);
  CU_ASSERT_EQUAL(res4, 0ULL);
  CU_ASSERT_PTR_EQUAL(end4, str4);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toll, _returns_number_for_end_pointer_that_is_null_pointer)(void)
{
  __W_CHAR_PTR str1 = __W_L("123456");
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \t 6789");
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toll)(str1, NULL, 10);
  CU_ASSERT_EQUAL(res1, 123456ULL);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toll)(str2, NULL, 10);
  CU_ASSERT_EQUAL(res2, 6789ULL);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toul, _returns_number)(void)
{
  __W_CHAR_PTR str1 = __W_L("24680");
  __W_CHAR_PTR end1;
  unsigned long res1;
  __W_CHAR_PTR str2 = __W_L("3789756");
  __W_CHAR_PTR end2;
  unsigned long res2;
  errno = 0;
  res1 = __W_STR_NAME(toul)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 24680UL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toul)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 3789756UL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 7);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toul, _returns_ulong_max_and_sets_errno_for_overflow)(void)
{
  __W_CHAR str1[1000];
  __W_CHAR str2[1000];
  __W_CHAR_PTR end1;
  unsigned long res1;
  __W_CHAR_PTR end2;
  unsigned long res2;
#if __W == 'c'
  sprintf(str1, "%lu0", ULONG_MAX);
  sprintf(str2, "-%lu0", ULONG_MAX);
#else
  swprintf(str1, 1000, L"%lu0", ULONG_MAX);
  swprintf(str2, 1000, L"-%lu0", ULONG_MAX);
#endif
  errno = 0;
  res1 = __W_STR_NAME(toul)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, ULONG_MAX);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end1, str1 + strlen(str1));
#else
  CU_ASSERT_PTR_EQUAL(end1, str1 + wcslen(str1));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(toul)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, ULONG_MAX);
#if __W == 'c'
  CU_ASSERT_PTR_EQUAL(end2, str2 + strlen(str2));
#else
  CU_ASSERT_PTR_EQUAL(end2, str2 + wcslen(str2));
#endif
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(toul, _returns_number_for_end_pointer_that_is_null_pointer)(void)
{
  __W_CHAR_PTR str1 = __W_L("15789");
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \t 78902");
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toul)(str1, NULL, 10);
  CU_ASSERT_EQUAL(res1, 15789L);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toul)(str2, NULL, 10);
  CU_ASSERT_EQUAL(res2, 78902L);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_10)(void)
{
  __W_CHAR_PTR str1 = __W_L("12345");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("9075341532");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 12345ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 9075341532ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_2)(void)
{
  __W_CHAR_PTR str1 = __W_L("10100110");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("0010110101010100");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 2);
  CU_ASSERT_EQUAL(res1, 0xa6ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 2);
  CU_ASSERT_EQUAL(res2, 0x2d54ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 16);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_8)(void)
{
  __W_CHAR_PTR str1 = __W_L("74673241");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("0234056012");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 8);
  CU_ASSERT_EQUAL(res1, 074673241ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 8);
  CU_ASSERT_EQUAL(res2, 0234056012ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_16)(void)
{
  __W_CHAR_PTR str1 = __W_L("1ada2fe");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("febca4d901236785");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("0x123456789a");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("0xECaDe");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  __W_CHAR_PTR str5 = __W_L("F1BE56");
  __W_CHAR_PTR end5;
  unsigned long long res5;
  __W_CHAR_PTR str6 = __W_L("0X235FEA");
  __W_CHAR_PTR end6;
  unsigned long long res6;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 16);
  CU_ASSERT_EQUAL(res1, 0x1ada2feULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 16);
  CU_ASSERT_EQUAL(res2, 0xfebca4d901236785ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 16);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 16);
  CU_ASSERT_EQUAL(res3, 0x123456789aULL);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 12);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 16);
  CU_ASSERT_EQUAL(res4, 0xecadeULL);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(toull)(str5, &end5, 16);
  CU_ASSERT_EQUAL(res5, 0xf1be56ULL);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(toull)(str6, &end6, 16);
  CU_ASSERT_EQUAL(res6, 0x235feaULL);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_11)(void)
{
  __W_CHAR_PTR str1 = __W_L("12A4");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("90a54A");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 11);
  CU_ASSERT_EQUAL(res1, ((1 * 11ULL + 2) * 11ULL + 10) * 11ULL + 4);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 11);
  CU_ASSERT_EQUAL(res2, ((((9 * 11ULL + 0) * 11ULL + 10) * 11ULL + 5) * 11ULL + 4) * 11ULL + 10);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_36)(void)
{
  __W_CHAR_PTR str1 = __W_L("1abc2h3iz");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("Z6c7b8A9");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 36);
  CU_ASSERT_EQUAL(res1, (((((((1 * 36ULL + 10) * 36ULL + 11) * 36ULL + 12) * 36ULL + 2) * 36ULL + 17) * 36ULL + 3) *36ULL + 18) * 36ULL + 35);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 9);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 36);
  CU_ASSERT_EQUAL(res2, ((((((35 * 36ULL + 6) * 36ULL + 12) * 36ULL + 7) * 36ULL + 11) * 36ULL + 8) * 36ULL + 10) * 36ULL + 9);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_zero)(void)
{
  __W_CHAR_PTR str1 = __W_L("123456");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("0751056");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("0xfbcd");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("0X10aA24BC");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 0);
  CU_ASSERT_EQUAL(res1, 123456ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 0);
  CU_ASSERT_EQUAL(res2, 0751056ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 7);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 0);
  CU_ASSERT_EQUAL(res3, 0xfbcdULL);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 0);
  CU_ASSERT_EQUAL(res4, 0x10aa24bcULL);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_zero_for_zero_string)(void)
{
  __W_CHAR_PTR str1 = __W_L("0");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("0");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("000");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("0000");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  __W_CHAR_PTR str5 = __W_L("0x000");
  __W_CHAR_PTR end5;
  unsigned long long res5;
  __W_CHAR_PTR str6 = __W_L("0X0");
  __W_CHAR_PTR end6;
  unsigned long long res6;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 0ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 2);
  CU_ASSERT_EQUAL(res2, 0ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 8);
  CU_ASSERT_EQUAL(res3, 0ULL);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 11);
  CU_ASSERT_EQUAL(res4, 0ULL);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(toull)(str5, &end5, 16);
  CU_ASSERT_EQUAL(res5, 0ULL);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(toull)(str6, &end6, 16);
  CU_ASSERT_EQUAL(res6, 0ULL);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 3);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_ullong_max_for_ullong_max_string)(void)
{
  __W_CHAR_PTR str1 = __W_L("18446744073709551615");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("1111111111111111111111111111111111111111111111111111111111111111");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("1777777777777777777777");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("ffffffffffffffff");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  __W_CHAR_PTR str5 = __W_L("335500516a429071284");
  __W_CHAR_PTR end5;
  unsigned long long res5;
  __W_CHAR_PTR str6 = __W_L("3w5e11264sgsf");
  __W_CHAR_PTR end6;
  unsigned long long res6;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 20);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 2);
  CU_ASSERT_EQUAL(res2, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 64);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 8);
  CU_ASSERT_EQUAL(res3, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 22);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 16);
  CU_ASSERT_EQUAL(res4, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 16);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(toull)(str5, &end5, 11);
  CU_ASSERT_EQUAL(res5, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 19);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(toull)(str6, &end6, 36);
  CU_ASSERT_EQUAL(res6, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 13);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_ullong_max_and_sets_errno_for_overflow)(void)
{
  __W_CHAR_PTR str1 = __W_L("18446744073709551616");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("10000000000000000000000000000000000000000000000000000000000000000");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("2000000000000000000000");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("10000000000000000");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  __W_CHAR_PTR str5 = __W_L("335500516a429071285");
  __W_CHAR_PTR end5;
  unsigned long long res5;
  __W_CHAR_PTR str6 = __W_L("3w5e11264sgsg");
  __W_CHAR_PTR end6;
  unsigned long long res6;
  __W_CHAR_PTR str7 = __W_L("123456789012345678901234567890");
  __W_CHAR_PTR end7;
  unsigned long long res7;
  __W_CHAR_PTR str8 = __W_L("24567fecde5bcdefa12567");
  __W_CHAR_PTR end8;
  unsigned long long res8;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 20);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 2);
  CU_ASSERT_EQUAL(res2, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 65);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 8);
  CU_ASSERT_EQUAL(res3, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 22);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 16);
  CU_ASSERT_EQUAL(res4, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 17);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res5 = __W_STR_NAME(toull)(str5, &end5, 11);
  CU_ASSERT_EQUAL(res5, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end5, str5 + 19);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res6 = __W_STR_NAME(toull)(str6, &end6, 36);
  CU_ASSERT_EQUAL(res6, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end6, str6 + 13);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res7 = __W_STR_NAME(toull)(str7, &end7, 10);
  CU_ASSERT_EQUAL(res7, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end7, str7 + 30);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res8 = __W_STR_NAME(toull)(str8, &end8, 16);
  CU_ASSERT_EQUAL(res8, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end8, str8 + 22);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(toull, _returns_zero_and_sets_errno_for_incorrect_base)(void)
{
  __W_CHAR_PTR str = __W_L("12345");
  __W_CHAR_PTR end1;
  __W_CHAR_PTR end2;
  __W_CHAR_PTR end3;
  __W_CHAR_PTR end4;
  unsigned long long res1, res2, res3, res4;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str, &end1, -1);
  CU_ASSERT_EQUAL(res1, 0ULL);
  CU_ASSERT_PTR_EQUAL(end1, str);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str, &end2, 1);
  CU_ASSERT_EQUAL(res2, 0ULL);
  CU_ASSERT_PTR_EQUAL(end2, str);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str, &end3, 37);
  CU_ASSERT_EQUAL(res3, 0ULL);
  CU_ASSERT_PTR_EQUAL(end3, str);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str, &end4, -2);
  CU_ASSERT_EQUAL(res4, 0ULL);
  CU_ASSERT_PTR_EQUAL(end4, str);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

void __W_TEST_STR_NAME(toull, _sets_end_pointer_for_digit_that_is_too_great)(void)
{
  __W_CHAR_PTR str1 = __W_L("167824");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("101020");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("0febceg12345");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("145A6H7cf1276");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  __W_CHAR_PTR str5 = __W_L("210101");
  __W_CHAR_PTR end5;
  unsigned long long res5;
  __W_CHAR_PTR str6 = __W_L("8562");
  __W_CHAR_PTR end6;
  unsigned long long res6;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 8);
  CU_ASSERT_EQUAL(res1, 0167ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 2);
  CU_ASSERT_EQUAL(res2, 0xaULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 16);
  CU_ASSERT_EQUAL(res3, 0x0febceULL);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 6);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 16);
  CU_ASSERT_EQUAL(res4, 0x145a6ULL);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res5 = __W_STR_NAME(toull)(str5, &end5, 2);
  CU_ASSERT_EQUAL(res5, 0ULL);
  CU_ASSERT_PTR_EQUAL(end5, str5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res6 = __W_STR_NAME(toull)(str6, &end6, 8);
  CU_ASSERT_EQUAL(res6, 0ULL);
  CU_ASSERT_PTR_EQUAL(end6, str6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _sets_end_pointer_for_incorrect_character)(void)
{
  __W_CHAR_PTR str1 = __W_L("12345*12");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("fcef$145");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 12345ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 16);
  CU_ASSERT_EQUAL(res2, 0xfcefULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 4);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_string_with_spaces)(void)
{
  __W_CHAR_PTR str1 = __W_L(" \t\n 4679");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" 56771");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 4679ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 8);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 56771ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_string_with_sign)(void)
{
  __W_CHAR_PTR str1 = __W_L("+123456789");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("-8fba12e4");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L("-18446744073709551615");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("+0xffffffffffffffff");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 123456789ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 16);
  CU_ASSERT_EQUAL(res2, 0ULL - 0x8fba12e4ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 9);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 10);
  CU_ASSERT_EQUAL(res3, 0ULL - ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end3, str3 + 21);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 0);
  CU_ASSERT_EQUAL(res4, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end4, str4 + 19);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_string_with_spaces_and_sign)(void)
{
  __W_CHAR_PTR str1 = __W_L(" \t\r\n +12367");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" -345656");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 12367ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 11);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 0LL - 345656ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 8);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_ullong_max_and_sets_errno_for_overflow_and_string_with_sign)(void)
{
  __W_CHAR_PTR str1 = __W_L("+100000000000000000000000");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L("-100000000000000000000000");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end1, str1 + 25);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, ULLONG_MAX);
  CU_ASSERT_PTR_EQUAL(end2, str2 + 25);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_STR_NAME(toull, _returns_zero_for_string_without_digits)(void)
{
  __W_CHAR_PTR str1 = __W_L("");
  __W_CHAR_PTR end1;
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \t ");
  __W_CHAR_PTR end2;
  unsigned long long res2;
  __W_CHAR_PTR str3 = __W_L(" !@#$%^&*()");
  __W_CHAR_PTR end3;
  unsigned long long res3;
  __W_CHAR_PTR str4 = __W_L("|&|");
  __W_CHAR_PTR end4;
  unsigned long long res4;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, &end1, 10);
  CU_ASSERT_EQUAL(res1, 0ULL);
  CU_ASSERT_PTR_EQUAL(end1, str1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, &end2, 10);
  CU_ASSERT_EQUAL(res2, 0ULL);
  CU_ASSERT_PTR_EQUAL(end2, str2);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res3 = __W_STR_NAME(toull)(str3, &end3, 10);
  CU_ASSERT_EQUAL(res3, 0ULL);
  CU_ASSERT_PTR_EQUAL(end3, str3);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res4 = __W_STR_NAME(toull)(str4, &end4, 10);
  CU_ASSERT_EQUAL(res4, 0ULL);
  CU_ASSERT_PTR_EQUAL(end4, str4);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_STR_NAME(toull, _returns_number_for_end_pointer_that_is_null_pointer)(void)
{
  __W_CHAR_PTR str1 = __W_L("123456");
  unsigned long long res1;
  __W_CHAR_PTR str2 = __W_L(" \t 6789");
  unsigned long long res2;
  errno = 0;
  res1 = __W_STR_NAME(toull)(str1, NULL, 10);
  CU_ASSERT_EQUAL(res1, 123456ULL);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  res2 = __W_STR_NAME(toull)(str2, NULL, 10);
  CU_ASSERT_EQUAL(res2, 6789ULL);
  CU_ASSERT_EQUAL(errno, 0);
}

int __W_ADD_SUITE_NAME(stdlib)(void)
{
  CU_pSuite suite;
  suite = CU_add_suite(__W_SUITE_STRING(stdlib), __W_INITIALIZE_NAME(stdlib), __W_CLEANUP_NAME(stdlib));
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tod, " returns number"),
    __W_TEST_STR_NAME(tod, _returns_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tod, " returns HUGE_VAL and sets errno for overflow"),
    __W_TEST_STR_NAME(tod, _returns_huge_val_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tod, " returns -HUGE_VAL and sets errno for overflow"),
    __W_TEST_STR_NAME(tod, _returns_minus_huge_val_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tof, " returns number"),
    __W_TEST_STR_NAME(tof, _returns_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tof, " returns HUGE_VALF and sets errno for overflow"),
    __W_TEST_STR_NAME(tof, _returns_huge_val_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tof, " returns -HUGE_VALF and sets errno for overflow"),
    __W_TEST_STR_NAME(tof, _returns_minus_huge_val_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tol, " returns number"),
    __W_TEST_STR_NAME(tol, _returns_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tol, " returns LONG_MAX and sets errno for overflow"),
    __W_TEST_STR_NAME(tol, _returns_long_max_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(tol, " returns LONG_MIN and sets errno for overflow"),
    __W_TEST_STR_NAME(tol, _returns_long_min_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for decimal system"),
    __W_TEST_STR_NAME(told, _returns_number_for_decimal_system)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number with exponent for decimal system"),
    __W_TEST_STR_NAME(told, _returns_number_with_exponent_for_decimal_system)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for hexadecimal system"),
    __W_TEST_STR_NAME(told, _returns_number_for_hexadecimal_system)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number with exponent for hexadecimal system"),
    __W_TEST_STR_NAME(told, _returns_number_with_exponent_for_hexadecimal_system)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns zero for zero string"),
    __W_TEST_STR_NAME(told, _returns_zero_for_zero_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns LDBL_MAX for LDBL_MAX string"),
    __W_TEST_STR_NAME(told, _returns_ldbl_max_for_ldbl_max_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns LDBL_MIN for LDBL_MIN string"),
    __W_TEST_STR_NAME(told, _returns_ldbl_min_for_ldbl_min_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns HUGE_VALL and sets errno for overflow"),
    __W_TEST_STR_NAME(told, _returns_huge_vall_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns zero for underflow"),
    __W_TEST_STR_NAME(told, _returns_zero_and_sets_errno_for_underflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " sets end pointer for incorrect character in significand part"),
    __W_TEST_STR_NAME(told, _sets_end_pointer_for_incorrect_character_in_significand_part)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " sets end pointer for incorrect character in exponent part"),
    __W_TEST_STR_NAME(told, _sets_end_pointer_for_incorrect_character_in_exponent_part)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for string with spaces"),
    __W_TEST_STR_NAME(told, _returns_number_for_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for string with spaces and sign"),
    __W_TEST_STR_NAME(told, _returns_number_for_string_with_spaces_and_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for string with sign"),
    __W_TEST_STR_NAME(told, _returns_number_for_string_with_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns HUGE_VALL and sets errno for overflow and string with sign"),
    __W_TEST_STR_NAME(told, _returns_huge_vall_and_sets_errno_for_overflow_and_string_with_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns -HUGE_VALL and sets errno for overflow and string with sign"),
    __W_TEST_STR_NAME(told, _returns_minus_huge_vall_and_sets_errno_for_overflow_and_string_with_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns zero for string without digits"),
    __W_TEST_STR_NAME(told, _returns_zero_for_string_without_digits)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for end pointer that is null pointer"),
    __W_TEST_STR_NAME(told, _returns_number_for_end_pointer_that_is_null_pointer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for first dot character in significand part"),
    __W_TEST_STR_NAME(told, _returns_number_for_first_dot_character_in_significand_part)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(told, " returns number for last dot character in significand part"),
    __W_TEST_STR_NAME(told, _returns_number_for_last_dot_character_in_significand_part)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns number"),
    __W_TEST_STR_NAME(toll, _returns_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns LLONG_MAX for LLONG_MAX string"),
    __W_TEST_STR_NAME(toll, _returns_llong_max_for_llong_max_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns LLONG_MIN for LLONG_MIN string"),
    __W_TEST_STR_NAME(toll, _returns_llong_min_for_llong_min_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns LLONG_MAX and sets errno for overflow"),
    __W_TEST_STR_NAME(toll, _returns_llong_max_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns LLONG_MIN and sets errno for overflow"),
    __W_TEST_STR_NAME(toll, _returns_llong_min_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns zero for string without digits"),
    __W_TEST_STR_NAME(toll, _returns_zero_for_string_without_digits)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toll, " returns number for end pointer that is null pointer"),
    __W_TEST_STR_NAME(toll, _returns_number_for_end_pointer_that_is_null_pointer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toul, " returns number"),
    __W_TEST_STR_NAME(toul, _returns_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toul, " returns ULONG_MAX and sets errno for overflow"),
    __W_TEST_STR_NAME(toul, _returns_ulong_max_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toul, " returns number for end pointer that is null pointer"),
    __W_TEST_STR_NAME(toul, _returns_number_for_end_pointer_that_is_null_pointer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 10"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_10)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 2"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_2)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 8"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_8)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 16"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_16)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 11"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_11)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is 36"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_36)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for base that is zero"),
    __W_TEST_STR_NAME(toull, _returns_number_for_base_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns zero for zero string"),
    __W_TEST_STR_NAME(toull, _returns_zero_for_zero_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns ULLONG_MAX for ULLONG_MAX string"),
    __W_TEST_STR_NAME(toull, _returns_ullong_max_for_ullong_max_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns ULLONG_MAX and sets errno for overflow"),
    __W_TEST_STR_NAME(toull, _returns_ullong_max_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns zero and sets errno for incorrect base"),
    __W_TEST_STR_NAME(toull, _returns_zero_and_sets_errno_for_incorrect_base)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " sets end pointer for digit that is too great"),
    __W_TEST_STR_NAME(toull, _sets_end_pointer_for_digit_that_is_too_great)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " sets end pointer for incorrect character"),
    __W_TEST_STR_NAME(toull, _sets_end_pointer_for_incorrect_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for string with spaces"),
    __W_TEST_STR_NAME(toull, _returns_number_for_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for string with sign"),
    __W_TEST_STR_NAME(toull, _returns_number_for_string_with_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for string with spaces and sign"),
    __W_TEST_STR_NAME(toull, _returns_number_for_string_with_spaces_and_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns ULLONG_MAX and sets errno for overflow and string with sign"),
    __W_TEST_STR_NAME(toull, _returns_ullong_max_and_sets_errno_for_overflow_and_string_with_sign)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns zero for string without digits"),
    __W_TEST_STR_NAME(toull, _returns_zero_for_string_without_digits)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(toull, " returns number for end pointer that is null pointer"),
    __W_TEST_STR_NAME(toull, _returns_number_for_end_pointer_that_is_null_pointer)) == NULL) return -1;
  return 0;
}
