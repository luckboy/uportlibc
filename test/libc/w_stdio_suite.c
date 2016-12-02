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
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "stdio_vars.h"
#include "sys_mock.h"
#include "sys_mock_priv.h"
#include "uportlibc.h"
#include "stdio_priv.h"
#define __W W
#include "w_cunit.h"
#define __W W
#include "w_uportlibc.h"
#define __W W
#include "w_uportlibc_name.h"
#include "w_test_name.h"

#if __W != 'w'
#define WIDE_MODE               (-1)
#else
#define WIDE_MODE               1
#endif

int __W_INITIALIZE_NAME(stdio)(void)
{
  init_sys_mock();
  save_stdio_vars();
  return 0;
}

int __W_CLEANUP_NAME(stdio)(void)
{
  restore_stdio_vars();
  final_sys_mock();
  return 0;
}

void __W_TEST_NAME(fget, c, _reads_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "12345678", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), '2');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 2);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), '3');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _does_not_read_character_after_last_character_reading)(void)
{
  static char str[4];
  int i, c;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "12345", 4);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 4;
  errno = 0;
  for(i = 0; i < 4; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "1234", 4);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 4);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _does_not_read_character_if_eof_flag_is_set)(void)
{
  static char str[4];
  int i, c;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  errno = 0;
  for(i = 0; i < 8; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "abcdefgh", 4);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _reads_character_when_buffer_is_full)(void)
{
  static char buf[8];
  static char str[8];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 8), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh12345678ABCDEFGH", 24);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 24;
  errno = 0;
  for(i = 0; i < 8; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "abcdefgh", 8);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  errno = 0;
  for(i = 0; i < 7; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "2345678", 7);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'A');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "ABCDEFGH", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _reads_character_for_line_buffered_i_o)(void)
{
  static char buf[UPORTLIBC_BUFSIZ];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, UPORTLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'b');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'c');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _reads_character_for_unbuffered_i_o)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, NULL, UPORTLIBC__IONBF, 0), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "ABCDEFGHIJ", 10);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 10;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'A');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'B');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'C');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _reads_character_after_writing)(void)
{
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "ABCDEFGHIJKL", 12);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 12;
  errno = 0;
  str = "12345";
  for(i = 0; i < 5; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'F');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 7);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "12345FGHIJKL", 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 12);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "FGHIJKL", 7);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 7);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _writes_character_after_unreading)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('a', stream), 'a');
#if MB_LEN_MAX > 1
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('b', stream), 'b');
#endif
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
#if MB_LEN_MAX > 1
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "ab", 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 2);
#else
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "a", 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 1);
#endif
  /* Reads character. */
  errno = 0;
#if MB_LEN_MAX > 1
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'b');
#else
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'a');
#endif
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
#if MB_LEN_MAX > 1
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "a", 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 1);
#else
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
#endif
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

#if __W == 'w'
void test_fgetwc_reads_wide_character(void)
{
    uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "\304\204\342\207\206", 5);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 5;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetwc(stream), L'\U00000104');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\304\204\342\207\206", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 2);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetwc(stream), L'\U000021c6');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\304\204\342\207\206", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fgetwc_sets_error_flag_and_errno_for_part_of_wide_character(void)
{
  mbstate_t state;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "\360\237\230", 3);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 3;
  memcpy(&state, &(stream->mb_state), sizeof(mbstate_t));
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetwc(stream), UPORTLIBC_WEOF);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 3);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\360\237\230", 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  CU_ASSERT_NOT_EQUAL(memcmp(&(stream->mb_state), &state, sizeof(mbstate_t)), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}
#endif

