#ifndef _PWM_H
#define _PWM_H

#include <avr/io.h>
#include "stdint.h"

/*
*	LF motors orientations
*
*   sensors
* 	 --------
*	   |  |
*	   |  |       LEFT  	RIGHT
*  L -------- R	  L1 UP		R1	UP
* |-|  PCB   |-|  L2 DOWN	R2	DOWN
*	=========
*/

/* PCB nets
*	ATMEGA16		DRV8833			MOTORS
*	PIND6			AIN1			L1	
*	PIND5 (OC1A)	AIN2			L2
*	PIND4 (OC1B)	BIN2			R1
*	PIND3			BIN1			R2
*/

/*	PWM control		RIGHT DIR		LEFT DIR
*	R1 H 			FORWARD			-
*	R2 L							-
********************************************
*	R1 L			BACKWARD		-
*	R2 H							-
********************************************
*	L1 H			-				FORWARD
*	L2 L			-
********************************************
*	L1 L			-				BACKWARD
*	L2 H			-
********************************************
*/



#define PIN_DIR_RIGHT	PIND3
#define PIN_PWM_RIGHT	PIND4

#define PIN_DIR_LEFT	PIND6
#define PIN_PWM_LEFT	PIND5

/* MOTORS initi procedure
*	The counter1 is configured as phase & fecquency correct PWM mode
* 	The output frecquency of PWM signals shall be 50kHz
*	The TOP value is calculated using the following formula:
*	TOP = (F_CPU / 2 * N * 50kHZ)
*	where N is the prescaler. For 50kHz signal the top is: 160 (N = 1)
*/

//the output is generated as: clear on compare match while upcounting and
//set on compare match while downcounting
#define MOTORS_INIT_TCCR1A TCCR1A |= (0x1 << COM1A1) | (0x1 << COM1B1) | (0x1 << WGM10); //| (0x1 << WGM11);
//the mode of operantions is 8, where the TOP value is defined by ICR1, and the
//PWM resolution is 16bits. The prescaler used is 1.	
#define MOTORS_INIT_TCCR1B TCCR1B |= (0x1 << CS11); //| (0x1 << CS10);
// The top value was calculated according to above formula (in header)
#define MOTORS_SET_TOP	ICR1 = (uint16_t)320

//this value was calculated according to defined pins for directions and PWMs
#define PORT_DIR_SETUP_VAL	0x78
#define MOTOR_DDR_SETUP		DDRD |= PORT_DIR_SETUP_VAL

#define MOTOR_FORWARD	0x1
#define MOTOR_BACKWARD	0x2

#define SET_RIGHT_FORWARD		PORTD |= 0x8//PORTD &= 0xBF
#define SET_RIGHT_BACKWARD		PORTD &= 0xF7//PORTD |= 0x40 
#define SET_RIGHT_FW_PWM(val)	OCR1B = 0xFF - (uint8_t)val//OCR1A = 0xFF - (uint8_t)val
#define SET_RIGHT_BK_PWM(val) 	OCR1B = (uint8_t)val//OCR1A = (uint8_t)val

#define SET_LEFT_BACKWARD		PORTD &= 0xBF//PORTD |= 0x8
#define SET_LEFT_FORWARD		PORTD |= 0x40//PORTD &= 0xF7
#define SET_LEFT_BK_PWM(val)	OCR1A = (uint8_t)val//OCR1B = 0xFF - (uint8_t)val
#define SET_LEFT_FW_PWM(val)	OCR1A = 0xFF - (uint8_t)val//OCR1B = (uint8_t)val

extern void pwm_init();
extern void pwm_setMotors(uint8_t right_dir,uint8_t right_val, uint8_t left_dir, uint8_t left_val);
extern void pwm_setMotorsSigned(int16_t right_val, int16_t left_val);
#endif
