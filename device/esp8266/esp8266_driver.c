#include "esp8266_driver.h"
#include "heartbeat.h"
#include "string.h"
#include "led.h"
#include "stdbool.h"
#include "lcd.h"
#include "lcd_helper.h"
#include "stm32f10x_usart.h"
#include "sys.h"
#include "misc.h"

ESP_UART_Info esp_uart_info;


bool espResponseOK=false;

//串口2 PA2 TX PA3 RX
void esp8266_uart_init(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//开时钟
	RCC->APB2ENR|=1<<2;
	RCC->APB1ENR|=1<<17;
	
	//GPIO初始化 复用推挽输出
	GPIOA->CRL&=~(0XFF<<8);
	GPIOA->CRL|=0X4B<<8;
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2
}


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		esp_uart_info.buf[esp_uart_info.count++]=res;
		if((esp_uart_info.count>2)&&(esp_uart_info.buf[esp_uart_info.count-1]=='\n')&&(esp_uart_info.buf[esp_uart_info.count-2]=='\r'))
		{
			espProcess();
		}		
    } 

} 


void espSendStringToPC(char buf[])
{
	int i;
	for(i=0;i<strlen((char*)buf);i++)
	{
		if(buf[i]=='\0')
		{
			break;
		}
		USART2->DR =buf[i]; 
		while((USART2->SR&0X40)==0);
	}
	
}




//数据接收初始化
void espReceiveInit(void)
{
	memset(esp_uart_info.buf,0,ESP_MAX_RECEIVE_SIZE*sizeof(uint8_t));//初始化缓冲数组
	esp_uart_info.count=0;
	esp_uart_info.finish=false;
}


void espProcess()
{
	if(strstr((char*)esp_uart_info.buf,"OK")||strstr((char*)esp_uart_info.buf,"ready"))
	{
		espResponseOK=true;
		delay_ms(500);
	}
	espReceiveInit();
}






