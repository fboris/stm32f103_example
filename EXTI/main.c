#include "stm32f10x.h"
#include "stm32f10x_conf.h"
extern uint8_t EXTI4_IsFalling;
extern uint8_t EXTI5_IsFalling;
extern uint8_t EXTI10_IsFalling;
extern uint8_t EXTI11_IsFalling;
extern uint8_t EXTI12_IsFalling;
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
void init_exti()
{

	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	/* Configure PA.00 pin as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 || GPIO_Pin_5 || GPIO_Pin_10
  		|| GPIO_Pin_11 ||GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input pull up
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  	 /* Enable AFIO clock */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  	  /* Connect EXTI0 Line to PA.00 pin */
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);

  	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStructure);

}
int main(void)
{
	init_led();
	init_exti();
	init_usart1();
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|
                GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);	
	while (1) {

		gpio_toggle(GPIOA, GPIO_Pin_0);
		gpio_toggle(GPIOA, GPIO_Pin_1);
		if ( EXTI4_IsFalling == 1 ){

			puts("PC4 was reset!\r\n");
			EXTI4_IsFalling = 0;

		} else if ( EXTI5_IsFalling == 1){
			puts("PC5 was reset!\r\n");
			EXTI5_IsFalling = 0;

		} else if ( EXTI10_IsFalling == 1){

			puts("PC10 was reset!\r\n");
			EXTI10_IsFalling = 0;	

		} else if ( EXTI11_IsFalling == 1){

			puts("PC11 was reset!\r\n");
			EXTI11_IsFalling = 0;

		} else if ( EXTI12_IsFalling == 1){

			puts("PC12 was reset!\r\n");
			EXTI12_IsFalling = 0;
		}
		delay(5000000);

	}
}
