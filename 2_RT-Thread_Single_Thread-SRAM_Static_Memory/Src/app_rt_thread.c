#include "rtthread.h"
#include "main.h"
#include "stdio.h"

/* 定义线程控制块 */
static struct rt_thread led1_thread;
/* 定义线程控栈时要求 RT_ALIGN_SIZE 个字节对齐 */
ALIGN(RT_ALIGN_SIZE)
/* 定义线程栈 */
static rt_uint8_t rt_led1_thread_stack[1024];
static void led1_thread_entry(void* parameter);

//初始化线程函数
void MX_RT_Thread_Init(void)
{
	//初始化LED线程
	rt_thread_init(&led1_thread, /* 线程控制块 */
								 "led1", /* 线程名字 */
								 led1_thread_entry, /* 线程入口函数 */
								 RT_NULL, /* 线程入口函数参数 */
								 &rt_led1_thread_stack[0], /* 线程栈起始地址 */
								 sizeof(rt_led1_thread_stack), /* 线程栈大小 */
								 3, /* 线程的优先级 */
								 20); /* 线程时间片 */
	rt_thread_startup(&led1_thread); /* 启动线程，开启调度 */
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
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
		rt_thread_delay(500); /* 延时 500 个 tick */
	}
}

