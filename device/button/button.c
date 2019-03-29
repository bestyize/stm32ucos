#include "button.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "types.h"
#include "heartbeat.h"




//PE2 PE3 PE4 PA0
void button_init(void)
{
	RCC->APB2ENR|=1<<6;
	GPIOE->CRL&=0XFFF000FF;
	GPIOE->CRL|=0X00088800;
	GPIOE->BSRR |=(7<<2);
	
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL&=0XFFFFFFF0;
	GPIOA->CRL|=0X00000008;
	GPIOA->BRR|=1<<0;
}

u8 button_scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(BUTTON_RIGHT==0||BUTTON_DOWN==0||BUTTON_LEFT==0||BUTTON_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(BUTTON_RIGHT==0)return right_press;
		else if(BUTTON_DOWN==0)return down_press;
		else if(BUTTON_LEFT==0)return left_press;
		else if(BUTTON_UP==1)return up_press;
	}else if(BUTTON_RIGHT==1&&BUTTON_DOWN==1&&BUTTON_LEFT==1&&BUTTON_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}

