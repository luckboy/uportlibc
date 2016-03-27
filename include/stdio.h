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
#ifndef _STDIO_H
#define _STDIO_H

#include <uportlibc/mbstate.h>
#define __W 'c'
#include <uportlibc/w_stdio.h>
#include <uportsys/sys.h>

#define BUFSIZ                  __UPORTSYS_PIPE_BUF
#define EOF                     (-1)
#define FILENAME_MAX            __UPORTSYS_PATH_MAX
#define FOPEN_MAX               __UPORTSYS_INT_MAX

#define _IONBF                  0
#define _IOFBF                  1
#define _IOLBF                  2

#ifndef SEEK_SET
#define SEEK_SET                __UPORTSYS_SEEK_SET
#define SEEK_CUR                __UPORTSYS_SEEK_CUR
#define SEEK_END                __UPORTSYS_SEEK_END
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

typedef struct
{
  off_t offset;
  __uportlibc_mbstate_t mb_state;
} fpos_t;

extern char *optarg;
extern int opterr;
extern int optind;
extern int optopt;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* System functions. */

int rename(const char *old_path_name, const char *new_path_name);

/* Other functions. */

void clearerr(FILE *stream);
int fclose(FILE *stream);
FILE *fdopen(int fd, const char *mode);
int feof(FILE *stream);
int ferror(FILE *stream);
int fflush(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
int fileno(FILE *stream);
void flockfile(FILE *stream);
FILE *fopen(const char *file_name, const char *mode);
size_t fread(void *ptr, size_t elem_size, size_t elem_count, FILE *stream);
FILE *freopen(const char *file_name, const char *mode, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
int fseeko(FILE *stream, off_t offset, int whence);
int fsetpos(FILE *stream, const fpos_t *pos);
long ftell(FILE *stream);
off_t ftello(FILE *stream);
int ftrylockfile(FILE *stream);
int funlockfile(FILE *stream);
size_t fwrite(const void *ptr, size_t elem_size, size_t elem_count, FILE *stream);
int getopt(int argc, char *const *argv, const char *options);
char *gets(char *str);
int getw(FILE *stream);
int pclose(FILE *stream);
void perror(const char *str);
FILE *popen(const char *command, const char *mode);
int puts(const char *str);
int putw(int w, FILE *stream);
int remove(const char *path_name);
void rewind(FILE *stream);
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int type, size_t size);
int sprintf(char *str, const char *format, ...);
int vsprintf(char *str, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
