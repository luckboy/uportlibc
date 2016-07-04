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
#include <string.h>
#include <wchar.h>
#include <CUnit/Basic.h>
#include "uportlibc.h"
#include "stdio_priv.h"

int initialize_wchar(void) { return 0; }

int cleanup_wchar(void) { return 0; }

void test_mbrlen_returns_number_of_bytes_in_multibyte_character(void)
{
  uportlibc_mbstate_t state;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrlen("\342\207\276", 3, &state), 3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrlen("\305\273", 2, &state), 2);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_one_character_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "A", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, 'A');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "b", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, 'b');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "1", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\177", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, '\177');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_two_characters_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\304\204", 2, &state), 2);
  CU_ASSERT_EQUAL(wc, L'\U00000104');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\316\261", 2, &state), 2);
  CU_ASSERT_EQUAL(wc, L'\U000003b1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\302\200", 2, &state), 2);
  CU_ASSERT_EQUAL(wc, L'\200');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\337\277", 2, &state), 2);
  CU_ASSERT_EQUAL(wc, L'\U000007ff');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_three_characters_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\342\202\254", 3, &state), 3);
  CU_ASSERT_EQUAL(wc, L'\U000020ac');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\352\234\260", 3, &state), 3);
  CU_ASSERT_EQUAL(wc, L'\U0000a730');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\340\240\200", 3, &state), 3);
  CU_ASSERT_EQUAL(wc, L'\U00000800');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\357\277\277", 3, &state), 3);
  CU_ASSERT_EQUAL(wc, L'\U0000ffff');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_four_characters_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360\220\215\210", 4, &state), 4);
  CU_ASSERT_EQUAL(wc, L'\U00010348');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360\237\230\216", 4, &state), 4);
  CU_ASSERT_EQUAL(wc, L'\U0001f60e');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360\220\200\200", 4, &state), 4);
  CU_ASSERT_EQUAL(wc, L'\U00010000');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\364\217\277\277", 4, &state), 4);
  CU_ASSERT_EQUAL(wc, L'\U0010ffff');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_null_terminator_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\0", 1, &state), 0);
  CU_ASSERT_EQUAL(wc, 0);
  CU_ASSERT_EQUAL(state.count, 0);
  CU_ASSERT_EQUAL(state.wc, 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_next_characters_to_wide_character(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\342\200", 2, &state), ((size_t) -2));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\246", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, L'\U00002026');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360", 1, &state), ((size_t) -2));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\237\230\272", 3, &state), 3);
  CU_ASSERT_EQUAL(wc, L'\U0001f63a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360\237", 2, &state), ((size_t) -2));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\202\261", 2, &state), 2);
  CU_ASSERT_EQUAL(wc, L'\U0001f0b1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\304", 1, &state), ((size_t) -2));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\207", 1, &state), 1);
  CU_ASSERT_EQUAL(wc, L'\U00000107');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_returns_minus_one_and_sets_errno_for_incorrect_character_sequence(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\240\220\210", 3, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\350\47\202", 3, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\342\212", 2, &state), ((size_t) (-2)));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360", 2, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\367\277\277\277", 4, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
}

void test_mbrtowc_converts_for_string_pointer_that_is_null_pointer(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc = 'a';
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, NULL, 0, &state), 0);
  CU_ASSERT_EQUAL(wc, 'a');
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbrtowc_converts_for_wide_character_pointer_that_is_null_pointer(void)
{
  uportlibc_mbstate_t state;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(NULL, "\342\224\205", 3, &state), 3);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbsinit_returns_non_zero_for_state_pointer_that_is_null_pointer(void)
{ CU_ASSERT_NOT_EQUAL(uportlibc_mbsinit(NULL), 0); }

void test_mbsinit_returns_non_zero_for_initial_state(void)
{
  uportlibc_mbstate_t state;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_NOT_EQUAL(uportlibc_mbsinit(&state), 0);
}

void test_mbsinit_returns_zero_for_non_initial_state(void)
{
  uportlibc_mbstate_t state;
  wchar_t wc;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbrtowc(&wc, "\360", 1, &state), ((size_t) -2));
  CU_ASSERT_EQUAL(uportlibc_mbsinit(&state), 0);
}

