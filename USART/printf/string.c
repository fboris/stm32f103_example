#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "string.h"

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)                                                                      
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

#define SS (sizeof(size_t))

char *strchr(const char *s, int c)
{
        for (; *s && *s != c; s++);
        return (*s == c) ? (char *)s : NULL;
}

char *strcpy(char *dest, const char *src)
{
        const  char *s = src;
        char *d = dest;
        while ((*d++ = *s++));
        return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
        const char *s = src;
        char *d = dest;
        while (n-- && (*d++ = *s++));
        return dest;
}

size_t strlen(const char *s)
{
        const char *a = s;
        const size_t *w;
        for (; (uintptr_t) s % ALIGN; s++)
                if (!*s) return (s - a);
        for (w = (const void *) s; !HASZERO(*w); w++);
        for (s = (const void *) w; *s; s++);
        return (s - a);
}

int strcmp(const char* s1, const char* s2)
{
        while(*s1 && (*s1==*s2))
                s1++,s2++;
        return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

char *strcat(char* dst, char* src)
{
        if(src == NULL)
                return NULL;

        char* retAddr = dst;

        /* --- Find last position --- */
        while (*dst++ != '\0');

        dst--;
        while ((*dst++ = *src++));
        return retAddr;
}


void *memset(void *dest, int c, size_t n)
{
        unsigned char *s = dest;
        c = (unsigned char)c;
        for (; ((uintptr_t)s & ALIGN) && n; n--) *s++ = c;
        if (n) {
                size_t *w, k = ONES * c;
                for (w = (void *)s; n>=SS; n-=SS, w++) *w = k;
                for (s = (void *)w; n; n--, s++) *s = c;
        }
        return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
        void *ret = dest;

        //Cut rear
        uint8_t *dst8 = dest;
        const uint8_t *src8 = src;
        switch (n % 4) {
                case 3 : *dst8++ = *src8++;
                case 2 : *dst8++ = *src8++;
                case 1 : *dst8++ = *src8++;
                case 0 : ;
        }

        //stm32 data bus width
        uint32_t *dst32 = (void *)dst8;
        const uint32_t *src32 = (void *)src8;
        n = n / 4;
        while (n--) {
                *dst32++ = *src32++;
        }

        return ret;
}

void *memmove(void *dest, const void *src, size_t count)
{
        char *tmp;
        const char *s;

        if(dest <= src) {
                tmp = dest;
                s = src;
                while(count--)
                        *tmp++ = *s++;
        } else {
                tmp = dest;
                tmp += count;
                s= src;
                s += count;
                while(count--)
                        *--tmp = *--s;
        }
        return dest;
}

int memcmp(const void *cs, const void *ct, unsigned long count)
{
	const unsigned char *su1, *su2;
	int res = 0;
 
	for (su1=cs, su2=ct; 0<count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
 
	return res;
}
