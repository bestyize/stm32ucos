#ifndef _BUTTON_H
#define _BUTTON_H
#include "types.h"
#define BUTTON_RIGHT  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define BUTTON_DOWN  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define BUTTON_LEFT  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define BUTTON_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 


typedef enum
{
	right_press=1,
	down_press=2,
	left_press=3,
	up_press=4,
}button_number;



void button_init(void);
u8 button_scan(u8 mode);


#endif

