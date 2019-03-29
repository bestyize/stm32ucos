#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


//初始化PC0-PC7
void led_init(void)
{
	RCC->APB2ENR|=1<<4;//开时钟
	GPIOC->CRL=0x3333333;//推挽输出50MHz
	GPIOC->ODR|=0xFF;
}
