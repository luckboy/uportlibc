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
#include <CUnit/Basic.h>
#include <stddef.h>
#define __W W
#include "w_cunit.h"
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_uportlibc_name.h"
#include "w_test_name.h"

int __W_INITIALIZE_NAME(string)(void) { return 0; }

int __W_CLEANUP_NAME(string)(void) { return 0; }

void __W_TEST_MEM_NAME(chr, _returns_pointer_to_first_occurence_of_character)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abc\0dXeghXi\0jXk");
  CU_ASSERT_PTR_EQUAL(__W_MEM_NAME(chr)(str, __W_L('X'), 15), str + 5);
  str = __W_L("zyx1wv\0utsr1qp\0on");
  CU_ASSERT_PTR_EQUAL(__W_MEM_NAME(chr)(str, __W_L('1'), 17), str + 3);
  str = __W_L("24680");
  CU_ASSERT_PTR_EQUAL(__W_MEM_NAME(chr)(str, __W_L('2'), 5), str);
  str = __W_L("abcdefgh");
  CU_ASSERT_PTR_EQUAL(__W_MEM_NAME(chr)(str, __W_L('h'), 8), str + 7);
}

void __W_TEST_MEM_NAME(chr, _returns_null_pointer_for_character_that_does_not_occur)(void)
{
  CU_ASSERT_PTR_NULL(__W_MEM_NAME(chr)(__W_L("abcdef"), __W_L('g'), 6));
  CU_ASSERT_PTR_NULL(__W_MEM_NAME(chr)(__W_L("zyxw"), __W_L('v'), 4));
  CU_ASSERT_PTR_NULL(__W_MEM_NAME(chr)(__W_L("ABCDEFGH"), __W_L('I'), 8));
  CU_ASSERT_PTR_NULL(__W_MEM_NAME(chr)(__W_L("1234567"), __W_L('8'), 7));
}

#if __W == 'w'
void test_wmemchr_returns_pointer_for_wide_character_memory_area(void)
{
  wchar_t *str;
  str = L"a\u0104bcd\u0106e\u0118l\00000141";
  CU_ASSERT_PTR_EQUAL(uportlibc_wmemchr(str, L'\u0106', 10), str + 5);
  str = L"e\u0119l\u0142";
  CU_ASSERT_PTR_NULL(uportlibc_wmemchr(str, L'\u0118', 4));
}
#endif

void __W_TEST_MEM_NAME(cmp, _returns_zero_for_two_same_memory_areas)(void)
{
  static __W_CHAR str1[100];
  static __W_CHAR str2[100];
  __W_NATIVE_MEMCPY(str1, __W_L("abc\0def"), 7);
  __W_NATIVE_MEMCPY(str2, __W_L("abc\0def"), 7);
  CU_ASSERT_EQUAL(__W_MEM_NAME(cmp)(str1, str2, 7), 0);
  __W_NATIVE_MEMCPY(str1, __W_L("zyx\0wvut\0sr"), 11);
  __W_NATIVE_MEMCPY(str2, __W_L("zyx\0wvut\0sr"), 11);
  CU_ASSERT_EQUAL(__W_MEM_NAME(cmp)(str1, str2, 11), 0);
}

void __W_TEST_MEM_NAME(cmp, _returns_zero_for_two_same_pointers)(void)
{
  __W_CHAR_PTR str = __W_L("abcdef");
  CU_ASSERT_PTR_EQUAL(__W_MEM_NAME(cmp)(str, str, 6), 0);
}

void __W_TEST_MEM_NAME(cmp, _returns_negative_number_for_first_memory_area_that_is_lesser)(void)
{
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("ab\0cd2ef"), __W_L("ab\0cd3ef"), 8) < 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("zy\0xWvu\0tsr"), __W_L("zy\0xwvu\0tsr"), 10) < 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("233"), __W_L("234"), 3) < 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("AB1CDE"), __W_L("ab1cde"), 6) < 0);
}

void __W_TEST_MEM_NAME(cmp, _returns_positive_number_for_first_memory_area_that_is_greater)(void)
{
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("\0ab8cdef"), __W_L("\0ab4cdef"), 8) > 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("zyxwv2utrs"), __W_L("zyxwv1utrs"), 10) > 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("10102"), __W_L("10101"), 8) > 0);
  CU_ASSERT(__W_MEM_NAME(cmp)(__W_L("ab1cdefgh"), __W_L("AB1CDEFGH"), 9) > 0);
}

#if __W == 'w'
void test_wmemcmp_returns_number_for_two_wide_character_memory_areas(void)
{
  static wchar_t str1[100];
  static wchar_t str2[100];
  wmemcpy(str1, L"m\u0143n\0s\u015a", 6);
  wmemcpy(str2, L"m\u0143n\0s\u015a", 6);
  CU_ASSERT_EQUAL(uportlibc_wmemcmp(str1, str2, 6), 0);
  CU_ASSERT(uportlibc_wmemcmp(L"a\u03b1b\u03b2", L"a\u03b7b\u03b2", 4) < 0);
  CU_ASSERT(uportlibc_wmemcmp(L"\u041f\u03c0\u03c6\u03c4\u03c9", L"\u041f\u03c0\u03c4\u03c4\u03c9", 5) > 0);
}
#endif

