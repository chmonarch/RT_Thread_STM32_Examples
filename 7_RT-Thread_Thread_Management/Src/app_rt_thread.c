#include "rtthread.h"
#include "main.h"
#include "stdio.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;

static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);


int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT thread management experiment! \n");
	rt_kprintf("Press K1 to suspend the thread, and Press K2 to restore the thread! \n"); 
	led1_thread = /* 线程控制块指针 */
	rt_thread_create( "led1", /* 线程名字 */
							led1_thread_entry, /* 线程入口函数 */
							RT_NULL, /* 线程入口函数参数 */
							512, /* 线程栈大小 */
							4, /* 线程的优先级 */
							20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (led1_thread != RT_NULL)
		rt_thread_startup(led1_thread);
	else
		return -1;
	
	key_thread = /* 线程控制块指针 */
	rt_thread_create( "key", /* 线程名字 */
								key_thread_entry, /* 线程入口函数 */
								RT_NULL, /* 线程入口函数参数 */
								512, /* 线程栈大小 */
								3, /* 线程的优先级 */
								20); /* 线程时间片 */
								/* 启动线程，开启调度 */
	if (key_thread != RT_NULL)
		rt_thread_startup(key_thread);
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
	while (1) {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		rt_thread_delay(500); /* 延时 500 个 tick */
		rt_kprintf("led1_thread running,LED1_ON\r\n");
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		rt_thread_delay(500); /* 延时 500 个 tick */
		rt_kprintf("led1_thread running,LED1_OFF\r\n");
	}
}
static void key_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1) {/* K1 被按下 */ 
		if ( HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_SET ) { 
			rt_kprintf("Suspend LED1 thread!\n");
			uwRet = rt_thread_suspend(led1_thread);/* 挂起 LED1 线程 */
			if (RT_EOK == uwRet) {
				rt_kprintf("LED1 thread suspended successfully!\n");
			} else {
				rt_kprintf("Suspended LED1 thread failed! Failed code: 0x%lx\n",uwRet);
			}
		}/* K2 被按下 */
		if ( HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_SET ) {
			rt_kprintf("Restore LED1 thread!\n");
			uwRet = rt_thread_resume(led1_thread);/* 恢复 LED1 线程！ */
			if (RT_EOK == uwRet) {
				rt_kprintf("Restore LED1 thread successfully!\n");
			} else {
				rt_kprintf("Restore LED1 thread failed! Failed code: 0x%lx\n",uwRet);
			}
		}
		rt_thread_delay(20);
	}
}
