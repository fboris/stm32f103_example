USART Example 
==============
This directory has three demo. 
`ehco` demo let MCU transmit same message as user typing. MCU will transmit message until USART receive data from PC. This method also is called `polling`. 

`echo_interrupt` acts like `echo`. But MCU will interrupt instead of polling.

`repeat_send` just is a simple transmitting demo. MCU will continuously send message to PC.