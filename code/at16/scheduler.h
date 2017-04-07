/*
 * scheduler.h
 *
 * Created: 14.05.2015 17:42:34
 *  Author: Andrei
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

#define TASK_1 0x1
#define TASK_2 0x2
#define TASK_3 0x4
#define TASK_4 0x8
#define TASK_5 0x10
#define TASK_6 0x20
#define TASK_7 0x40
#define TASK_8 0x80

#define ENABLE_TASK 	0x1
#define DISABLE_TASK 	0x2


extern void sch_init();
extern void sch_setTask( void *funct, uint8_t priority, uint16_t period );
extern void sch_main();
extern void sch_setTaskStatus(uint8_t task_no,uint8_t status);

#endif /* SCHEDULER_H_ */
