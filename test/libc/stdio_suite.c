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

union word
{
  char bytes[WORD_BIT / 8];
  int word;
};

int initialize_stdio(void)
{
  init_sys_mock();
  save_stdio_vars();
  return 0;
}

int cleanup_stdio(void)
{
  restore_stdio_vars();
  final_sys_mock();
  return 0; 
}

void test_init_stdio_initializes(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, SYS_MOCK_STDIN_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, SYS_MOCK_STDOUT_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  uportlibc_stdin->prev = uportlibc_stdin->next = NULL;
  uportlibc_stdout->prev = uportlibc_stdout->next = NULL;
  uportlibc_stderr->prev = uportlibc_stderr->next = NULL;
  uportlibc_stdin->buf_type = UPORTLIBC__IOFBF;
  uportlibc_stdout->buf_type = UPORTLIBC__IOFBF;
}

void test_init_stdio_initializes_for_stdin_as_terminal(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  sys_mock_files[SYS_MOCK_STDIN_FILENO]->is_tty = 1;
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, SYS_MOCK_STDIN_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, SYS_MOCK_STDOUT_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
}

void test_init_stdio_initializes_for_stdout_as_terminal(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  sys_mock_files[SYS_MOCK_STDOUT_FILENO]->is_tty = 1;
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, SYS_MOCK_STDIN_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, SYS_MOCK_STDOUT_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOLBF);
}

void test_fopen_opens_file_for_reading(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDONLY);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_or_creates_file_for_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_WRONLY | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_or_creates_file_for_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "a");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_WRONLY | O_APPEND | O_CREAT);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_file_for_reading_and_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDWR);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_or_creates_file_for_reading_and_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "w+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDWR | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_or_creates_file_for_reading_and_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "a+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDWR | O_APPEND | O_CREAT);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_opens_file_that_is_terminal(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  sys_mock_file_is_tty = 1;
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDWR);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[2].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "isatty");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_returns_null_pointer_and_sets_errno_when_error_occurs(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  sys_mock_open_res = -1;
  sys_mock_open_err_num = EIO;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].args[0].cp, "test.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, O_RDONLY);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, EIO);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fopen_returns_null_pointer_and_errno_for_incorrect_mode(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fopen("test.txt", "xxx");
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_reading(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(10, "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 10);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(20, "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 20);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(30, "a");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 30);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_r_plus_mode(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(40, "r+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 40);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_w_plus_mode(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(50, "w+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 50);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_associates_file_description_for_reading_and_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Assiociates file description. */
  errno = 0;
  stream = uportlibc_fdopen(60, "a+");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, 60);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fdopen_returns_null_pointer_and_errno_for_incorrect_mode(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_fdopen(10, "xxx");
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_file_for_reading(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "r", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_RDONLY);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_or_creates_file_for_writing(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "w", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_WRONLY | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_or_creates_file_for_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "a", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_WRONLY | O_APPEND | O_CREAT);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_file_for_reading_and_writing(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "r+", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_RDWR);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_or_creates_file_for_reading_and_writing(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "w+", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_RDWR | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_opens_or_creates_file_for_reading_and_writing_at_end_of_file(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_freopen("test2.txt", "a+", stream), stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_RDWR | O_APPEND | O_CREAT);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT(sys_mock_ops[4].res.i >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, old_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_returns_null_pointer_and_sets_error_flag_and_errno_when_error_occurs(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  sys_mock_open_res = -1;
  sys_mock_open_err_num = EIO;
  CU_ASSERT_PTR_NULL(uportlibc_freopen("test2.txt", "r", stream));
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "open");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].args[0].cp, "test2.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, O_RDONLY);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, old_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_freopen_returns_null_pointer_and_errno_for_incorrect_mode(void)
{
  uportlibc_FILE *stream;
  int old_fd;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test1.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  old_fd = stream->fd;
  /* Opens file. */
  errno = 0;
  CU_ASSERT_PTR_NULL(uportlibc_freopen("test2.txt", "xxx", stream));
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, old_fd);
  CU_ASSERT_EQUAL(stream->pid, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fclose_closes_stream(void)
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
  /* Closes stream. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fclose(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
}

void test_fclose_flushes_stream(void)
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
  CU_ASSERT_EQUAL(uportlibc_fputs("abcdefgh", stream), 8);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Flushes stream. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fclose(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
}

void test_fclose_closes_process(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Closes process. */
  errno = 0;
  sys_mock_waitpid_res = 2;
  sys_mock_waitpid_status = 1234;
  CU_ASSERT_EQUAL(uportlibc_fclose(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "waitpid");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 2);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(*((int *) (sys_mock_ops[5].args[1].vp)), 1234);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].vp, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 2);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
}

void test_fclose_returns_eof_when_error_occurs(void)
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
  /* Closes streams. */
  errno = 0;
  sys_mock_close_res = -1;
  sys_mock_close_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fclose(stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, sys_mock_last_opened_fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
}