void __W_TEST_MEM_NAME(cpy, _copies_memory_area)(void)
{
  static __W_CHAR buf[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(buf , __W_L('x'), 100);
  res = __W_MEM_NAME(cpy)(buf + 5, __W_L("abc\0def\0ghij"), 12);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxabc\0def\0ghijxxxxx"), 22), 0);
  __W_NATIVE_MEMSET(buf , __W_L('x'), 100);
  res = __W_MEM_NAME(cpy)(buf + 5, __W_L("zyxwvuts\0r"), 10);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxzyxwvuts\0rxxxxx"), 20), 0);
}

#if __W == 'w'
void test_wmemcpy_copies_wide_charater_memory_area(void)
{
  static wchar_t buf[100];
  wchar_t *res;
  wmemset(buf , L'x', 100);
  res = uportlibc_wmemcpy(buf + 5, L"\u03b1a\u03b2b\u03B3\0\u03b4c\u03b5d\u03b6", 11);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(wmemcmp(buf, L"xxxxx\u03b1a\u03b2b\u03B3\0\u03b4c\u03b5d\u03b6xxxxx", 21), 0);
}
#endif

void __W_TEST_MEM_NAME(move, _copies_memory_area)(void)
{
  static __W_CHAR buf[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  res = __W_MEM_NAME(move)(buf + 5, __W_L("abc\0d\0efgh"), 10);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxabc\0d\0efghxxxxx"), 10), 0);
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  res = __W_MEM_NAME(move)(buf + 5, __W_L("zyxwvuts"), 8);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxzyxwvutsxxxxx"), 18), 0);
}

void __W_TEST_MEM_NAME(move, _copies_left_memory_area_to_right_memory_area)(void)
{
  static __W_CHAR buf[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  __W_NATIVE_MEMCPY(buf, __W_L("abcdef"), 6);
  res = __W_MEM_NAME(move)(buf + 3, buf, 6);
  CU_ASSERT_PTR_EQUAL(res, buf + 3);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("abcabcdef"), 9), 0);
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  __W_NATIVE_MEMCPY(buf, __W_L("zyxwvuts"), 8);
  res = __W_MEM_NAME(move)(buf + 4, buf, 8);
  CU_ASSERT_PTR_EQUAL(res, buf + 4);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("zyxwzyxwvuts"), 12), 0);
}

void __W_TEST_MEM_NAME(move, _copies_right_memory_area_to_left_memory_area)(void)
{
  static __W_CHAR buf[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  __W_NATIVE_MEMCPY(buf + 3, __W_L("abcdef"), 6);
  res = __W_MEM_NAME(move)(buf, buf + 3, 6);
  CU_ASSERT_PTR_EQUAL(res, buf);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("abcdefdef"), 9), 0);
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  __W_NATIVE_MEMCPY(buf + 4, __W_L("zyxwvuts"), 8);
  res = __W_MEM_NAME(move)(buf, buf + 4, 8);
  CU_ASSERT_PTR_EQUAL(res, buf);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("zyxwvutsvuts"), 12), 0);
}

#if __W == 'w'
void test_wmemmove_copies_wide_charater_memory_area(void)
{
  static wchar_t buf[100];
  wchar_t *res;
  wmemset(buf , L'x', 100);
  res = uportlibc_wmemmove(buf + 5, L"a\u2190b\0c\0\u2191d", 8);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(wmemcmp(buf, L"xxxxxa\u2190b\0c\0\u2191dxxxxx", 18), 0);
}
#endif

void __W_TEST_MEM_NAME(set, _fills_memory_area)(void)
{
  static __W_CHAR buf[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  res = __W_MEM_NAME(set)(buf + 5, __W_L('a'), 5);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxaaaaaxxxxx"), 15), 0);
  __W_NATIVE_MEMSET(buf, __W_L('x'), 100);
  res = __W_MEM_NAME(set)(buf + 5, __W_L('z'), 8);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(__W_NATIVE_MEMCMP(buf, __W_L("xxxxxzzzzzzzzxxxxx"), 18), 0);
}

#if __W == 'w'
void test_wmemset_fills_wide_character_memory_area(void)
{
  static wchar_t buf[100];
  wchar_t *res;
  wmemset(buf, L'x', 100);
  res = uportlibc_wmemset(buf + 5, L'\u0105', 5);
  CU_ASSERT_PTR_EQUAL(res, buf + 5);
  CU_ASSERT_EQUAL(wmemcmp(buf, L"xxxxx\u0105\u0105\u0105\u0105\u0105xxxxx", 15), 0);
}
#endif

void __W_TEST_STR_NAME(cat, _concatenates_two_strings)(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("abcd"));
  res = __W_STR_NAME(cat)(str1, __W_L("efgh"));
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefgh"));
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("zyxwvu"));
  res = __W_STR_NAME(cat)(str1, __W_L("tsvrqp"));
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvutsvrqp"));
}

#if __W == 'w'
void test_wcscat_concatenates_two_wide_character_strings(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  wmemset(str1, L'x', 100);
  wcscpy(str1, L"a\u0104bC\u0107");
  res = uportlibc_wcscat(str1, L"de\u0119f");
  CU_ASSERT_PTR_EQUAL(res, str1);
  CU_ASSERT_EQUAL(wcscmp(res, L"a\u0104bC\u0107de\u0119f"), 0);
}
#endif

void __W_TEST_STR_NAME(chr, _returns_pointer_to_first_occurence_of_character)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abcdXefghXijklX");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('X')), str + 4);
  str = __W_L("zy4xwv4utsr4q");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('4')), str + 2);
  str = __W_L("12345678");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('8')), str + 7);
  str = __W_L("ABCDeF");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('e')), str + 4);
}

void __W_TEST_STR_NAME(chr, _returns_null_pointer_for_character_that_does_not_occur)(void)
{
  CU_ASSERT_PTR_NULL(__W_STR_NAME(chr)(__W_L("abcdefgh"), __W_L('i')));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(chr)(__W_L("zyxwvu"), __W_L('t')));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(chr)(__W_L("123456789"), __W_L('0')));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(chr)(__W_L("ABCDEF"), __W_L('G')));
}

void __W_TEST_STR_NAME(chr, _returns_pointer_to_null_terminater_for_null_terminator)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abcdefgh");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('\0')), str + 8);
  str = __W_L("zyxwvutsrq");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(chr)(str, __W_L('\0')), str + 10);
}

