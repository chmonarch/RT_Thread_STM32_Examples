#include "rtthread.h"
#include "main.h"
#include "stdio.h"


/* 定义线程控制块指针 */
static rt_thread_t led1_thread = RT_NULL;
static void led1_thread_entry(void* parameter);

//初始化线程函数
int MX_RT_Thread_Init(void)
{
	led1_thread = /* 线程控制块指针 */
	rt_thread_create( "led1", /* 线程名字 */
										led1_thread_entry, /* 线程入口函数 */
										RT_NULL, /* 线程入口函数参数 */
										512, /* 线程栈大小 */
										3, /* 线程的优先级 */
										20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (led1_thread != RT_NULL)
		rt_thread_startup(led1_thread);
	else
		return -1;
}

/*
*************************************************************************
* 线程定义
*************************************************************************
*/
static void led1_thread_entry(void* parameter)
{
	while (1)
	{
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
		rt_thread_delay(500); /* 延时 500 个 tick */
		rt_kprintf("led1_thread running,LED1_ON\r\n");
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
		rt_thread_delay(500); /* 延时 500 个 tick */
		rt_kprintf("led1_thread running,LED1_OFF\r\n");
	}
}

