#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	size_t len = 0;
	while(s[len]!='\0'){
		len++;
	}
  return len;
}

char *strcpy(char* dst,const char* src) {
  size_t i;
  for(i=0; src[i]!='\0'; i++){
    dst[i] = src[i];
	}
  dst[i]='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
	dst[n] = '\0';
  size_t i;
  for(i=0; src[i]!='\0' && i<n; i++){
    dst[i] = src[i];
	}
  dst[++i]='\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  char *tmp = dst;
  while(*dst){
		dst++;
	}
  while((*dst++ = *src++) != '\0');
  return tmp;
}

int strcmp(const char* s1, const char* s2) {
	while(*s1 && *s2 && *s1 == *s2){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
	while(--n && *s1 && *s2 && *s1 == *s2){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

void* memset(void* v,int c,size_t n) {
	void *dst = v;
	while(n--){
		*(char *)v++ = (char)c;
	}
	return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(out - in >= n);
  size_t i;
  for (i = 0; i < n; ++i){
  	printf("%s\n", (char *)out);
  	printf("%s\n", (char *)in);
    ((uint8_t *)out)[i] = ((uint8_t *)in)[i];
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
	if(!n) return 0;
	while(--n && *(char *)s1 == *(char *)s2){
		s1++;
		s2++;
	}
	return *(char *)s1 - *(char *)s2;
}

#endif
