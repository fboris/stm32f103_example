#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
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



int puts(const char* msg)
{   
    for(; *msg; msg++)
    serial.putch(*msg);

    return 1;
}


 
int printf(const char *format, ...)
{
  char str[128];
  va_list para;
  va_start(para, format);
  int curr_pos = 0;
  char ch[] = {'0', '\0'};
  char integer[11];
  str[0] = '\0';

  while (format[curr_pos] != '\0') {
    if (format[curr_pos] != '%') {
      ch[0] = format[curr_pos];
      strcat(str, ch);

    } else {
      switch (format[++curr_pos]) {
      case 's':
        strcat(str, va_arg(para, char *));
        break;

      case 'c':
        ch[0] = (char)va_arg(para, int);
        strcat(str, ch);
        break;

      case 'i':
      case 'f':
        strcat(str, ftoa(va_arg(para, double) ) );
        break;

      case 'd':
        strcat(str,  itoa(va_arg(para, int), integer) );
        break;

      case 'u':
        strcat(str, itoa(va_arg(para, unsigned), integer));
        break;

      default:
        break;
      }
    }

    curr_pos++;
  }

  va_end(para);
  puts(str);
  return 1;
}
