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
#include <uportlibc/w_stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <wchar.h>
#include "float_priv.h"
#define __W W
#include "w_vxprintf.h"
#define __W W
#include <uportlibc/w_name.h>

#define FLAG_HASH                   (1 << 0)
#define FLAG_ZERO                   (1 << 1)
#define FLAG_MINUS                  (1 << 2)
#define FLAG_SPACE                  (1 << 3)
#define FLAG_PLUS                   (1 << 4)

#define LENGTH_CHAR                 1
#define LENGTH_SHORT                2
#define LENGTH_LONG                 3
#define LENGTH_LONG_LONG            4
#define LENGTH_LONG_DOUBLE          5

#define ARG_TYPE_UNDEF              0
#define ARG_TYPE_CHAR_INT           1
#define ARG_TYPE_SCHAR              2
#define ARG_TYPE_SHORT              3
#define ARG_TYPE_INT                4
#define ARG_TYPE_LONG               5
#define ARG_TYPE_LONG_LONG          6
#define ARG_TYPE_UCHAR              7
#define ARG_TYPE_USHORT             8
#define ARG_TYPE_UINT               9
#define ARG_TYPE_ULONG              10
#define ARG_TYPE_ULONG_LONG         11
#define ARG_TYPE_DOUBLE             12
#define ARG_TYPE_LONG_DOUBLE        13
#define ARG_TYPE_WINT_T             14
#define ARG_TYPE_CONST_CHAR_PTR     15
#define ARG_TYPE_CONST_WCHAR_T_PTR  16
#define ARG_TYPE_INT_PTR            17
#define ARG_TYPE_VOID_PTR           18

union arg_value
{
  long long i;
  unsigned long long u;
  long double f;
  const char *const_cp;
  const wchar_t *const_wcp;
  int *ip;
  void *vp;
};

struct conv_spec
{
  signed char has_width_arg_idx;
  signed char has_prec_arg_idx;
  unsigned flags;
  unsigned arg_idx;
  union
  {
    long value;
    unsigned arg_idx;
  } width;
  union
  {
    long value;
    unsigned arg_idx;
  } prec;
  unsigned length;
  __W_CHAR conv_c;
};

struct counter
{
  int count;
  int has_overflow;
};

char *__uportlibc_ulltostr(unsigned long long x, int base, char *str, int is_uppercase);

