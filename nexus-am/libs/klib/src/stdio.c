#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/* refer: https://github.com/likmin/ics2019/blob/3bc63f31e248bfbb87fa4c491b5f146ecf798f76/nexus-am/libs/klib/src/stdio.c */

int printf(const char *fmt, ...) {
  va_list ap;
  char out[2048];
  va_start(ap, fmt);
  vsprintf(out, fmt, ap);
  va_end(ap);
  char *ptr = out;
  while (*ptr) {
    _putc(*ptr);
    ptr++;
  }
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int t;
  int sign = 0;
  unsigned int lim = 0;
  char *ptr = NULL;
  char *start = out;
  char buffer[50];
  char Representation[] = "0123456789abcdef";

  while (*fmt) {
    /*
       if (*fmt == '\') {
       switch (*++fmt) {
       case 'n':
     *start++ = '\n';
     default:
     assert(0);
     }
     }
     */

    if (*fmt == '%') {
      if (*(fmt + 1) == '0') {
        fmt++;
        lim = 0;
        for (int k = 0; k <= 9; k++) {
          if (*(fmt + 1) == Representation[k]) {
            lim = k;
            fmt++;
            break;
          }
        }
      }

      switch (*++fmt) {
        case 'd':
          t = va_arg(ap, int);
          ptr = &buffer[49];
          *ptr = '\0';
          if (t < 0) {
            if (t == (-2147483647 - 1)) {
              printf("INT_MIN!!!\n");
              assert(0);
            }
            t = -t;
            sign = 1;
          }
          do {
            *--ptr = Representation[t % 10];
            t /= 10;
          } while (t);
          while (&buffer[49] - ptr < lim) {
            *--ptr = Representation[0];
          }
          if (sign) *--ptr = '-';
          while (*ptr) {
            *start = *ptr;
            start++;
            ptr++;
          }
          break;

        case 'x':
          t = va_arg(ap, unsigned int);
          ptr = &buffer[49];
          *ptr = '\0';
          do {
            *--ptr = Representation[t % 16];
            t /= 16;
          } while (t);
          while (&buffer[49] - ptr < lim) {
            *--ptr = Representation[0];
          }
          while (*ptr) {
            *start++ = *ptr++;
          }
          break;

        case 'c':
          *start++ = (char)va_arg(ap, int);
          break;

        case 's':
          ptr = va_arg(ap, char *);
          while (*ptr) {
            *start++ = *ptr++;
          }
          break;

        case 'p':
          t = va_arg(ap, unsigned int);
          ptr = &buffer[49];
          *ptr = '\0';
          do {
            *--ptr = Representation[t % 16];
            t /= 16;
          } while (t);
          while (&buffer[49] - ptr < lim) {
            *--ptr = Representation[0];
          }

          *start++ = '0';
          *start++ = 'x';
          while (*ptr) {
            *start++ = *ptr++;
          }
          break;

        default:
          printf("unknow case %s\n", fmt);
          assert(0);
      }
    } else {
      *start = *fmt;
      start++;
    }
    fmt++;
  }
  *start = '\0';
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsprintf(out, fmt, ap);
  va_end(ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) { return 0; }


#endif
