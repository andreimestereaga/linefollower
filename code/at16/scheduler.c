/*
 * scheduler.c
 *
 * Created: 14.05.2015 17:42:22
 *  Author: Andrei
 */ 

#include "scheduler.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>


#include <stdio.h>

#define NULL_PTR	NULL
#define MAX_TASK_NUM	8

#define SCH_START 0x1
#define SCH_WAIT  0x2

uint8_t active_tasks;
//volatile uint16_t counter;

void dummy_task(void)
{
	
	
}

void (*task_array[MAX_TASK_NUM])(void);
uint16_t task_period[MAX_TASK_NUM];
volatile uint16_t counter_1ms = 0;
volatile uint8_t scheduler_status = 0;
uint16_t max_count_val = 0;

volatile uint8_t _it_count = 0;

uint8_t toggleVal = 0x0;

ISR(TIMER0_COMP_vect)
{
	//if( _it_count % 10 == 0){
		scheduler_status = SCH_START;
		counter_1ms++;
	//}



//	_it_count++;
}

void sch_init()
{
	task_array[0] = &dummy_task;
	task_array[1] = &dummy_task;
	task_array[2] = &dummy_task;
	task_array[3] = &dummy_task;
	task_array[4] = &dummy_task;
	task_array[5] = &dummy_task;
	task_array[6] = &dummy_task;
	task_array[7] = &dummy_task;
	
	timer_init();
	timer_setValue(TIMER250us);
	active_tasks = 0;
}

void sch_setTaskStatus(uint8_t task_no,uint8_t status)
{	
	if(status == ENABLE_TASK)
	{
		active_tasks |= task_no;
	}
	else if(status == DISABLE_TASK)
	{
		active_tasks &= ~task_no;
	}
}

void sch_setTask( void *funct, uint8_t priority, uint16_t period )
{
	if(priority <= MAX_TASK_NUM)
	{
		task_array[priority] = funct;
		task_period[priority] = period;
		
		if(period > max_count_val)
		{
			max_count_val = period;
		}
	}
}

void sch_main()
{
	
	if( scheduler_status == SCH_START)
	{
		for(uint8_t sch_index = 0; sch_index <= MAX_TASK_NUM;sch_index++)
		{
			if( ((active_tasks >> sch_index) & 1) &&   ( (counter_1ms % task_period[sch_index]) == 0) )
			{
				(*task_array[sch_index])();
			}
		}
		
		scheduler_status = SCH_WAIT;
	}
	else
	{
		while(scheduler_status == SCH_WAIT)
		{
			
		}
		
	}
	
}




 
