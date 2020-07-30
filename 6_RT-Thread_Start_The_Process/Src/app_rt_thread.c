#include "rtthread.h"
#include "main.h"
#include "stdio.h"


///* 定义线程控制块指针 */
static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;
static rt_thread_t thread3 = RT_NULL;

static void thread1_entry(void* parameter);
static void thread2_entry(void* parameter);
static void thread3_entry(void* parameter);

#define THREAD1_STACK_SIZE 	 512
#define THREAD1_PRIORITY		 3
#define THREAD1_TIMESLICE    20

#define THREAD2_STACK_SIZE   512
#define THREAD2_PRIORITY     4
#define THREAD2_TIMESLICE    20

#define THREAD3_STACK_SIZE   512
#define THREAD3_PRIORITY     5
#define THREAD3_TIMESLICE    20


//初始化线程函数
int MX_RT_Thread_Init(void)
{
	thread1 = /* 线程控制块指针 */
	rt_thread_create("thread1", /* 线程名字，字符串形式 */
										thread1_entry, /* 线程入口函数 */
										RT_NULL, /* 线程入口函数参数 */
										THREAD1_STACK_SIZE, /* 线程栈大小，单位为字节 */
										THREAD1_PRIORITY, /* 线程优先级，数值越大，优先级越小 */
										THREAD1_TIMESLICE); /* 线程时间片 */
	if (thread1 != RT_NULL)
		rt_thread_startup(thread1);
	else
		return -1;
	
	thread2 = /* 线程控制块指针 */
	rt_thread_create("thread2", /* 线程名字，字符串形式 */
										thread2_entry, /* 线程入口函数 */
										RT_NULL, /* 线程入口函数参数 */
										THREAD2_STACK_SIZE, /* 线程栈大小，单位为字节 */
										THREAD2_PRIORITY, /* 线程优先级，数值越大，优先级越小 */
										THREAD2_TIMESLICE); /* 线程时间片 */
	if (thread2 != RT_NULL)
		rt_thread_startup(thread2);
	else
		return -1;

	thread3 = /* 线程控制块指针 */
	rt_thread_create("thread3", /* 线程名字，字符串形式 */
										thread3_entry, /* 线程入口函数 */
										RT_NULL, /* 线程入口函数参数 */
										THREAD3_STACK_SIZE, /* 线程栈大小，单位为字节 */
										THREAD3_PRIORITY, /* 线程优先级，数值越大，优先级越小 */
										THREAD3_TIMESLICE); /* 线程时间片 */
	if (thread3 != RT_NULL)
		rt_thread_startup(thread3);
	else
		return -1;
}

/*
*************************************************************************
* 线程定义
*************************************************************************
*/
static void thread1_entry(void* parameter)
{
	while (1)
	{
		rt_kprintf("thread1 running\r\n");
		rt_thread_delay(500); /* 延时 500 个 tick */
	}
}

static void thread2_entry(void* parameter)
{
	while (1)
	{
		rt_kprintf("thread2 running\r\n");
		rt_thread_delay(1000); /* 延时 1000 个 tick */
	}
}

static void thread3_entry(void* parameter)
{
	while (1)
	{
		rt_kprintf("thread3 running\r\n");
		rt_thread_delay(1500); /* 延时 1500 个 tick */
	}
}



