#include "stm32f10x.h"
#include "stm32f10x_conf.h"

uint16_t CCR3_Val = 10;
uint16_t CCR4_Val = 20;
uint16_t PrescalerValue = 0;
TIM_OCInitTypeDef  TIM_OCInitStructure;
void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}

void init_tim4_pwm()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	

	/* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  	/* GPIOA and GPIOB clock enable */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	

  	/*GPIOB Configuration: TIM3 channel1, 2, 3 and 4 */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  	GPIO_Init(GPIOB, &GPIO_InitStructure);

 
  	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 7200 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 20 -1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel2 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  TIM_ARRPreloadConfig(TIM4, ENABLE);

	  /* TIM3 enable counter */
	  TIM_Cmd(TIM4, ENABLE);
}
int main(void)
{
	init_led();
	init_tim4_pwm();
	while (1) {

		CCR3_Val = CCR3_Val +10l;
		CCR4_Val = CCR4_Val +101;
		TIM4->CCR3 = CCR3_Val;
		TIM4->CCR4 = CCR4_Val;
		if ( (CCR3_Val>7000) || (CCR4_Val > 7000) ){

			CCR3_Val = 20;
			CCR4_Val = 20;

		} else if ( (CCR3_Val<20) || (CCR4_Val <20) ){

			CCR3_Val = 20;
			CCR4_Val = 20;

		}
		delay(500000);



	}
}