#if __W == 'w'
void test_wcschr_returns_pointer_for_wide_character_string(void)
{
  wchar_t *str;
  str = L"\u017b\u0179zyxwvut\u015a";
  CU_ASSERT_PTR_EQUAL(uportlibc_wcschr(str, L'\u0179'), str + 1);
  CU_ASSERT_PTR_NULL(uportlibc_wcschr(L"\u03c6\u03c8\u03c0\u03b7", L'\u03b1'));
}
#endif

void __W_TEST_STR_NAME(cmp, _returns_zero_for_two_same_strings)(void)
{
  static __W_CHAR str1[100];
  static __W_CHAR str2[100];
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_MEMSET(str2, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("abcd"));
  __W_NATIVE_STRCPY(str2, __W_L("abcd"));
  CU_ASSERT_EQUAL(__W_STR_NAME(cmp)(str1, str2), 0);
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_MEMSET(str2, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("zyxwvu"));
  __W_NATIVE_STRCPY(str2, __W_L("zyxwvu"));
  CU_ASSERT_EQUAL(__W_STR_NAME(cmp)(str1, str2), 0);
}

void __W_TEST_STR_NAME(cmp, _returns_zero_for_two_same_pointers)(void)
{
  __W_CHAR_PTR str = __W_L("abcdefgh");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(cmp)(str, str), 0);
}

void __W_TEST_STR_NAME(cmp, _returns_negative_number_for_first_string_that_is_lesser)(void)
{
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("abcd1efgh"), __W_L("abcd2efgh")) < 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("zyxWvutsrqp"), __W_L("zyxwvutsrqp")) < 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("12345"), __W_L("12346")) < 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("ABCDEF"), __W_L("abcdef")) < 0);
}

void __W_TEST_STR_NAME(cmp, _returns_positive_number_for_first_string_that_is_greater)(void)
{
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("ab2cdefgh"), __W_L("ab1cdefgh")) > 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("zyxwvut"), __W_L("zYxwvut")) > 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("2469"), __W_L("2468")) > 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("abcdefgh"), __W_L("ABCDEFGH")) > 0);
}

void __W_TEST_STR_NAME(cmp, _returns_negative_number_for_first_string_that_is_shorter)(void)
{
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("abcdef"), __W_L("abcdefgh")) < 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("zyxwvuts"), __W_L("zyxwvutsrq")) < 0);
}

void __W_TEST_STR_NAME(cmp, _returns_positive_number_for_first_string_that_is_longer)(void)
{
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("abcdefgh"), __W_L("abcd")) > 0);
  CU_ASSERT(__W_STR_NAME(cmp)(__W_L("zyxwvu"), __W_L("zyx")) > 0);
}

#if __W == 'w'
void test_wcscmp_returns_number_for_two_wide_character_strings(void)
{
  static wchar_t str1[100];
  static wchar_t str2[100];
  wmemset(str1, L'x', 100);
  wmemset(str2, L'x', 100);
  wcscpy(str1, L"a\u03b1b\u03b2");
  wcscpy(str2, L"a\u03b1b\u03b2");
  CU_ASSERT_EQUAL(uportlibc_wcscmp(str1, str2), 0);
  CU_ASSERT(uportlibc_wcscmp(L"a\u0105bc\u0106def", L"a\u0105bc\u0107def") < 0);
  CU_ASSERT(uportlibc_wcscmp(L"\u017bzyx\u015a", L"\u017bzyx\u0143") > 0);
}
#endif

void __W_TEST_STR_NAME(cpy, _copies_string)(void)
{
  static __W_CHAR dst[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  res = __W_STR_NAME(cpy)(dst, __W_L("abcdefghijkl"));
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefghijkl"));
}

#if __W == 'w'
void test_wcscpy_copies_wide_character_string(void)
{
  static wchar_t dst[100];
  wchar_t *res;
  wmemset(dst, L'x', 100);
  res = uportlibc_wcscpy(dst, L"abc\u03b1def\u03bcghijkl\u03c0");
  CU_ASSERT_PTR_EQUAL(res, dst);
  CU_ASSERT_EQUAL(wcscmp(res, L"abc\u03b1def\u03bcghijkl\u03c0"), 0);
}
#endif

void __W_TEST_STR_NAME(cspn, _returns_initial_substring_length_for_string_that_has_reject_characters)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(cspn)(__W_L("abcdef123415678290"), __W_L("142")), 6U);
  CU_ASSERT_EQUAL(__W_STR_NAME(cspn)(__W_L("zyxwabcdef"), __W_L("abcdef")), 4U);
}

void __W_TEST_STR_NAME(cspn, _returns_string_length_for_string_that_has_not_reject_characters)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(cspn)(__W_L("abcdefghijklmn"), __W_L("1234567890")), 14U);
  CU_ASSERT_EQUAL(__W_STR_NAME(cspn)(__W_L("zyxwvutsrq"), __W_L("abcdef")), 10U);
}

#if __W == 'w'
void test_wcscspn_returns_number_for_wide_character_strings(void)
{
  CU_ASSERT_EQUAL(uportlibc_wcscspn(L"abc\u2190\u2191\u2192\u2193", L"\u2191\u2192"), 4U);
  CU_ASSERT_EQUAL(uportlibc_wcscspn(L"\u0104\u0106\u0118\u0141nm", L"\u0179\u017b"), 6U);
}
#endif

void __W_TEST_STR_NAME(len, _returns_string_length)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(len)(__W_L("abcdefghij")), 10U);
  CU_ASSERT_EQUAL(__W_STR_NAME(len)(__W_L("zyxwvu")), 6U);
  CU_ASSERT_EQUAL(__W_STR_NAME(len)(__W_L("12345678")), 8U);
  CU_ASSERT_EQUAL(__W_STR_NAME(len)(__W_L("ABCD")), 4U);
}

