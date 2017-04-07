/*
 * timer.h
 *
 * Created: 14.05.2015 15:42:59
 *  Author: Andrei
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define TIMER250us 250
#define TIMER500us 500

extern void timer_init();
extern void timer_setValue(uint8_t timer_value);




#endif /* TIMER_H_ */
