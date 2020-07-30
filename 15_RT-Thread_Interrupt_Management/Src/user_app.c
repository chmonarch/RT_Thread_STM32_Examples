#include "stm32f1xx_hal.h"
#include "rtthread.h"
#include "gpio.h"
#include "usart.h"


/* 外部定义消息队列控制块 */
extern rt_mq_t test_mq;
extern rt_sem_t test_sem;

uint32_t send_data1 = 1;
uint32_t send_data2 = 2;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
void DEBUG_USART_IRQHandler(void);


/************************************************************************
* @ 函数名 ： 
* @ 功能说明： 外部中断服务函数
* @ 参数 ： 无
* @ 返回值 ： 无
********************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* 进入中断 */
	rt_interrupt_enter();
	switch(GPIO_Pin)
	{
		case KEY1_Pin:
				/* 将数据写入（发送）到队列中，等待时间为 0 */
				rt_mq_send( test_mq, /* 写入（发送）队列的 ID(句柄) */
									&send_data1, /* 写入（发送）的数据 */
									sizeof(send_data1)); /* 数据的长度 */
				break;
		case KEY2_Pin:
				/* 将数据写入（发送）到队列中，等待时间为 0 */
				rt_mq_send( test_mq, /* 写入（发送）队列的 ID(句柄) */
									&send_data2, /* 写入（发送）的数据 */
									sizeof(send_data2)); /* 数据的长度 */
				break;
		default:
				break;
	}
	/* 离开中断 */
	rt_interrupt_leave();
}


// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	uint32_t tmp_flag = 0;
	uint32_t temp;
	/* 进入中断 */
	rt_interrupt_enter();
	
	tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart1); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// 获取DMA中未传输的数据个数   
		temp  = hdma_usart1_rx.Instance->CNDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数
		//这句和上面那句等效
		//rx_len = USART_RBUFF_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		//给出二值信号量，发送接收到新数据标志，供前台程序查询
		rt_sem_release(test_sem);  
	 }
	/* 离开中断 */
	rt_interrupt_leave();
}


