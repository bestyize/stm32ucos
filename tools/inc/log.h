#ifndef _LOG_H
#define _LOG_H
#include "types.h"


typedef struct
{
	u8 type;
	u8 info[128];
} Log;
extern Log log_info;
extern bool logFlag;

void showLog(char *params);


#define ERROR_CHECK(n)   {if(n!=0){showLog("Error !");}}

#endif
