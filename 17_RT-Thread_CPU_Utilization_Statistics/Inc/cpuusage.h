#ifndef __CPUUSAGE_H__
#define __CPUUSAGE_H__
#include <rtthread.h>
#include <rthw.h>


/* 获取 cpu 利用率 */
void cpu_usage_init(void);
void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);


#endif