void __W_TEST_NAME(fget, c, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads character. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, c, _sets_error_flag_and_errno_for_incorrect_stream_orientation)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  stream->wide_mode = -WIDE_MODE;
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_line)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh\n123456\n", 16);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 16;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 16);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("abcdefgh\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh\n123456\n", 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 16);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("123456\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh\n123456\n", 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 16);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_too_long_line)(void)
{
  static __W_CHAR line_buf[7];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcd123456zyxw", 14);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 14;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 5, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 14);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("abcd"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcd123456zyxw", 14);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 14);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 7, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("123456"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcd123456zyxw", 14);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 10);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 14);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_line_without_newline_to_end_of_file)(void)
{
  static __W_CHAR line_buf[10];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "zyxwvu", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 10, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("zyxwvu"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _does_not_read_line_for_number_of_characters_that_is_less_than_2)(void)
{
  __W_CHAR c;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  memcpy(sys_mock_files[stream->fd]->buf, "ABCD", 4);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 4;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(&c, 1, stream), &c);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(c, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads line. */
  c = 'a';
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(&c, 0, stream), &c);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads line. */
  c = 'b';
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(&c, -10, stream), &c);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _does_not_read_line_if_eof_flag_is_set)(void)
{
  static __W_CHAR line_buf[256];
  static char str[6];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  errno = 0;
  for(i = 0; i < 8; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "abcdef", 6);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_NULL(__W_NAME(fget, s)(line_buf, 256, stream));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_NULL(__W_NAME(fget, s)(line_buf, 256, stream));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_line_for_line_buffered_i_o)(void)
{
  static char buf[UPORTLIBC_BUFSIZ];
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, UPORTLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "1234\nabcd\n", 10);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 10;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 9);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[7].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[8].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("1234\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_line_for_unbuffered_i_o)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, NULL, UPORTLIBC__IONBF, 0), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef\n1234\n", 12);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 12;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 11);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[7].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[8].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[9].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[9].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[10].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[10].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[10].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[10].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[10].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("abcdef\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _reads_line_after_writing)(void)
{
  static __W_CHAR line_buf[256];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890\nabcd", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  errno = 0;
  str = "abcdef";
  for(i = 0; i < 6; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdef", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Reads character. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdef7890\nabcd", 15);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 15);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 15);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("7890\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "7890\nabcd", 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _writes_line_after_unreading)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "12345678\n", 9);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 9;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('a', stream), 'a');
#if MB_LEN_MAX > 1
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('b', stream), 'b');
#endif
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
#if MB_LEN_MAX > 1
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "ab", 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 2);
#else
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "a", 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 1);
#endif
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
#if MB_LEN_MAX > 1
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("ba12345678\n"));
#else
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("a12345678\n"));
#endif
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678\n", 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

#if __W == 'w'
void test_fgetws_reads_line_with_wide_characters(void)
{
  static wchar_t line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "\342\224\202a\304\205bc\304\207de\304\231f\342\224\234\n", 19);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 19;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 19);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, L"\U00002502a\U00000105bc\U00000107de\U00000119f\U0000251c\n");
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\342\224\202a\304\205bc\304\207de\304\231f\342\224\234\n", 19);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 19);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 19);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}
#endif

