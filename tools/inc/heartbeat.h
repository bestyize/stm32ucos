#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H
#include "types.h"


extern u32 current_running_ms;


void delay_init(void);
void delay_us(u32 count);
void delay_ms(u32 count);
char *getSystemTime(void);
#endif

