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
#include <stdio.h>
#define __W W
#include "w_cunit.h"
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_uportlibc_name.h"
#include "w_test_name.h"

int __W_INITIALIZE_NAME(snprintf)(void) { return 0; }

int __W_CLEANUP_NAME(snprintf)(void) { return 0; }

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d"), 1234), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d"), 567890), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("567890"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_integer_number_for_d_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d"), -123456), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-123456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d"), -789), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_plus_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%+d"), 123456), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("+123456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%+d"), -1234), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1234"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_space_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("% d"), 12345), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L(" 12345"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("% d"), -1234), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1234"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_hash_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#d"), 12345), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12345"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#d"), 6789), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("6789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_i_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%i"), 54321), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("54321"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%i"), 6789012), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("6789012"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_integer_number_for_i_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%i"), -987), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-987"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%i"), -12345), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-12345"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_u_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%u"), 1987654321), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1987654321"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%u"), 34321), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("34321"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_o_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%o"), 0123456), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%o"), 0712), 3);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("712"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_o_conversion_and_hash_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#o"), 012345), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("012345"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#o"), 0765), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0765"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_zero_for_o_conversion_and_hash_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#o"), 0), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_x_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%x"), 0x12ab), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12ab"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%x"), 0xff1122), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ff1122"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_uppercase_x_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%X"), 0xabc12), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABC12"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%X"), 0xf890123a), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("F890123A"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_x_conversion_and_hash_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#x"), 0x12345), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x12345"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%#x"), 0x12ff45), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x12ff45"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_positive_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%8d"), 124321), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  124321"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%10x"), 0x1b2c), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("      1b2c"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_positive_width_and_precision)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%8.6d"), 4321), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  004321"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%4.3o"), 017), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L(" 017"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_negative_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-6d"), 1234), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234  "));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-10x"), 0xabcdef), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdef    "));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_zero)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%d"), 0), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_width_and_zero_flag)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%010d"), 12345), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0000012345"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%05d"), 123), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("00123"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_apostrophe_flag)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%'d"), 1234), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%'u"), 56789), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("56789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_hh_length)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%hhd"), (signed char) 123), 3);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%hhx"), (unsigned char) 0x45), 2);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("45"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_h_length)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%hd"), (short) 1234), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%ho"), (unsigned short) 04567), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("4567"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_l_length)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%ld"), 123456L), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%lx"), 0xabcd1234UL), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcd1234"));
}

void __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_ll_length)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%ld"), 12345678901234LL), 14);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12345678901234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%lx"), 0x123456abcdULL), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123456abcd"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%f"), 12.34), 9);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12.340000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%f"), 0.567), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0.567000"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_f_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%F"), 123.4), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123.400000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%F"), 5.67), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("5.670000"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_f_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%f"), -43.21), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-43.210000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%f"), -0.08765), 9);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-0.087650"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_precision_that_is_zero)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0f"), 12.34), 2);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0f"), 56789.4), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("56789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_carry)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%f"), 123.4599996), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123.460000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%f"), 0.5979996), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0.598000"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_carry_idx_that_is_zero)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%f"), 9.9999996), 9);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("10.000000"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_digits_that_is_one)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%f"), 0.000001234), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0.000001"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_f_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%f"), 0.0), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0.000000"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 12345.6), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.234560e+04"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 0.056789), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("5.678900e-02"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_e_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%E"), 123456.7), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.234567E+05"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%E"), 0.006789), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("6.789000E-03"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_e_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), -5432.1), 13);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-5.432100e+03"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), -0.000123), 13);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1.230000e-04"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_precision_that_is_zero)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0e"), 1500.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("2e+03"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0e"), 400000.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("4e+05"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_carry)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 123459.96), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.234600e+05"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 0.00059789996), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("5.979000e-04"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_carry_idx_that_is_zero)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 99.9999996), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.000000e+02"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_e_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 0.0), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0.000000e+00"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 1234567.0), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.23457e+06"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 456789.0), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("456789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_g_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%G"), 12345670.0), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.23457E+07"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%G"), 4567.0), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("4567"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_g_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), -123450000.0), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1.2345e+08"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), -5678.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-5678"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_is_e_that_is_one)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 1234560.0), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.23456e+06"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 0.0000123456), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.23456e-05"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_precision_that_is_zero)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0g"), 15000.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("2e+04"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.0g"), 100.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1e+02"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_carry)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 1239596.0), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.2396e+06"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 459789.6), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("459790"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_carry_idx_that_is_zero)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 999999.6), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1e+06"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 9.999996), 2);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("10"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_g_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%g"), 0.0), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), 0x8.2345p2), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x8.2345p+2"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), 0xa.bcp15), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0xa.bcp+15"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_a_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%A"), 0x8.765p2), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0X8.765P+2"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%A"), 0x8.abcp15), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0X8.ABCP+15"));
}