#if __W == 'w'
void test_wcslen_returns_wide_character_string_length(void)
{
  CU_ASSERT_EQUAL(uportlibc_wcslen(L"a\u0105bc\u0107de\u0119fgh"), 11U);
  CU_ASSERT_EQUAL(uportlibc_wcslen(L"\u017c\u017azyxwvut\u015bsrqp\u00f3o"), 16U);
}
#endif

void __W_TEST_STR_NAME(ncat, _concatenates_two_strings)(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  __W_NATIVE_STRCPY(str1, __W_L("abcdef"));
  res = __W_STR_NAME(ncat)(str1, __W_L("ghij"), 20);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefghij"));
  __W_NATIVE_STRCPY(str1, __W_L("zyxw"));
  res = __W_STR_NAME(ncat)(str1, __W_L("vuts"), 20);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvuts"));
}

void __W_TEST_STR_NAME(ncat, _appends_null_terminator_for_too_long_second_string)(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("abcde"));
  __W_NATIVE_MEMSET(str1 + 6, __W_L('X'), 4);
  __W_NATIVE_STRCPY(str1 + 10, __W_L("12345"));
  res = __W_STR_NAME(ncat)(str1, __W_L("fghij"), 5);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefghij"));
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("zyxw"));
  __W_NATIVE_MEMSET(str1 + 5, __W_L('X'), 3);
  __W_NATIVE_STRCPY(str1 + 12, __W_L("1234"));
  res = __W_STR_NAME(ncat)(str1, __W_L("vutsrqpo"), 8);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvutsrqpo"));
}

void __W_TEST_STR_NAME(ncat, _does_not_append_characters_more_than_limit)(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("abcd"));
  __W_NATIVE_MEMSET(str1 + 5, __W_L('X'), 7);
  __W_NATIVE_STRCPY(str1 + 12, __W_L("123456"));
  res = __W_STR_NAME(ncat)(str1, __W_L("efghijklmn"), 8);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefghijkl"));
  __W_NATIVE_MEMSET(str1, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str1, __W_L("zyxwv"));
  __W_NATIVE_MEMSET(str1 + 6, __W_L('X'), 4);
  __W_NATIVE_STRCPY(str1 + 10, __W_L("12345"));
  res = __W_STR_NAME(ncat)(str1, __W_L("utsrqp"), 5);
  CU_ASSERT_PTR_EQUAL(res, str1);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvutsrq"));
}

#if __W == 'w'
void test_wcsncat_concatenates_two_wide_character_strings(void)
{
  static __W_CHAR str1[100];
  __W_CHAR_PTR res;
  wcscpy(str1, L"\u0105b\u0107d\u0119f");
  res = uportlibc_wcsncat(str1, L"\u0104C\u0143\u015a", 20);
  CU_ASSERT_PTR_EQUAL(res, str1);
  CU_ASSERT_EQUAL(wcscmp(res, L"\u0105b\u0107d\u0119f\u0104C\u0143\u015a"), 0);
}
#endif

void __W_TEST_STR_NAME(ncmp, _returns_zero_for_two_same_strings)(void)
{
  static __W_CHAR str1[100];
  static __W_CHAR str2[100];
  __W_NATIVE_STRCPY(str1, __W_L("abcdefghij"));
  __W_NATIVE_STRCPY(str2, __W_L("abcdefghij"));
  CU_ASSERT_EQUAL(__W_STR_NAME(ncmp)(str1, str2, 20), 0);
  __W_NATIVE_STRCPY(str1, __W_L("zyxwvuts"));
  __W_NATIVE_STRCPY(str2, __W_L("zyxwvuts"));
  CU_ASSERT_EQUAL(__W_STR_NAME(ncmp)(str1, str2, 20), 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_zero_for_two_same_pointers)(void)
{
  __W_CHAR_PTR str = __W_L("abcdef");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(ncmp)(str, str, 20), 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_that_is_lesser)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("ab2cdefgh"), __W_L("ab4cdefgh"), 20) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwvuTs"), __W_L("zyxwvuts"), 20) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("123455"), __W_L("123456"), 20) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("ABCD"), __W_L("abcd"), 20) < 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_first_string_that_is_greater)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcd9efgh"), __W_L("abcd1efgh"), 20) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwvutsrp"), __W_L("zyXwvutsrp"), 20) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("123457"), __W_L("123455"), 20) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcdef"), __W_L("ABCDEF"), 20) > 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_that_is_shorter)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcd"), __W_L("abcdef"), 20) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwv"), __W_L("zyxwvuts"), 20) < 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_first_string_that_is_longer)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcde"), __W_L("abcd"), 20) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwvutsrq"), __W_L("zyxwvu"), 20) > 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_zero_for_same_first_characters_of_two_strings)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(ncmp)(__W_L("abcdef12a3b4c"), __W_L("abcdef23a4b5c"), 4), 0);
  CU_ASSERT_EQUAL(__W_STR_NAME(ncmp)(__W_L("zyxwvu2222"), __W_L("zyxwvu4444"), 6), 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_lesser_initial_substring_of_first_string)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abc8defghij"), __W_L("abc9defghij"), 6) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zy1xwvutsrq"), __W_L("zy2xwvuts"), 4) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("1234567890"), __W_L("234567891"), 8) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("ABCDEFGH"), __W_L("abcdefghij"), 5) < 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_greater_initial_substring_of_first_string)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcde7fghij"), __W_L("abcde6fghij"), 8) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwv5utsrqpo"), __W_L("zyxwv2utsrqpo"), 6) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("423456789"), __W_L("2234578"), 4) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcdefgh"), __W_L("ABCDEF"), 5) > 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_length_less_than_limit)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcd"), __W_L("abcdefghij"), 6) < 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwvu"), __W_L("zyxwvuts"), 8) < 0);
}

