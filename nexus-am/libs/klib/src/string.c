#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	size_t len = -1;
	while(s[++len]!='\0');
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
  dst[i]='\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  char *tmp = dst;
  while(*dst++);
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
	while(--n){
		*(int *)v++ = c;
	}
	return v;
}

void* memcpy(void* out, const void* in, size_t n) {
	while(--n){
		*(int *)out++ = *(int *)in++;
	}
	return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
	while(--n && *(int *)s1 && *(int *)s2 && *(int *)s1 == *(int *)s2){
		(int *)s1++;
		(int *)s2++;
	}
	return *(int *)s1 - *(int *)s2;
}

#endif
