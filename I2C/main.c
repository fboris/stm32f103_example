#include "bool.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "MPU6050.h"
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
void _exit(int status)
{}
int main(void)
{
	int16_t buff[6];
	float acc[3],gyro[3],num=3.4444;
	init_led();
	init_usart1();
	MPU6050_I2C_Init();
	MPU6050_Initialize();
	if( MPU6050_TestConnection() == TRUE)
	{
	   puts("connection success\r\n");
	}else {
	   puts("connection failed\r\n");
	}
	printf("test float%f\r\n",num);
	while (1) {
		//puts("running now\r\n");
		MPU6050_GetRawAccelGyro(buff);
		for ( int i = 0; i<3; i++)
			acc[i] = (int16_t)buff[i]/16384.0;
		for ( int i = 0; i<3; i++)
			gyro[i] = (int16_t)buff[i+2]/131.0;
		printf("acc_x,%f,acc_y,%f,acc_z,%f,gyro_x,%f,gyro_y,%f,gyro_z,%f\r\n",
			acc[0], acc[1], acc[2],
			gyro[0], gyro[1], gyro[2]);
		gpio_toggle(GPIOA, GPIO_Pin_0);
		gpio_toggle(GPIOA, GPIO_Pin_1);
		delay(500000);

	}
}
