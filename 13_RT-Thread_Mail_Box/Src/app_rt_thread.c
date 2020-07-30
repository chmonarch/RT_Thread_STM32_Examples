#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"




/* 定义线程控制块 */
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义邮箱控制块 */
static rt_mailbox_t test_mail = RT_NULL;

/* 变量声明  */
char test_str1[] = "this is a mail test 1";/* 邮箱消息 test1 */
char test_str2[] = "this is a mail test 2";/* 邮箱消息 test2 */

/* 函数声明 */
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT email message experiment!\n");
	rt_kprintf("Press K1 | K2 for mailbox experiment test!\n");
	/* 创建一个邮箱 */
	test_mail = rt_mb_create("test_mail", /* 邮箱名字 */
						10, /* 邮箱大小 */
						RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
						if (test_mail != RT_NULL)
						rt_kprintf("Email created successfully!\n\n");
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
	rt_err_t uwRet = RT_EOK;
	char *r_str;
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		/* 等待接邮箱消息 */
		uwRet = rt_mb_recv(test_mail, /* 邮箱对象句柄 */
					(rt_uint32_t*)&r_str, /* 接收邮箱消息 */
					RT_WAITING_FOREVER);/* 指定超时事件,一直等 */
		if (RT_EOK == uwRet) { /* 如果接收完成并且正确 */
			rt_kprintf ( "The contents of the mailbox are:%s\n\n",r_str);
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);//LED1 反转
		} else
		rt_kprintf ( "Error receiving email! Error code is 0x%x\n",uwRet);
	}
}

static void send_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		//如果 KEY1 被单击
		if ( HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_SET ) {
		rt_kprintf ( "KEY1 is clicked\n" );
		/* 发送一个邮箱消息 1 */
		uwRet = rt_mb_send(test_mail,(rt_uint32_t)&test_str1);
		if (RT_EOK == uwRet)
			rt_kprintf ( "The email message has been sent successfully\n" );
		else
			rt_kprintf ( "The mailbox message failed to send\n" );
		}
		//如果 KEY2 被单击
		
		if ( HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_SET ) {
			rt_kprintf ( "KEY2 is clicked\n" );
		/* 发送一个邮箱 2 */
		uwRet = rt_mb_send(test_mail,(rt_uint32_t)&test_str2);
		if (RT_EOK == uwRet)
			rt_kprintf ( "The email message has been sent successfully\n" );
		else
			rt_kprintf ( "The mailbox message failed to send\n" );
		}
		rt_thread_delay(20); //每 20ms 扫描一次
	}
}


