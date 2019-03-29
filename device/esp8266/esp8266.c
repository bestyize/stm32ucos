
#include "stm32f10x_usart.h"
#include "sys.h"
#include "misc.h"
#include "string.h"
#include "stdbool.h"
#include "devices.h"




ESP_State esp_state;

void espSendCmd(char cmd[])
{
	espSendStringToPC(cmd);
}

//透传
void espSendString(char cmd[])
{
	espSendStringToPC(cmd);
}
//进入透传模式
void espContinueTransportInit()
{
	
	char CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.4.1\",8089\r\n";
	char CIPMODE[]="AT+CIPMODE=1\r\n";
	char CIPSEND[]="AT+CIPSEND\r\n";
	LCD_ShowMyString(0,MIDDLE_LINE_7,16,(u8*)"Build Link START");
	espSendCmd(CIPSTART);
	while(!espResponseOK);
	LCD_ShowMyString(0,MIDDLE_LINE_1,16,(u8*)"Link Successful!");
	espSendCmd(CIPMODE);
	while(!espResponseOK);
	LCD_ShowMyString(0,MIDDLE_LINE_2,16,(u8*)"Transport Mode  ");
	espSendCmd(CIPSEND);
	while(!espResponseOK);
	LCD_ShowMyString(0,MIDDLE_LINE_7,16,(u8*)"Transport Start ");	
	esp_state.waitMode=false;
	esp_state.continueMode=true;
	
}

//离开透传模式
void espContinueTransportExit()
{
	espSendString("+++");
	delay_ms(3000);
	espSendString("+++");
	delay_ms(3000);
	LCD_ShowMyString(0,MIDDLE_LINE_2,16,(u8*)"Wait Mode      ");
	esp_state.waitMode=true;
	esp_state.continueMode=false;
}

//创建一个客户端
void espCreatClient()
{

	char CWMODE[]="AAT+CWMODE=1\r\n";
	char RST[]="AT+RST\r\n";
	char CWJAP[]="AT+CWJAP=\"demo\",\"12345678\"\r\n";
	espSendStringToPC(CWMODE);
	LCD_ShowMyString(0,MIDDLE_LINE_1,16,(u8*)"Client Mode     ");
	while(!espResponseOK);
	espResponseOK=false;
	espSendStringToPC(RST);
	LCD_ShowMyString(0,MIDDLE_LINE_1,16,(u8*)"Restart         ");
	while(!espResponseOK);
	espResponseOK=false;
	espSendStringToPC(CWJAP);
	LCD_ShowMyString(0,MIDDLE_LINE_1,16,(u8*)"Join WiFi       ");
	while(!espResponseOK);
	LCD_ShowMyString(0,MIDDLE_LINE_1,16,(u8*)"WiFi Connected  ");
	espResponseOK=false;
	
	esp_state.clientMode=true;
	esp_state.waitMode=true;
	esp_state.continueMode=false;
	esp_state.serverMode=false;
}

void espInit(u32 baudrate)
{
	esp8266_uart_init(baudrate);
	espCreatClient();
}
