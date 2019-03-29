#include "stdio.h"
#include "includes.h"
#include "log.h"
#include "devices.h"
#include "services.h"

u8 create_task(void (*function)(void *params),int priorty,CPU_STK stk[],OS_TCB tcb);






//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define LED0_TASK_PRIO		7
//任务堆栈大小	
#define LED0_STK_SIZE 		128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

#define BUTTON_SCAN_TASK_PRIO 6
#define BUTTON_SCAN_STK_SIZE  128
OS_TCB  buttonScanTaskTCB;
CPU_STK BUTTON_SCAN_TASK_STK[BUTTON_SCAN_STK_SIZE];
void button_scan_task(void *params);


#define UART1_TASK_PRIO      4
#define UART1_STK_SIZE       128
CPU_STK UART1_TASK_STK[UART1_STK_SIZE];
OS_TCB  uart1TaskTCB;
void uart1Task(void *params);


#define LOG_TASK_PRIO      5
#define LOG_STK_SIZE       128
CPU_STK LOG_TASK_STK[LOG_STK_SIZE];
OS_TCB  logTaskTCB;
void logTask(void *params);



#define CLOCK_TASK_PRIO      8
#define CLOCK_STK_SIZE       128
CPU_STK CLOCK_TASK_STK[CLOCK_STK_SIZE];
OS_TCB  clockTaskTCB;
void clockTask(void *params);





void services_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
}


//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED0任务
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	//创建按键任务
	OSTaskCreate((OS_TCB 	* )&buttonScanTaskTCB,		
				 (CPU_CHAR	* )"button task", 		
                 (OS_TASK_PTR )button_scan_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )BUTTON_SCAN_TASK_PRIO,     
                 (CPU_STK   * )&BUTTON_SCAN_TASK_STK[0],	
                 (CPU_STK_SIZE)BUTTON_SCAN_STK_SIZE/10,	
                 (CPU_STK_SIZE)BUTTON_SCAN_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
    //创建串口任务
    OSTaskCreate(
				 (OS_TCB     *)&uart1TaskTCB,
				 (CPU_CHAR   *)"uart1 task",
				 (OS_TASK_PTR )uart1Task,
				 (void       *)0,
				 (OS_PRIO     )UART1_TASK_PRIO,
				 (CPU_STK    *)&UART1_TASK_STK[0],
				 (CPU_STK_SIZE)UART1_STK_SIZE/10,
				 (CPU_STK_SIZE)UART1_STK_SIZE,
				 (OS_MSG_QTY  )0,
				 (OS_TICK     )0,
				 (void       *)0,
				 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				 (OS_ERR     *)&err);
	//创建日志任务
    OSTaskCreate(
				 (OS_TCB     *)&logTaskTCB,
				 (CPU_CHAR   *)"log task",
				 (OS_TASK_PTR )logTask,
				 (void       *)0,
				 (OS_PRIO     )LOG_TASK_PRIO,
				 (CPU_STK    *)&LOG_TASK_STK[0],
				 (CPU_STK_SIZE)LOG_STK_SIZE/10,
				 (CPU_STK_SIZE)LOG_STK_SIZE,
				 (OS_MSG_QTY  )0,
				 (OS_TICK     )0,
				 (void       *)0,
				 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				 (OS_ERR     *)&err);
	//创建时钟任务
    OSTaskCreate(
				 (OS_TCB     *)&clockTaskTCB,
				 (CPU_CHAR   *)"clock task",
				 (OS_TASK_PTR )clockTask,
				 (void       *)0,
				 (OS_PRIO     )CLOCK_TASK_PRIO,
				 (CPU_STK    *)&CLOCK_TASK_STK[0],
				 (CPU_STK_SIZE)CLOCK_STK_SIZE/10,
				 (CPU_STK_SIZE)CLOCK_STK_SIZE,
				 (OS_MSG_QTY  )0,
				 (OS_TICK     )0,
				 (void       *)0,
				 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				 (OS_ERR     *)&err);
//	create_task(clockTask,CLOCK_TASK_PRIO,CLOCK_TASK_STK,clockTaskTCB);		 
	
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
}

//为了方便创建任务，写了这个函数帮助创建任务
//function 		:执行任务的函数 
//prority  		:任务优先级
//stk      		:任务堆栈
//tcb      		:任务表
//return @err	:返回0表示任务创建成功 
u8 create_task(void (*function)(void *params),int priorty,CPU_STK stk[],OS_TCB tcb)
{
	static int i=0;
	char task_char[20]={0};
	int default_stk_size=128;
	CPU_ERR err;
	sprintf(task_char,"task %d",i++);
	//创建时钟任务
    OSTaskCreate(
				 (OS_TCB     *)&tcb,
				 (CPU_CHAR   *)task_char,
				 (OS_TASK_PTR )&function,
				 (void       *)0,
				 (OS_PRIO     )priorty,
				 (CPU_STK    *)&stk[0],
				 (CPU_STK_SIZE)default_stk_size/10,
				 (CPU_STK_SIZE)default_stk_size,
				 (OS_MSG_QTY  )0,
				 (OS_TICK     )0,
				 (void       *)0,
				 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				 (OS_ERR     *)&err);
	return err;
}


//led0任务函数
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		showLog("LED3 ON ");
		LED3=0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
		showLog("LED3 OFF");
		LED3=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

void uart1Task(void *params)
{
	OS_ERR err;
	int i=0;
	while(1)
	{
		if(uart_info.finish==true)
		{
			printf("%s",uart_info.buf);
			POINT_COLOR=RED;
			LCD_ShowMyString(0,i,16,(u8*)uart_info.buf);
			receiveInit();
			i+=16;
		}
		OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
	}	
}
//调试日志服务
void logTask(void *params)
{
	OS_ERR err;
	while(1)
	{
		if(logFlag==true)
		{
			printf("Debug Log:%s",log_info.info);
			POINT_COLOR=RED;
			LCD_ShowMyString(0,MIDDLE_LINE_12,16,(u8*)log_info.info);
			logFlag=false;
		}
		OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}


//按键扫描
void button_scan_task(void *params)
{
	OS_ERR err;
	while(1)
	{
		u8 key=button_scan(1);
		switch(key)
		{
			case right_press:
				espContinueTransportInit();
				LCD_ShowMyString(0,MIDDLE_LINE_13,16,(u8*)"Enter continue mode");
			    
			break;
			
			case down_press:
				espContinueTransportExit();
				LCD_ShowMyString(0,MIDDLE_LINE_13,16,(u8*)"Exit continue mode ");
			break;
			
			case left_press:
				espSendString("Left   press\r\n");
				LCD_ShowMyString(0,MIDDLE_LINE_13,16,(u8*)"Left   press       ");
			break;
			
			case up_press:
				espSendString("Up   press\r\n");
				LCD_ShowMyString(0,MIDDLE_LINE_13,16,(u8*)"Up     press       ");
			break;
			
			default:
				
			break;
		}
		OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err); //延时80ms
	}
		
		
}

//系统时钟服务
void clockTask(void *params)
{
	OS_ERR err;
	while(1)
	{
		char *timestamp=getSystemTime();
		LCD_ShowMyString(0,MIDDLE_LINE_0,16,(u8*)timestamp);
		if(esp_state.continueMode==true)
		{
			espSendString(timestamp);
		}
		OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_HMSM_STRICT,&err); //延时800ms
	}
}