void __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_a_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), -0x8.abcdp4), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-0x8.abcdp+4"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), -0xc.56p10), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-0xc.56p+10"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_precision_that_is_zero)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%.0a"), 0x8.8p4), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x9p+4"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%.0a"), 0x8p10), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x8p+10"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_carry)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), 0x8.2345f9p2), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x8.2346p+2"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%a"), 0xa.bcfff9p15), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0xa.bdp+15"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_carry_idx_that_is_zero)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%a"), 0xf.fffff9p2), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x8p+3"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_a_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%a"), 0.0), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x0p+0"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_positive_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%15f"), 123.45), 15);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("     123.450000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%14e"), 1515.0), 14);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  1.515000e+03"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_negative_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-12f"), 12.345), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12.345000   "));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-10g"), 1234.0), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234      "));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_plus_flag)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%+f"), 1234.567), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("+1234.567000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%+g"), -1234.5), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1234.5"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_space_flag)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("% f"), 1234.5), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L(" 1234.500000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%+g"), -1234.56), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("-1234.56"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_precision)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%.10f"), 1.2345678901), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.2345678901"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%.4e"), 9.8765e10), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("9.8765e+10"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%.7g"), 1.235678e10), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.235678e+10"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_width_and_zero_flag)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%015f"), 1234.0), 15);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("00001234.000000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%05g"), 123.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("00123"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%015e"), 123.40), 15);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0001.234000e+02"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_apostrophe_flag)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%'f"), 1234.0), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234.000000"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%'g"), 56789.0), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("56789"));
}

void __W_TEST_SNPRINTF_NAME(_converts_big_floating_point_number)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%e"), 1.234e30), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.234000e+30"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%g"), 5.678e32), 9);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("5.678e+32"));
}

void __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_l_length)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%Lf"), 1234.5678L), 11);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234.567800"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%Le"), 123456.0L), 12);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1.234560e+05"));
}

void __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%c"), 'c'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("c"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%c"), 'a'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("a"));
}

void __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion_and_positive_width)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%5c"), 'a'), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    a"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%8c"), 'b'), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("       b"));
}

void __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion_and_negative_width)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%-4c"), 'a'), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("a   "));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%-6c"), 'b'), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("b     "));
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_lc_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%lc"), L'd'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("d"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%lc"), L'b'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("b"));
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 10, "%lc", L'\U00000104'), 2);
  CU_ASSERT_STRING_EQUAL(buf, "\304\204");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 10, L"%lc", L'\U00000104'), 1);
  CU_ASSERT(wcscmp(buf, L"\U00000104") == 0);
#endif
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 10, "%lc", L'\U0001f804'), 4);
  CU_ASSERT_STRING_EQUAL(buf, "\360\237\240\204");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 10, L"%lc", L'\U0001f804'), 1);
  CU_ASSERT(wcscmp(buf, L"\U0001f804") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_lc_conversion_and_width)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%6lc"), L'a'), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("     a"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%4lc"), L'\U00000105'), 4);
#if __W != 'w'
  CU_ASSERT_STRING_EQUAL(buf, "  \304\205");