void test_popen_opens_process_for_reading(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("echo test", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[0] >= 0);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[1] >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "fork");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 2);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[1]);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_PIPE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(stream->pid, 2);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_executes_command_in_process_for_reading(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_fork_res = 0;
  sys_mock_execve_err_num = ENOEXEC;
  stream = uportlibc_popen("echo test", "r");
  CU_ASSERT_EQUAL(errno, ENOEXEC);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[0] >= 0);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[1] >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "fork");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "dup2");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[1]);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, SYS_MOCK_STDOUT_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "execve");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].args[0].cp, "/bin/sh");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[0], "sh");
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[1], "-c");
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[2], "echo test");
  CU_ASSERT_PTR_NULL(((char * const *) (sys_mock_ops[6].args[1].vp))[3]);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[2].vp, uportlibc_sys_mock_environ);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, ENOEXEC);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "_exit");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 127);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_NULL(stream);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_opens_process_for_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("echo test", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[0] >= 0);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[1] >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "fork");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 2);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_PTR_NOT_NULL(stream);
  CU_ASSERT_PTR_NOT_NULL(stream->prev);
  CU_ASSERT_PTR_NULL(stream->next);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_PIPE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_EQUAL(stream->fd, ((int *) (sys_mock_ops[2].args[0].vp))[1]);
  CU_ASSERT_EQUAL(stream->pid, 2);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_executes_command_in_process_for_writing(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_fork_res = 0;
  sys_mock_execve_err_num = ENOEXEC;
  stream = uportlibc_popen("echo test", "w");
  CU_ASSERT_EQUAL(errno, ENOEXEC);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[0] >= 0);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[1] >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "fork");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "dup2");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, SYS_MOCK_STDIN_FILENO);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[1]);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "execve");
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].args[0].cp, "/bin/sh");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[6].args[1].vp);
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[0], "sh");
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[1], "-c");
  CU_ASSERT_STRING_EQUAL(((char * const *) (sys_mock_ops[6].args[1].vp))[2], "echo test");
  CU_ASSERT_PTR_NULL(((char * const *) (sys_mock_ops[6].args[1].vp))[3]);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[2].vp, uportlibc_sys_mock_environ);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, ENOEXEC);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "_exit");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 127);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_NULL(stream);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_returns_null_pointer_and_sets_errno_when_error_occurs_for_fork(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_fork_res = -1;
  sys_mock_fork_err_num = EAGAIN;
  stream = uportlibc_popen("echo test", "r");
  CU_ASSERT_EQUAL(errno, EAGAIN);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[0] >= 0);
  CU_ASSERT(((int *) (sys_mock_ops[2].args[0].vp))[1] >= 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "fork");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, EAGAIN);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[1]);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_PTR_NULL(stream);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_returns_null_pointer_and_sets_errno_when_error_occurs_for_pipe(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens process. */
  errno = 0;
  sys_mock_pipe_res = -1;
  sys_mock_pipe_err_num = ENFILE;
  stream = uportlibc_popen("echo test", "r");
  CU_ASSERT_EQUAL(errno, ENFILE);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "pipe");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, ENFILE);
  CU_ASSERT_PTR_NULL(stream);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_popen_returns_null_pointer_and_errno_for_incorrect_mode(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Opens file. */
  errno = 0;
  stream = uportlibc_popen("echo test", "xxx");
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_pclose_closes_process(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Closes process. */
  errno = 0;
  sys_mock_waitpid_res = 2;
  sys_mock_waitpid_status = 1234;
  CU_ASSERT_EQUAL(uportlibc_pclose(stream), 1234);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "waitpid");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 2);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(*((int *) (sys_mock_ops[5].args[1].vp)), 1234);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].vp, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 2);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
}

void test_pclose_sets_errno_when_error_occurs_for_waitpid(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Closes process. */
  errno = 0;
  sys_mock_waitpid_res = -1;
  sys_mock_waitpid_err_num = ECHILD;
  CU_ASSERT_EQUAL(uportlibc_pclose(stream), -1);
  CU_ASSERT_EQUAL(errno, ECHILD);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "waitpid");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 2);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].vp, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, ECHILD);
}

void test_pclose_sets_errno_when_error_occurs_for_close(void)
{
  uportlibc_FILE *stream;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  sys_mock_fork_res = 2;
  stream = uportlibc_popen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Closes process. */
  errno = 0;
  sys_mock_waitpid_res = 2;
  sys_mock_waitpid_status = 5678;
  sys_mock_close_res = -1;
  sys_mock_close_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_pclose(stream), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "waitpid");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 2);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[5].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].vp, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 2);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "close");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, ((int *) (sys_mock_ops[2].args[0].vp))[0]);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, EIO);
}

void test_setvbuf_sets_buffer_with_fully_buffered_i_o(void)
{
  static char buf[256];
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
  /* Sets buffer. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOFBF, 256), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_PTR_EQUAL(stream->buf, buf);
  CU_ASSERT_EQUAL(stream->buf_size, 256);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  if(stream != NULL) free(stream);
}

void test_setvbuf_sets_buffer_with_line_buffered_i_o(void)
{
  static char buf[8192];
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
  /* Sets buffer. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IOLBF, 8192), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_PTR_EQUAL(stream->buf, buf);
  CU_ASSERT_EQUAL(stream->buf_size, 8192);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  if(stream != NULL) free(stream);
}

void test_setvbuf_disables_buffering(void)
{
  static char buf[16];
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
  /* Disables buffering. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, buf, UPORTLIBC__IONBF, 16), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, 0);
  CU_ASSERT_PTR_NULL(stream->buf_data_cur);
  CU_ASSERT_PTR_NULL(stream->buf_data_end);
  if(stream != NULL) free(stream);
}

void test_setbuf_sets_buffer_with_fully_buffered_i_o(void)
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
  /* Sets buffer. */
  errno = 0;
  uportlibc_setbuf(stream, buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_PTR_EQUAL(stream->buf, buf);
  CU_ASSERT_EQUAL(stream->buf_size, UPORTLIBC_BUFSIZ);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  if(stream != NULL) free(stream);
}

void test_setbuf_disables_buffering(void)
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
  /* Disables buffering. */
  errno = 0;
  uportlibc_setbuf(stream, NULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->buf_size, 0);
  CU_ASSERT_PTR_NULL(stream->buf_data_cur);
  CU_ASSERT_PTR_NULL(stream->buf_data_end);
  if(stream != NULL) free(stream);
}

