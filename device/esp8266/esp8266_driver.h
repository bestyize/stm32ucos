#ifndef _ESP8266_DRIVER_H
#define _ESP8266_DRIVER_H
#include "stdio.h"	
#include "sys.h" 
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#define ESP_MAX_RECEIVE_SIZE 256


#define ESP_MODE "AAT+CWMODE=1\r\n"
#define ESP_RST  "AT+RST\r\n"
#define ESP_JAP  "AT+CWJAP=\"seuwx305\",\"seuwx305\"\r\n"
#define ESP_LINK "AT+CIPSTART=\"TCP\",\"192.168.4.1\",8089\r\n"


typedef struct{
	uint8_t buf[ESP_MAX_RECEIVE_SIZE];
	uint8_t count;
	bool finish;
}ESP_UART_Info;

extern ESP_UART_Info esp_uart_info;


typedef struct
{
	bool continueMode;
	bool waitMode;
	bool clientMode;
	bool serverMode;
}ESP_State;

extern ESP_State esp_state;


extern bool espResponseOK;
void esp8266_uart_init(u32 baudrate);
void espSendStringToPC(char buf[]);
void espReceiveInit(void);



void espProcess(void);





#endif