#else
  CU_ASSERT(wcscmp(buf, L"   \U00000105") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_uppercase_c_conversion)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%C"), L'a'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("a"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%C"), L'b'), 1);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("b"));
}

void __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%s"), "abcdef"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdef"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%s"), "ABCDEFGH"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABCDEFGH"));
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 20, "%s", "A\304\204BC\304\206DE\304\230F"), 12);
  CU_ASSERT_STRING_EQUAL(buf, "A\304\204BC\304\206DE\304\230F");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 20, L"%s", "A\304\204BC\304\206DE\304\230F"), 9);
  CU_ASSERT(wcscmp(buf, L"A\U00000104BC\U00000106DE\U00000118F") == 0);
#endif
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 20, "%s", "ABC\360\237\240\204\360\237\240\205\360\237\240\206"), 15);
  CU_ASSERT_STRING_EQUAL(buf, "ABC\360\237\240\204\360\237\240\205\360\237\240\206");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 20, L"%s", "ABC\360\237\240\204\360\237\240\205\360\237\240\206"), 6);
  CU_ASSERT(wcscmp(buf, L"ABC\U0001f804\U0001f805\U0001f806") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_positive_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%8s"), "abcdef"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  abcdef"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%10s"), "ABCDEF"), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    ABCDEF"));
}

void __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_negative_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-10s"), "abcdefgh"), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdefgh  "));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%-6s"), "abcd"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcd  "));
}

void __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_precision)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.4s"), "abcdef"), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcd"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.6s"), "ABCD1234"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABCD12"));
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%ls"), L"abcd1234"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcd1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%ls"), L"ABCdef"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABCdef"));
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 20, "%ls", L"a\U00000105bc\U00000107de\U00000119f"), 12);
  CU_ASSERT_STRING_EQUAL(buf, "a\304\205bc\304\207de\304\231f");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 20, L"%ls", L"a\U00000105bc\U00000107de\U00000119f"), 9);
  CU_ASSERT(wcscmp(buf, L"a\U00000105bc\U00000107de\U00000119f") == 0);
#endif
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 20, "%ls", L"abc\U0001f802\U0001f917\U0001f800"), 15);
  CU_ASSERT_STRING_EQUAL(buf, "abc\360\237\240\202\360\237\244\227\360\237\240\200");
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 20, L"%ls", L"abc\U0001f802\U0001f917\U0001f800"), 6);
  CU_ASSERT(wcscmp(buf, L"abc\U0001f802\U0001f917\U0001f800") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion_and_width)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%8ls"), L"abcdef"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  abcdef"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%10ls"), L"A\U00000104BC\U00000106"), 10);
#if __W != 'w'
  CU_ASSERT_STRING_EQUAL(buf, "   A\304\204BC\304\206");
#else
  CU_ASSERT(wcscmp(buf, L"     A\U00000104BC\U00000106") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion_and_precision)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.3ls"), L"abcdef"), 3);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abc"));
#if __W != 'w'
  CU_ASSERT_EQUAL(uportlibc_snprintf(buf, 10, "%.5ls", L"ABCD\U00000104\U00000106\U00000118"), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABCD"));
#else
  CU_ASSERT_EQUAL(uportlibc_swprintf(buf, 10, L"%.5ls", L"ABCD\U00000104\U00000106\U00000118"), 5);
  CU_ASSERT(wcscmp(buf, L"ABCD\U00000104") == 0);
#endif
}

void __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_uppercase_s_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%ls"), L"abcdef"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdef"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%ls"), L"ABCD1234"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ABCD1234"));
}

void __W_TEST_SNPRINTF_NAME(_converts_pointer_for_p_conversion)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%p"), (void *) 0x1234), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0x1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%p"), (void *) 0xffff5678), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("0xffff5678"));
}

void __W_TEST_SNPRINTF_NAME(_prints_characters_from_format)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("abcdef")), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdef"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("ab%dcd"), 1234), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ab1234cd"));
}

void __W_TEST_SNPRINTF_NAME(_prints_percent_from_format)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("ab%%cd")), 5);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("ab%cd"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("a%%b%dcd"), 1234), 9);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("a%b1234cd"));
}