void test_clearerr_clears_eof_flag_and_error_flag(void)
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
  stream->flags |= FILE_FLAG_EOF | FILE_FLAG_ERROR;
  uportlibc_clearerr(stream);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_feof_returns_one_if_eof_flag_is_set(void)
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
  stream->flags |= FILE_FLAG_EOF;
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Checks EOF flag. */
  CU_ASSERT_EQUAL(uportlibc_feof(stream), 1);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_feof_returns_zero_if_eof_flag_is_not_set(void)
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
  /* Checks EOF flag. */
  CU_ASSERT_EQUAL(uportlibc_feof(stream), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_ferror_returns_one_if_error_flag_is_set(void)
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
  stream->flags |= FILE_FLAG_ERROR;
  CU_ASSERT_PTR_NOT_NULL(stream);
  /* Checks error flag. */
  CU_ASSERT_EQUAL(uportlibc_ferror(stream), 1);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_ferror_returns_zero_if_error_flag_is_not_set(void)
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
  /* Checks error flag. */
  CU_ASSERT_EQUAL(uportlibc_ferror(stream), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fileno_returns_file_description(void)
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
  /* Gets file description. */
  CU_ASSERT_EQUAL(uportlibc_fileno(stream), stream->fd);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fflush_flushes_stream(void)
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
  CU_ASSERT_EQUAL(uportlibc_fputs("some text", stream), 9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Flushes stream. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fflush(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "some text", 9);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 9);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 9);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fflush_flushes_all_streams(void)
{
  uportlibc_FILE *stream1;
  uportlibc_FILE *stream2;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream1 = uportlibc_fopen("test1.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream1);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  stream2 = uportlibc_fopen("test2.txt", "w");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_PTR_NOT_NULL(stream2);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("stdout text", uportlibc_stdout), 11);
  CU_ASSERT_EQUAL(errno, 0); 
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("stderr text", uportlibc_stderr), 11);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("stream1 text", stream1), 12);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("stream2 text", stream2), 12);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  /* Flushes all streams. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fflush(NULL), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 10);
  /* stdin */
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, 0);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
  /* stdout */
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, uportlibc_stdout->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[1].vp, uportlibc_stdout->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[uportlibc_stdout->fd]->buf, "stdout text", 11);
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stdout->fd]->buf_data_pos, 11);
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stdout->fd]->buf_data_size, 11);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
  /* stderr */
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, uportlibc_stderr->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].args[1].vp, uportlibc_stderr->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[2].i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf, "stderr text", 11);
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf_data_pos, 11);
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf_data_size, 11);
  CU_ASSERT_EQUAL(uportlibc_stderr->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stderr->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(uportlibc_stderr->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stderr->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stderr->buf_data_cur, uportlibc_stderr->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stderr->buf_data_end, uportlibc_stderr->buf);
  CU_ASSERT_EQUAL(uportlibc_stderr->pushed_c_count, 0);
  /* stream1 */
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, stream1->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[8].args[1].vp, stream1->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[2].i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[8].res.i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream1->fd]->buf, "stream1 text", 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream1->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream1->fd]->buf_data_size, 12);
  CU_ASSERT_EQUAL(stream1->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream1->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream1->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream1->buf);
  CU_ASSERT_PTR_EQUAL(stream1->buf_data_cur, stream1->buf);
  CU_ASSERT_PTR_EQUAL(stream1->buf_data_end, stream1->buf);
  CU_ASSERT_EQUAL(stream1->pushed_c_count, 0);
  /* stream2 */
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, stream2->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[9].args[1].vp, stream2->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[2].i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[9].res.i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream2->fd]->buf, "stream2 text", 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream2->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(sys_mock_files[stream2->fd]->buf_data_size, 12);
  CU_ASSERT_EQUAL(stream2->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream2->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stderr->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream2->buf);
  CU_ASSERT_PTR_EQUAL(stream2->buf_data_cur, stream2->buf);
  CU_ASSERT_PTR_EQUAL(stream2->buf_data_end, stream2->buf);
  CU_ASSERT_EQUAL(stream2->pushed_c_count, 0);
  if(stream2 != NULL && stream2->buf != NULL) free(stream2->buf);
  if(stream2 != NULL) free(stream2);
  if(stream1 != NULL && stream1->buf != NULL) free(stream1->buf);
  if(stream1 != NULL) free(stream1);
}

