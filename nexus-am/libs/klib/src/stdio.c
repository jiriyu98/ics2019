#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list arg;
  int done;
  va_start(arg, fmt);
  done = vsprintf(NULL, fmt, arg);
  va_end(arg);
 
  return done;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	char* p;
	char tmp[256];
	va_list p_next_arg = ap;
	for (p = out; *fmt; fmt++){
	  if (*fmt != '%'){
	      *p++ = *fmt;
	      continue;
	  }
	  fmt++;
	  switch (*fmt){
			case 'd':
				itoa(tmp, *((int*)p_next_arg), 10);
        strcpy(p, tmp);
        p_next_arg += 4;
        p += strlen(tmp);
        break;
      case 'x':
        itoa(tmp, *((int*)p_next_arg), 16);
        strcpy(p, tmp);
        p_next_arg += 4;
        p += strlen(tmp);
        break;
      case 's':
        strcpy(p, tmp);
        p_next_arg += 4;
        p += strlen(tmp);
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
