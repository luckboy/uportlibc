/*
 * Copyright (c) 2017 Łukasz Szpakowski
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
#include <CUnit/Basic.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <wchar.h>
#define __W W
#include "w_cunit.h"
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_uportlibc_name.h"
#include "w_test_name.h"

#ifndef LDBL_MAX
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

#define FLOAT_EPS               FLT_EPSILON
#define DOUBLE_EPS              DBL_EPSILON
#define LONG_DOUBLE_EPS         DBL_EPSILON

int __W_INITIALIZE_NAME(sscanf)(void) { return 0; }

int __W_CLEANUP_NAME(sscanf)(void) { return 0; }

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("567890"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 567890);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion_and_string_with_plus)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("+3456"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 3456);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("+65434"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 65434);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_negative_integer_number_for_d_conversion)(void)
{
  int x = 1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-12345"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, -12345);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-67890"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, -67890);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion_and_string_with_spaces)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \r\n\t 2345  "), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 2345);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  98765  "), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 98765);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_i_conversion)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("56789"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, 56789);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("12345"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, 12345);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0765"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, 0765);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xabc2"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xabc2);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_zero_for_i_conversion)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_negative_integer_number_for_i_conversion)(void)
{
  int x = 1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-1234"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, -1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-5678"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, -5678);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-0755"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, -0755);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-0xabcdef"), __W_L("%i"), &x), 1);
  CU_ASSERT_EQUAL(x, -0xabcdef);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_u_conversion)(void)
{
  unsigned x = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("2345"), __W_L("%u"), &x), 1);
  CU_ASSERT_EQUAL(x, 2345);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("45678"), __W_L("%u"), &x), 1);
  CU_ASSERT_EQUAL(x, 45678);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_o_conversion)(void)
{
  unsigned x = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("4321"), __W_L("%o"), &x), 1);
  CU_ASSERT_EQUAL(x, 04321);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("744"), __W_L("%o"), &x), 1);
  CU_ASSERT_EQUAL(x, 0744);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_x_conversion)(void)
{
  unsigned x = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abc123"), __W_L("%x"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xabc123);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xfedc"), __W_L("%x"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xfedc);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0abcd"), __W_L("%x"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xabcd);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_uppercase_x_conversion)(void)
{
  unsigned x = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123abc"), __W_L("%X"), &x), 1);
  CU_ASSERT_EQUAL(x, 0x123abc);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0XABCDEF"), __W_L("%X"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xabcdef);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_maximal_width)(void)
{
  int x = -1;
  unsigned y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("23456"), __W_L("%2d"), &x), 1);
  CU_ASSERT_EQUAL(x, 23);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xabcd"), __W_L("%4x"), &y), 1);
  CU_ASSERT_EQUAL(y, 0xab);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_maximal_width_and_string_with_spaces)(void)
{
  int x = -1;
  unsigned y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("   2345"), __W_L("%3i"), &x), 1);
  CU_ASSERT_EQUAL(x, 234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("    75567"), __W_L("%4o"), &y), 1);
  CU_ASSERT_EQUAL(y, 07556);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_and_sets_errno_for_overflow)(void)
{
  int x = -1;
  unsigned y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("18446744073709551616"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, INT_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-1000000000000000000000"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, INT_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("10000000000000000"), __W_L("%x"), &y), 1);
  CU_ASSERT_EQUAL(y, UINT_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("2147483648"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, INT_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-2147483649"), __W_L("%d"), &x), 1);
  CU_ASSERT_EQUAL(x, INT_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("100000000"), __W_L("%x"), &y), 1);
  CU_ASSERT_EQUAL(y, UINT_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_hh_length)(void)
{
  signed char x = -1;
  unsigned char y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123"), __W_L("%hhd"), &x), 1);
  CU_ASSERT_EQUAL(x, 123);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xfe"), __W_L("%hhx"), &y), 1);
  CU_ASSERT_EQUAL(y, 0xfe);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_h_length)(void)
{
  short x = -1;
  unsigned short y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("12345"), __W_L("%hi"), &x), 1);
  CU_ASSERT_EQUAL(x, 12345);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("7567"), __W_L("%ho"), &y), 1);
  CU_ASSERT_EQUAL(y, 07567);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_l_length)(void)
{
  long x = -1L;
  unsigned long y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123456"), __W_L("%ld"), &x), 1);
  CU_ASSERT_EQUAL(x, 123456);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%lx"), &y), 1);
  CU_ASSERT_EQUAL(y, 0xabcdef);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_ll_length)(void)
{
  long long x = -1LL;
  unsigned long long y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xffabc"), __W_L("%lli"), &x), 1);
  CU_ASSERT_EQUAL(x, 0xffabc);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123abc"), __W_L("%llx"), &y), 1);
  CU_ASSERT_EQUAL(y, 0x123abc);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _does_not_convert_to_integer_number_for_incorrect_string)(void)
{
  int x = -1LL;
  unsigned y = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%d"), &x), 0);
  CU_ASSERT_EQUAL(x, -1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ghij"), __W_L("%x"), &y), 0);
  CU_ASSERT_EQUAL(y, 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123.45"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 123.45, FLOAT_EPS * 100.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("567.89e10"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 567.89e10, FLOAT_EPS * 100.0e10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xfedc.ab"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0xfedc.abp0, FLOAT_EPS * 0x100.0p0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0x1.23fep10"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0x1.23fep10, FLOAT_EPS * 0x1.0p10);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0.1234"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0.1234, FLOAT_EPS * 0.1);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_zero_for_f_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0.0"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0.0, FLOAT_EPS * 1.0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion_and_string_with_plus)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("+12.345"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 12.345, FLOAT_EPS * 10.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("+4567"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 4567.0, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_negative_floating_point_number_for_f_conversion)(void)
{
  float x = 1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-5432e5"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, -5432e5, FLOAT_EPS * 1000.0e5);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-0x123p-6"), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, -0x123p-6, FLOAT_EPS * 0x100p-6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion_and_string_with_spaces)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \r\n\t 1234  "), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1234, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" 5678  "), __W_L("%f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 5678.0, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_f_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("12345"), __W_L("%F"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 12345, FLOAT_EPS * 10000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("5.678"), __W_L("%F"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 5.678, FLOAT_EPS);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_e_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1.2345"), __W_L("%e"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1.2345, FLOAT_EPS);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("4.567e20"), __W_L("%e"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 4.567e20, FLOAT_EPS * 1.0e20);
  CU_ASSERT_EQUAL(errno, 0);
}


void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_e_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1.234e15"), __W_L("%E"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1.234e15, FLOAT_EPS * 1.0e15);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("6.789E-5"), __W_L("%E"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 6.789e-5f, FLOAT_EPS * 1.0e-5);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_g_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0.01234"), __W_L("%g"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0.01234, FLOAT_EPS * 0.01);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("4.567e20"), __W_L("%g"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 4.567e20, FLOAT_EPS * 1.0e20);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_g_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("4.321e+24"), __W_L("%G"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 4.321e24, FLOAT_EPS * 1.0e24);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("5.678E-10"), __W_L("%G"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 5.678e-10f, FLOAT_EPS * 1.0e-10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_a_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234"), __W_L("%a"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1234.0, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0X1234F"), __W_L("%a"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0x1234fp0, FLOAT_EPS * 0x1000.0p0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_a_conversion)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0x1.345ap16"), __W_L("%A"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0x1.345ap16, FLOAT_EPS * 0x1.0p16);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0XFEDCP10"), __W_L("%A"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 0xfedcp10, FLOAT_EPS * 0x1000.0p10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_maximal_width)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234.56e10"), __W_L("%4f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1234.0, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1.2e256"), __W_L("%6f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1.2e25, FLOAT_EPS * 1.0e25);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_maximal_width_and_string_with_spaces)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  1234"), __W_L("%2f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 12.0, FLOAT_EPS * 10.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("    5678e10"), __W_L("%4f"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 5678.0, FLOAT_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_valf_for_overflow)(void)
{
  static __W_CHAR str[1000];
  float x = -1.0;
#if __W == 'c'
  sprintf(str, "1e%d", (int) ceil(log(FLT_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"1e%d", (int) ceil(log(FLT_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_valf_for_overflow)(void)
{
  static __W_CHAR str[1000];
  float x = 1.0;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) ceil(log(FLT_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) ceil(log(FLT_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_valf_for_infinity)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("inf"), __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("Infinity"), __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_valf_for_minus_infinity)(void)
{
  float x = 1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-inf"), __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-Infinity"), __W_L("%f"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALF);
  CU_ASSERT_EQUAL(errno, (HUGE_VALF == FLT_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_nan_for_nan)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("nan"), __W_L("%f"), &x), 1);
  CU_ASSERT(isnanf(x));
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("NaN"), __W_L("%f"), &x), 1);
  CU_ASSERT(isnanf(x));
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_l_length)(void)
{
  double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("12345"), __W_L("%lf"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 12345.0, DOUBLE_EPS * 10000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123e10"), __W_L("%lf"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 123.0e10, DOUBLE_EPS * 100.0e10);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_val_for_l_length_and_overflow)(void)
{
  static __W_CHAR str[1000];
  double x = -1.0;
#if __W == 'c'
  sprintf(str, "1e%d", (int) ceil(log(DBL_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"1e%d", (int) ceil(log(DBL_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_val_for_l_length_and_overflow)(void)
{
  static __W_CHAR str[1000];
  double x = 1.0;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) ceil(log(DBL_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) ceil(log(DBL_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_val_for_l_length_and_infinity)(void)
{
  double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("inf"), __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("Infinity"), __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_val_for_l_length_and_minus_infinity)(void)
{
  double x = 1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-inf"), __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-Infinity"), __W_L("%lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VAL);
  CU_ASSERT_EQUAL(errno, (HUGE_VAL == DBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_nan_for_l_length_and_nan)(void)
{
  double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("nan"), __W_L("%lf"), &x), 1);
  CU_ASSERT(isnan(x));
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("NaN"), __W_L("%lf"), &x), 1);
  CU_ASSERT(isnan(x));
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_l_length)(void)
{
  long double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234.567"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 1234.567, LONG_DOUBLE_EPS * 1000.0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("98.76e14"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_DOUBLE_EQUAL(x, 98.76e14, LONG_DOUBLE_EPS * 10.0e14);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_vall_for_uppercase_l_length_and_overflow)(void)
{
  static __W_CHAR str[1000];
  long double x = -1.0;
#if __W == 'c'
  sprintf(str, "1e%d", (int) ceil(logl(LDBL_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"1e%d", (int) ceil(logl(LDBL_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_vall_for_uppercase_l_length_and_overflow)(void)
{
  static __W_CHAR str[1000];
  long double x = 1.0;
#if __W == 'c'
  sprintf(str, "-1e%d", (int) ceil(logl(LDBL_MAX) / log(10.0) + 1.0));
#else
  swprintf(str, 1000, L"-1e%d", (int) ceil(logl(LDBL_MAX) / log(10.0) + 1.0));
#endif
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(str, __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_huge_vall_for_uppercase_l_length_and_infinity)(void)
{
  long double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("inf"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("Infinity"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_minus_huge_vall_for_uppercase_l_length_and_minus_infinity)(void)
{
  long double x = 1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-inf"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-Infinity"), __W_L("%Lf"), &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VALL);
  CU_ASSERT_EQUAL(errno, (HUGE_VALL == LDBL_MAX ? ERANGE : 0));
}

void __W_TEST_NAME(s, scanf, _converts_to_nan_for_uppercase_l_length_and_nan)(void)
{
  long double x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("nan"), __W_L("%Lf"), &x), 1);
  CU_ASSERT(isnanl(x));
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("NaN"), __W_L("%Lf"), &x), 1);
  CU_ASSERT(isnanl(x));
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _does_not_convert_to_floating_point_number_for_incorrect_string)(void)
{
  float x = -1.0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%f"), &x), 0);
  CU_ASSERT_EQUAL(x, -1.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xghij"), __W_L("%f"), &x), 0);
  CU_ASSERT_EQUAL(x, -1.0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion)(void)
{
  char buf[MB_LEN_MAX];
  char c = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("a"), __W_L("%c"), &c), 1);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("b"), __W_L("%c"), &c), 1);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_EQUAL(errno, 0);
}

#if __W == 'w'
void test_swcanf_converts_to_characters_for_c_conversion_and_wide_string(void)
{
  char buf[4] = { -1, -1, -1, -1 };
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U0001f800", L"%c", buf), 1);    
  CU_ASSERT_NSTRING_EQUAL(buf, "\360\237\240\200", 4);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U00000105", L"%c", buf), 1);        
  CU_ASSERT_NSTRING_EQUAL(buf, "\304\205", 2);
  CU_ASSERT_EQUAL(errno, 0);
}
#endif

void __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion_and_string_with_spaces)(void)
{
  char c = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \r\n a"), __W_L("%c"), &c), 1);
  CU_ASSERT_EQUAL(c, ' ');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("\tb"), __W_L("%c"), &c), 1);
  CU_ASSERT_EQUAL(c, '\t');
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion_and_maximal_width)(void)
{
  char c = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("A"), __W_L("%2c"), &c), 1);
  CU_ASSERT_EQUAL(c, 'A');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("D"), __W_L("%1c"), &c), 1);
  CU_ASSERT_EQUAL(c, 'D');
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_lc_conversion)(void)
{
  wchar_t c = WEOF;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("a"), __W_L("%lc"), &c), 1);
  CU_ASSERT_EQUAL(c, L'a');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("b"), __W_L("%lc"), &c), 1);
  CU_ASSERT_EQUAL(c, L'b');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("\304\205", "%lc", &c), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U00000105", L"%lc", &c), 1);
#endif
  CU_ASSERT_EQUAL(c, L'\U00000105');
  CU_ASSERT_EQUAL(errno, 0);
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("\360\237\240\201", "%lc", &c), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U0001f801", L"%lc", &c), 1);
#endif
  CU_ASSERT_EQUAL(c, L'\U0001f801');
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_lc_conversion_and_maximal_width)(void)
{
  wchar_t c = WEOF;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("b"), __W_L("%1lc"), &c), 1);
  CU_ASSERT_EQUAL(c, L'b');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("c"), __W_L("%1lc"), &c), 1);
  CU_ASSERT_EQUAL(c, L'c');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("\304\206", "%2lc", &c), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U00000106", L"%2lc", &c), 1);
#endif
  CU_ASSERT_EQUAL(c, L'\U00000106');
  CU_ASSERT_EQUAL(errno, 0);
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("\342\206\220", "%3lc", &c), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"\U00002190", L"%3lc", &c), 1);
#endif
  CU_ASSERT_EQUAL(c, L'\U00002190');
  CU_ASSERT_EQUAL(errno, 0);
}

#if __W == 'c'
void test_sscanf_does_not_convert_to_wide_character_and_sets_errno_for_lc_conversion_and_maximal_width(void)
{
  wchar_t c = WEOF;
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_sscanf("\342\206\221", "%2lc", &c), 0);
  CU_ASSERT_EQUAL(c, WEOF);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_sscanf("\360\237\240\204", "%1lc", &c), 0);
  CU_ASSERT_EQUAL(c, WEOF);
  CU_ASSERT_EQUAL(errno, EILSEQ);
}
#endif

void __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_uppercase_c_conversion)(void)
{
  wchar_t c = WEOF;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("c"), __W_L("%C"), &c), 1);
  CU_ASSERT_EQUAL(c, L'c');
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("d"), __W_L("%C"), &c), 1);
  CU_ASSERT_EQUAL(c, L'd');
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion)(void)
{
  static char str[20];
  memset(str, (char) (-1), 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdefgh"), __W_L("%s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcdefgh");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234"), __W_L("%s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "1234");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("a\304\205bc\304\207de\304\231f", "%s", str), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"a\U00000105bc\U00000107de\U00000119f", L"%s", str), 1);
#endif
  CU_ASSERT_STRING_EQUAL(str, "a\304\205bc\304\207de\304\231f");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("ABC\360\237\240\210\360\237\240\211\360\237\240\212", "%s", str), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"ABC\U0001f808\U0001f809\U0001f80a", L"%s", str), 1);
#endif
  CU_ASSERT_STRING_EQUAL(str, "ABC\360\237\240\210\360\237\240\211\360\237\240\212");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_string_with_spaces)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \n ABCDEF  "), __W_L("%s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "ABCDEF");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  abcdef  "), __W_L("%s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcdef");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_maximal_width)(void)
{
  static char str[20];
  memset(str, (char) (-1), 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%4s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcd");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234567890"), __W_L("%5s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "12345");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_maximal_width_and_string_with_spaces)(void)
{
  static char str[20];
  memset(str, (char) (-1), 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \n abcdefgh"), __W_L("%6s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcdef");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  ABCDEF"), __W_L("%4s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "ABCD");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_empty_string)(void)
{
  static char str[20];
  memset(str, (char) (-1), 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(""), __W_L("%s"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion)(void)
{
  static wchar_t str[20];
  wmemset(str, -1, 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"abcdef") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABC"), __W_L("%ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"ABC") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("A\304\204BC\304\206DE\304\230F", "%ls", str), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"A\U00000104BC\U00000106DE\U00000118F", L"%ls", str), 1);
#endif
  CU_ASSERT(wcscmp(str, L"A\U00000104BC\U00000106DE\U00000118F") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_sscanf("abc\360\237\240\200\360\237\240\201\360\237\240\202", "%ls", str), 1);
#else
  CU_ASSERT_EQUAL(uportlibc_swscanf(L"abc\U0001f800\U0001f801\U0001f802", L"%ls", str), 1);
#endif
  CU_ASSERT(wcscmp(str, L"abc\U0001f800\U0001f801\U0001f802") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_string_with_spaces)(void)
{
  static wchar_t str[10];
  wmemset(str, -1, 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \t\r\n abcdef  "), __W_L("%ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"abcdef") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  1234  "), __W_L("%ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"1234") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_maximal_width)(void)
{
  static wchar_t str[20];
  wmemset(str, -1, 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("fedcba"), __W_L("%3ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"fed") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDEF"), __W_L("%4ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"ABCD") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_maximal_width_and_string_with_spaces)(void)
{
  static wchar_t str[20];
  wmemset(str, -1, 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \t abcdef"), __W_L("%3ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"abc") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("  1234567"), __W_L("%5ls"), str), 1);
  CU_ASSERT(wcscmp(str, L"12345") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

#if __W == 'c'
void test_sscanf_does_not_convert_to_wide_string_and_sets_errno_for_ls_conversion_and_maximal_width(void)
{
  static wchar_t str[10];
  wmemset(str, -1, 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_sscanf("abc\360\237\240\200", "%5ls", str), 0);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_sscanf("12345\304\205", "%6ls", str), 0);
  CU_ASSERT_EQUAL(errno, EILSEQ);
}
#endif

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_uppercase_s_conversion)(void)
{
  static wchar_t str[20];
  wmemset(str, -1, 19);
  str[19] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdefgh"), __W_L("%S"), str), 1);
  CU_ASSERT(wcscmp(str, L"abcdefgh") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("12345"), __W_L("%S"), str), 1);
  CU_ASSERT(wcscmp(str, L"12345") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdefgh"), __W_L("%[abcd]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcd");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234"), __W_L("%[123]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "123");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abc^ABC"), __W_L("%[^ABC]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abc^");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234509876"), __W_L("%[^67890]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "12345");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_ranges_in_set)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abyz1289ABC"), __W_L("%[a-z1-9]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abyz1289");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcd123"), __W_L("%[^0-9A-Z]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcd");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcd"), __W_L("%[a-b]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "ab");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1212345"), __W_L("%[1-2]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "1212");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_minus_in_set)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcd-ef"), __W_L("%[-abcd]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcd-");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("-ABCDEF"), __W_L("%[ABCD-]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "-ABCD");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("5678-1234"), __W_L("%[^-1234]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "5678");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("defabc"), __W_L("%[^abc-]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "def");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_right_bracket_in_set)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ab]c"), __W_L("%[]abc]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "ab]c");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDEF]"), __W_L("%[^]1234]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "ABCDEF");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_string_with_spaces)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \r\n ABCD"), __W_L("%[ABCD]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \n ABCD"), __W_L("%[^abcd]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, " \n ABCD");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_maximal_width)(void)
{
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%4[abcdef]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "abcd");
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123456"), __W_L("%5[^abcd]"), str), 1);
  CU_ASSERT_STRING_EQUAL(str, "12345");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion)(void)
{
  static wchar_t str[10];
  wmemset(str, -1, 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDEFGH"), __W_L("%l[ABCDEF]"), str), 1);
  CU_ASSERT(wcscmp(str, L"ABCDEF") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef"), __W_L("%l[abcd]"), str), 1);
  CU_ASSERT(wcscmp(str, L"abcd") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCD^ef"), __W_L("%l[^abcdef]"), str), 1);
  CU_ASSERT(wcscmp(str, L"ABCD^") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123FED"), __W_L("%l[^ABCDEF]"), str), 1);
  CU_ASSERT(wcscmp(str, L"123") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion_and_string_with_spaces)(void)
{
  static wchar_t str[10];
  memset(str, -1, 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \r\n 1234"), __W_L("%l[1234]"), str), 1);
  CU_ASSERT(wcscmp(str, L"") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L(" \t abcd"), __W_L("%l[^ABCD]"), str), 1);
  CU_ASSERT(wcscmp(str, L" \t abcd") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion_and_maximal_width)(void)
{
  static wchar_t str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDEF"), __W_L("%4l[ABCDEF]"), str), 1);
  CU_ASSERT(wcscmp(str, L"ABCD") == 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdefgh"), __W_L("%6l[^ABCDEF]"), str), 1);
  CU_ASSERT(wcscmp(str, L"abcdef") == 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_pointer_for_p_conversion)(void)
{
  void *ptr = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abc123"), __W_L("%p"), &ptr), 1);
  CU_ASSERT_EQUAL(ptr, (void *) 0xabc123);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("0xabcdef"), __W_L("%p"), &ptr), 1);
  CU_ASSERT_EQUAL(ptr, (void *) 0xabcdef);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _matches_characters_from_format)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abc123def1234"), __W_L("abc123def%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDEF1234"), __W_L("ABCDEF%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _matches_percent_from_format)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ab%cd%ef1234"), __W_L("ab%%cd%%ef%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABC%DEF1234"), __W_L("ABC%%DEF%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _matches_spaces_from_format)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ab cd\n  ef1234"), __W_L("ab\ncd ef%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABC DEF1234"), __W_L("ABC DEF%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("[]1234"), __W_L("[ ]%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abc def1234"), __W_L("abc\tdef%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _does_not_match_characters_from_format_for_incorrect_string)(void)
{
  int x = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcef1234"), __W_L("abcdef%d"), &x), 0);
  CU_ASSERT_EQUAL(x, -1);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("ABCDF1234"), __W_L("ABCDEF%d"), &x), 0);
  CU_ASSERT_EQUAL(x, -1);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_many_unnumbered_arguments)(void)
{
  int x = -1;
  float y = -1.0;
  unsigned z = 0;
  static char str[10];
  memset(str, (char) (-1), 9);
  str[9] = 0;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234 0.4321 12abc abcdef"), __W_L("%d%f%x%s"), &x, &y, &z, str), 4);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_DOUBLE_EQUAL(y, 0.4321, FLOAT_EPS * 0.1);
  CU_ASSERT_EQUAL(z, 0x12abc);
  CU_ASSERT_STRING_EQUAL(str, "abcdef");
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_to_many_numbered_arguments)(void)
{
  float x = -1.0;
  unsigned y = 0;
  int z = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234 1.234 0xabc"), __W_L("%3$d%1$f%2$x"), &x, &y, &z), 3);
  CU_ASSERT_DOUBLE_EQUAL(x, 1.234, FLOAT_EPS);
  CU_ASSERT_EQUAL(y, 0xabc);
  CU_ASSERT_EQUAL(z, 1234);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _converts_for_conversion_with_star_flag)(void)
{
  int x = -1, y = -1;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1 1234 2"), __W_L("%d%*d%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 1);
  CU_ASSERT_EQUAL(y, 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("2 12.34 3"), __W_L("%d%*f%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 2);
  CU_ASSERT_EQUAL(y, 3);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("3a 4"), __W_L("%d%*c%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 3);
  CU_ASSERT_EQUAL(y, 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("4b 5"), __W_L("%d%*lc%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 4);
  CU_ASSERT_EQUAL(y, 5);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("abcdef 6"), __W_L("%*s%d"), &x), 1);
  CU_ASSERT_EQUAL(x, 6);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("6 abcdef 7"), __W_L("%d%*ls%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 6);
  CU_ASSERT_EQUAL(y, 7);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("7 0xabcd 8"), __W_L("%d%*p%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 7);
  CU_ASSERT_EQUAL(y, 8);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("8 9"), __W_L("%d%*n%d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 8);
  CU_ASSERT_EQUAL(y, 9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("9 1234 10"), __W_L("%2$d%3$*d%1$d"), &x, &y), 2);
  CU_ASSERT_EQUAL(x, 10);
  CU_ASSERT_EQUAL(y, 9);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _gets_number_of_characters_for_n_conversion)(void)
{
  int x = -1, y = -1, count = -1;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234 2345"), __W_L("%d%n%d"), &x, &count, &y), 2);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(count, 4);
  CU_ASSERT_EQUAL(y, 2345);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("123456"), __W_L("%d%n"), &x, &count, &y), 1);
  CU_ASSERT_EQUAL(x, 123456);
  CU_ASSERT_EQUAL(count, 6);
  CU_ASSERT_EQUAL(errno, 0);
}

void __W_TEST_NAME(s, scanf, _returns_minus_one_and_sets_errno_for_unsupported_conversion)(void)
{
  int x = -1;
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("1234"), __W_L("%z"), &x), -1);
  CU_ASSERT_EQUAL(x, -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(__W_NAME(s, scanf)(__W_L("2345"), __W_L("%y"), &x), -1);
  CU_ASSERT_EQUAL(x, -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

int __W_ADD_SUITE_NAME(sscanf)(void)
{
  CU_pSuite suite;
  suite = CU_add_suite(__W_SUITE_STRING(sscanf), __W_INITIALIZE_NAME(sscanf), __W_CLEANUP_NAME(sscanf));
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for d conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for d conversion and string with plus"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion_and_string_with_plus)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to negative integer number for d conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_negative_integer_number_for_d_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to negative integer number for d conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_d_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for i conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_i_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer zero for i conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_zero_for_i_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to negative integer number for i conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_negative_integer_number_for_i_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for u conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_u_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for o conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_o_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for x conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_x_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for X conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_uppercase_x_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for maximal width and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_maximal_width_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number and sets errno for overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_and_sets_errno_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for hh length"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_hh_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for h length"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_h_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for l length"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_l_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for ll length"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_ll_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " does not convert to integer number for incorrect string"),
    __W_TEST_NAME(s, scanf, _does_not_convert_to_integer_number_for_incorrect_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to integer number for ll length"),
    __W_TEST_NAME(s, scanf, _converts_to_integer_number_for_ll_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for f conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point zero for f conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_zero_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for f conversion and string with plus"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion_and_string_with_plus)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to negative floating-point number for f conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_negative_floating_point_number_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for f conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_f_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for F conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for e conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for E conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for g conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for G conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for a conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for A conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for maximal width and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_maximal_width_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VALF for overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_valf_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VALF for overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_valf_for_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VALF for infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_valf_for_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VALF for -infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_valf_for_minus_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to NaN for NaN"),
    __W_TEST_NAME(s, scanf, _converts_to_nan_for_nan)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for l length"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_l_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VAL for l length and overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_val_for_l_length_and_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VAL for l length and overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_val_for_l_length_and_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VAL for l length and infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_val_for_l_length_and_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VAL for l length and -infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_val_for_l_length_and_minus_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to NaN for l length and NaN"),
    __W_TEST_NAME(s, scanf, _converts_to_nan_for_l_length_and_nan)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to floating-point number for L length"),
    __W_TEST_NAME(s, scanf, _converts_to_floating_point_number_for_uppercase_l_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VALL for L length and overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_vall_for_uppercase_l_length_and_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VALL for L length and overflow"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_vall_for_uppercase_l_length_and_overflow)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to HUGE_VALL for L length and infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_huge_vall_for_uppercase_l_length_and_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to -HUGE_VALL for L length and -infinity"),
    __W_TEST_NAME(s, scanf, _converts_to_minus_huge_vall_for_uppercase_l_length_and_minus_infinity)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to NaN for L length and NaN"),
    __W_TEST_NAME(s, scanf, _converts_to_nan_for_uppercase_l_length_and_nan)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " does not convert to floating point number for incorrect string"),
    __W_TEST_NAME(s, scanf, _does_not_convert_to_floating_point_number_for_incorrect_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to character for c conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "swscanf converts to characters for c conversion and wide string",
    test_swcanf_converts_to_characters_for_c_conversion_and_wide_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to character for c conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to character for c conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_character_for_c_conversion_and_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide_character for lc conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_lc_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide_character for lc conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_lc_conversion_and_maximal_width)) == NULL) return -1;
#if __W == 'c'
  if(CU_add_test(suite,
    "sscanf does not convert to wide character and sets errno for lc conversion and maximal width",
    test_sscanf_does_not_convert_to_wide_character_and_sets_errno_for_lc_conversion_and_maximal_width) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide character for C conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_character_for_uppercase_c_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for s conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for s conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for s conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for s conversion and maximal width and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_maximal_width_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for s conversion and empty string"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_s_conversion_and_empty_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for ls conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for ls conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for ls conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for ls conversion and maximal width and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_ls_conversion_and_maximal_width_and_string_with_spaces)) == NULL) return -1;
#if __W == 'c'
  if(CU_add_test(suite,
    "sscanf does not convert to wide string and sets errno for ls conversion and maximal width",
    test_sscanf_does_not_convert_to_wide_string_and_sets_errno_for_ls_conversion_and_maximal_width) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for S conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_uppercase_s_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion and ranges in set"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_ranges_in_set)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion and - in set"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_minus_in_set)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion and ] in set"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_right_bracket_in_set)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for [ conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_string_for_left_bracket_conversion_and_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for l[ conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to wide string for l[ conversion and string with spaces"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion_and_string_with_spaces)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to string for l[ conversion and maximal width"),
    __W_TEST_NAME(s, scanf, _converts_to_wide_string_for_l_left_bracket_conversion_and_maximal_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to pointer for p conversion"),
    __W_TEST_NAME(s, scanf, _converts_to_pointer_for_p_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " matches characters from format"),
    __W_TEST_NAME(s, scanf, _matches_characters_from_format)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " matches percent from format"),
    __W_TEST_NAME(s, scanf, _matches_percent_from_format)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " matches spaces from format"),
    __W_TEST_NAME(s, scanf, _matches_spaces_from_format)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " does not match characters from format for incorrect string"),
    __W_TEST_NAME(s, scanf, _does_not_match_characters_from_format_for_incorrect_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to many unnumbered arguments"),
    __W_TEST_NAME(s, scanf, _converts_to_many_unnumbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts to many numbered arguments"),
    __W_TEST_NAME(s, scanf, _converts_to_many_numbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " converts for conversion with * flag"),
    __W_TEST_NAME(s, scanf, _converts_for_conversion_with_star_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " gets number of characters for n conversion"),
    __W_TEST_NAME(s, scanf, _gets_number_of_characters_for_n_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(s, scanf, " returns -1 and sets errno for unsupported conversion"),
    __W_TEST_NAME(s, scanf, _returns_minus_one_and_sets_errno_for_unsupported_conversion)) == NULL) return -1;
  return 0;
}