void test_fflush_does_not_flush_stream_for_stream_buffer_that_is_empty(void)
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
  /* Flushes stream. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fflush(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 0);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fflush_does_not_flush_stream_that_just_is_readable(void)
{
  static char line_buf[11];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890", 10);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 10;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 11, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Flushes stream. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fflush(stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 10);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 10);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fflush_sets_errno_when_error_occurs(void)
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
  CU_ASSERT_EQUAL(uportlibc_fputs("some text", stream), 9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Flushes stream. */
  errno = 0;
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fflush(stream), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 9);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes(void)
{
  static char fread_buf[16];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefghijklmnopqr", 18);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 18;
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 8, 2, stream), 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fread_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 16);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 16);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, "abcdefghijklmnop", 16);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_for_non_empty_stream_buffer(void)
{
  static char fread_buf[15];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890ABCDEFGHIJ", 20);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 20;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgetc(stream), '1');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 5, 3, stream), 3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, "234567890ABCDEF", 15);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + 16);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + 20);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_after_unreading(void)
{
  static char fread_buf[MB_LEN_MAX];
  static char expected_fread_buf[MB_LEN_MAX];
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
  for(i = 0; i < MB_LEN_MAX; i++) expected_fread_buf[i] = (MB_LEN_MAX - i - 1) + 'a';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (uportlibc_ungetc(i + 'a', stream) != UPORTLIBC_EOF);
  }
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(errno, 0);
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, MB_LEN_MAX, 1, stream), 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, expected_fread_buf, MB_LEN_MAX);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_after_unreading_for_full_stream_buffer_with_data_part(void)
{
  static char buf[8];
  static char fread_buf[MB_LEN_MAX + 15];
  static char expected_fread_buf[MB_LEN_MAX + 15];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh12345678", 16);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 20;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgetc(stream), 'a');
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  for(i = 0; i < MB_LEN_MAX; i++) expected_fread_buf[i] = (MB_LEN_MAX - i - 1) + 'A';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (uportlibc_ungetc(i + 'A', stream) != UPORTLIBC_EOF);
  }
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(expected_fread_buf + MB_LEN_MAX, "bcdefgh12345678", 15);
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, MB_LEN_MAX + 15, 1, stream), 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, fread_buf + MB_LEN_MAX + 7);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, expected_fread_buf, MB_LEN_MAX + 15);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_after_unreading_for_unbuffered_i_o(void)
{
  static char fread_buf[MB_LEN_MAX + 10];
  static char expected_fread_buf[MB_LEN_MAX + 10];
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
  CU_ASSERT_EQUAL(uportlibc_setvbuf(stream, NULL, UPORTLIBC__IONBF, 0), 0);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890", 16);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 20;
  for(i = 0; i < MB_LEN_MAX; i++) expected_fread_buf[i] = (MB_LEN_MAX - i - 1) + 'a';
  errno = 0;
  b = 1;
  for(i = 0; i < MB_LEN_MAX; i++) {
    b &= (uportlibc_ungetc(i + 'a', stream) != UPORTLIBC_EOF);
  }
  CU_ASSERT(b);
  CU_ASSERT_EQUAL(errno, 0);
  memcpy(expected_fread_buf + MB_LEN_MAX, "1234567890", 10);
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, MB_LEN_MAX + 10, 1, stream), 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fread_buf + MB_LEN_MAX);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, expected_fread_buf, MB_LEN_MAX + 10);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_to_end_of_file(void)
{
  static char fread_buf[15];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890", 10);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 10;
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 5, 3, stream), 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fread_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, fread_buf + 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, "1234567890", 10);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_reads_bytes_after_string_writing(void)
{
  static char fread_buf[16];
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
  memcpy(sys_mock_files[stream->fd]->buf, "zyxwvutsrqponmlk", 16);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 16;
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("123456", stream), 6);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 5, 2, stream), 2);
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
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, fread_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "123456tsrqponmlk", 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 16);
  CU_ASSERT_NSTRING_EQUAL(fread_buf, "tsrqponmlk", 16);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_sets_error_flag_and_errno_when_error_occurs(void)
{
  static char fread_buf[10];
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
  /* Reads bytes. */
  errno = 0;
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 5, 2, stream), 0);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fread_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fread_sets_error_flag_and_errno_for_incorrect_stream_orientation(void)
{
  static char fread_buf[10];
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
  stream->wide_mode = 1;
  /* Reads bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fread(fread_buf, 5, 2, stream), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_gets_reads_line(void)
{
  static char line_buf[10];
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  memcpy(sys_mock_files[uportlibc_stdin->fd]->buf, "abcdef\n1234", 11);
  sys_mock_files[uportlibc_stdin->fd]->buf_data_pos = 0;
  sys_mock_files[uportlibc_stdin->fd]->buf_data_size = 11;
  memset(line_buf, 'X', 10);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_gets(line_buf), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(line_buf, "abcdef");
  CU_ASSERT_NSTRING_EQUAL(line_buf + 7, "XXX", 3);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdin->buf, "abcdef\n1234", 11);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf + 7);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf + 11);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void test_gets_reads_line_without_newline(void)
{
  static char line_buf[20];
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  memcpy(sys_mock_files[uportlibc_stdin->fd]->buf, "1234567890", 10);
  sys_mock_files[uportlibc_stdin->fd]->buf_data_pos = 0;
  sys_mock_files[uportlibc_stdin->fd]->buf_data_size = 10;
  memset(line_buf, 'X', 20);
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_gets(line_buf), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, 10);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(line_buf, "1234567890");
  CU_ASSERT_NSTRING_EQUAL(line_buf + 11, "XXXXXXXXX", 9);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void test_gets_sets_error_flag_and_errno_when_error_occurs(void)
{
  static char line_buf[256];
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Reads line. */
  errno = 0;
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  CU_ASSERT_PTR_NULL(uportlibc_gets(line_buf));
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdin->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, EIO);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void test_gets_sets_error_flag_and_errno_for_incorrect_stream_orientation(void)
{
  static char line_buf[256];
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  uportlibc_stdin->wide_mode = 1;
  /* Reads line. */
  errno = 0;
  CU_ASSERT_PTR_NULL(uportlibc_gets(line_buf));
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_EQUAL(uportlibc_stdin->flags, FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(uportlibc_stdin->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdin->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_cur, uportlibc_stdin->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdin->buf_data_end, uportlibc_stdin->buf);
  CU_ASSERT_EQUAL(uportlibc_stdin->pushed_c_count, 0);
}

void test_getw_reads_word(void)
{
  union word word_union;
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
  word_union.word = (int) (1234L * 5678L * 90L);
  memcpy(sys_mock_files[stream->fd]->buf, word_union.bytes, WORD_BIT / 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = WORD_BIT / 8;
  /* Reads word. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_getw(stream), word_union.word);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, WORD_BIT / 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, word_union.bytes, WORD_BIT / 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf + WORD_BIT / 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + WORD_BIT / 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_getw_does_not_read_word_for_too_small_bytes_in_file(void)
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
  memcpy(sys_mock_files[stream->fd]->buf, "a", 1);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 1;
  /* Reads word. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_getw(stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "read");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, UPORTLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, "a", 1);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_getw_sets_error_flag_and_errno_when_error_occurs(void)
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
  /* Reads word. */
  errno = 0;
  sys_mock_read_res = -1;
  sys_mock_read_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_getw(stream), UPORTLIBC_EOF);
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
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_getw_sets_error_flag_and_errno_for_incorrect_stream_orientation(void)
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
  stream->wide_mode = 1;
  /* Reads word. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_getw(stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fwrite_writes_bytes(void)
{
  static char fwrite_buf[16];
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
  memcpy(fwrite_buf, "abcdefghij123456", 16);
  /* Writes bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fwrite(fwrite_buf, 8, 2, stream), 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fwrite_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 16);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 16);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdefghij123456", 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 16);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 16);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fwrite_writes_bytes_after_string_writing(void)
{
  static char fwrite_buf[8];
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
  memcpy(fwrite_buf, "abcdefgh", 8);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fputs("some text", stream), 9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Writes bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fwrite(fwrite_buf, 4, 2, stream), 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[1].vp, fwrite_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "some textabcdefgh", 17);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 17);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 17);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fwrite_writes_bytes_after_line_reading(void)
{
  static char line_buf[256];
  static char fwrite_buf[6];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh\n123456", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  memcpy(fwrite_buf, "ABCDEF", 6);
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Writes bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fwrite(fwrite_buf, 3, 2, stream), 2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -6);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 9);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[1].vp, fwrite_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdefgh\nABCDEF", 15);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 15);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 15);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fwrite_sets_error_flag_and_errno_when_error_occurs(void)
{
  static char fwrite_buf[8];
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
  memcpy(fwrite_buf, "12345678", 8);
  /* Writes bytes. */
  errno = 0;
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fwrite(fwrite_buf, 4, 2, stream), 0);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, fwrite_buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fwrite_sets_error_flag_and_errno_for_incorrect_stream_orientation(void)
{
  static char fwrite_buf[10];
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
  stream->wide_mode = 1;
  memcpy(fwrite_buf, "1234567890", 10);
  /* Writes bytes. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fwrite(fwrite_buf, 5, 2, stream), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_puts_writes_string_with_newline(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Writes string with newline. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_puts("abcdefgh"), 9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdout->buf, "abcdefgh\n", 9);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf + 9);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
}

void test_puts_sets_error_flag_and_errno_when_error_occurs(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  sys_mock_files[SYS_MOCK_STDOUT_FILENO]->is_tty = 1;
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  /* Writes string with newline. */
  errno = 0;
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_puts("1234"), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stdout->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stdout->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, EIO);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(uportlibc_stdout->buf, "1234\n", 5);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf + 5);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
}

