#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"



/* 定义线程控制块 */
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义互斥量控制块 */
static rt_mutex_t test_mux = RT_NULL;
/* 全局变量声明 */
uint8_t ucValue [ 2 ] = { 0x00, 0x00 };
/* 函数声明 */
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT mutex synchronization experiment!\n");
	rt_kprintf("Synchronous success output success, otherwise output failure\n");
	/* 创建一个互斥量 */
	test_mux = rt_mutex_create("test_mux",RT_IPC_FLAG_PRIO);
	if (test_mux != RT_NULL)
		rt_kprintf("Mutex created successfully!\n\n");
	/* 线程控制块指针 */
	receive_thread = rt_thread_create( "receive", /* 线程名字 */
				receive_thread_entry, /* 线程入口函数 */
				RT_NULL, /* 线程入口函数参数 */
				512, /* 线程栈大小 */
				3, /* 线程的优先级 */
				20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (receive_thread != RT_NULL)
		rt_thread_startup(receive_thread);
	else
		return -1;
	/* 线程控制块指针 */
	send_thread = rt_thread_create( "send", /* 线程名字 */
				send_thread_entry, /* 线程入口函数 */
				RT_NULL, /* 线程入口函数参数 */
				512, /* 线程栈大小 */
				2, /* 线程的优先级 */
				20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (send_thread != RT_NULL)
		rt_thread_startup(send_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void receive_thread_entry(void* parameter)
{
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		rt_mutex_take(test_mux, /* 获取互斥量 */
		RT_WAITING_FOREVER); /* 等待时间：一直等 */
		if ( ucValue [ 0 ] == ucValue [ 1 ] ) {
			rt_kprintf ( "Successful\n" );
		} else {
			rt_kprintf ( "Fail\n" );
		}
		rt_mutex_release( test_mux ); //释放互斥量
		rt_thread_delay ( 1000 ); //每 1s 读一次
	}
}
static void send_thread_entry(void* parameter)
{
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		rt_mutex_take(test_mux, /* 获取互斥量 */
		RT_WAITING_FOREVER); /* 等待时间：一直等 */
		ucValue [ 0 ] ++;
		rt_thread_delay ( 100 ); /* 延时 100ms */
		ucValue [ 1 ] ++;
		rt_mutex_release(test_mux); //释放互斥号量
		rt_thread_yield(); //放弃剩余时间片，进行一次线程切换
	}
}