void test_wcrtomb_converts_wide_character_to_one_character(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'a', &state), 1);
  CU_ASSERT_NSTRING_EQUAL(buf, "a", 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'B', &state), 1);
  CU_ASSERT_NSTRING_EQUAL(buf, "B", 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'2', &state), 1);
  CU_ASSERT_NSTRING_EQUAL(buf, "2", 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\177', &state), 1);
  CU_ASSERT_NSTRING_EQUAL(buf, "\177", 1);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcrtomb_converts_wide_character_to_two_characters(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U00000104', &state), 2);
  CU_ASSERT_NSTRING_EQUAL(buf, "\304\204", 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U000003c0', &state), 2);
  CU_ASSERT_NSTRING_EQUAL(buf, "\317\200", 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\200', &state), 2);
  CU_ASSERT_NSTRING_EQUAL(buf, "\302\200", 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U000007ff', &state), 2);
  CU_ASSERT_NSTRING_EQUAL(buf, "\337\277", 1);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcrtomb_converts_wide_character_to_three_characters(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U000020ac', &state), 3);
  CU_ASSERT_NSTRING_EQUAL(buf, "\342\202\254", 3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U00002618', &state), 3);
  CU_ASSERT_NSTRING_EQUAL(buf, "\342\230\230", 3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U00000800', &state), 3);
  CU_ASSERT_NSTRING_EQUAL(buf, "\340\240\200", 3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U0000ffff', &state), 3);
  CU_ASSERT_NSTRING_EQUAL(buf, "\357\277\277", 3);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcrtomb_converts_wide_character_to_four_characters(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U00010348', &state), 4);
  CU_ASSERT_NSTRING_EQUAL(buf, "\360\220\215\210", 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U0001f0df', &state), 4);
  CU_ASSERT_NSTRING_EQUAL(buf, "\360\237\203\237", 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U00010000', &state), 4);
  CU_ASSERT_NSTRING_EQUAL(buf, "\360\220\200\200", 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\U0010ffff', &state), 4);
  CU_ASSERT_NSTRING_EQUAL(buf, "\364\217\277\277", 4);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcrtomb_converts_wide_null_terminator_to_one_character(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, L'\0', &state), 1);
  CU_ASSERT_NSTRING_EQUAL(buf, "\0", 1);
  CU_ASSERT_EQUAL(state.count, 0);
  CU_ASSERT_EQUAL(state.wc, 0);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcrtomb_returns_minus_one_and_sets_errno_for_incorrect_wide_character(void)
{
  uportlibc_mbstate_t state;
  char buf[UPORTLIBC_MB_LEN_MAX];
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, ((wchar_t) (-1)), &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, ((wchar_t) 0x110000), &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, ((wchar_t) (-10)), &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(buf, ((wchar_t) 0x200000), &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(errno, EILSEQ);
}

void test_wcrtomb_converts_for_string_pointer_that_is_null_pointer(void)
{
  uportlibc_mbstate_t state;
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcrtomb(NULL, L'\U00000118', &state), 1);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbsrtowcs_converts_string_to_wide_character_string(void)
{
  uportlibc_mbstate_t state;
  static wchar_t buf[100];
  const char *str, *ptr;
  ptr = str = "A\304\204b\352\234\260C";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 100, &state), 5);
  CU_ASSERT_EQUAL(wcscmp(buf, L"A\U00000104b\U0000a730C"), 0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  ptr = str = "1234\360\220\215\210ab";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 100, &state), 7);
  CU_ASSERT_EQUAL(wcscmp(buf, L"1234\U00010348ab"), 0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbsrtowcs_converts_for_too_long_string(void)
{
  uportlibc_mbstate_t state;
  static wchar_t buf[100];
  const char *str, *ptr;
  ptr = str = "a\304\205bc\304\207de\304\231";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  wmemset(buf, L'X', 10);
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 5, &state), 5);
  CU_ASSERT_EQUAL(wcsncmp(buf, L"a\U00000105bc\U00000107", 5), 0);
  CU_ASSERT_EQUAL(wmemcmp(buf + 5, L"XXXXX", 5), 0);
  CU_ASSERT_EQUAL(ptr, str + 7);
  CU_ASSERT_EQUAL(errno, 0);
  ptr = str = "zyx\360\237\230\274";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  wmemset(buf, L'X', 10);
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 3, &state), 3);
  CU_ASSERT_EQUAL(wcsncmp(buf, L"zyx", 3), 0);
  CU_ASSERT_EQUAL(wmemcmp(buf + 3, L"XXXXXXX", 7), 0);
  CU_ASSERT_EQUAL(ptr, str + 3);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_mbsrtowcs_returns_minus_one_and_sets_errno_for_incorrect_character_sequence(void)
{
  uportlibc_mbstate_t state;
  static wchar_t buf[100];
  const char *str, *ptr;
  ptr = str = "\304\230ab\360\212dc";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  wmemset(buf, L'X', 10);
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 100, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(wcsncmp(buf, L"\U00000118ab", 3), 0);
  CU_ASSERT_EQUAL(wmemcmp(buf + 3, L"XXXXXXX", 7), 0);
  CU_ASSERT_EQUAL(ptr, str + 4);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  ptr = str = "\340abc";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  wmemset(buf, L'X', 10);
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(buf, &ptr, 100, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(wmemcmp(buf, L"XXXXXXXXXX", 10), 0);
  CU_ASSERT_EQUAL(ptr, str + 0);
  CU_ASSERT_EQUAL(errno, EILSEQ);
}

void test_mbsrtowcs_converts_for_wide_character_string_pointer_that_is_null_pointer(void)
{
  uportlibc_mbstate_t state;
  const char *str, *ptr;
  ptr = str = "a\304\204b\304\230";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_mbsrtowcs(NULL, &ptr, 2, &state), 4);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcsrtombs_converts_wide_character_string_to_string(void)
{
  uportlibc_mbstate_t state;
  static char buf[100];
  const wchar_t *str, *ptr;
  ptr = str = L"ab\U0000017bcd\U0001f607";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 100, &state), 10);
  CU_ASSERT_EQUAL(strcmp(buf, "ab\305\273cd\360\237\230\207"), 0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  ptr = str = L"zy\U000003c6x";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 100, &state), 5);
  CU_ASSERT_EQUAL(strcmp(buf, "zy\317\206x"), 0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcsrtombs_converts_for_too_long_wide_character_string(void)
{
  uportlibc_mbstate_t state;
  static char buf[100];
  const wchar_t *str, *ptr;
  ptr = str = L"1ad\U000003a9x\U0001f60f";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  memset(buf, 'X', 10);
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 7, &state), 6);
  CU_ASSERT_EQUAL(strncmp(buf, "1ad\316\251x", 6), 0);
  CU_ASSERT_EQUAL(memcmp(buf + 6, "XXXX", 4), 0);
  CU_ASSERT_EQUAL(ptr, str + 5);
  CU_ASSERT_EQUAL(errno, 0);
  ptr = str = L"90\U00002195Babcd";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  memset(buf, 'X', 10);
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 6, &state), 6);
  CU_ASSERT_EQUAL(strncmp(buf, "90\342\206\225B", 6), 0);
  CU_ASSERT_EQUAL(memcmp(buf + 6, "XXXX", 4), 0);
  CU_ASSERT_EQUAL(ptr, str + 4);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_wcsrtombs_returns_minus_one_and_sets_errno_for_incorrect_character_sequence(void)
{
  uportlibc_mbstate_t state;
  static char buf[100];
  const wchar_t *str, *ptr;
  ptr = str = L"d\U0000219def\U001fffffg";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  memset(buf, 'X', 10);
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 100, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(strncmp(buf, "d\342\206\235ef", 6), 0);
  CU_ASSERT_EQUAL(memcmp(buf + 6, "XXXX", 4), 0);
  CU_ASSERT_EQUAL(ptr, str + 4);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  ptr = str = L"\U00110000";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  memset(buf, 'X', 10);
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(buf, &ptr, 100, &state), ((size_t) (-1)));
  CU_ASSERT_EQUAL(memcmp(buf, "XXXXXXXXXX", 10), 0);
  CU_ASSERT_EQUAL(ptr, str + 0);
  CU_ASSERT_EQUAL(errno, EILSEQ);
}

void test_wcsrtombs_converts_for_string_pointer_that_is_null_pointer(void)
{
  uportlibc_mbstate_t state;
  const wchar_t *str, *ptr;
  ptr = str = L"\U0000220fa\U00002211bcd";
  errno = 0;
  memset(&state, 0, sizeof(uportlibc_mbstate_t));
  CU_ASSERT_EQUAL(uportlibc_wcsrtombs(NULL, &ptr, 3, &state), 10);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
}

void test_fwide_returns_stream_orientation(void)
{
  uportlibc_FILE stream;
  lock_init(&(stream.lock));
  stream.wide_mode = 0;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 0), 0);
  CU_ASSERT_EQUAL(stream.wide_mode, 0);
  stream.wide_mode = 1;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 0), 1);
  CU_ASSERT_EQUAL(stream.wide_mode, 1);
  stream.wide_mode = -1;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 0), -1);
  CU_ASSERT_EQUAL(stream.wide_mode, -1);
  lock_destroy(&(stream.lock));
}