void __W_TEST_NAME(fget, s, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads line. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_PTR_NULL(__W_NAME(fget, s)(line_buf, 256, stream));
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s, _sets_error_flag_and_errno_for_incorrect_stream_orientation)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  stream->wide_mode = -WIDE_MODE;
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_NULL(__W_NAME(fget, s)(line_buf, 256, stream));
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s_unlocked, _reads_line)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "123456\nabcdefgh", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(__W_NAME(fget, s_unlocked)(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  __W_CU_ASSERT_STRING_EQUAL(line_buf, __W_L("123456\n"));
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456\nabcdefgh", 15);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 7);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 15);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fget, s_unlocked, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static __W_CHAR line_buf[256];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads line. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_PTR_NULL(__W_NAME(fget, s_unlocked)(line_buf, 256, stream));
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('a', stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "a", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('b', stream), 'b');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "ab", 2);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('c', stream), 'c');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abc", 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 3);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_character_before_stream_buffer_flushing)(void)
{
  static char buf[10];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 10), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "ABCDEFGHI";
  for(i = 0; i < 9; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "ABCDEFGHI", 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('J', stream), 'J');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "ABCDEFGHIJ", 10);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 10);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 10);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  errno = 0;
  str = "123456789";
  for(i = 0; i < 9; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456789", 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('0', stream), '0');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "ABCDEFGHIJ1234567890", 20);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 20);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 20);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_character_for_line_buffered_i_o)(void)
{
  static char buf[UPORTLIBC_BUFSIZ];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, UPORTLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('1', stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF| FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('2', stream), '2');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12", 2);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('3', stream), '3');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123", 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 3);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_newline_for_line_buffered_i_o)(void)
{
  static char buf[UPORTLIBC_BUFSIZ];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, UPORTLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "abcdef";
  for(i = 0; i < 6; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdef", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('\n', stream), '\n');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 7);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 7);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdef\n", 7);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 7);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 7);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  errno = 0;
  str = "1234";
  for(i = 0; i < 4; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('\n', stream), '\n');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdef\n1234\n", 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 12);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_character_for_unbuffered_i_o)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, NULL, UPORTLIBC__IONBF, 0), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('z', stream), 'z');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "z", 1);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 1);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 1);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('y', stream), 'y');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "zy", 2);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 2);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 2);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('x', stream), 'x');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "zyx", 3);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 3);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 3);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _writes_character_after_reading)(void)
{
  static char str[4];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890", 10);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 10;
  errno = 0;
  for(i = 0; i < 4; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "1234", 4);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234567890", 10);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 10);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('a', stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -6);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 4);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "a", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

#if __W == 'w'
void test_fputwc_writes_wide_character(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputwc(L'\U00002606', stream), L'\U00002606');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\342\230\206", 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 3);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputwc(L'\U0000017a', stream), L'\U0000017a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "\342\230\206\305\272", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}
#endif

void __W_TEST_NAME(fput, c, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[6];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 6), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "12345";
  for(i = 0; i < 5; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('6', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, c, _sets_error_flag_and_errno_for_incorrect_stream_orientation)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  stream->wide_mode = -WIDE_MODE;
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, c)('7', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_string)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("abcdefghij"), stream), 10);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefghij", 10);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 10);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("123456"), stream), 6);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefghij123456", 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 16);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_string_before_stream_buffer_flushing)(void)
{
  static char buf[8];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 8), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "abcd";
  for(i = 0; i < 4; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcd", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("1234"), stream), 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcd1234", 8);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 8);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 8);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  errno = 0;
  str = "0987";
  for(i = 0; i < 4; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "0987", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("zyxw"), stream), 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcd12340987zyxw", 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 16);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_string_that_is_too_long_to_fit_in_stream_buffer)(void)
{
  static char buf[10];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 10), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("1234567890abcdefghijzyxwv"), stream), 25);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "1234567890abcdefghij", 20);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 20);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 20);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "zyxwv", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_string_for_line_buffered_i_o)(void)
{
  static char buf[UPORTLIBC_BUFSIZ];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, UPORTLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("1234\nabcdef\nABCD"), stream), 16);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 7);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 7);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "1234\nabcdef\n", 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 12);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "ABCD", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_string_for_unbuffered_i_o)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, NULL, UPORTLIBC__IONBF, 0), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("12345"), stream), 5);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 9);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[7].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[8].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "12345", 5);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 5);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _writes_line_after_reading)(void)
{
  static char str[6];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefghijklmnop", 14);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 14;
  errno = 0;
  for(i = 0; i < 6; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "abcdef", 6);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 14);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefghijklmnop", 14);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 14);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("1234"), stream), 4);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 6);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

#if __W == 'w'
void test_fputws_writes_string_with_wide_characters(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputws(L"A\U00000104BC\U00000106DE\U00000118F\U0001f60e", stream), 10);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "A\304\204BC\304\206DE\304\230F\360\237\230\216", 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 16);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}
#endif

