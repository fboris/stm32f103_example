#ifndef __USART_H


typedef struct {
	char(*getch)();		   //If declare as getc will cause naming conflict
	void (*putch)(char str); 	  //If declare as putc will cause naming conflict
} serial_ops;

extern serial_ops serial;
int printf(const char *format, ...);
int puts(const char* msg);
char *utoa(unsigned int num, char *dst, unsigned int base);
char *itoa(int num, char *dst, int base);
void init_usart1();
#endif