void test_fwide_sets_stream_orientation(void)
{
  uportlibc_FILE stream;
  lock_init(&(stream.lock));
  stream.wide_mode = 0;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 1), 1);
  CU_ASSERT_EQUAL(stream.wide_mode, 1);
  stream.wide_mode = 0;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, -1), -1);
  CU_ASSERT_EQUAL(stream.wide_mode, -1);
  lock_destroy(&(stream.lock));
}

void test_fwide_does_not_set_stream_orientation_after_stream_orientation_setting(void)
{
  uportlibc_FILE stream;
  lock_init(&(stream.lock));
  stream.wide_mode = 0;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 1), 1);
  CU_ASSERT_EQUAL(stream.wide_mode, 1);
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, -1), 1);
  CU_ASSERT_EQUAL(stream.wide_mode, 1);
  stream.wide_mode = 0;
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, -1), -1);
  CU_ASSERT_EQUAL(stream.wide_mode, -1);
  CU_ASSERT_EQUAL(uportlibc_fwide(&stream, 1), -1);
  CU_ASSERT_EQUAL(stream.wide_mode, -1);
  lock_destroy(&(stream.lock));
}

int add_wchar_suite(void)
{
  CU_pSuite suite;
  suite = CU_add_suite("wchar suite", initialize_wchar, cleanup_wchar);
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    "mbrlen returns number of bytes in multibyte character",
    test_mbrlen_returns_number_of_bytes_in_multibyte_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts one character to wide character",
    test_mbrtowc_converts_one_character_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts two characters to wide character",
    test_mbrtowc_converts_two_characters_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts three characters to wide character",
    test_mbrtowc_converts_three_characters_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts four characters to wide character",
    test_mbrtowc_converts_four_characters_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts null terminator to wide character",
    test_mbrtowc_converts_null_terminator_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts next characters to wide character",
    test_mbrtowc_converts_next_characters_to_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc returns -1 and sets errno for incorrect character sequence",
    test_mbrtowc_returns_minus_one_and_sets_errno_for_incorrect_character_sequence) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts for string pointer that is null pointer",
    test_mbrtowc_converts_for_string_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "mbrtowc converts for wide character pointer that is null pointer",
    test_mbrtowc_converts_for_wide_character_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsinit returns non zero for state pointer that is null pointer",
    test_mbsinit_returns_non_zero_for_state_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsinit returns non zero for initial state",
    test_mbsinit_returns_non_zero_for_initial_state) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsinit returns zero for non initial state",
    test_mbsinit_returns_zero_for_non_initial_state) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts wide character to one character",
    test_wcrtomb_converts_wide_character_to_one_character) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts wide character to two characters",
    test_wcrtomb_converts_wide_character_to_two_characters) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts wide character to three characters",
    test_wcrtomb_converts_wide_character_to_three_characters) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts wide character to four characters",
    test_wcrtomb_converts_wide_character_to_four_characters) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts wide null terminator to one character",
    test_wcrtomb_converts_wide_null_terminator_to_one_character) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb returns -1 and sets errno for incorrect wide character",
    test_wcrtomb_returns_minus_one_and_sets_errno_for_incorrect_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "wcrtomb converts for string pointer that is null pointer",
    test_wcrtomb_converts_for_string_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsrtowcs converts string to wide character string",
    test_mbsrtowcs_converts_string_to_wide_character_string) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsrtowcs converts for too long string",
    test_mbsrtowcs_converts_for_too_long_string) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsrtowcs returns -1 and sets errno for incorrect character sequence",
    test_mbsrtowcs_returns_minus_one_and_sets_errno_for_incorrect_character_sequence) == NULL) return -1;
  if(CU_add_test(suite,
    "mbsrtowcs converts for wide character string pointer that is null pointer",
    test_mbsrtowcs_converts_for_wide_character_string_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "wcsrtombs converts wide character string to string",
    test_wcsrtombs_converts_wide_character_string_to_string) == NULL) return -1;
  if(CU_add_test(suite,
    "wcsrtombs converts for too long wide character string",
    test_wcsrtombs_converts_for_too_long_wide_character_string) == NULL) return -1;
  if(CU_add_test(suite,
    "wcsrtombs returns -1 and sets errno for incorrect character sequence",
    test_wcsrtombs_returns_minus_one_and_sets_errno_for_incorrect_character_sequence) == NULL) return -1;
  if(CU_add_test(suite,
    "wcsrtombs converts for string pointer that is null pointer",
    test_wcsrtombs_converts_for_string_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "fwide returns stream orientation",
    test_fwide_returns_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "fwide sets stream orientation",
    test_fwide_sets_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "fwide does not set stream orientation after stream orientation setting",
    test_fwide_does_not_set_stream_orientation_after_stream_orientation_setting) == NULL) return -1;
  return 0;
}
