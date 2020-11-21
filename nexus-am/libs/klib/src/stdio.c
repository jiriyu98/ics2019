#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
/*
  va_list arg;
  int done;
  va_start(arg, fmt);
  done = vsprintf(NULL, fmt, arg);
  va_end(arg);
  return done;
*/
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char * str;
  const char* s;
  int num;
  static char numtrans[16]="0123456789ABCDEF";
  for(str=out;*fmt;fmt++){
	  if(*fmt!='%'){
		  *str++=*fmt;
		  continue;
	  }
	  ++fmt;
	  switch(*fmt){
		  case 's':
			  s=va_arg(ap,char *);
			  while(*s)
				  *str++=*s++;
			  break;
		 case 'd':
			  num=va_arg(ap,int);
			  int i=0;char tmp[20];
			  if(num==0){
				  *str++='0';
				  break;
			  }
			  while(num){
				  tmp[i]=numtrans[num%10];
				  num/=10;
				  i++;
			  }
			  while(i){
				  i--;
				  *str++=tmp[i];
			  }
			  break;
		default:;
	  }
  }
  *str='\0';
  return str-out;
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