void __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_second_string_length_less_than_limit)(void)
{
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("abcdefghij"), __W_L("abcdef"), 8) > 0);
  CU_ASSERT(__W_STR_NAME(ncmp)(__W_L("zyxwvu"), __W_L("zyxw"), 6) > 0);
}

#if __W == 'w'
void test_wcsncmp_returns_number_for_two_wide_character_strings(void)
{
  static wchar_t str1[100];
  static wchar_t str2[100];
  wcscpy(str1, L"\u2190a\u2191b\u2192c");
  wcscpy(str2, L"\u2190a\u2191b\u2192c");
  CU_ASSERT_EQUAL(uportlibc_wcsncmp(str1, str2, 20), 0);
  CU_ASSERT(uportlibc_wcsncmp(L"\u03b1ab\u03b2c\u03bfd", L"\u03b1ab\u03d5c\u03bfd", 20) < 0);
  CU_ASSERT(uportlibc_wcsncmp(L"z\u0105y\u010ax", L"z\u0104y\u0105x", 20) > 0);
}
#endif

void __W_TEST_STR_NAME(ncpy, _copies_string)(void)
{
  static __W_CHAR dst[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  res = __W_STR_NAME(ncpy)(dst, __W_L("abcdefghijkl"), 20U);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdefghijkl"));
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  res = __W_STR_NAME(ncpy)(dst, __W_L("zyxwvuts"), 20U);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvuts"));
}

void __W_TEST_STR_NAME(ncpy, _does_not_copy_null_terminator_for_too_long_string)(void)
{
  static __W_CHAR dst[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  __W_NATIVE_MEMSET(dst, __W_L('X'), 6U);
  __W_NATIVE_STRCPY(dst + 6, __W_L("12345"));
  res = __W_STR_NAME(ncpy)(dst, __W_L("abcdef"), 6);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcdef12345"));
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  __W_NATIVE_MEMSET(dst, __W_L('X'), 4U);
  __W_NATIVE_STRCPY(dst + 4, __W_L("12345"));
  res = __W_STR_NAME(ncpy)(dst, __W_L("zyxw"), 4);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxw12345"));
}

void __W_TEST_STR_NAME(ncpy, _does_not_copy_characters_more_than_limit)(void)
{
  static __W_CHAR dst[100];
  __W_CHAR_PTR res;
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  __W_NATIVE_MEMSET(dst, __W_L('X'), 4U);
  __W_NATIVE_STRCPY(dst + 4, __W_L("12345678"));
  res = __W_STR_NAME(ncpy)(dst, __W_L("abcdefgh"), 4);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("abcd12345678"));
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  __W_NATIVE_MEMSET(dst, __W_L('X'), 6U);
  __W_NATIVE_STRCPY(dst + 6, __W_L("123456"));
  res = __W_STR_NAME(ncpy)(dst, __W_L("zyxwvutsrq"), 6);
  CU_ASSERT_PTR_EQUAL(res, dst);
  __W_CU_ASSERT_STRING_EQUAL(res, __W_L("zyxwvu123456"));
}

#if __W == 'w'
void test_wcsncpy_copies_wide_character_string(void)
{
  static wchar_t dst[100];
  wchar_t *res;
  wmemset(dst, L'x', 100);
  res = uportlibc_wcsncpy(dst, L"a\u0105c\u0107e\u0119l\u0142n\u0144o\u00f3s\u015b", 20U);
  CU_ASSERT_PTR_EQUAL(res, dst);
  CU_ASSERT_EQUAL(wcscmp(res, L"a\u0105c\u0107e\u0119l\u0142n\u0144o\u00f3s\u015b"), 0);
}
#endif

void __W_TEST_STR_NAME(pbrk, _returns_pointer_to_first_occurence_of_accept_character)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abcdefgh");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(pbrk)(str, __W_L("dfh")), str + 3);
  str = __W_L("zyxwvuts");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(pbrk)(str, __W_L("xwv")), str + 2);
}

void __W_TEST_STR_NAME(pbrk, _returns_null_pointer_for_string_that_has_not_accept_characters)(void)
{
  CU_ASSERT_PTR_NULL(__W_STR_NAME(pbrk)(__W_L("abcdefgh"), __W_L("1234")));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(pbrk)(__W_L("zyxwvu"), __W_L("abdef")));
}

#if __W == 'w'
void test_wcspbrk_returns_pointer_for_wide_character_strings(void)
{
  wchar_t *str;
  str = L"a\u0105bc\u0107de\u0119fgh";
  CU_ASSERT_PTR_EQUAL(uportlibc_wcspbrk(str, __W_L("\u0107de\u0119")), str + 4);
  CU_ASSERT_PTR_NULL(uportlibc_wcspbrk(L"\u017c\u017azyxwvut\u015bs", L"a\u0105bc\u0107"));
}
#endif

void __W_TEST_STR_NAME(rchr, _returns_pointer_to_last_occurence_of_character)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abXcXdeXfgh");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('X')), str + 7);
  str = __W_L("zy2xw2vuts");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('2')), str + 5);
  str = __W_L("4234");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('4')), str + 3);
  str = __W_L("ABcDEF");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('c')), str + 2);
}

void __W_TEST_STR_NAME(rchr, _returns_null_pointer_for_character_that_does_not_occur)(void)
{
  CU_ASSERT_PTR_NULL(__W_STR_NAME(rchr)(__W_L("abcdef"), __W_L('g')));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(rchr)(__W_L("zyxw"), __W_L('v')));
}

void __W_TEST_STR_NAME(rchr, _returns_pointer_to_null_terminater_for_null_terminator)(void)
{
  __W_CHAR_PTR str, res;
  str = __W_L("abcdefghij");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('\0')), str + 10);
  str = __W_L("zyxwvuts");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(rchr)(str, __W_L('\0')), str + 8);
}

