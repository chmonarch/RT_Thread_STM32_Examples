#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"



/* 定义线程控制块 */
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义信号量控制块 */
static rt_sem_t test_sem = RT_NULL;

/* 全局变量声明 */
uint8_t ucValue [ 2 ] = { 0x00, 0x00 };

/* 函数声明 */
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);


int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT binary semaphore synchronization experiment!\n");
	rt_kprintf("Synchronous success output success, otherwise output failure\n");
	/* 创建一个信号量 */
	test_sem = rt_sem_create("test_sem",/* 信号量名字 */
					1, /* 信号量初始值，默认有一个信号量 */
					RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
	if (test_sem != RT_NULL)
		rt_kprintf("Semaphore created successfully!\n\n");
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
*************************************************************************
* 线程定义
*************************************************************************
*/
static void receive_thread_entry(void* parameter)
{
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		rt_sem_take(test_sem, /* 获取信号量 */
		RT_WAITING_FOREVER); /* 等待时间：一直等 */
		if ( ucValue [ 0 ] == ucValue [ 1 ] ) {
			rt_kprintf ( "Successful\n" );
		} else {
			rt_kprintf ( "Fail\n" );
		}
		rt_sem_release( test_sem ); //释放二值信号量
		rt_thread_delay ( 1000 ); //每 1s 读一次
	}
}

static void send_thread_entry(void* parameter)
{
/* 线程都是一个无限循环，不能返回 */
	while (1) {
		rt_sem_take(test_sem, /* 获取信号量 */
		RT_WAITING_FOREVER); /* 等待时间：一直等 */
		ucValue [ 0 ] ++;
		rt_thread_delay ( 100 ); /* 延时 100ms */
		ucValue [ 1 ] ++;
		rt_sem_release(test_sem); //释放二值信号量
		rt_thread_yield(); //放弃剩余时间片，进行一次线程切换
	}
}


