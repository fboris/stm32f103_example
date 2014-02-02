#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

void init_usart1()
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable peripheral clocks. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Configure USART1 Rx pin as floating input. */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Tx as alternate function push-pull. */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the USART2 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable transmit and receive interrupts for the USART2. */
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable the USART1 IRQ in the NVIC module (so that the USART1 interrupt
	 * handler is enabled). */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the RS232 port. */
	USART_Cmd(USART1, ENABLE);

}

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