#include "stm32f10x.h"

void busyLoop(uint32_t delay )
{
  while(delay) delay--;
}

int main(void)
{

    while(1) {
       GPIOC->BRR = 0x00001000;
       busyLoop(500000);
       GPIOC->BSRR = 0x00001000;
       busyLoop(500000);
    }
}
