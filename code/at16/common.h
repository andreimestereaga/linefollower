#ifndef COMMON_H
#define COMMON_H 1


#include <avr/io.h>


#define INIT_STATUS_LED DDRC |= 0x2

#define STATUS_LED_OFF  PORTC |= 0x2
#define STATUS_LED_ON	PORTC &= 0xFD



#endif