#if __W == 'w'
void test_wcsrchr_returns_pointer_for_wide_character_string(void)
{
  wchar_t *str;
  str = L"a\u0105bc\u0107de\u0119fg";
  CU_ASSERT_PTR_EQUAL(uportlibc_wcsrchr(str, L'\u0107'), str + 4);
  CU_ASSERT_PTR_NULL(uportlibc_wcsrchr(L"\u017c\u017azyxwvut\u015bs", L'\u0119'));
}
#endif

void __W_TEST_STR_NAME(spn, _returns_initial_substring_length_for_string_that_has_not_accept_characters)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(spn)(__W_L("abcdef123456"), __W_L("abcedfghijklm")), 6U);
  CU_ASSERT_EQUAL(__W_STR_NAME(spn)(__W_L("zyxwabcdef"), __W_L("zyxwvutsrq")), 4U);
}

void __W_TEST_STR_NAME(spn, _returns_string_length_for_string_that_just_has_accept_characters)(void)
{
  CU_ASSERT_EQUAL(__W_STR_NAME(spn)(__W_L("abcdeabcde"), __W_L("abcdefghij")), 10U);
  CU_ASSERT_EQUAL(__W_STR_NAME(spn)(__W_L("zyxwvuts"), __W_L("zyxwvutsrq")), 8U);
}

#if __W == 'w'
void test_wcsspn_returns_number_for_wide_character_strings(void)
{
  CU_ASSERT_EQUAL(uportlibc_wcsspn(L"\u2190\u2191\u2192\u2193\u2194abcd", L"\u2190\u2191\u2192\u2193\u2194\u2195"), 5U);
  CU_ASSERT_EQUAL(uportlibc_wcsspn(L"\u03b1\u03b2\u03b3\u03b4", L"\u03b1\u03b2\u03B3\u03b4\u03b5"), 4U);
}
#endif

void __W_TEST_STR_NAME(str, _returns_pointer_to_first_occurence_of_searching_string)(void)
{
  __W_CHAR_PTR str;
  str = __W_L("abcdefghdef");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(str)(str, __W_L("def")), str + 3);
  str = __W_L("zyxwvutsrqutsrq");
  CU_ASSERT_PTR_EQUAL(__W_STR_NAME(str)(str, __W_L("uts")), str + 5);
}

void __W_TEST_STR_NAME(str, _returns_null_pointer_for_searching_string_that_does_not_occur)(void)
{
  CU_ASSERT_PTR_NULL(__W_STR_NAME(str)(__W_L("abcdef"), __W_L("123456")));
  CU_ASSERT_PTR_NULL(__W_STR_NAME(str)(__W_L("zyxwvuts"), __W_L("abcdef")));
}

#if __W == 'w'
void test_wcsstr_returns_pointer_for_wide_character_strings(void)
{
  wchar_t *str;
  str = L"a\u0105bc\u0107de\u0119fghde\u0119f";
  CU_ASSERT_PTR_EQUAL(uportlibc_wcsstr(str, L"\u0119fgh"), str + 7);
  CU_ASSERT_PTR_NULL(uportlibc_wcsstr(L"\u2190\u2191\u2192", L"\u03b1\u03b2\u03b3"));
}
#endif

void __W_TEST_STRTOK_R_NAME(_splits_string)(void)
{
  static __W_CHAR str[100];
  __W_CHAR_PTR tmp_ptr;
  __W_NATIVE_MEMSET(str, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str, __W_L(" abc  def\n \tgh  "));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(str, __W_L(" \t\n"), &tmp_ptr), str + 1);
  __W_CU_ASSERT_STRING_EQUAL(str + 1, __W_L("abc"));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(NULL, __W_L(" \t\n"), &tmp_ptr), str + 6);
  __W_CU_ASSERT_STRING_EQUAL(str + 6, __W_L("def"));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(NULL, __W_L(" \t\n"), &tmp_ptr), str + 12);
  __W_CU_ASSERT_STRING_EQUAL(str + 12, __W_L("gh"));
  CU_ASSERT_PTR_NULL(__W_STRTOK_R(NULL, __W_L(" \t\n"), &tmp_ptr));
  __W_NATIVE_MEMSET(str, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str, __W_L("zyx90wvuts1234rqpo"));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(str, __W_L("1234567890"), &tmp_ptr), str);
  __W_CU_ASSERT_STRING_EQUAL(str, __W_L("zyx"));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(NULL, __W_L("1234567890"), &tmp_ptr), str + 5);
  __W_CU_ASSERT_STRING_EQUAL(str + 5, __W_L("wvuts"));
  CU_ASSERT_PTR_EQUAL(__W_STRTOK_R(NULL, __W_L("1234567890"), &tmp_ptr), str + 14);
  __W_CU_ASSERT_STRING_EQUAL(str + 14, __W_L("rqpo"));
  CU_ASSERT_PTR_NULL(__W_STRTOK_R(NULL, __W_L("1234567890"), &tmp_ptr));
}

void __W_TEST_STRTOK_R_NAME(_does_not_split_string_that_has_not_tokens)(void)
{
  static __W_CHAR str[100];
  __W_CHAR_PTR tmp_ptr;
  __W_NATIVE_MEMSET(str, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str, __W_L(" \t\n"));
  CU_ASSERT_PTR_NULL(__W_STRTOK_R(str, __W_L(" \t\n"), &tmp_ptr));
  __W_NATIVE_MEMSET(str, __W_L('x'), 100);
  __W_NATIVE_STRCPY(str, __W_L(""));
  CU_ASSERT_PTR_NULL(__W_STRTOK_R(str, __W_L(" \t\n"), &tmp_ptr));
}

