#include "rtthread.h"
#include "main.h"
#include "stdio.h"
#include <string.h>





/* 定义线程控制块 */
static rt_thread_t test1_thread = RT_NULL;
static rt_thread_t test2_thread = RT_NULL;


/* 函数声明 */
static void test1_thread_entry(void* parameter);
static void test2_thread_entry(void* parameter);


int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT bidirectional linked list operation experiment!\n");
	test1_thread = /* 线程控制块指针 */
	rt_thread_create( "test1", /* 线程名字 */
					test1_thread_entry, /* 线程入口函数 */
					RT_NULL, /* 线程入口函数参数 */
					512, /* 线程栈大小 */
					2, /* 线程的优先级 */
					20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (test1_thread != RT_NULL)
		rt_thread_startup(test1_thread);
	else
		return -1;
	
	test2_thread = /* 线程控制块指针 */
	rt_thread_create( "test2", /* 线程名字 */
					test2_thread_entry, /* 线程入口函数 */
					RT_NULL, /* 线程入口函数参数 */
					512, /* 线程栈大小 */
					3, /* 线程的优先级 */
					20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (test2_thread != RT_NULL)
		rt_thread_startup(test2_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void test1_thread_entry(void* parameter)
{
	rt_list_t *head; /* 定义一个双向链表的头节点 */
	rt_list_t *node1; /* 定义一个双向链表的头节点 */
	rt_list_t *node2; /* 定义一个双向链表的头节点 */
	
	head = rt_malloc(sizeof(rt_list_t));/* 申请动态内存 */
	if (RT_NULL == head) /* 没有申请成功 */
		rt_kprintf("Dynamic memory application failed!\n");
	else
		rt_kprintf("Dynamic memory application is successful, the address of the head node is %d!\n",head);
	
	rt_kprintf("\n Bidirectional linked list initialization......\n");
	rt_list_init(head);
	
	if (rt_list_isempty(head))
		rt_kprintf("Bidirectional linked list initialization succeeded!\n\n");
	
	/* 插入节点：顺序插入与从末尾插入 */
	rt_kprintf("Add node and tail node additions......\n");
	
	/* 动态申请第一个结点的内存 */
	node1 = rt_malloc(sizeof(rt_list_t));
	
	/* 动态申请第二个结点的内存 */
	node2 = rt_malloc(sizeof(rt_list_t));
	
	rt_kprintf("Add the first and second nodes.....\n");
	/* 因为这是在某个节点后面添加一个节点函数
	为后面的 rt_list_insert_before（某个节点之前）
	添加节点做铺垫,两个函数添加完之后的顺序是
	head -> node1 -> node2 */
	rt_list_insert_after(head,node2);
	rt_list_insert_before(node2,node1);
	
	if ((node1->prev == head) && (node2->prev == node1))
		rt_kprintf("Add node successfully!\n\n");
	else
		rt_kprintf("Add node failed!\n\n");
	
	rt_kprintf("Remove nodes......\n"); /* 删除已有节点 */
	rt_list_remove(node1);
	rt_free(node1);/* 释放第一个节点的内存 */
	if (node2->prev == head)
		rt_kprintf("Node removal successful\n\n");
	
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		rt_thread_delay(500); //每 500ms 扫描一次
	}
}

static void test2_thread_entry(void* parameter)
{
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		rt_kprintf("Thread running!\n");
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		rt_thread_delay(1000); //每 1000ms 扫描一次
	}
}