void __W_TEST_NAME(fput, s, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[10];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 10), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("1234567890"), stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234567890", 10);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 10);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s, _sets_error_flag_and_errno_for_incorrect_stream_orientation)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  stream->wide_mode = -WIDE_MODE;
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("abcdefgh"), stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s_unlocked, _writes_string)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes string. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s_unlocked)(__W_L("12345678"), stream), 8);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(fput, s_unlocked, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[8];
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 8), 0);
  CU_ASSERT_EQUAL(errno, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fput, s)(__W_L("abcdefgh"), stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _pushes_character_to_stream)(void)
{
  static char str[8];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  errno = 0;
  for(i = 0; i < 6; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "abcdef", 6);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('H', stream), 'H');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "H", 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 1);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('G', stream), 'G');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "HG", 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 2);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('F', stream), 'F');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "HGF", 3);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 3);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _pushes_character_to_stream_after_mb_len_max_pushed_characters)(void)
{
  static char str[MB_LEN_MAX + 6];
  static char expected_str[MB_LEN_MAX + 6];
  static char expected_buf[MB_LEN_MAX + 6];
  static char expected_pushed_cs[MB_LEN_MAX];
  int i, b;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  for(i = 0; i < MB_LEN_MAX + 6; i++) expected_str[i] = i + 'a';
  memcpy(expected_buf, expected_str, MB_LEN_MAX + 6);
  for(i = 0; i < MB_LEN_MAX + 6; i++) sys_mock_files[stream->fd]->buf[i] = i + 'a';
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = MB_LEN_MAX + 6;
  errno = 0;
  for(i = 0; i < MB_LEN_MAX + 3; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, expected_str, MB_LEN_MAX + 3);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, MB_LEN_MAX + 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, expected_buf, MB_LEN_MAX + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + MB_LEN_MAX + 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + MB_LEN_MAX + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  for(i = 0; i < MB_LEN_MAX; i++) expected_pushed_cs[i] = i + 'A';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (__W_NAME(unget, c)(i + 'A', stream) != __W_EOF);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, expected_buf, MB_LEN_MAX + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + MB_LEN_MAX + 3);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + MB_LEN_MAX + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  expected_buf[MB_LEN_MAX + 2] = 'A';
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '1';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('1', stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, expected_buf, MB_LEN_MAX + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + MB_LEN_MAX + 2);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + MB_LEN_MAX + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  expected_buf[MB_LEN_MAX + 1] = 'B';
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '2';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('2', stream), '2');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, expected_buf, MB_LEN_MAX + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + MB_LEN_MAX + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + MB_LEN_MAX + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  expected_buf[MB_LEN_MAX + 0] = 'C';
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '3';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('3', stream), '3');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, expected_buf, MB_LEN_MAX + 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + MB_LEN_MAX + 0);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + MB_LEN_MAX + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _pushes_character_to_stream_with_stream_buffer_movement_after_mb_len_max_pushed_characters)(void)
{
  static char expected_pushed_cs[MB_LEN_MAX];
  int i, b;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdef", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  for(i = 0; i < MB_LEN_MAX; i++) expected_pushed_cs[i] = i + 'A';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (__W_NAME(unget, c)(i + 'A', stream) != __W_EOF);
  }
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = MB_LEN_MAX + 'A';
  b &= (__W_NAME(unget, c)(MB_LEN_MAX + 'A', stream) != -1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "Abcdef", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '1';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('1', stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "BAbcdef", 7);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 7);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '2';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('2', stream), '2');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "CBAbcdef", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = '3';
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('3', stream), '3');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "DCBAbcdef", 9);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_for_too_small_stream_buffer)(void)
{
  static char buf[8];
  static char expected_pushed_cs[MB_LEN_MAX];
  int i, b;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 8), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(fget, c)(stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  for(i = 0; i < MB_LEN_MAX; i++) expected_pushed_cs[i] = i + 'A';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (__W_NAME(unget, c)(i + 'A', stream) != __W_EOF);
  }
  memmove(expected_pushed_cs, expected_pushed_cs + 1, MB_LEN_MAX - 1);
  expected_pushed_cs[MB_LEN_MAX - 1] = MB_LEN_MAX + 'A';
  b &= (__W_NAME(unget, c)(MB_LEN_MAX + 'A', stream) != __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "Abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('1', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "Abcdefgh", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_for_incorrect_stream_orientation)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  stream->wide_mode = -WIDE_MODE;
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('8', stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_with_data_to_write_for_full_pushed_cs)(void)
{
  char *str;
  static char expected_pushed_cs[MB_LEN_MAX];
  int i, b;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  errno = 0;
  str = "1234";
  for(i = 0; i < 4; i++) __W_NAME(fput, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);  
  for(i = 0; i < MB_LEN_MAX; i++) expected_pushed_cs[i] = i + 'a';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (__W_NAME(unget, c)(i + 'a', stream) != __W_EOF);
  }
  CU_ASSERT(b);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('2', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 4);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, expected_pushed_cs, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->pushed_c_count, MB_LEN_MAX);
  if(stream != NULL) free(stream);
}