void test_puts_sets_error_flag_and_errno_when_for_incorrect_stream_orientation(void)
{
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  uportlibc_stdout->wide_mode = 1;
  /* Writes string with newline. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_puts("12345678"), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_EQUAL(uportlibc_stdout->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(uportlibc_stdout->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(uportlibc_stdout->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_cur, uportlibc_stdout->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stdout->buf_data_end, uportlibc_stdout->buf);
  CU_ASSERT_EQUAL(uportlibc_stdout->pushed_c_count, 0);
}

void test_putw_writes_word(void)
{
  union word word_union;
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
  word_union.word = (int) (112233L * 556677L);
  /* Writes word. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_putw(word_union.word, stream), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_NSTRING_EQUAL(stream->buf, word_union.bytes, WORD_BIT / 8);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf + WORD_BIT / 8);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_putw_sets_error_flag_and_errno_when_error_occurs(void)
{
  union word word_union;
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
  word_union.word = (int) (111L * 222L * 333L * 222L);
  /* Writes word. */
  errno = 0;
  sys_mock_write_res = -1;
  sys_mock_write_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_putw(word_union.word, stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[4].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IONBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL) free(stream);
}

void test_putw_sets_error_flag_and_errno_for_incorrect_stream_orientation(void)
{
  union word word_union;
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
  stream->wide_mode = 1;
  word_union.word = (int) (11L * 22L * 33L * 44L * 33L * 22L);
  /* Writes word. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_putw(word_union.word, stream), UPORTLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_perror_writes_string_with_error(void)
{
  static char str[1024];
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  strcat(strcat(strcpy(str, "some error: "), strerror(E2BIG)), "\n");
  /* Writes string with errro. */
  errno = E2BIG;
  uportlibc_perror("some error");
  CU_ASSERT_EQUAL(errno, E2BIG);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, uportlibc_stderr->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].args[1].vp, uportlibc_stderr->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, strlen(str));
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, strlen(str));
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf, str, strlen(str));
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf_data_pos, strlen(str));
  CU_ASSERT_EQUAL(sys_mock_files[uportlibc_stderr->fd]->buf_data_size, strlen(str));
  CU_ASSERT_EQUAL(uportlibc_stderr->flags, FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF);
  CU_ASSERT_EQUAL(uportlibc_stderr->buf_type, UPORTLIBC__IOLBF);
  CU_ASSERT_EQUAL(uportlibc_stderr->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(uportlibc_stderr->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stderr->buf_data_cur, uportlibc_stderr->buf);
  CU_ASSERT_PTR_EQUAL(uportlibc_stderr->buf_data_end, uportlibc_stderr->buf);
  CU_ASSERT_EQUAL(uportlibc_stderr->pushed_c_count, 0);
}

void test_ftell_returns_file_position_for_stream_buffer_with_read_data(void)
{
  static char line_buf[8];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef\n1234", 11);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 11;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 8, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Gets offset. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_ftell(stream), 7);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_ftell_returns_file_position_for_stream_buffer_with_data_to_write(void)
{
  static char line_buf[8];
  static char buf[8];
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
  CU_ASSERT_PTR_EQUAL(uportlibc_fputs("abcdefgh12345678ABCDEFGH1234", stream), 28);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  /* Gets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_ftell(stream), 28);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[7].res.i, 24);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  if(stream != NULL) free(stream);
}

void test_ftell_returns_minus_one_and_sets_errno_when_error_occurs(void)
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
  /* Gets offset. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_ftell(stream), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_sets_file_position_for_seek_set(void)
{
  static char line_buf[21];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890abcdefghij", 20);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 20;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 21, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, 15, SEEK_SET), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 15);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_sets_file_position_for_seek_cur_and_stream_that_just_is_readable(void)
{
  static char line_buf[16];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890abcdefghijABCDEFGHIJ", 30);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 30;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 16, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, 5, SEEK_CUR), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -10);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 20);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 20);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_sets_file_position_for_seek_cur_and_stream_that_just_is_writable(void)
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
  CU_ASSERT_PTR_EQUAL(uportlibc_fputs("abcdefgh", stream), 8);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, -2, SEEK_CUR), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "write");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].args[1].vp, stream->buf);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -2);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 6);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_NSTRING_EQUAL(sys_mock_files[stream->fd]->buf, "abcdefgh", 8);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 6);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_size, 8);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_WRITABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_sets_file_position_for_seek_end(void)
{
  static char line_buf[11];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefghij1234567890", 20);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 20;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 11, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, -15, SEEK_END), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, -15);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_END);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 5);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 5);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_does_not_set_file_position_for_seek_cur_and_too_small_offset(void)
{
  static char line_buf[9];
  uportlibc_FILE *stream;
  off_t min_offset = -((((((off_t) 1) << ((sizeof(off_t) * 8) - 2)) - 1) << 1) + 1) - 1;
  clear_sys_mock();
  restore_stdio_vars();
  uportlibc_init_stdio();
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  errno = 0;
  stream = uportlibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_PTR_NOT_NULL(stream);
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh1234567890", 18);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 18;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 9, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, min_offset + (18 - 8) - 1, SEEK_CUR), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseek_sets_errno_when_error_occurs(void)
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
  /* Sets file position. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fseek(stream, 0, SEEK_SET), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
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

void test_ftello_returns_file_position(void)
{
  static char line_buf[6];
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234\nabcdefghij", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 6, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Gets offset. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_ftello(stream), 5);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_ftello_returns_minus_one_and_sets_errno_when_error_occurs(void)
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
  /* Gets offset. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_ftello(stream), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseeko_sets_file_position(void)
{
  static char line_buf[19];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh1234567890", 18);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 18;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 19, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fseeko(stream, 8, SEEK_SET), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 8);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fseeko_sets_errno_when_error_occurs(void)
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
  /* Sets file position. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fseeko(stream, 0, SEEK_SET), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
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

void test_fgetpos_gets_file_position(void)
{
  static char line_buf[256];
  uportlibc_fpos_t pos;
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh\n123456", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Gets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 15);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(pos.offset, 9);
  CU_ASSERT_EQUAL(memcmp(&(pos.mb_state), &(stream->mb_state), sizeof(mbstate_t)), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fgetpos_gets_file_position_for_part_of_wide_character(void)
{
  static wchar_t line_buf[256];
  mbstate_t state;
  uportlibc_fpos_t pos;
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdef\360\237", 8);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 8;
  memcpy(&state, &(stream->mb_state), sizeof(mbstate_t));
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetws(line_buf, 256, stream), NULL);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  /* Gets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 8);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(pos.offset, 8);
  CU_ASSERT_EQUAL(memcmp(&(pos.mb_state), &(stream->mb_state), sizeof(mbstate_t)), 0);
  CU_ASSERT_NOT_EQUAL(memcmp(&(stream->mb_state), &state, sizeof(mbstate_t)), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fgetpos_sets_errno_when_error_occurs(void)
{
  uportlibc_fpos_t pos;
  mbstate_t zero_mb_state;
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
  memset(&zero_mb_state, 0, sizeof(mbstate_t));
  pos.offset = 1234;
  memcpy(&(pos.mb_state), &zero_mb_state, sizeof(mbstate_t));
  /* Gets file position. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_CUR);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(pos.offset, 1234);
  CU_ASSERT_EQUAL(memcmp(&(pos.mb_state), &(zero_mb_state), sizeof(mbstate_t)), 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fsetpos_sets_file_position(void)
{
  static char line_buf[256];
  uportlibc_fpos_t pos;
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
  memcpy(sys_mock_files[stream->fd]->buf, "1234567890\nzyxwv\n", 17);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 17;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fsetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[1].i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 11);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 11);
  CU_ASSERT_EQUAL(pos.offset, 11);
  CU_ASSERT_EQUAL(memcmp(&(stream->mb_state), &(pos.mb_state), sizeof(mbstate_t)), 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fsetpos_sets_file_position_for_part_of_wide_character(void)
{
  static wchar_t line_buf[256];
  mbstate_t state;
  uportlibc_fpos_t old_pos, pos;
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
  memcpy(sys_mock_files[stream->fd]->buf, "zyxwvusr\n\360\237\240", 12);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 12;
  memcpy(&state, &(stream->mb_state), sizeof(mbstate_t));
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgetws(line_buf, 256, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &old_pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgetwc(stream), UPORTLIBC_WEOF);
  CU_ASSERT_EQUAL(errno, EILSEQ);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fsetpos(stream, &old_pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 9);
  /* Sets file position. */
  errno = 0;
  CU_ASSERT_EQUAL(uportlibc_fsetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 10);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[1].i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[9].res.i, 12);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 12);
  CU_ASSERT_EQUAL(pos.offset, 12);
  CU_ASSERT_EQUAL(memcmp(&(stream->mb_state), &(pos.mb_state), sizeof(mbstate_t)), 0);
  CU_ASSERT_NOT_EQUAL(memcmp(&(stream->mb_state), &state, sizeof(mbstate_t)), 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, 1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_fsetpos_sets_errno_when_error_occurs(void)
{
  static char line_buf[256];
  uportlibc_fpos_t pos;
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
  CU_ASSERT_EQUAL(uportlibc_fgetpos(stream, &pos), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  CU_ASSERT_EQUAL(uportlibc_fsetpos(stream, &pos), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, EIO);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 0);
  CU_ASSERT_EQUAL(pos.offset, 0);
  CU_ASSERT_EQUAL(memcmp(&(stream->mb_state), &(pos.mb_state), sizeof(mbstate_t)), 0);
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

void test_remove_removes_file(void)
{
  clear_sys_mock();
  errno = 0;
  sys_mock_lstat_res = 0;
  memset(&sys_mock_lstat_buf, 0, sizeof(struct stat));
  sys_mock_lstat_buf.st_mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  /* Removes file. */
  CU_ASSERT_EQUAL(uportlibc_remove("file.txt"), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "lstat");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].cp, "file.txt");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[0].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "unlink");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].cp, "file.txt");
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
}

