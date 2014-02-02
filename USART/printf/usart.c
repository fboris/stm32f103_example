#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

char *utoa(unsigned int num, char *dst, unsigned int base)
{
	char buf[33] = {0};
	char *p = &buf[32];

	if (num == 0){
    	*--p = '0';
    }
  	else{
    	for (; num; num/=base)
      	*--p = "0123456789ABCDEF" [num % base];
  	}
  	return strcpy(dst, p);
} 
char *itoa(int num, char *dst, int base)
{
 	if (base == 10 && num < 0) {
		utoa(-num, dst+1, base);
    	*dst = '-';
  	}
  	else
   		utoa(num, dst, base);

  	return dst;
} 

/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : getch_base()  &  putch_base(char str)
**功能 : Serial read/write callback functions
**輸入 :
**輸出 :
**使用 :
**=====================================================================================================*/
/*=====================================================================================================*/
char getch_base(void)
{

}

void putch_base(char str)
{
	USART_SendData(USART1, (uint16_t)str);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

/* Serial read/write callback functions */
serial_ops serial = {
    .getch = getch_base,
    .putch = putch_base,
};

void ltoa(char *buf, unsigned long i, int base)
{
	char *s;
#define LEN        25
	int rem;
	char rev[LEN + 1];

	if (i == 0)
		s = "0";
	else {
		rev[LEN] = 0;
		s = &rev[LEN];

		while (i) {
			rem = i % base;

			if (rem < 10)
				*--s = rem + '0';
			else if (base == 16)
				*--s = "abcdef"[rem - 10];

			i /= base;
		}
	}

	strcpy(buf, s);
}


int puts(const char* msg)
{   
    for(; *msg; msg++)
    serial.putch(*msg);

    return 1;
}

int sprintf(char *dst, const char *fmt, ...)
{
  union {
    int i;
    const char *s;
    unsigned u;
  } argv;
  char *p = dst;
  va_list arg_list;

  va_start(arg_list, fmt);
  for (; *fmt; ++fmt) {
    if (*fmt == '%') {
      switch (*++fmt) {
        case '%':
          *p++ = '%';
        break;
        case 'c':
          argv.i = va_arg(arg_list, int);
          *p++ = (char)argv.i;
        break;
        case 'd':
        case 'i':
          argv.i = va_arg(arg_list, int);
          itoa(argv.i, p, 10);
          p += strlen(p);
        break;
        case 'u':
          argv.u = va_arg(arg_list, unsigned);
          utoa(argv.u, p, 10);
          p += strlen(p);
        break;
        case 's':
          argv.s = va_arg(arg_list, const char *);
          strcpy(p, argv.s);
          p += strlen(p);
        break;
        case 'x':
        case 'X':
          argv.u = va_arg(arg_list, unsigned);
          utoa(argv.u, p, 16);
          p += strlen(p);
        break;
      }
    }
    else
      *p++ = *fmt;
  }
  va_end(arg_list);
  *p = '\0';

  return p - dst;
}
 
int printf(const char *fmt, ...)
{
    char buf[8];
    union {
        int i;
        const char *s;
        unsigned u;
    } argv;
    va_list arg_list;
    
    va_start(arg_list, fmt);
    for (; *fmt; ++fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
                case '%':
                    serial.putch('%');
                break;
                case 'c':
                    argv.i = va_arg(arg_list, int);
                    serial.putch(argv.i);
                break;
                case 'd':
                case 'i':
                    argv.i = va_arg(arg_list, int);
                    itoa(argv.i, buf, 10);
                    puts(buf);
                break;
                case 'u':
                    argv.u = va_arg(arg_list, unsigned);
                    utoa(argv.u, buf, 10);
                    puts(buf);
                break;
                case 's':
                    argv.s = va_arg(arg_list, const char *);
                    puts(argv.s);
                break;
                case 'x':
                case 'X':
                    argv.u = va_arg(arg_list, unsigned);
                    utoa(argv.u, buf, 16);
                    puts(buf);
                break;
            }
        }
        else {
            serial.putch(*fmt);
        }
    } 
    va_end(arg_list);
    return 1;

}