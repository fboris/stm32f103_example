#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

char *strchr(const char *s, int c);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcat(char *dst, char *src);
char *strdup(const char *str);

void *memset(void *dest, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *cs, const void *ct, unsigned long count);

#endif
