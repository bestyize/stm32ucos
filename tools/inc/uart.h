#ifndef _UART_H_
#define _UART_H_


#include "stdio.h"	
#include "sys.h" 
#include "stdint.h"
#include "stdbool.h"
#include "string.h"


#define MAX_RECEIVE_SIZE 256


typedef struct{
	uint8_t buf[MAX_RECEIVE_SIZE];
	uint8_t count;
	bool finish;
}UART_Info;

extern UART_Info uart_info;
extern u8 responseFlag;



void uart_init(u32 bound);
void receiveInit(void);
void sendToPC(uint8_t buf[]);
void sendStringToPC(uint8_t buf[]);


#endif

