#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include <stdio.h>
extern uint8_t TxBuffer[];
extern DMA_InitTypeDef DMA_InitStructure;
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
	int32_t counter = 0;
	while(1){
		counter++;
		sprintf( TxBuffer, "%d\r\n",counter);
		usart_dma_send(TxBuffer);
		delay(20000);
		
	}

}

