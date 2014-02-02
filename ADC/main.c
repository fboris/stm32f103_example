#include "stm32f10x.h"

#include "stm32f10x_conf.h"
#include "adc.h"
#include "usart.h"
volatile uint32_t ADCConvertedValue;
void delay(uint32_t delay_count )
{
  while(delay_count) delay_count--;
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
void gpio_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

int16_t get_temperature(uint32_t advalue)   
{   
    uint32_t temp_sensor;   
    int32_t current_temp;   
      
    temp_sensor = advalue * 330 / 4096;   
    current_temp = (int32_t)(143 - temp_sensor)*10000/43 + 2500;   
    return (int16_t)current_temp;   
}    
int main(void)
{
  init_led();
  init_usart1();
  init_adc();
  int16_t temp_measure;
  while(1) {

       gpio_toggle(GPIOA,GPIO_Pin_0);
       gpio_toggle(GPIOA,GPIO_Pin_1);
       temp_measure = get_temperature(ADCConvertedValue);
       printf("currten temp:%d\r\n",temp_measure);
       delay(500000);

    }
}