void __W_TEST_SNPRINTF_NAME(_converts_many_unnumbered_arguments)(void)
{
  static __W_CHAR buf[30];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 30, __W_L("%d,%g;%x:%s"), 1234, 1.234, 0xabc1, "abcd"), 20);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234,1.234;abc1:abcd"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_that_is_unnumbered_argument)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%*d"), 6, 1234), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%*g"), 10, 1234.56), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("   1234.56"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%*c"), 4, 'a'), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("   a"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%*s"), 8, "abcd"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    abcd"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_precision_that_is_unnumbered_argument)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.*d"), 8, 1234), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("00001234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.*f"), 3, 123.456), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123.456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%.*s"), 4, "abcdef"), 4);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcd"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_and_precision_which_are_unnumbered_arguments)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%*.*d"), 8, 6, 1234), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  001234"));
}

void __W_TEST_SNPRINTF_NAME(_converts_many_numbered_arguments)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%4$d,%5$x;%1$s:%2$g:%3$c"), "abc", 1.234, 'c', 1234, 0xa1), 19);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234,a1;abc:1.234:c"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_that_is_numbered_argument)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%1$*2$d"), 1234, 8), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    1234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%1$*2$g"), 1234.5, 8), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("  1234.5"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%2$*1$c"), 6, 'a'), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("     a"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%2$*1$s"), 10, "abcdef"), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    abcdef"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_precision_that_is_numbered_argument)(void)
{
  static __W_CHAR buf[10];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%1$.*2$d"), 1234, 6), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("001234"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%1$.*2$f"), 12.3456, 4), 7);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("12.3456"));
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%2$.*1$s"), 6, "abcdefgh"), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("abcdef"));
}

void __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_and_precision_which_are_numbered_arguments)(void)
{
  static __W_CHAR buf[20];
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 20, __W_L("%1$*2$.*3$d"), 1234, 10, 6), 10);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("    001234"));
}

void __W_TEST_SNPRINTF_NAME(_gets_number_of_characters_for_n_conversion)(void)
{
  static __W_CHAR buf[10];
  int count;
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d%n%s"), 1234, &count, "abcd"), 8);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("1234abcd"));
  CU_ASSERT_EQUAL(count, 4);
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%d%n"), 123456, &count), 6);
  __W_CU_ASSERT_STRING_EQUAL(buf, __W_L("123456"));
  CU_ASSERT_EQUAL(count, 6);
}