#if __W == 'w'
void test_ungetwc_pushes_wide_character_to_stream(void)
{
  static wchar_t str[8];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "a\304\205bc\304\207de\304\231", 12);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 12;
  errno = 0;
  for(i = 0; i < 6; i++) {
    str[i] = uportlibc_fgetwc(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, L"a\U00000105bc\U00000107de\U00000119", 8);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "a\304\205bc\304\207de\304\231", 12);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 12);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_ungetwc(L'\U0000017c', stream), L'\U0000017c');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "a\304\205bc\304\207de\304\231", 12);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 12);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "\274\305", 2);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 2);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}
#endif

void __W_TEST_NAME(unget, c_unlocked, _pushes_character_to_stream)(void)
{
  static char str[8];
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "123456", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  errno = 0;
  for(i = 0; i < 4; i++) {
    str[i] = __W_NAME(fget, c)(stream);
  }
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_NSTRING_EQUAL(str, "1234", 6);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456", 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Pushes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(unget, c)('a', stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 4);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_NSTRING_EQUAL(stream->pushed_cs, "a", 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 1);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(get, c, _reads_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef", 6);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 6;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, c)(stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "abcdef", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
}

void __W_TEST_NAME(get, c, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads character. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, c)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(get, c_unlocked, _reads_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "12345678", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, c_unlocked)(stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
}

void __W_TEST_NAME(get, c_unlocked, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Reads character. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, c_unlocked)(stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(get, char, _reads_character)(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  memcpy(sys_mock_files[uportlibc_stdin->fd]->buf, "1234", 4);
  sys_mock_files[uportlibc_stdin->fd]->buf_data_pos = 0;
  sys_mock_files[uportlibc_stdin->fd]->buf_data_size = 4;
  /* Reads character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, char)(), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 4);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdin->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf + 1);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf + 4);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void __W_TEST_NAME(get, char, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  /* Reads character. */
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(get, char)(), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, EIO);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, WIDE_MODE);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void __W_TEST_NAME(put, c, _writes_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, c)('1', stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(put, c, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[8];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 8), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "1234567";
  for(i = 0; i < 7; i++) __W_NAME(put, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1234567", 7);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 7);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, c)('8', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345678", 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(put, c_unlocked, _writes_character)(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, c_unlocked)('1', stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "1", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 1);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(put, c_unlocked, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[6];
  char *str;
  int i;
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 6), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "12345";
  for(i = 0; i < 5; i++) __W_NAME(put, c)(str[i], stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "12345", 5);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 5);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, c_unlocked)('6', stream), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "123456", 6);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 6);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void __W_TEST_NAME(put, char, _writes_character)(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  /* Writes character. */
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, char)('2'), '2');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdout->buf, "2", 1);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf + 1);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
}