static int __W_NAME(, parse_conv_spec_num)(__W_CONST_CHAR_PTR *format_ptr)
{
  unsigned long res;
  int saved_errno = errno;
  errno = 0;
  res = __W_STR_NAME(toul)(*format_ptr, (__W_CHAR_PTR *) format_ptr, 10);
  if(res == ULONG_MAX && errno != 0) {
    errno = EOVERFLOW;
    return -1;
  }
  if(res <= INT_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  errno = saved_errno;
  return res;
}

static int __W_NAME(, parse_arg_pos)(__W_CONST_CHAR_PTR *format_ptr, unsigned *curr_arg_idx_ptr, unsigned *arg_count_ptr)
{
  __W_CONST_CHAR_PTR ptr;
  unsigned arg_idx;
  for(ptr = *format_ptr; *ptr >= '0' && *ptr <= '9'; ptr++);
  if(*ptr == '$') {
    __W_CONST_CHAR_PTR format = *format_ptr;
    int res = __W_NAME(, parse_conv_spec_num)(&format);
    if(res == -1) return -1;
    if(res == 0) {
      errno = EOVERFLOW;
      return -1;
    }
    arg_idx = res - 1;
    *format_ptr = ptr;
  } else {
    arg_idx = *curr_arg_idx_ptr;
    (*curr_arg_idx_ptr)++;
    if(*curr_arg_idx_ptr + 1 > *arg_count_ptr) *arg_count_ptr = *curr_arg_idx_ptr + 1;
  }
  if(arg_idx >= NL_ARGMAX) {
    errno = EINVAL;
    return -1;
  }
  return (int) arg_idx;
}

static int __W_NAME(, parse_conv_spec)(__W_CONST_CHAR_PTR *format_ptr, struct conv_spec *spec, unsigned char *arg_types, unsigned *curr_arg_idx_ptr, unsigned *arg_count_ptr)
{
  __W_CONST_CHAR_PTR format = *format_ptr;
  unsigned curr_arg_idx = *curr_arg_idx_ptr;
  unsigned arg_count = *arg_count_ptr;
  if(*format == '%') {
    format++;
    if(*format != '%') {
      unsigned new_arg_type;
      int arg_idx, value, is_prec;
      arg_idx = __W_NAME(, parse_arg_pos)(&format, &curr_arg_idx, &arg_count);
      if(arg_idx == -1) return -1;
      spec->arg_idx = arg_idx;
      /* Parses flag characters. */
      spec->flags = 0;
      for(; *format != 0; format++) {
        int is_flag_c;
        switch(*format) {
        case '#':
          spec->flags |= FLAG_HASH;
          is_flag_c = 1;
          break;
        case '0':
          spec->flags |= FLAG_ZERO;
          is_flag_c = 1;
          break;
        case '-':
          spec->flags |= FLAG_MINUS;
          is_flag_c = 1;
          break;
        case ' ':
          spec->flags |= FLAG_SPACE;
          is_flag_c = 1;
          break;
        case '+':
          spec->flags |= FLAG_PLUS;
          is_flag_c = 1;
          break;
        case '\'':
          break;
        default:
          is_flag_c = 0;
          break;
        }
        if(is_flag_c) break;
      }
      /* Parses a width. */
      if(*format == '*') {
        spec->has_width_arg_idx = 1;
        format++;
        arg_idx = __W_NAME(, parse_arg_pos)(&format, &curr_arg_idx, &arg_count);
        if(arg_idx == -1) return -1;
        spec->width.arg_idx = arg_idx;
        if(arg_types != NULL) arg_types[spec->width.arg_idx] = ARG_TYPE_INT;
      } else {
        spec->has_width_arg_idx = 0;
        if(*format >= '0' && *format <= '9') {
          value = __W_NAME(, parse_conv_spec_num)(&format);
          if(value == -1) return -1;
          spec->width.value = value;
        } else
          spec->width.value = -1;
      }
      /* Parses a precision. */
      if(*format == '.') {
        format++;
        is_prec = 1;
        if(*format == '*') {
          spec->has_prec_arg_idx = 1;
          format++;
          arg_idx = __W_NAME(, parse_arg_pos)(&format, &curr_arg_idx, &arg_count);
          if(arg_idx == -1) return -1;
          spec->prec.arg_idx = arg_idx;
          if(arg_types != NULL) arg_types[spec->prec.arg_idx] = ARG_TYPE_INT;
        } else {
          spec->has_prec_arg_idx = 0;
          value = __W_NAME(, parse_conv_spec_num)(&format);
          if(value == -1) return -1;
          spec->prec.value = value;
        }
      } else
        is_prec = 0;
      /* Parses a length. */
      spec->length = 0;
      switch(*format) {
      case 'h':
        if(format[1] == 'h') {
          spec->length = LENGTH_CHAR;
          format += 2;
        } else {
          spec->length = LENGTH_SHORT;
          format++;
        }
        break;
      case 'l':
        if(format[1] == 'l') {
          spec->length = LENGTH_LONG_LONG;
          format += 2;
        } else {
          spec->length = LENGTH_LONG;
          format++;
        }
        break;
      case 'L':
        spec->length = LENGTH_LONG_DOUBLE;
        format++;
        break;
      }
      /* Parses a conversion character. */
      spec->conv_c = *format;
      switch(*format) {
      case 'd':
      case 'i':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_INT;
          break;
        case LENGTH_CHAR:
          new_arg_type = ARG_TYPE_SCHAR;
          break;
        case LENGTH_SHORT:
          new_arg_type = ARG_TYPE_SHORT;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_LONG;
          break;
        case LENGTH_LONG_LONG:
          new_arg_type = ARG_TYPE_LONG_LONG;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'u':
      case 'o':
      case 'X':
      case 'x':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_UINT;
          break;
        case LENGTH_CHAR:
          new_arg_type = ARG_TYPE_UCHAR;
          break;
        case LENGTH_SHORT:
          new_arg_type = ARG_TYPE_USHORT;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_ULONG;
          break;
        case LENGTH_LONG_LONG:
          new_arg_type = ARG_TYPE_ULONG_LONG;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'f':
      case 'E':
      case 'e':
      case 'G':
      case 'g':
      case 'A':
      case 'a':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_DOUBLE;
          break;
        case LENGTH_LONG_DOUBLE:
          new_arg_type = ARG_TYPE_LONG_DOUBLE;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'c':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_CHAR_INT;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_WINT_T;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'C':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_WINT_T;
        spec->conv_c = 'c';
        spec->length = LENGTH_LONG;
        break;
      case 's':
        switch(spec->length) {
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_CONST_WCHAR_T_PTR;
          break;
        default:
          new_arg_type = ARG_TYPE_CONST_CHAR_PTR;
          break;
        }
        break;
      case 'S':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_CONST_WCHAR_T_PTR;
        spec->conv_c = 's';
        spec->length = LENGTH_LONG;
        break;
      case 'p':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        if(is_prec && !spec->has_prec_arg_idx) spec->prec.value = 1;
        new_arg_type = ARG_TYPE_VOID_PTR;
        break;
      case 'n':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_INT_PTR;
        break;
      default:
        errno = EINVAL;
        return -1;
      }
      if(arg_types != NULL) arg_types[spec->arg_idx] = new_arg_type;
      format++;
      *format_ptr = format;
      *curr_arg_idx_ptr = curr_arg_idx;
      *arg_count_ptr = arg_count;
    }
  }
  return 0;
}

static int __W_NAME(, print_char)(struct __W_NAME(vx, printf_stream) *stream, __W_CHAR_INT c, struct counter *counter)
{
  if(stream->put_char(stream->data, c) == -1) return -1;
  if(counter->count < INT_MAX)
    counter->count++;
  else
    counter->has_overflow = 1;
  return 0;
}

static int __W_NAME(, print_fill)(struct __W_NAME(vx, printf_stream) *stream, __W_CHAR_INT c, size_t count, struct counter *counter)
{
  size_t i;
  for(i = 0; i < count; i++) {
    if(__W_NAME(, print_char)(stream, c, counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, print_bytes)(struct __W_NAME(vx, printf_stream) *stream, const char *str, size_t len, struct counter *counter)
{
  size_t i;
  for(i = 0; i < len; i++) {
    if(__W_NAME(, print_char)(stream, str[i], counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, convert_int)(struct __W_NAME(vx, printf_stream) *stream, const struct conv_spec *spec, const union arg_value *arg_values, const union arg_value *value, struct counter *counter)
{
  char buf[sizeof(unsigned long long) * 8 + 1];
  unsigned long long x;
  int len, width, prec, is_prec, spaces, zeros;
  int base, is_unsigned = 0, is_uppercase = 0;
  switch(spec->conv_c) {
  case 'd':
  case 'i':
    base = 10;
    break;
  case 'u':
    base = 10;
    is_unsigned = 1;
    break;
  case 'o':
    base = 8;
    is_unsigned = 1;
    break;
  case 'X':
    base = 16;
    is_unsigned = is_uppercase = 1;
  case 'x':
    base = 16;
    is_unsigned = 1;
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  /* Prepares. */
  if(!is_unsigned) {
    if(value->i >= 0)
      x = (unsigned long long) value->i;
    else
      x = (unsigned long long) (-value->i);
  } else
    x = value->u;
  __uportlibc_ulltostr(x, base, buf, is_uppercase);
  len = (int) strlen(buf);
  width = (spec->has_width_arg_idx ? arg_values[spec->width.arg_idx].i : spec->width.value);
  prec = (spec->has_prec_arg_idx ? arg_values[spec->prec.arg_idx].i : spec->prec.value);
  if(prec >= 0) {
    is_prec = 1;
  } else {
    prec = 1;
    is_prec = 0;
  }
  if((spec->flags & FLAG_MINUS) != 0 && width >= 0) width = -width;
  spaces = (width >= 0 ? width : -width) - len;
  zeros = prec - len;
  spaces -= zeros;
  if(!is_unsigned) {
    if((spec->flags & FLAG_PLUS) != 0 || (spec->flags & FLAG_SPACE) != 0) {
      spaces--;
    } else {
      if(arg_values[spec->arg_idx].i < 0) spaces--;
    }
  }
  if(spec->flags & FLAG_HASH && zeros + len > 0) {
    if(base == 8)
      spaces--;
    else if(base == 16)
      spaces -= 2;
  }
  if((spec->flags & FLAG_ZERO) != 0 && !is_prec) {
    zeros = spaces;
    spaces = 0;
  }
  spaces = (spaces > 0 ? spaces : 0);
  zeros = (zeros > 0 ? zeros : 0);
  /* Prints. */
  if(width >= 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  if(!is_unsigned) {
    __W_CHAR sign_c;
    int is_sign_c;
    if((spec->flags & FLAG_PLUS) != 0) {
      sign_c = (arg_values[spec->arg_idx].i >= 0 ? '+' : '-');
      is_sign_c = 1;
    } else if((spec->flags & FLAG_PLUS) != 0) {
      sign_c = (arg_values[spec->arg_idx].i >= 0 ? ' ' : '-');
      is_sign_c = 1;
    } else {
      is_sign_c = 0;
      if(arg_values[spec->arg_idx].i < 0) {
        sign_c = '-';
        is_sign_c = 1;
      }
    }
    if(is_sign_c) {
      if(__W_NAME(, print_char)(stream, sign_c, counter) == -1) return -1;
    }
  }
  if((spec->flags & FLAG_HASH) != 0 && zeros + len > 0) {
    if(base == 8) {
      if(__W_NAME(, print_char)(stream, '0', counter) == -1) return -1;
    } else if(base == 16) {
      if(__W_NAME(, print_char)(stream, '0', counter) == -1) return -1;
      if(__W_NAME(, print_char)(stream, (is_uppercase ? 'X' : 'x'), counter) == -1) return -1;
    }
  }
  if(__W_NAME(, print_fill)(stream, '0', zeros, counter) == -1) return -1;
  if(__W_NAME(, print_bytes)(stream, buf, len, counter) == -1) return -1;
  if(width < 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, convert_float)(struct __W_NAME(vx, printf_stream) *stream, const struct conv_spec *spec, const union arg_value *arg_values, const union arg_value *value, struct counter *counter)
{
  char exp_buf[sizeof(unsigned long long) * 8 + 1];
  int exp_len = 0;
  long double x, tmp_x;
  int is_sign;
  int width, prec, spaces, zeros;
  long double base, exp_base;
  unsigned digit_base;
  int exp = 0, min_exp;
  int64_t i, len, digits = 0, last_int_digit_idx = 0, carry_idx = -1, last_zero_idx = -1;
  int is_e = 0, is_g = 0, is_hex = 0, is_uppercase = 0;
  char a_c, exp_c;
  __W_CHAR sign_c;
  int is_sign_c;
  const char *str = NULL;
  switch(spec->conv_c) {
  case 'F':
    is_uppercase = 1;
    break;
  case 'f':
    break;
  case 'E':
    is_e = is_uppercase = 1;
    break;
  case 'e':
    is_e = is_uppercase = 1;
    break;
  case 'G':
    is_g = is_uppercase = 1;
    break;
  case 'g':
    is_g = 1;
    break;
  case 'A':
    is_e = is_g = is_hex = is_uppercase = 1;
    break;
  case 'a':
    is_e = is_g = is_hex = 1;
    break;
  default:
    errno = EINVAL;
    break;
  }
  a_c = (is_uppercase ? 'A' : 'a');
  if(is_hex) {
    base = 16.0;
    exp_base = 2.0;
    digit_base = 16;
    min_exp = LDBL_MIN_HEX_EXP;
    exp_c = (is_uppercase ? 'P' : 'p');
  } else {
    base = 10.0;
    exp_base = 2.0;
    digit_base = 10;
    min_exp = LDBL_MIN_DEC_EXP;
    exp_c = (is_uppercase ? 'E' : 'e');
  }
  /* Prepares. */
  x = value->f;
  if(isnanl(x)) {
    str = (is_uppercase ? "NAN" : "nan");
    is_sign = 0;
  } else {
    if(x >= 0) {
      is_sign = 0;
    } else {
      x = -x;
      is_sign = 1;
    }
    if(x == HUGE_VALL && HUGE_VALL != LDBL_MAX) {
      str = (is_uppercase ? "INF" : "inf");
    } else {
      if(x != 0.0) {
        exp = floorl(logl(x) / logl(exp_base));
        if(exp >= min_exp) {
          x /= powl(exp_base, exp);
        } else {
          x /= powl(exp_base, exp + 2);
          x *= exp_base * exp_base;
        }
      } else
        exp = 0;
    }
  }
  width = (spec->has_width_arg_idx ? arg_values[spec->width.arg_idx].i : spec->width.value);
  prec = (spec->has_prec_arg_idx ? arg_values[spec->prec.arg_idx].i : spec->prec.value);
  if((spec->flags & FLAG_MINUS) != 0 && width >= 0) width = -width;
  if(prec < 0) prec = (is_hex ? LDBL_MAX_HEX_MANT_DIG : 6);
  if(is_g && prec == 0) prec = 1;
  if(str == NULL) {
    /* Prepares for a floating-point number that isn't NaN and infinity. */
    if(is_g && !is_e) is_e = (exp >= prec || exp <= -prec);
    do {
      if(is_e) {
        last_int_digit_idx = 0;
        digits = prec + 1;
        if(is_g) digits--;
      } else {
        last_int_digit_idx = exp;
        digits = (prec + exp >= 0 ? prec + exp : 0);
      }
      tmp_x = x;
      carry_idx = last_zero_idx = -1;
      for(i = 0; i < digits; i++) {
        unsigned digit = floorl(tmp_x);
        if(digit < 0) digit = 0;
        if(digit >= digit_base) digit = digit_base - 1;
        if(digit >= base - 1)
          carry_idx = (carry_idx == -1 ? i : carry_idx);
        else
          carry_idx = -1;
        if(digit == 0)
          last_zero_idx = (last_zero_idx == -1 ? i : last_zero_idx);
        else
          last_zero_idx = -1;
        tmp_x = (tmp_x - digit) * base;
      }
      if(tmp_x >= base / 2.0) {
        if(carry_idx == -1) carry_idx = digits;
      } else
        carry_idx = -1;
      if(carry_idx == 0) {
        x = 1.0;
        exp++;
        continue;
      }
    } while(0);
    if(is_g) {
      int64_t int_digits = (last_int_digit_idx >= 0 ? last_int_digit_idx + 1 : 0);
      if(carry_idx != -1) digits = (int_digits < carry_idx ? carry_idx : int_digits);
      if(last_zero_idx != -1) digits = (int_digits < last_zero_idx ? last_zero_idx : int_digits);
    }
    len = digits;
    if(last_int_digit_idx < 0) len -= last_int_digit_idx;
    if(last_int_digit_idx < digits || (spec->flags & FLAG_HASH) != 0) len++;
    if(is_e) {
      unsigned long long abs_exp = (unsigned long long) (exp > 0 ? exp : -exp);
      __uportlibc_ulltostr(abs_exp, 10, exp_buf, 0);
      exp_len = (int) strlen(exp_buf);
      len += exp_len + 2;
      if(!is_hex && exp_len < 2) len += 2 - exp_len;
    }
  } else {
    /* Prepares for a floating-point number that is NaN or infinity. */
    len = strlen(str);
  }
  spaces = (width >= 0 ? width : -width);
  spaces = (spaces - len >= INT_MIN ? spaces - len : INT_MIN);
  zeros = 0;
  if((spec->flags & FLAG_PLUS) != 0 || (spec->flags & FLAG_SPACE) != 0) {
    spaces--;
  } else {
    if(is_sign) spaces--;
  }
  if(is_hex) spaces -= 2;
  if((spec->flags & FLAG_ZERO) != 0 && str != NULL) {
    zeros = spaces;
    spaces = 0;
  }
  spaces = (spaces > 0 ? spaces : 0);
  zeros = (zeros > 0 ? zeros : 0);
  /* Prints. */
  if(width >= 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  if((spec->flags & FLAG_PLUS) != 0) {
    sign_c = (!is_sign ? '+' : '-');
    is_sign_c = 1;
  } else if((spec->flags & FLAG_PLUS) != 0) {
    sign_c = (!is_sign ? ' ' : '-');
    is_sign_c = 1;
  } else {
    is_sign_c = 0;
    if(!is_sign) {
      sign_c = '-';
      is_sign_c = 1;
    }
  }
  if(is_sign_c) {
    if(__W_NAME(, print_char)(stream, sign_c, counter) == -1) return -1;
  }
  if(str == NULL) {
    /* Prints a floating-point number that isn't NaN and infinity. */
    if(is_hex) {
      if(__W_NAME(, print_char)(stream, '0', counter) == -1) return -1;
      if(__W_NAME(, print_char)(stream, (is_uppercase ? 'X' : 'x'), counter) == -1) return -1;
    }
    if(__W_NAME(, print_fill)(stream, '0', zeros, counter) == -1) return -1;
    if(last_int_digit_idx < 0) {
      int first_zeros = -last_int_digit_idx;
      if(first_zeros > 0) {
        if(__W_NAME(, print_char)(stream, '0', counter) == -1) return -1;
        if(__W_NAME(, print_char)(stream, '.', counter) == -1) return -1;
        if(__W_NAME(, print_fill)(stream, '0', first_zeros - 1, counter) == -1) return -1;
      }
    }
    for(i = 0; i < digits; i++) {
      unsigned digit = floorl(x);
      __W_CHAR c;
      if(digit < 0) digit = 0;
      if(digit >= digit_base) digit = digit_base - 1;
      if(carry_idx != -1) {
        if(i + 1 == carry_idx)
          digit += 1;
        else if(i >= carry_idx)
          digit = '0';
      }
      c = (digit < 10 ? digit + '0' : digit + a_c - 10);
      if(__W_NAME(, print_char)(stream, c, counter) == -1) return -1;
      if(last_int_digit_idx == i) {
        if(i < digits || (spec->flags & FLAG_HASH) != 0) {
          if(__W_NAME(, print_char)(stream, '.', counter) == -1) return -1;
        }
      }
      x = (x - digit) * base;
    }
    if(is_e) {
      if(__W_NAME(, print_char)(stream, exp_c, counter) == -1) return -1;
      if(__W_NAME(, print_char)(stream, (exp >= 0 ? '+' : '-'), counter) == -1) return -1;
      if(!is_hex && exp_len < 2) {
        if(__W_NAME(, print_fill)(stream, '0', 2 - exp_len, counter) == -1) return -1;
      }
      if(__W_NAME(, print_bytes)(stream, exp_buf, exp_len, counter) == -1) return -1;
    }
  } else {
    /* Prints a floating-point that is NaN or infinity. */
    if(__W_NAME(, print_bytes)(stream, str, len, counter) == -1) return -1;
  }
  if(width < 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, convert_char)(struct __W_NAME(vx, printf_stream) *stream, const struct conv_spec *spec, union arg_value *arg_values, const union arg_value *value, struct counter *counter)
{
  __W_CHAR_INT c = 0;
#if __W != 'w'
  char buf[MB_LEN_MAX];
  size_t len;
#endif
  int width, spaces;
  /* Prepares. */
#if __W != 'w'
  if(spec->length != LENGTH_LONG) {
    c = (int) ((unsigned char) (value->i));
    len = 1;
  } else {
    mbstate_t state;
    wint_t wc = (wint_t) (value->i);
    memset(&state, 0, sizeof(mbstate_t));
    len = wcrtomb(buf, (wchar_t) wc, &state);
    if(len == ((size_t) (-1))) return -1;
  }
#else
  if(spec->length != LENGTH_LONG) {
    c = btowc((int) ((unsigned char) (value->i)));
    if(c == WEOF) {
      errno = EILSEQ;
      return -1;
    }
  } else
    c = (wchar_t) ((wint_t) (value->i));
#endif
  width = (spec->has_width_arg_idx ? arg_values[spec->width.arg_idx].i : spec->width.value);
  if((spec->flags & FLAG_MINUS) != 0 && width >= 0) width = -width;
#if __W != 'w'
  spaces = (width >= 0 ? width : -width) - ((size_t) len);
#else
  spaces = (width >= 0 ? width : -width) - 1;
#endif
  if(spaces < 0) spaces = 0; 
  /* Prints. */
  if(width >= 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
#if __W != 'w'
  if(spec->length != LENGTH_LONG) {
    if(__W_NAME(, print_char)(stream, c, counter) == -1) return -1;
  } else {
    size_t i;
    for(i = 0; i < len; i++) {
      if(__W_NAME(, print_char)(stream, buf[i], counter) == -1) return -1;
    }
  }
#else
  if(spec->length != LENGTH_LONG) {
    if(__W_NAME(, print_char)(stream, (wchar_t) c, counter) == -1) return -1;
  } else {
    if(__W_NAME(, print_char)(stream, c, counter) == -1) return -1;
  }
#endif
  if(width < 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, convert_str)(struct __W_NAME(vx, printf_stream) *stream, const struct conv_spec *spec, const union arg_value *arg_values, const union arg_value *value, struct counter *counter)
{
  const char *str = NULL;
  const wchar_t *wstr = NULL;
  size_t len, count;
  int width, prec, spaces;
  /* Prepares. */
  if(spec->length != LENGTH_LONG)
    str = value->const_cp;
  else
    wstr = value->const_wcp;
  width = (spec->has_width_arg_idx ? arg_values[spec->width.arg_idx].i : spec->width.value);
  prec = (spec->has_prec_arg_idx ? arg_values[spec->prec.arg_idx].i : spec->prec.value);
  count = (prec >= 0 ? prec : SSIZE_MAX * 2UL + 1UL);
#if __W != 'w'
  if(spec->length != LENGTH_LONG) {
    for(len = 0; len < count && str[len] != 0; len++);
  } else {
    mbstate_t state;
    size_t i;
    memset(&state, 0, sizeof(mbstate_t));
    len = 0;
    for(i = 0; wstr[i] != 0; i++) {
      size_t mb_len = wcrtomb(NULL, wstr[i], &state);
      if(mb_len == ((size_t) (-1))) return -1;
      len += mb_len;
      if(len >= count) {
        len = count;
        break;
      }
    }
  }
#else
  if(spec->length != LENGTH_LONG) {
    mbstate_t state;
    size_t i;
    memset(&state, 0, sizeof(mbstate_t));
    len = 0;
    for(i = 0; str[i] != 0; i++) {
      size_t res = mbrlen(str + i, 1, &state);
      if(res == ((size_t) (-1))) return -1;
      if(res != ((size_t) (-2))) len++;
      if(len >= count) break;
    }
  } else {
    for(len = 0; len < count && wstr[len] != 0; len++);
  }
#endif
  if((spec->flags & FLAG_MINUS) != 0 && width >= 0) width = -width;
  spaces = (width >= 0 ? width : -width);
  spaces = (spaces - ((int64_t) len) >= INT_MIN ? spaces - len : INT_MIN);
  if(spaces < 0) spaces = 0; 
  /* Prints. */
  if(width >= 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
#if __W != 'w'  
  if(spec->length != LENGTH_LONG) {
    size_t i;
    for(i = 0; i < len; i++) {
      if(__W_NAME(, print_char)(stream, str[i], counter) == -1) return -1;
    }
  } else {
    char buf[MB_LEN_MAX];
    mbstate_t state;
    size_t i, j;
    memset(&state, 0, sizeof(mbstate_t));
    i = 0;
    for(j = 0; str[j] != 0; j++) {
      size_t k, mb_len = wcrtomb(NULL, wstr[j], &state);
      if(mb_len == ((size_t) (-1))) return -1;
      i += mb_len;
      if(i >= len) break;
      for(k = 0; k < mb_len; k++) {
        if(__W_NAME(, print_char)(stream, buf[k], counter) == -1) return -1;
      }
    }
  }
#else
  if(spec->length != LENGTH_LONG) {
    mbstate_t state;
    size_t i, j;
    memset(&state, 0, sizeof(mbstate_t));
    i = 0;
    for(j = 0; str[j] != 0; j++) {
      wchar_t wc;
      size_t res = mbrtowc(&wc, str + j, 1, &state);
      if(res == ((size_t) (-1))) return -1;
      if(res != ((size_t) (-2))) len++;
      if(i >= len) break;
      if(__W_NAME(, print_char)(stream, wc, counter) == -1) return -1;
    }
  } else {
    size_t i;
    for(i = 0; i < len; i++) {
      if(__W_NAME(, print_char)(stream, wstr[i], counter) == -1) return -1;
    }
  }
#endif
  if(width < 0) {
    if(__W_NAME(, print_fill)(stream, ' ', spaces, counter) == -1) return -1;
  }
  return 0;
}

static int __W_NAME(, convert_ptr)(struct __W_NAME(vx, printf_stream) *stream, const struct conv_spec *spec, const union arg_value *arg_values, const union arg_value *value, struct counter *counter)
{
  struct conv_spec new_spec = *spec;
  union arg_value new_value;
  new_spec.flags |= FLAG_HASH;
  new_spec.length = LENGTH_LONG;
  new_spec.conv_c = 'x';
  new_value.u = (unsigned long long) (value->vp);
  return __W_NAME(, convert_int)(stream, &new_spec, arg_values, &new_value, counter);
}

int __W_NAME(__uportlibc_vx, printf)(struct __W_NAME(vx, printf_stream) *stream, __W_CONST_CHAR_PTR format, va_list ap)
{
  __W_CONST_CHAR_PTR tmp_format = format;
  unsigned char arg_types[NL_ARGMAX];
  union arg_value arg_values[NL_ARGMAX];
  unsigned i, curr_arg_idx = 0, arg_count = 0;
  struct counter counter;
  for(i = 0; i < NL_ARGMAX; i++) arg_types[i] = ARG_TYPE_UNDEF;
  while(*tmp_format != 0) {
    struct conv_spec spec;
    int res = __W_NAME(, parse_conv_spec)(&tmp_format, &spec, arg_types, &curr_arg_idx, &arg_count);
    if(res == -1) return -1;
    if(res == 0) tmp_format++;
  }
  for(i = 0; i < arg_count; i++) {
    switch(arg_types[i]) {
    case ARG_TYPE_CHAR_INT:
      arg_values[i].i = (long long) ((unsigned char) va_arg(ap, int));
      break;
    case ARG_TYPE_SCHAR:
      arg_values[i].i = (signed char) va_arg(ap, int);
      break;
    case ARG_TYPE_SHORT:
      arg_values[i].i = (short) va_arg(ap, int);
      break;
    case ARG_TYPE_INT:
      arg_values[i].i = va_arg(ap, int);
      break;
    case ARG_TYPE_LONG:
      arg_values[i].i = va_arg(ap, long);
      break;
    case ARG_TYPE_LONG_LONG:
      arg_values[i].i = va_arg(ap, long long);
      break;
    case ARG_TYPE_UCHAR:
      arg_values[i].u = (unsigned char) va_arg(ap, unsigned);
      break;
    case ARG_TYPE_USHORT:
      arg_values[i].u = (unsigned short) va_arg(ap, unsigned);
      break;
    case ARG_TYPE_UINT:
      arg_values[i].u = va_arg(ap, unsigned);
      break;
    case ARG_TYPE_ULONG:
      arg_values[i].u = va_arg(ap, unsigned long);
      break;
    case ARG_TYPE_ULONG_LONG:
      arg_values[i].u = va_arg(ap, unsigned long long);
      break;
    case ARG_TYPE_DOUBLE:
      arg_values[i].f = va_arg(ap, double);
      break;
    case ARG_TYPE_LONG_DOUBLE:
      arg_values[i].f = va_arg(ap, long double);
      break;
    case ARG_TYPE_WINT_T:
#if __SIZEOF_WINT_T__ >= __SIZEOF_INT__
      arg_values[i].u = (unsigned long long) va_arg(ap, wint_t);
#else
      arg_values[i].u = (unsigned long long) ((wint_t) va_arg(ap, unsigned));
#endif
      break;
    case ARG_TYPE_CONST_CHAR_PTR:
      arg_values[i].const_cp = va_arg(ap, const char *);
      break;
    case ARG_TYPE_CONST_WCHAR_T_PTR:
      arg_values[i].const_wcp = va_arg(ap, const wchar_t *);
      break;
    case ARG_TYPE_INT_PTR:
      arg_values[i].ip = va_arg(ap, int *);
      break;
    case ARG_TYPE_VOID_PTR:
      arg_values[i].ip = va_arg(ap, void *);
      break;
    default:
      errno = EINVAL;
      return -1;
    }
  }
  counter.count = 0;
  counter.has_overflow = 0;
  curr_arg_idx = 0;
  while(*format != 0) {
    struct conv_spec spec;
    int res = __W_NAME(, parse_conv_spec)(&format, &spec, NULL, &curr_arg_idx, &arg_count);
    if(res == -1) return -1;
    if(res != 0) {
      switch(spec.conv_c) {
      case 'd':
      case 'i':
      case 'u':
      case 'o':
      case 'X':
      case 'x':
        if(__W_NAME(, convert_int)(stream, &spec, arg_values, &(arg_values[spec.arg_idx]), &counter) == -1)
          return -1;
        break;
      case 'F':
      case 'f':
      case 'E':
      case 'e':
      case 'G':
      case 'g':
      case 'A':
      case 'a':
        if(__W_NAME(, convert_float)(stream, &spec, arg_values, &(arg_values[spec.arg_idx]), &counter) == -1)
          return -1;
        break;
      case 'c':
        if(__W_NAME(, convert_char)(stream, &spec, arg_values, &(arg_values[spec.arg_idx]), &counter) == -1)
          return -1;
        break;
      case 's':
        if(__W_NAME(, convert_str)(stream, &spec, arg_values, &(arg_values[spec.arg_idx]), &counter) == -1)
          return -1;
        break;
      case 'p':
        if(__W_NAME(, convert_ptr)(stream, &spec, arg_values, &(arg_values[spec.arg_idx]), &counter) == -1)
          return -1;
        break;
      case 'n':
        *(arg_values[spec.arg_idx].ip) = counter.count;
        break;
      }
    } else {
      if(__W_NAME(, print_char)(stream, *format, &counter) == -1) return -1;
      format++;
    }
  }
  if(!counter.has_overflow) {
    return counter.count;
  } else {
    errno = EOVERFLOW;
    return -1;
  }
}