void test_remove_removes_directory(void)
{
  clear_sys_mock();
  errno = 0;
  sys_mock_lstat_res = 0;
  memset(&sys_mock_lstat_buf, 0, sizeof(struct stat));
  sys_mock_lstat_buf.st_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  /* Removes directory. */
  CU_ASSERT_EQUAL(uportlibc_remove("dir"), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "lstat");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].cp, "dir");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[0].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "rmdir");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].cp, "dir");
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
}

void test_remove_sets_errno_when_error_occurs(void)
{
  clear_sys_mock();
  errno = 0;
  sys_mock_lstat_res = -1;
  sys_mock_lstat_err_num = EIO;
  /* Removes file. */
  CU_ASSERT_EQUAL(uportlibc_remove("file.txt"), -1);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "lstat");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].cp, "file.txt");
  CU_ASSERT_PTR_NOT_NULL(sys_mock_ops[0].args[1].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, EIO);
}

void test_rewind_resets_file_position(void)
{
  static char line_buf[15];
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
  memcpy(sys_mock_files[stream->fd]->buf, "abcdefgh\n123456", 15);
  sys_mock_files[stream->fd]->buf_data_pos = 0;
  sys_mock_files[stream->fd]->buf_data_size = 15;
  errno = 0;
  CU_ASSERT_PTR_EQUAL(uportlibc_fgets(line_buf, 15, stream), line_buf);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  /* Sets file position. */
  errno = 0;
  uportlibc_rewind(stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 0);
  CU_ASSERT_EQUAL(stream->flags, FILE_FLAG_READABLE);
  CU_ASSERT_EQUAL(stream->buf_type, UPORTLIBC__IOFBF);
  CU_ASSERT_EQUAL(stream->wide_mode, -1);
  CU_ASSERT_PTR_NOT_NULL(stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_cur, stream->buf);
  CU_ASSERT_PTR_EQUAL(stream->buf_data_end, stream->buf);
  CU_ASSERT_EQUAL(stream->pushed_c_count, 0);
  if(stream != NULL && stream->buf != NULL) free(stream->buf);
  if(stream != NULL) free(stream);
}

void test_rewind_clears_error_flag(void)
{
  static char line_buf[15];
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
  stream->flags |= FILE_FLAG_ERROR;
  /* Sets file position. */
  errno = 0;
  uportlibc_rewind(stream);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 0);
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

void test_rewind_sets_errno_when_error_occurs(void)
{
  static char line_buf[15];
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
  stream->flags |= FILE_FLAG_ERROR;
  /* Sets file position. */
  errno = 0;
  sys_mock_lseek_res = -1;
  sys_mock_lseek_err_num = EIO;
  uportlibc_rewind(stream);
  CU_ASSERT_EQUAL(errno, EIO);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "lseek");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, stream->fd);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, SEEK_SET);
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, EIO);
  CU_ASSERT_EQUAL(sys_mock_files[stream->fd]->buf_data_pos, 0);
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

