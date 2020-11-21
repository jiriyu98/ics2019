#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char pbuf[1024];
  va_list ap;
  int i;
  va_start(ap,fmt);
  i=vsprintf(pbuf,fmt,ap);
  va_end(ap);
  int j=0;
  while(pbuf[j]!='\0'){
     _putc(pbuf[j]);
	 j++;
  }
  _putc('\0');
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	char* p;
	char tmp[256], *tmps;
	for (p = out; *fmt; fmt++){
	  if (*fmt != '%'){
	      *p++ = *fmt;
	      continue;
	  }
	  fmt++;
	  switch (*fmt){
			case 'd':
				itoa(tmp, va_arg(ap, int), 10);
        strcpy(p, tmp);
        p += strlen(tmp);
        break;
      case 'x':
        itoa(tmp, va_arg(ap, int), 16);
        strcpy(p, tmp);
        p += strlen(tmp);
        break;
      case 's':
        tmps = va_arg(ap, char *);
				strcpy(p, tmps);
        p += strlen(tmps);
        break;
      default:
        break;
	  }
	}
	return (p - out);
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	int ret;
	va_start(ap, fmt);
	ret = vsprintf(out, fmt, ap);
	va_end(ap);

	return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
	return 0;
}

#endif