void __W_TEST_SNPRINTF_NAME(_returns_minus_one_and_sets_errno_for_unsupported_conversion)(void)
{
  static __W_CHAR buf[10];
  errno = 0;
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%y"), 1234), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  CU_ASSERT_EQUAL(__W_SNPRINTF(buf, 10, __W_L("%z"), 5678), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

int __W_ADD_SUITE_NAME(snprintf)(void)
{
  CU_pSuite suite;
  suite = CU_add_suite(__W_SUITE_STRING(snprintf), __W_INITIALIZE_NAME(snprintf), __W_CLEANUP_NAME(snprintf));
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for d conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative integer number for d conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_integer_number_for_d_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for d conversion and + flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_plus_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for d conversion and ' ' flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_space_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for d conversion and # flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_d_conversion_and_hash_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for i conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_i_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative integer number for i conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_integer_number_for_i_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for u conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_u_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for o conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_o_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for o conversion and # flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_o_conversion_and_hash_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer zero for o conversion and # flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_zero_for_o_conversion_and_hash_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for x conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_x_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for x conversion and # flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_x_conversion_and_hash_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for X conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_uppercase_x_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for positive width"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_positive_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for positive width and precision"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_positive_width_and_precision)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for negative width"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_negative_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer zero"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for width and 0 flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_width_and_zero_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for ' flag"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_apostrophe_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for hh length"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_hh_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for h length"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_h_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for l length"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_l_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts integer number for ll length"),
    __W_TEST_SNPRINTF_NAME(_converts_integer_number_for_ll_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for f conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for F conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative floating-point number for f conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for f conversion and precision that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_precision_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for f conversion and carry"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_carry)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for f conversion and carry_idx that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_carry_idx_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for f conversion and digits that is 1"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_f_conversion_and_digits_that_is_one)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point zero for f conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_f_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for e conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for E conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative floating-point number for e conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for e conversion and precision that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_precision_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for e conversion and carry"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_carry)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for e conversion and carry_idx that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_e_conversion_and_carry_idx_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point zero for e conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_e_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for g conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for G conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative floating-point number for g conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for g conversion and is_e that is 1"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_is_e_that_is_one)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for g conversion and precision that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_precision_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for g conversion and carry"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_carry)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for g conversion and carry_idx that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_g_conversion_and_carry_idx_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point zero for g conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_g_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for a conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for A conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts negative floating-point number for a conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_negative_floating_point_number_for_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for a conversion and precision that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_precision_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for a conversion and carry"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_carry)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for a conversion and carry_idx that is zero"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_a_conversion_and_carry_idx_that_is_zero)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point zero for a conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_zero_for_a_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for positive width"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_positive_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for negative width"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_negative_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for + flag"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_plus_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for ' ' flag"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_space_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for precision"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_precision)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for width and 0 flag"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_width_and_zero_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for ' flag"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_apostrophe_flag)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts big floating-point number"),
    __W_TEST_SNPRINTF_NAME(_converts_big_floating_point_number)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts floating-point number for L length"),
    __W_TEST_SNPRINTF_NAME(_converts_floating_point_number_for_uppercase_l_length)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts character for c conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts character for c conversion and positive width"),
    __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion_and_positive_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts character for c conversion and negative width"),
    __W_TEST_SNPRINTF_NAME(_converts_character_for_c_conversion_and_negative_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide character for lc conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_lc_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide character for lc conversion and width"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_lc_conversion_and_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts character for C conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_character_for_uppercase_c_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts string for s conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts string for s conversion and positive width"),
    __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_positive_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts string for s conversion and negative width"),
    __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_negative_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts string for s conversion and precision"),
    __W_TEST_SNPRINTF_NAME(_converts_string_for_s_conversion_and_precision)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide string for ls conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide string for ls conversion and width"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion_and_width)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide string for ls conversion and precision"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_ls_conversion_and_precision)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts wide string for S conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_wide_string_for_uppercase_s_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts pointer for p conversion"),
    __W_TEST_SNPRINTF_NAME(_converts_pointer_for_p_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" prints characters from format"),
    __W_TEST_SNPRINTF_NAME(_prints_characters_from_format)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" prints percent from format"),
    __W_TEST_SNPRINTF_NAME(_prints_percent_from_format)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts many unnumbered arguments"),
    __W_TEST_SNPRINTF_NAME(_converts_many_unnumbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for width that is unnumbered argument"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_that_is_unnumbered_argument)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for precision that is unnumbered argument"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_precision_that_is_unnumbered_argument)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for width and precision which are unnumbered arguments"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_and_precision_which_are_unnumbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts many numbered arguments"),
    __W_TEST_SNPRINTF_NAME(_converts_many_numbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for width that is numbered argument"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_that_is_numbered_argument)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for precision that is numbered argument"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_precision_that_is_numbered_argument)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" converts argument for width and precision which are numbered arguments"),
    __W_TEST_SNPRINTF_NAME(_converts_argument_for_width_and_precision_which_are_numbered_arguments)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" gets number of characters for n conversion"),
    __W_TEST_SNPRINTF_NAME(_gets_number_of_characters_for_n_conversion)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_SNPRINTF_STRING(" returns -1 and sets errno for unsupported conversion"),
    __W_TEST_SNPRINTF_NAME(_returns_minus_one_and_sets_errno_for_unsupported_conversion)) == NULL) return -1;
  return 0;
}
