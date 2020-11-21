#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
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
	int i;
	va_start(ap,fmt);
    i=vsprintf(out,fmt,ap);
	va_end(ap);
  	return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
