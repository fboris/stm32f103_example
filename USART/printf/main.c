#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}
void init_led()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure PA0 and PA1 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void gpio_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->ODR ^= GPIO_Pin;
}

void send_byte(uint8_t b)
{
	/* Send one byte */
	USART_SendData(USART1, b);

	/* Loop until USART2 DR register is empty */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void send_string(uint8_t *str)
{
	while ((*str) != '\0') {

		send_byte(*str);
		str++;
	}
}
int main(void)
{
	init_led();
	init_usart1();

	char buf[8]={0};
	char buf2[8]={0};
	char *str = "test c-style string";
	char ch = 'a';
	int num = 100;
	
	puts("start test printf!\r\n");
    	puts("test puts");
    	puts("test printf:\r\n");
    	printf("test string = %s\r\ntest ch = %c\r\ntest decimal number = %d\r\ntest hex number = %x\r\n", str, ch, num, num);
	printf("test utoa: 100=%s, -100=%s\r\n", utoa(100, buf, 10), utoa(-100, buf2, 10));
	printf("test itoa: 100=%s, -100=%s\r\n", itoa(100, buf, 10), itoa(-100, buf2, 10));

}