int add_stdio_suite(void)
{
  CU_pSuite suite;
  suite = CU_add_suite("stdio suite", initialize_stdio, cleanup_stdio);
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    "init_stdio initializes",
    test_init_stdio_initializes) == NULL) return -1;
  if(CU_add_test(suite,
    "init_stdio initializes for stdin as terminal",
    test_init_stdio_initializes_for_stdin_as_terminal) == NULL) return -1;
  if(CU_add_test(suite,
    "init_stdio initializes for stdout as terminal",
    test_init_stdio_initializes_for_stdout_as_terminal) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens file for reading",
    test_fopen_opens_file_for_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens file for writing",
    test_fopen_opens_or_creates_file_for_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens file for writing at end of file",
    test_fopen_opens_or_creates_file_for_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens file for reading and writing",
    test_fopen_opens_file_for_reading_and_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens or creates file for reading and writing",
    test_fopen_opens_or_creates_file_for_reading_and_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens or creates file for reading and writing at end of file",
    test_fopen_opens_or_creates_file_for_reading_and_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen opens file that is terminal",
    test_fopen_opens_file_that_is_terminal) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen returns null pointer and sets errno when error occurs",
    test_fopen_returns_null_pointer_and_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fopen returns null pointer and errno for incorrect mode",
    test_fopen_returns_null_pointer_and_errno_for_incorrect_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for reading",
    test_fdopen_associates_file_description_for_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for writing",
    test_fdopen_associates_file_description_for_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for writing at end of file",
    test_fdopen_associates_file_description_for_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for r+ mode",
    test_fdopen_associates_file_description_for_r_plus_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for w+ mode",
    test_fdopen_associates_file_description_for_w_plus_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen associates file description for reading and writing at end of file",
    test_fdopen_associates_file_description_for_reading_and_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen returns null pointer and errno for incorrect mode",
    test_fdopen_returns_null_pointer_and_errno_for_incorrect_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens file for reading",
    test_freopen_opens_file_for_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens or creates file for writing",
    test_freopen_opens_or_creates_file_for_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens or creates file for writing at end of file",
    test_freopen_opens_or_creates_file_for_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens file for reading and writing",
    test_freopen_opens_file_for_reading_and_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens or creates file for reading and writing",
    test_freopen_opens_or_creates_file_for_reading_and_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen opens or creates file for reading and writing end of file",
    test_freopen_opens_or_creates_file_for_reading_and_writing_at_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "freopen returns null pointer and sets error flag and errno when error occurs",
    test_freopen_returns_null_pointer_and_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fdopen returns null pointer and errno for incorrect mode",
    test_fdopen_returns_null_pointer_and_errno_for_incorrect_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "fclose closes stream",
    test_fclose_closes_stream) == NULL) return -1;
  if(CU_add_test(suite,
    "fclose flushes stream",
    test_fclose_flushes_stream) == NULL) return -1;
  if(CU_add_test(suite,
    "fclose closes process",
    test_fclose_closes_process) == NULL) return -1;
  if(CU_add_test(suite,
    "fclose returns EOF when error occurs",
    test_fclose_returns_eof_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "popen opens process for reading",
    test_popen_opens_process_for_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "popen executes command in process for reading",
    test_popen_executes_command_in_process_for_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "popen opens process for writing",
    test_popen_opens_process_for_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "popen executes command in process for writing",
    test_popen_executes_command_in_process_for_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "popen returns null pointer and sets errno when error occurs for fork",
    test_popen_returns_null_pointer_and_sets_errno_when_error_occurs_for_fork) == NULL) return -1;
  if(CU_add_test(suite,
    "popen returns null pointer and sets errno when error occurs for pipe",
    test_popen_returns_null_pointer_and_sets_errno_when_error_occurs_for_pipe) == NULL) return -1;
  if(CU_add_test(suite,
    "popen returns null pointer and errno for incorrect mode",
    test_popen_returns_null_pointer_and_errno_for_incorrect_mode) == NULL) return -1;
  if(CU_add_test(suite,
    "pclose closes process",
    test_pclose_closes_process) == NULL) return -1;
  if(CU_add_test(suite,
    "pclose sets errno when error occurs for waitpid",
    test_pclose_sets_errno_when_error_occurs_for_waitpid) == NULL) return -1;
  if(CU_add_test(suite,
    "pclose sets errno when error occurs for close",
    test_pclose_sets_errno_when_error_occurs_for_close) == NULL) return -1;
  if(CU_add_test(suite,
    "setvbuf sets buffer with fully buffered I/O",
    test_setvbuf_sets_buffer_with_fully_buffered_i_o) == NULL) return -1;
  if(CU_add_test(suite,
    "setvbuf sets buffer with line buffered I/O",
    test_setvbuf_sets_buffer_with_line_buffered_i_o) == NULL) return -1;
  if(CU_add_test(suite,
    "setvbuf disables buffering",
    test_setvbuf_disables_buffering) == NULL) return -1;
  if(CU_add_test(suite,
    "setbuf sets buffer with fully buffered I/O",
    test_setbuf_sets_buffer_with_fully_buffered_i_o) == NULL) return -1;
  if(CU_add_test(suite,
    "setbuf disables buffering",
    test_setbuf_disables_buffering) == NULL) return -1;
  if(CU_add_test(suite,
    "clearerr clears EOF flag and error flag",
    test_clearerr_clears_eof_flag_and_error_flag) == NULL) return -1;
  if(CU_add_test(suite,
    "feof returns one if EOF flag is set",
    test_feof_returns_one_if_eof_flag_is_set) == NULL) return -1;
  if(CU_add_test(suite,
    "feof returns zero if EOF flag is not set",
    test_feof_returns_zero_if_eof_flag_is_not_set) == NULL) return -1;
  if(CU_add_test(suite,
    "ferror returns one if error flag is set",
    test_ferror_returns_one_if_error_flag_is_set) == NULL) return -1;
  if(CU_add_test(suite,
    "ferror returns zero if error flag is not set",
    test_ferror_returns_zero_if_error_flag_is_not_set) == NULL) return -1;
  if(CU_add_test(suite,
    "fileno returns file description",
    test_fileno_returns_file_description) == NULL) return -1;
  if(CU_add_test(suite,
    "fflush flushes stream",
    test_fflush_flushes_stream) == NULL) return -1;
  if(CU_add_test(suite,
    "fflush flushes all streams",
    test_fflush_flushes_all_streams) == NULL) return -1;
  if(CU_add_test(suite,
    "fflush does not flush stream for stream buffer that is empty",
    test_fflush_does_not_flush_stream_for_stream_buffer_that_is_empty) == NULL) return -1;
  if(CU_add_test(suite,
    "fflush does not flush stream that just is readable",
    test_fflush_does_not_flush_stream_that_just_is_readable) == NULL) return -1;
  if(CU_add_test(suite,
    "fflush sets errno when error occurs",
    test_fflush_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes",
    test_fread_reads_bytes) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes for non-empty stream buffer",
    test_fread_reads_bytes_for_non_empty_stream_buffer) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes after unreading",
    test_fread_reads_bytes_after_unreading) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes after unreading for full stream buffer with data part",
    test_fread_reads_bytes_after_unreading_for_full_stream_buffer_with_data_part) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes after unreading for unbuffered I/O",
    test_fread_reads_bytes_after_unreading_for_unbuffered_i_o) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes to end of file",
    test_fread_reads_bytes_to_end_of_file) == NULL) return -1;
  if(CU_add_test(suite,
    "fread reads bytes after string writing",
    test_fread_reads_bytes_after_string_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fread sets error flag and errno when error occurs",
    test_fread_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fread sets error flag and errno for incorrect stream orientation",
    test_fread_sets_error_flag_and_errno_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "gets reads line without newline",
    test_gets_reads_line_without_newline) == NULL) return -1;
  if(CU_add_test(suite,
    "gets reads line",
    test_gets_reads_line) == NULL) return -1;
  if(CU_add_test(suite,
    "gets sets error flag and errno when error occurs",
    test_gets_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "gets sets error flag and errno for incorrect stream orientation",
    test_gets_sets_error_flag_and_errno_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "getw reads word",
    test_getw_reads_word) == NULL) return -1;
  if(CU_add_test(suite,
    "getw does not read word for too small bytes in file",
    test_getw_does_not_read_word_for_too_small_bytes_in_file) == NULL) return -1;
  if(CU_add_test(suite,
    "getw sets error flag and errno when error occurs",
    test_getw_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "getw sets error flag and errno for incorrect stream orientation",
    test_getw_sets_error_flag_and_errno_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "fwrite writes bytes",
    test_fwrite_writes_bytes) == NULL) return -1;
  if(CU_add_test(suite,
    "fwrite writes bytes after string writing",
    test_fwrite_writes_bytes_after_string_writing) == NULL) return -1;
  if(CU_add_test(suite,
    "fwrite writes bytes after line reading",
    test_fwrite_writes_bytes_after_line_reading) == NULL) return -1;
  if(CU_add_test(suite,
    "fwrite sets error flag and errno when error occurs",
    test_fwrite_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fwrite sets error flag and errno for incorrect stream orientation",
    test_fwrite_sets_error_flag_and_errno_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "puts writes string with newline",
    test_puts_writes_string_with_newline) == NULL) return -1;
  if(CU_add_test(suite,
    "puts sets error flag and errno when error occurs",
    test_puts_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "puts sets error flag and errno when for incorrect stream orientation",
    test_puts_sets_error_flag_and_errno_when_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "putw writes word",
    test_putw_writes_word) == NULL) return -1;
  if(CU_add_test(suite,
    "putw sets error flag and errno when error occurs",
    test_putw_sets_error_flag_and_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "putw sets error flag and errno for incorrect stream orientation",
    test_putw_sets_error_flag_and_errno_for_incorrect_stream_orientation) == NULL) return -1;
  if(CU_add_test(suite,
    "perror writes string with error",
    test_perror_writes_string_with_error) == NULL) return -1;
  if(CU_add_test(suite,
    "ftell returns file position for stream buffer with read data",
    test_ftell_returns_file_position_for_stream_buffer_with_read_data) == NULL) return -1;
  if(CU_add_test(suite,
    "ftell returns file position for stream buffer with data to write",
    test_ftell_returns_file_position_for_stream_buffer_with_data_to_write) == NULL) return -1;
  if(CU_add_test(suite,
    "ftell returns -1 and sets errno when error occurs",
    test_ftell_returns_minus_one_and_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek sets file position for SEEK_SET",
    test_fseek_sets_file_position_for_seek_set) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek sets file position for SEEK_CUR and stream that just is readable",
    test_fseek_sets_file_position_for_seek_cur_and_stream_that_just_is_readable) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek sets file position for SEEK_CUR and stream that just is writable",
    test_fseek_sets_file_position_for_seek_cur_and_stream_that_just_is_writable) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek sets file position for SEEK_END",
    test_fseek_sets_file_position_for_seek_end) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek does not set file position for SEEK_CUR and too small offset",
    test_fseek_does_not_set_file_position_for_seek_cur_and_too_small_offset) == NULL) return -1;
  if(CU_add_test(suite,
    "fseek sets errno when error occurs",
    test_fseek_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "ftello returns file position",
    test_ftello_returns_file_position) == NULL) return -1;
  if(CU_add_test(suite,
    "ftello returns minus one and sets errno when error occurs",
    test_ftello_returns_minus_one_and_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fseeko sets file position",
    test_fseeko_sets_file_position) == NULL) return -1;
  if(CU_add_test(suite,
    "fseeko sets errno when error occurs",
    test_fseeko_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fgetpos gets file position",
    test_fgetpos_gets_file_position) == NULL) return -1;
  if(CU_add_test(suite,
    "fgetpos gets file position for part of wide character",
    test_fgetpos_gets_file_position_for_part_of_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "fgetpos sets errno when error occurs",
    test_fgetpos_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "fsetpos sets file position",
    test_fsetpos_sets_file_position) == NULL) return -1;
  if(CU_add_test(suite,
    "fsetpos sets file position for part of wide character",
    test_fsetpos_sets_file_position_for_part_of_wide_character) == NULL) return -1;
  if(CU_add_test(suite,
    "fsetpos sets errno when error occurs",
    test_fsetpos_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "remove removes file",
    test_remove_removes_file) == NULL) return -1;
  if(CU_add_test(suite,
    "remove removes directory",
    test_remove_removes_directory) == NULL) return -1;
  if(CU_add_test(suite,
    "remove sets errno when error occurs",
    test_remove_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "rewind resets file position",
    test_rewind_resets_file_position) == NULL) return -1;
  if(CU_add_test(suite,
    "rewind clears error flag",
    test_rewind_clears_error_flag) == NULL) return -1;
  if(CU_add_test(suite,
    "rewind sets errno when error occurs",
    test_rewind_sets_errno_when_error_occurs) == NULL) return -1;
  return 0;
}