#if __W == 'w'
void test_wcstok_splits_wide_character_string(void)
{
  static wchar_t str[100];
  wchar_t *tmp_ptr;
  wmemset(str, L'x', 100);
  wcscpy(str, __W_L("a\u0105b\u03b1\u03b2c\u0107de\u03b3\u03b2\u0119fgh"));
  CU_ASSERT_PTR_EQUAL(uportlibc_wcstok(str, L"\u03b1\u03b2\u03b3", &tmp_ptr), str);
  CU_ASSERT_EQUAL(wcscmp(str, L"a\u0105b"), 0);
  CU_ASSERT_PTR_EQUAL(uportlibc_wcstok(NULL, L"\u03b1\u03b2\u03b3", &tmp_ptr), str + 5);
  CU_ASSERT_EQUAL(wcscmp(str + 5, L"c\u0107de"), 0);
  CU_ASSERT_PTR_EQUAL(uportlibc_wcstok(NULL, L"\u03b1\u03b2\u03b3", &tmp_ptr), str + 11);
  CU_ASSERT_EQUAL(wcscmp(str + 11, L"\u0119fgh"), 0);
  CU_ASSERT_PTR_NULL(uportlibc_wcstok(NULL, L"\u03b1\u03b2\u03b3", &tmp_ptr));
}
#endif

void __W_TEST_STR_NAME(xfrm, _transforms_string)(void)
{
  static __W_CHAR dst[100];
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  CU_ASSERT_EQUAL(__W_STR_NAME(xfrm)(dst, __W_L("abcdefgh"), 10U), 8U);
  __W_CU_ASSERT_STRING_EQUAL(dst, __W_L("abcdefgh"));
  __W_NATIVE_MEMSET(dst, __W_L('x'), 100);
  CU_ASSERT_EQUAL(__W_STR_NAME(xfrm)(dst, __W_L("zyxwvu"), 8U), 6U);
  __W_CU_ASSERT_STRING_EQUAL(dst, __W_L("zyxwvu"));
}

#if __W == 'w'
void test_wcsxfrm_transforms_wide_character_string(void)
{
  static wchar_t dst[100];
  wmemset(dst, L'x', 100);
  CU_ASSERT_EQUAL(wcsxfrm(dst, L"a\u0105bc\u0107de\u0119fgh", 15U), 11U);
  CU_ASSERT_EQUAL(wcscmp(dst, L"a\u0105bc\u0107de\u0119fgh"), 0);
}
#endif

