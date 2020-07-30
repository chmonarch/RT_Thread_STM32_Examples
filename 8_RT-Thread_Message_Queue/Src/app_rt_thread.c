#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"


/* 定义线程控制块 */
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义消息队列控制块 */
static rt_mq_t test_mq = RT_NULL;

static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);


int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT thread management experiment!\n");
	rt_kprintf("Press K1 or K2 to send a queue message\n");
	rt_kprintf("The thread receives the message and echoes it in the serial port\n");
	/* 创建一个消息队列 */
	test_mq = rt_mq_create("test_mq",/* 消息队列名字 */
						40, /* 消息的最大长度 */
						20, /* 消息队列的最大容量 */
						RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
	if (test_mq != RT_NULL)
		rt_kprintf("Message queue created successfully!\n\n");
	receive_thread = /* 线程控制块指针 */
	rt_thread_create( "receive", /* 线程名字 */
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
	send_thread = /* 线程控制块指针 */
	rt_thread_create( "send", /* 线程名字 */
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
	rt_err_t uwRet = RT_EOK;
	uint32_t r_queue;
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		/* 队列读取（接收），等待时间为一直等待 */
		uwRet = rt_mq_recv(test_mq, /* 读取（接收）队列的 ID(句柄) */
						&r_queue, /* 读取（接收）的数据保存位置 */
						sizeof(r_queue), /* 读取（接收）的数据的长度 */
						RT_WAITING_FOREVER); /* 等待时间：一直等 */
		if (RT_EOK == uwRet) {
			rt_kprintf("The data received this time are:%d\n",r_queue);
		} else {
			rt_kprintf("Data receiving error, error code:0x%lx\n",uwRet);
		}
	rt_thread_delay(200);
	}
}
static void send_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;
	while (1) { /* K1 被按下 */
		if ( HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_SET ) {
			/* 将数据写入（发送）到队列中，等待时间为 0 */
			uwRet = rt_mq_send(test_mq,/* 写入（发送）队列的 ID(句柄) */
							&send_data1,/* 写入（发送）的数据 */
							sizeof(send_data1)); /* 数据的长度 */
			if (RT_EOK != uwRet) {
				rt_kprintf("Data cannot be sent to message queue! Error code:%lx\n",uwRet);
			}
		}/* K2 被按下 */
		if ( HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_SET ) {
			/* 将数据写入（发送）到队列中，等待时间为 0 */
			uwRet = rt_mq_send(test_mq, /* 写入（发送）队列的 ID(句柄) */
							&send_data2, /* 写入（发送）的数据 */
							sizeof(send_data2)); /* 数据的长度 */
			if (RT_EOK != uwRet) {
				rt_kprintf("Data cannot be sent to message queue! Error code:%lx\n",uwRet);
			}
		}
		rt_thread_delay(20);
	}
}

