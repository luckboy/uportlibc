/*
 * Copyright (c) 2016, 2018 Łukasz Szpakowski
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
#ifndef _SYS_MOCK_PRIV_H
#define _SYS_MOCK_PRIV_H

#include <sys/types.h>
#include <sys/stat.h>

#define MAX_SYS_MOCK_ARG_COUNT       8
#define MAX_SYS_MOCK_OP_COUNT        1024
#define MAX_SYS_MOCK_FILE_COUNT      256
#define SYS_MOCK_FILE_BUF_SIZE       (16 * 1024)
#define MAX_SYS_MOCK_ENV_COUNT       1024
#define MAX_SYS_MOCK_DATA_SEG_SIZE   (512 * 1024)
#define MAX_SYS_MOCK_MEM_AREA_COUNT  256

union sys_mock_value
{
  long i;
  unsigned long u;
  off_t o;
  char *cp;
  int *ip;
  void *vp;
};

struct sys_mock_op
{
  const char *fun_name;
  union sys_mock_value args[MAX_SYS_MOCK_ARG_COUNT];
  union sys_mock_value res;
  int err_num;
};

struct sys_mock_file
{
  int count;
  signed char is_static;
  signed char has_static_buf;
  signed char is_tty;
  const char *file_name;
  int flags;
  mode_t mode;
  char *buf;
  size_t buf_size;
  off_t buf_data_pos;
  off_t buf_data_size;
};

struct sys_mock_mem_area
{
  char *ptr;
  void *ptr_to_free;
  size_t len;
};

extern unsigned sys_mock_op_count;
extern struct sys_mock_op sys_mock_ops[MAX_SYS_MOCK_OP_COUNT];

extern int sys_mock_last_opened_fd;
extern size_t sys_mock_file_buf_size;
extern int sys_mock_file_is_tty;
extern struct sys_mock_file *sys_mock_files[MAX_SYS_MOCK_FILE_COUNT];
extern char sys_mock_data_seg[MAX_SYS_MOCK_DATA_SEG_SIZE];
extern char *sys_mock_break;
extern struct sys_mock_mem_area sys_mock_mem_areas[MAX_SYS_MOCK_MEM_AREA_COUNT];

extern void *sys_mock_mmap_res;
extern int sys_mock_mmap_err_num;

extern int sys_mock_munmap_res;
extern int sys_mock_munmap_err_num;

extern int sys_mock_lstat_res;
extern int sys_mock_lstat_err_num;
extern struct stat sys_mock_lstat_buf;

extern int sys_mock_waitpid_res;
extern int sys_mock_waitpid_err_num;
extern int sys_mock_waitpid_status;

extern int sys_mock_open_res;
extern int sys_mock_open_err_num;

extern int sys_mock_close_res;
extern int sys_mock_close_err_num;

extern int sys_mock_execve_err_num;

extern pid_t sys_mock_fork_res;
extern int sys_mock_fork_err_num;

extern off_t sys_mock_lseek_res;
extern int sys_mock_lseek_err_num;

extern int sys_mock_pipe_res;
extern int sys_mock_pipe_err_num;

extern ssize_t sys_mock_read_res;
extern int sys_mock_read_err_num;

extern int sys_mock_rmdir_res;
extern int sys_mock_rmdir_err_num;

extern void *sys_mock_sbrk_res;
extern int sys_mock_sbrk_err_num;

extern ssize_t sys_mock_write_res;
extern int sys_mock_write_err_num;

extern int sys_mock_unlink_res;
extern int sys_mock_unlink_err_num;

void init_sys_mock(void);
void final_sys_mock(void);
void clear_sys_mock(void);

int add_sys_mock_op(const struct sys_mock_op *op);

int add_sys_mock_file(const char *file_name, int flags, mode_t mode);
int delete_sys_mock_file(int fd);
int dup_sys_mock_file(int old_fd, int new_fd);
int is_opened_sys_mock_file(int fd);

#endif
