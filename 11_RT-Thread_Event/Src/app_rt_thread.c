#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"




/* 定义线程控制块 */
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义事件控制块(句柄) */
static rt_event_t test_event = RT_NULL; 

/* 全局变量声明 */
#define KEY1_EVENT (0x01 << 0)//设置事件掩码的位 0
#define KEY2_EVENT (0x01 << 1)//设置事件掩码的位 1

/* 函数声明 */
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT event flag set experiment!\n");
	/* 创建一个事件 */
	test_event = rt_event_create("test_event",/* 事件标志组名字 */
						 RT_IPC_FLAG_PRIO); /* 事件模式 FIFO(0x00)*/
	if (test_event != RT_NULL)
		rt_kprintf("Event created successfully!\n");
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
	rt_uint32_t recved;
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		/* 等待接收事件标志 */
		rt_event_recv(test_event, /* 事件对象句柄 */
						KEY1_EVENT|KEY2_EVENT,/* 接收线程感兴趣的事件 */
						RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,/* 接收选项 */
						RT_WAITING_FOREVER,/* 指定超时事件,一直等 */
						&recved); /* 指向接收到的事件 */
		if (recved == (KEY1_EVENT|KEY2_EVENT)) { /* 如果接收完成并且正确 */
			rt_kprintf ( "Press Key1 and Key2\n");
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);//LED1 反转
		} 
		else
			rt_kprintf ( "Event error!\n");
	}
}
static void send_thread_entry(void* parameter)
{
	/* 线程都是一个无限循环，不能返回 */
	while (1) { //如果 KEY2 被单击
		if ( HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_SET ) {
			rt_kprintf ( "KEY1 is clicked\n" );
			/* 发送一个事件 1 */
			rt_event_send(test_event,KEY1_EVENT);
		}
		//如果 KEY2 被单击
		if ( HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_SET ) {
			rt_kprintf ( "KEY2 is clicked\n" );
			/* 发送一个事件 2 */
			rt_event_send(test_event,KEY2_EVENT);
		}
		rt_thread_delay(20); //每 20ms 扫描一次
	}
}


