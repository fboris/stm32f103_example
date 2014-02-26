stm32f103_example
=================
This collection aim for some stm32f103RC basic example. I use customized board in these exmaples.

##Prerequisite
- [openocd](http://sourceforge.net/projects/openocd/)
- [st-link](https://github.com/texane/stlink)
```
sudo apt-get install automake* libtool libusb-1.0-0-dev
git clone http://github.com/texane/stlink.git
cd stlink
./autogen.sh
./configure --prefix=/usr
make
sudo make install
sudo cp 49-stlinkv2.rules /etc/udev/rules.d/
```
- [GNU Tools for ARM Embedded Processors](https://launchpad.net/~terry.guo/+archive/gcc-arm-embedded)

```
sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi
```

##Demo Introduction

1.GPIO toggling

This demo shows how to toggle your GPIO(PA0 , PA1).

2.USART

I use `PA10` as USART1 RX, `PA9` as USART1 TX.

`ehco`: demo let MCU transmit same message as user typing. MCU will transmit message until USART receive data from PC. This method also is called polling. 

`echo_interrupt`: acts like `echo`. But MCU will interrupt instead of polling.

`repeat_send` just is a simple transmitting demo. MCU will continuously send message to PC.

`printf` include a portable printf function

`retarget_printf` : Because the my printf have some floating point issue, I let Makefile link software floating point with newlib-nano printf.

3.[ADC]

This demo still have some problem now.

4.[EXTI]

This demo shows how to use External interrupt/event controller
to handle incoming signal. In this case, these 5 GPIO connect to a switch.

5.[I2C]

I2C with MPU6050. You will see acceleration and angular rate through comport.

6.[PWM]

This demo shows how to drive a servo motor

7.[TIMER]

Toggling GPIO by more accurately time base