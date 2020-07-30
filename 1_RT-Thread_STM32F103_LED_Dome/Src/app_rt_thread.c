#include "rtthread.h"
#include "main.h"
#include "stdio.h"

struct rt_thread led_thread;
rt_uint8_t rt_led_thread_stack[128];
void led_task_entry(void *parameter);

//初始化线程函数
void MX_RT_Thread_Init(void)
{
	//初始化LED线程
	rt_thread_init(&led_thread,"led",led_task_entry,RT_NULL,&rt_led_thread_stack[0],sizeof(rt_led_thread_stack),3,20);
	//开启线程调度
	rt_thread_startup(&led_thread);
}

//主任务
void MX_RT_Thread_Process(void)
{
	printf("heihei Hello RT-Thread !!!");
	rt_thread_delay(2000);
}

//LED任务
void led_task_entry(void *parameter)
{
	while(1)
	{
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin, GPIO_PIN_RESET);
		rt_thread_delay(500);
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin, GPIO_PIN_SET);
		rt_thread_delay(500);
	}
}
