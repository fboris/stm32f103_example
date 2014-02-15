#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include <stdio.h>
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
	int16_t int16_num[] = {123,32767,-32678,0,327678};
	uint16_t uint16_t_num[] = {123, 0xffff, 0, -1};
	float float_num[] = {33.33,0xffff,-0xffff};
	while(1){

		printf("int16_t number: 123=%d, 32767=%d, -32678=%d, 0=%d ",
			int16_num[0], int16_num[1], int16_num[2], int16_num[3]);
		printf("327678 = %d\r\n", int16_num[4]);

		printf("uint16_t number: 123 = %d, 0xffff = %d, 0 = %d, -1 = %d\r\n",
			uint16_t_num[0], uint16_t_num[1], uint16_t_num[2], 
			uint16_t_num[3]);

		printf("float number: 33.33 = %f, 0xffff=%f,-0xffff = %f\r\n",
			float_num[0], float_num[1], float_num[2] );
		delay(50000000);
	}

}

