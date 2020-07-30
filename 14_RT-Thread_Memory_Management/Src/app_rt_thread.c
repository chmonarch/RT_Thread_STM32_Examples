#include "rtthread.h"
#include "main.h"
#include "stdio.h"



#if 1
//静态内存管理

/* 定义线程控制块 */
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;
/* 定义内存池控制块 */
static rt_mp_t test_mp = RT_NULL;   
/* 定义申请内存的指针 */
static rt_uint32_t *p_test = RT_NULL;


/* 变量声明  */

/* 相关宏定义 */
#define BLOCK_COUNT 20 //内存块数量
#define BLOCK_SIZE 3 //内存块大小


/* 函数声明 */
static void alloc_thread_entry(void* parameter);
static void free_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT static memory management experiment!\n");
	rt_kprintf("Creating a memory pool...........\n");
	/* 创建一个静态内存池 */
	test_mp = rt_mp_create("test_mp", 
					BLOCK_COUNT,
					BLOCK_SIZE);
	if (test_mp != RT_NULL)
		rt_kprintf("Static memory pool created successfully!\n\n");
	/* 创建一个线程 */
	alloc_thread = /* 线程控制块指针 */
				rt_thread_create( "alloc", /* 线程名字 */
				alloc_thread_entry, /* 线程入口函数 */
				RT_NULL, /* 线程入口函数参数 */
				512, /* 线程栈大小 */
				1, /* 线程的优先级 */
				20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (alloc_thread != RT_NULL)
		rt_thread_startup(alloc_thread);
	else
		return -1;
	
	free_thread = /* 线程控制块指针 */
				rt_thread_create( "free", /* 线程名字 */
				free_thread_entry, /* 线程入口函数 */
				RT_NULL, /* 线程入口函数参数 */
				512, /* 线程栈大小 */
				2, /* 线程的优先级 */
				20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (free_thread != RT_NULL)
		rt_thread_startup(free_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void alloc_thread_entry(void* parameter)
{
	rt_kprintf("Requesting memory from the memory pool...........\n");
	p_test = rt_mp_alloc(test_mp,0);
	if (RT_NULL == p_test) /* 没有申请成功 */
		rt_kprintf("Static memory application failed!\n");
	else
		rt_kprintf("Static memory application successful, address is %d！ \n\n",p_test);
	rt_kprintf("Writing data to P test...........\n");
	*p_test = 1234;
	rt_kprintf("Data has been written to the P test address\n");
	rt_kprintf("*p_test = %.4d ,The address is:%d \n\n", *p_test,p_test);
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		rt_thread_delay(1000); //每 1000ms 扫描一次
	}
}
static void free_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	rt_kprintf("Freeing memory...........\n");
	rt_mp_free(p_test);
	rt_kprintf("Free memory successfully!\n\n");
	rt_kprintf("Deleting memory...........\n");
	uwRet = rt_mp_delete(test_mp);
	if (RT_EOK == uwRet)
		rt_kprintf("Memory removed successfully!\n");
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		rt_thread_delay(500); //每 500ms 扫描一次
	}
}

#else
//动态内存管理

/* 定义线程控制块 */
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;

/* 定义申请内存的指针 */
static rt_uint32_t *p_test = RT_NULL;

/* 变量声明  */

/* 相关宏定义 */
#define TEST_SIZE 100 //内存大小（字节）


/* 函数声明 */
static void alloc_thread_entry(void* parameter);
static void free_thread_entry(void* parameter);



int MX_RT_Thread_Init(void)
{
	rt_kprintf("This is an RTT dynamic memory management experiment!\n");
	/* 创建一个线程 */
	alloc_thread = /* 线程控制块指针 */
	rt_thread_create( "alloc", /* 线程名字 */
							 alloc_thread_entry, /* 线程入口函数 */
							 RT_NULL, /* 线程入口函数参数 */
							 512, /* 线程栈大小 */
							 1, /* 线程的优先级 */
							 20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (alloc_thread != RT_NULL)
		rt_thread_startup(alloc_thread);
	else
		return -1;
	
	free_thread = /* 线程控制块指针 */
	rt_thread_create( "free", /* 线程名字 */
							free_thread_entry, /* 线程入口函数 */
							RT_NULL, /* 线程入口函数参数 */
							512, /* 线程栈大小 */
							2, /* 线程的优先级 */
							20); /* 线程时间片 */
	/* 启动线程，开启调度 */
	if (free_thread != RT_NULL)
		rt_thread_startup(free_thread);
	else
		return -1;
}

/*
************************************************************
* 线程定义
*********************************************************
*/
static void alloc_thread_entry(void* parameter)
{
	rt_kprintf("Requesting memory from the memory pool...........\n");
	p_test = rt_malloc(TEST_SIZE); /* 申请动态内存 */
	if (RT_NULL == p_test) /* 没有申请成功 */
		rt_kprintf("Dynamic memory application failed!\n");
	else
		rt_kprintf("Dynamic memory application successful, address is %d！ \n\n",p_test);
	rt_kprintf("Writing data to P test...........\n");
	*p_test = 1234;
	rt_kprintf("Data has been written to the P test address\n");
	rt_kprintf("*p_test = %.4d ,Address is:%d \n\n", *p_test,p_test);
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		rt_thread_delay(1000); //每 1000ms 扫描一次
	}
}
static void free_thread_entry(void* parameter)
{
	rt_kprintf("Freeing memory...........\n");
	rt_free(p_test);
	rt_kprintf("Free memory successfully!\n\n");
	/* 线程都是一个无限循环，不能返回 */
	while (1) {
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		rt_thread_delay(500); //每 500ms 扫描一次
	}
}

#endif
