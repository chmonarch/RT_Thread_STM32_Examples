#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include <string.h>
#include "usart.h"


/* 定义线程控制块 */
static rt_thread_t key_thread = RT_NULL;
static rt_thread_t usart_thread = RT_NULL;
/* 定义消息队列控制块 */
rt_mq_t test_mq = RT_NULL;
/* 定义信号量控制块 */
rt_sem_t test_sem = RT_NULL;

/* 定义申请内存的指针 */
static rt_uint32_t *p_test = RT_NULL;


/* 变量声明  */

/* 相关宏定义 */
#define  USART_RBUFF_SIZE            1024
char Usart_Rx_Buf[USART_RBUFF_SIZE];


/* 函数声明 */
static void key_thread_entry(void* parameter);
static void usart_thread_entry(void* parameter);

void MX_UART_IT_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*)Usart_Rx_Buf, USART_RBUFF_SIZE);  
}

int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT interrupt management experiment!\n");
	rt_kprintf("Press KEY1 | KEY2 to trigger an interrupt!\n");
	rt_kprintf("Serial port send data trigger interrupt, thread process data!\n");
	/* 创建一个消息队列 */
	test_mq = rt_mq_create("test_mq",/* 消息队列名字 */
					4, /* 消息的最大长度 */
					2, /* 消息队列的最大容量 */
					RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
	if (test_mq != RT_NULL)
		rt_kprintf("Message queue created successfully!\n\n");
	/* 创建一个信号量 */
	test_sem = rt_sem_create("test_sem",/* 消息队列名字 */
					 0, /* 信号量初始值，默认有一个信号量 */
					 RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
	if (test_sem != RT_NULL)
		rt_kprintf("Semaphore created successfully!\n\n");
	/* 创建一个线程 */
	key_thread = /* 线程控制块指针 */
	rt_thread_create( "key", /* 线程名字 */
						 key_thread_entry, /* 线程入口函数 */
						 RT_NULL, /* 线程入口函数参数 */
						 512, /* 线程栈大小 */
						 1, /* 线程的优先级 */
						 20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (key_thread != RT_NULL)
		rt_thread_startup(key_thread);
	else
		return -1;
	usart_thread = /* 线程控制块指针 */
	rt_thread_create( "usart", /* 线程名字 */
							 usart_thread_entry, /* 线程入口函数 */
							 RT_NULL, /* 线程入口函数参数 */
							 512, /* 线程栈大小 */
							 2, /* 线程的优先级 */
							 20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (usart_thread != RT_NULL)
		rt_thread_startup(usart_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void key_thread_entry(void* parameter)
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
			rt_kprintf("The interrupt is triggered by KEY %d!\n",r_queue);
		} else {
			rt_kprintf("Data receiving error, error code:0x%lx\n",uwRet);
		}
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	}
}

static void usart_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		uwRet = rt_sem_take(test_sem, /* 获取串口中断的信号量 */
					0); /* 等待时间： 0 */
		if (RT_EOK == uwRet) {
			rt_kprintf("Receive the data:%s\n",Usart_Rx_Buf);
			memset(Usart_Rx_Buf,0,USART_RBUFF_SIZE);/* 清零 */
			HAL_UART_Receive_DMA(&huart1, (uint8_t*)Usart_Rx_Buf, USART_RBUFF_SIZE);  
		}
	}
}

