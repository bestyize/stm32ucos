#include "log.h"
#include "string.h"
#include "stdio.h"
#include "string.h"
#include "heartbeat.h"
char clock_buf[48];
bool logFlag=false;
Log log_info;

void showLog(char *params)
{
	memset(log_info.info,0,128*sizeof(char));
	sprintf((char*)log_info.info,(char*)"Log : %s",params);
	logFlag=true;
}

//获取当前时间
char *getSystemTime()
{
	u32 currentTime=current_running_ms/1000;
	
	int hour=currentTime/3600;
	int min=(currentTime-hour*3600)/60;
	int second=currentTime-hour*3600-min*60;
	memset(clock_buf,0,48*sizeof(u32));
	sprintf(clock_buf,"Time : %d:%d:%d ",hour,min,second);
	return clock_buf;
}