void __W_TEST_NAME(put, char, _sets_error_flag_and_errno_when_error_occurs)(void)
{
  static char buf[4];
  char *str;
  int i;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(uportlibc_stdout, buf, UPORTLIBC__IOFBF, 4), 0);
  CU_ASSERT_EQUAL(errno, 0);
  errno = 0;
  str = "123";
  for(i = 0; i < 3; i++) __W_NAME(put, char)(str[i]);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdout->buf, "123", 3);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf + 3);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
  /* Writes character. */
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  errno = 0;
  CU_ASSERT_EQUAL(__W_NAME(put, char)('4'), __W_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdout->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, 4);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, EIO);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, WIDE_MODE);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdout->buf, "1234", 4);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf + 4);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
}

int __W_ADD_SUITE_NAME(stdio)(void)
{
  CU_pSuite suite;
  suite = CU_add_suite(__W_SUITE_STRING(stdio), __W_INITIALIZE_NAME(stdio), __W_CLEANUP_NAME(stdio));
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character"),
    __W_TEST_NAME(fget, c, _reads_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " does not read character after last character reading"),
    __W_TEST_NAME(fget, c, _does_not_read_character_after_last_character_reading)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " does not read character if EOF flag is set"),
    __W_TEST_NAME(fget, c, _does_not_read_character_if_eof_flag_is_set)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character when buffer is full"),
    __W_TEST_NAME(fget, c, _reads_character_when_buffer_is_full)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character for line buffered I/O"),
    __W_TEST_NAME(fget, c, _reads_character_for_line_buffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character for unbuffered I/O"),
    __W_TEST_NAME(fget, c, _reads_character_for_unbuffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character after writing"),
    __W_TEST_NAME(fget, c, _reads_character_after_writing)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " reads character after unreading"),
    __W_TEST_NAME(fget, c, _writes_character_after_unreading)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "fgetwc reads wide character",
    test_fgetwc_reads_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "fgetwc sets error flag and errno for part of wide character",
    test_fgetwc_sets_error_flag_and_errno_for_part_of_wide_character) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fget, c, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, c, " sets error flag and errno for incorrect stream orientation"),
    __W_TEST_NAME(fget, c, _sets_error_flag_and_errno_for_incorrect_stream_orientation)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line"),
    __W_TEST_NAME(fget, s, _reads_line)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads too long line"),
    __W_TEST_NAME(fget, s, _reads_too_long_line)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line without newline to end of file"),
    __W_TEST_NAME(fget, s, _reads_line_without_newline_to_end_of_file)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " does not read line for number of characters that is less than 2"),
    __W_TEST_NAME(fget, s, _does_not_read_line_for_number_of_characters_that_is_less_than_2)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " does not read line if EOF flag is set"),
    __W_TEST_NAME(fget, s, _does_not_read_line_if_eof_flag_is_set)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line for line buffered I/O"),
    __W_TEST_NAME(fget, s, _reads_line_for_line_buffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line for unbuffered I/O"),
    __W_TEST_NAME(fget, s, _reads_line_for_unbuffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line after writing"),
    __W_TEST_NAME(fget, s, _reads_line_after_writing)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " reads line after unreading"),
    __W_TEST_NAME(fget, s, _writes_line_after_unreading)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "fgetws reads line with wide characters",
    test_fgetws_reads_line_with_wide_characters) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fget, s, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s, " sets error flag and errno for incorrect stream orientation"),
    __W_TEST_NAME(fget, s, _sets_error_flag_and_errno_for_incorrect_stream_orientation)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s_unlocked, " reads line"),
    __W_TEST_NAME(fget, s_unlocked, _reads_line)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fget, s_unlocked, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fget, s_unlocked, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes character"),
    __W_TEST_NAME(fput, c, _writes_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes character before stream buffer flushing"),
    __W_TEST_NAME(fput, c, _writes_character_before_stream_buffer_flushing)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes character for line buffered I/O"),
    __W_TEST_NAME(fput, c, _writes_character_for_line_buffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes newline for line buffered I/O"),
    __W_TEST_NAME(fput, c, _writes_newline_for_line_buffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes character for unbuffered I/O"),
    __W_TEST_NAME(fput, c, _writes_character_for_unbuffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " writes character after reading"),
    __W_TEST_NAME(fput, c, _writes_character_after_reading)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "fputwc writes wide character",
    test_fputwc_writes_wide_character) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fput, c, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, c, " sets error flag and errno for incorrect stream orientation"),
    __W_TEST_NAME(fput, c, _sets_error_flag_and_errno_for_incorrect_stream_orientation)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes string"),
    __W_TEST_NAME(fput, s, _writes_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes string before stream buffer flushing"),
    __W_TEST_NAME(fput, s, _writes_string_before_stream_buffer_flushing)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes string that is too long to fit in stream buffer"),
    __W_TEST_NAME(fput, s, _writes_string_that_is_too_long_to_fit_in_stream_buffer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes string for line buffered I/O"),
    __W_TEST_NAME(fput, s, _writes_string_for_line_buffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes string for unbuffered I/O"),
    __W_TEST_NAME(fput, s, _writes_string_for_unbuffered_i_o)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " writes line after reading"),
    __W_TEST_NAME(fput, s, _writes_line_after_reading)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "fputws writes string with wide characters",
    test_fputws_writes_string_with_wide_characters) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fput, s, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s, " sets error flag and errno for incorrect stream orientation"),
    __W_TEST_NAME(fput, s, _sets_error_flag_and_errno_for_incorrect_stream_orientation)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s_unlocked, " writes string"),
    __W_TEST_NAME(fput, s_unlocked, _writes_string)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(fput, s_unlocked, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(fput, s_unlocked, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " pushes characte to stream"),
    __W_TEST_NAME(unget, c, _pushes_character_to_stream)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " pushes character to stream after MB_LEN_MAX pushed characters"),
    __W_TEST_NAME(unget, c, _pushes_character_to_stream_after_mb_len_max_pushed_characters)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " pushes character to stream with stream buffer movement after MB_LEN_MAX pushed characters"),
    __W_TEST_NAME(unget, c, _pushes_character_to_stream_with_stream_buffer_movement_after_mb_len_max_pushed_characters)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " does not push character to stream for too small stream buffer"),
    __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_for_too_small_stream_buffer)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " does not push character to stream for incorrect stream orientation"),
    __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_for_incorrect_stream_orientation)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c, " does not push character to stream with data to write for full pushed_cs"),
    __W_TEST_NAME(unget, c, _does_not_push_character_to_stream_with_data_to_write_for_full_pushed_cs)) == NULL) return -1;