int __W_ADD_SUITE_NAME(string)(void)
{
  CU_pSuite suite;
  suite = CU_add_suite(__W_SUITE_STRING(string), __W_INITIALIZE_NAME(string), __W_CLEANUP_NAME(string));
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(chr, " returns pointer to first occurence of character"),
    __W_TEST_MEM_NAME(chr, _returns_pointer_to_first_occurence_of_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(chr, " returns null pointer for character that does not occur"),
    __W_TEST_MEM_NAME(chr, _returns_null_pointer_for_character_that_does_not_occur)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wmemchr returns pointer for wide character memory area",
    test_wmemchr_returns_pointer_for_wide_character_memory_area) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(cmp, " returns zero for two same memory areas"),
    __W_TEST_MEM_NAME(cmp, _returns_zero_for_two_same_memory_areas)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(cmp, " returns zero for two same pointers"),
    __W_TEST_MEM_NAME(cmp, _returns_zero_for_two_same_pointers)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(cmp, " returns negative number for first memory area that is lesser"),
    __W_TEST_MEM_NAME(cmp, _returns_negative_number_for_first_memory_area_that_is_lesser)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(cmp, " returns positive number for first memory area that is greater"),
    __W_TEST_MEM_NAME(cmp, _returns_positive_number_for_first_memory_area_that_is_greater)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wmemcmp returns number for two wide character memory areas",
    test_wmemcmp_returns_number_for_two_wide_character_memory_areas) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(cpy, " copies memory area"),
    __W_TEST_MEM_NAME(cpy, _copies_memory_area)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(move, " copies memory area"),
    __W_TEST_MEM_NAME(move, _copies_memory_area)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(move, " copies left memory area to right memory area"),
    __W_TEST_MEM_NAME(move, _copies_left_memory_area_to_right_memory_area)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(move, " copies right memory area to left memory area"),
    __W_TEST_MEM_NAME(move, _copies_right_memory_area_to_left_memory_area)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wmemmove copies wide charater memory area",
    test_wmemmove_copies_wide_charater_memory_area) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_MEM_STRING(set, " fills memory area"),
    __W_TEST_MEM_NAME(set, _fills_memory_area)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wmemset fills wide character memory area",
    test_wmemset_fills_wide_character_memory_area) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cat, " concatenates two strings"),
    __W_TEST_STR_NAME(cat, _concatenates_two_strings)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcscat concatenates two wide character strings",
    test_wcscat_concatenates_two_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(chr, " returns pointer to first occurence of character"),
    __W_TEST_STR_NAME(chr, _returns_pointer_to_first_occurence_of_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(chr, " returns null pointer for character that does not occur"),
    __W_TEST_STR_NAME(chr, _returns_null_pointer_for_character_that_does_not_occur)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(chr, " returns pointer to null terminator for null terminator"),
    __W_TEST_STR_NAME(chr, _returns_pointer_to_null_terminater_for_null_terminator)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcschr returns pointer for wide character string",
    test_wcschr_returns_pointer_for_wide_character_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns zero for two same strings"),
    __W_TEST_STR_NAME(cmp, _returns_zero_for_two_same_strings)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns zero for two same pointers"),
    __W_TEST_STR_NAME(cmp, _returns_zero_for_two_same_pointers)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns negative number for first string that is lesser"),
    __W_TEST_STR_NAME(cmp, _returns_negative_number_for_first_string_that_is_lesser)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns positive number for first string that is greater"),
    __W_TEST_STR_NAME(cmp, _returns_positive_number_for_first_string_that_is_greater)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns negative number for first string taht is shorter"),
    __W_TEST_STR_NAME(cmp, _returns_negative_number_for_first_string_that_is_shorter)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cmp, " returns positive number for first string that is longer"),
    __W_TEST_STR_NAME(cmp, _returns_positive_number_for_first_string_that_is_longer)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcscmp returns number for two wide character strings",
    test_wcscmp_returns_number_for_two_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cpy, " copies string"),
    __W_TEST_STR_NAME(cpy, _copies_string)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcscpy copies wide character string",
    test_wcscpy_copies_wide_character_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cspn, " returns initial substring length for string that has reject characters"),
    __W_TEST_STR_NAME(cspn, _returns_initial_substring_length_for_string_that_has_reject_characters)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(cspn, " returns string length for string that has not reject characters"),
    __W_TEST_STR_NAME(cspn, _returns_string_length_for_string_that_has_not_reject_characters)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcscspn returns number for wide character strings",
    test_wcscspn_returns_number_for_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(len, " returns string length"),
    __W_TEST_STR_NAME(len, _returns_string_length)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcslen returns wide character string length",
    test_wcslen_returns_wide_character_string_length) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncat, " concatenates two strings"),
    __W_TEST_STR_NAME(ncat, _concatenates_two_strings)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncat, " appends null terminator for too long second string"),
    __W_TEST_STR_NAME(ncat, _appends_null_terminator_for_too_long_second_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncat, " does not append characters more than limit"),
    __W_TEST_STR_NAME(ncat, _does_not_append_characters_more_than_limit)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsncat concatenates two wide character strings",
    test_wcsncat_concatenates_two_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns zero for two same strings"),
    __W_TEST_STR_NAME(ncmp, _returns_zero_for_two_same_strings)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns zero for two same pointers"),
    __W_TEST_STR_NAME(ncmp, _returns_zero_for_two_same_pointers)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns negative number for first string that is lesser"),
    __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_that_is_lesser)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns positive number for first string that is greater"),
    __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_first_string_that_is_greater)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns negative number for first string taht is shorter"),
    __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_that_is_shorter)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns positive number for first string that is longer"),
    __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_first_string_that_is_longer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns zero for same first characters of two strings"),
    __W_TEST_STR_NAME(ncmp, _returns_zero_for_same_first_characters_of_two_strings)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns negative number for lesser initial substring of first string"),
    __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_lesser_initial_substring_of_first_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns positive number for greater initial substring of first string"),
    __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_greater_initial_substring_of_first_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns negative number for first string length less than limit"),
    __W_TEST_STR_NAME(ncmp, _returns_negative_number_for_first_string_length_less_than_limit)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncmp, " returns positive number for second string length less than limit"),
    __W_TEST_STR_NAME(ncmp, _returns_positive_number_for_second_string_length_less_than_limit)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsncmp returns number for two wide character strings",
    test_wcsncmp_returns_number_for_two_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncpy, " copies string"),
    __W_TEST_STR_NAME(ncpy, _copies_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncpy, " does not copy null terminator for too long string"),
    __W_TEST_STR_NAME(ncpy, _does_not_copy_null_terminator_for_too_long_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(ncpy, " does not copy characters more than limit"),
    __W_TEST_STR_NAME(ncpy, _does_not_copy_characters_more_than_limit)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsncpy copies wide character string",
    test_wcsncpy_copies_wide_character_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(pbrk, " returns pointer to first occurence of accept character"),
    __W_TEST_STR_NAME(pbrk, _returns_pointer_to_first_occurence_of_accept_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(pbrk, " returns null pointer for string that has not accept characters"),
    __W_TEST_STR_NAME(pbrk, _returns_null_pointer_for_string_that_has_not_accept_characters)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcspbrk returns pointer for wide character strings",
    test_wcspbrk_returns_pointer_for_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(rchr, " returns pointer to last occurence of character"),
    __W_TEST_STR_NAME(rchr, _returns_pointer_to_last_occurence_of_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(rchr, " returns null pointer for character that does not occur"),
    __W_TEST_STR_NAME(rchr, _returns_null_pointer_for_character_that_does_not_occur)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(rchr, " returns pointer to null terminater for null terminator"),
    __W_TEST_STR_NAME(rchr, _returns_pointer_to_null_terminater_for_null_terminator)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsrchr returns pointer for wide character string",
    test_wcsrchr_returns_pointer_for_wide_character_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(spn, " returns initial substring length for string that has not accept characters"),
    __W_TEST_STR_NAME(spn, _returns_initial_substring_length_for_string_that_has_not_accept_characters)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(spn, " returns string length for string that just has accept characters"),
    __W_TEST_STR_NAME(spn, _returns_string_length_for_string_that_just_has_accept_characters)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsspn returns number for wide character strings",
    test_wcsspn_returns_number_for_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(str, " returns pointer to first occurence of searching string"),
    __W_TEST_STR_NAME(str, _returns_pointer_to_first_occurence_of_searching_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(str, " returns null pointer for searching string that does not occur"),
    __W_TEST_STR_NAME(str, _returns_null_pointer_for_searching_string_that_does_not_occur)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsstr returns pointer for wide character strings",
    test_wcsstr_returns_pointer_for_wide_character_strings) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRTOK_R_STRING(" splits string"),
    __W_TEST_STRTOK_R_NAME(_splits_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRTOK_R_STRING(" does not split string that has not tokens"),
    __W_TEST_STRTOK_R_NAME(_does_not_split_string_that_has_not_tokens)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcstok splits wide character string",
    test_wcstok_splits_wide_character_string) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STR_STRING(xfrm, " transforms string"),
    __W_TEST_STR_NAME(xfrm, _transforms_string)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "wcsxfrm transforms wide character string",
    test_wcsxfrm_transforms_wide_character_string) == NULL) return -1;
#endif
  return 0;
}
