#ifndef _MCAL_AT16_H
#define _MCAL_AT16_H 1

#include <avr/io.h>

/*LED defines*/
#define LED_PIN 	PINA3
#define LED_DRR		DDRA
#define LED_PORT	PORTA
/*TURBINE defines*/
#define TIMER_COMPARE_REG	OCR1A
#define TIMER_ICR_REG		ICR1
#define TOP_VALUE		10000
#define TURBINE_OUT_PIN	PINA6
#define TURBINE_DDR_PORT	DDRA


/*led macros*/
#define SET_LED		LED_DRR |= 0x1 << LED_PIN;
#define  LED_OFF		LED_PORT &= ~(0x1 << LED_PIN);
#define  LED_ON		LED_PORT |= 0x1 << LED_PIN;
/*Turbine macros*/
#define SET_TIMER_REG1	TCCR1A |=  0x1 << COM1A1
#define SET_TIMER_REG2	TCCR1B |= 0x1 << WGM13 | 0x1 << CS11
#define SET_TOP_LVL_ICR		ICR1 = TOP_VALUE
#define SET_TOP_LVL_OCR		TIMER_COMPARE_REG = TOP_VALUE
#define TURBINE_SET_PORT	TURBINE_DDR_PORT |= (0x1 << TURBINE_OUT_PIN);
/*Senzor*/
#define GET_DEC_LVL (uint8_t)(PINA & 1u)
#define SET_SENSOR_INPUT	PORTA |= 0x1

#endif /*_MCAL_AT16_H*/