#if __W == 'w'
  if(CU_add_test(suite,
    "ungetwc pushes wide character to stream",
    test_ungetwc_pushes_wide_character_to_stream) == NULL) return -1;
#endif
  if(CU_add_test(suite,
    __W_TEST_STRING(unget, c_unlocked, " pushes characte to stream"),
    __W_TEST_NAME(unget, c_unlocked, _pushes_character_to_stream)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, c, " reads character"),
    __W_TEST_NAME(get, c, _reads_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, c, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(get, c, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, c_unlocked, " reads character"),
    __W_TEST_NAME(get, c_unlocked, _reads_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, c_unlocked, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(get, c_unlocked, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, char, " reads character"),
    __W_TEST_NAME(get, char, _reads_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(get, char, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(get, char, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, c, " writes character"),
    __W_TEST_NAME(put, c, _writes_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, c, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(put, c, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, c_unlocked, " writes character"),
    __W_TEST_NAME(put, c_unlocked, _writes_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, c_unlocked, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(put, c_unlocked, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, char, " writes character"),
    __W_TEST_NAME(put, char, _writes_character)) == NULL) return -1;
  if(CU_add_test(suite,
    __W_TEST_STRING(put, char, " sets error flag and errno when error occurs"),
    __W_TEST_NAME(put, char, _sets_error_flag_and_errno_when_error_occurs)) == NULL) return -1;
  return 0;
}
