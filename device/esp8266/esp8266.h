#ifndef _ESP8266_H
#define _ESP8266_H
#include "types.h"

void espSendCmd(char cmd[]);
void espSendString(char cmd[]);
void espContinueTransportInit(void);
void espContinueTransportExit(void);
void espCreatClient(void);
void espInit(u32 baudrate);





#endif


