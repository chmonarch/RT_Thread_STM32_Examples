#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include <string.h>
#include "cpuusage.h"



/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t get_cpu_use_thread = RT_NULL;


/* 函数声明 */
static void led1_thread_entry(void* parameter);
static void get_cpu_use_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	
	rt_kprintf("This is an RTT-CPU utilization statistics experiment\r\n");
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
	
	get_cpu_use_thread = /* 线程控制块指针 */
	rt_thread_create( "get_cpu_use", /* 线程名字 */
				get_cpu_use_thread_entry, /* 线程入口函数 */
				RT_NULL, /* 线程入口函数参数 */
				512, /* 线程栈大小 */
				5, /* 线程的优先级 */
				20); /* 线程时间片 */
	if (get_cpu_use_thread != RT_NULL)
		rt_thread_startup(get_cpu_use_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void led1_thread_entry(void* parameter)
{
	rt_uint16_t i;
	while (1) {
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		/* 模拟占用 CPU 资源，修改数值作为模拟测试 */
		for (i = 0; i < 1000; i++);
		
		rt_thread_delay(5); /* 延时 5 个 tick */
	}
}

static void get_cpu_use_thread_entry(void* parameter)
{
	rt_uint8_t major,minor;
	while (1) {
		/* 获取 CPU 利用率数据 */
		cpu_usage_get(&major,&minor);
		/* 打印 CPU 利用率 */
		rt_kprintf("CPU utilization = %d.%d%\r\n",major,minor);
		rt_thread_delay(100); /* 延时 100 个 tick */
	}
}



