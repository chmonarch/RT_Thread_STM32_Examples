#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"




/* 定义软件定时器控制块 */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

/* 全局变量声明 */
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

/* 函数声明 */
static void swtmr1_callback(void* parameter);
static void swtmr2_callback(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT software timer experiment!\n");
	rt_kprintf("The timer timeout function 1 executes only once and is destroyed\n");
	rt_kprintf("The timer timeout function 2 is executed in a loop\n");
	/* 创建一个软件定时器 */
	swtmr1 = rt_timer_create("swtmr1_callback", /* 软件定时器的名称 */
				 swtmr1_callback,/* 软件定时器的超时函数 */
				 0, /* 定时器超时函数的入口参数 */
				 5000, /* 软件定时器的超时时间(周期超时时间) */
				 RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
	/* 软件定时器模式 一次模式 */
	/* 启动定时器 */
	if (swtmr1 != RT_NULL)
		rt_timer_start(swtmr1);
	/* 创建一个软件定时器 */
	swtmr2 = rt_timer_create("swtmr2_callback", /* 软件定时器的名称 */
				 swtmr2_callback,/* 软件定时器的超时函数 */
				 0, /* 定时器超时函数的入口参数 */
				 1000, /* 软件定时器的超时时间(周期超时时间) */
				 RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	 /* 软件定时器模式 周期模式 */
	 /* 启动定时器 */
	 if (swtmr2 != RT_NULL)
		rt_timer_start(swtmr2);
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void swtmr1_callback(void* parameter)
{
	uint32_t tick_num1;
	TmrCb_Count1++; /* 每调用一次加一 */
	tick_num1 = (uint32_t)rt_tick_get(); /* 获取滴答定时器的计数值 */
	rt_kprintf("The SWtMR1 Callback function is executed %d times\n", TmrCb_Count1);
	rt_kprintf("Tick timer value =%d\n", tick_num1);
}
static void swtmr2_callback(void* parameter)
{
	uint32_t tick_num2;
	TmrCb_Count2++; /* 每调用一次加一 */
	tick_num2 = (uint32_t)rt_tick_get(); /* 获取滴答定时器的计数值 */
	rt_kprintf("The SWtMR2 Callback function is executed %d times\n", TmrCb_Count2);
	rt_kprintf("Tick timer value =%d\n", tick_num2);
}


