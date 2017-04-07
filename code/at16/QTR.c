#include "QTR.h"
#include <avr/io.h>

#define F_CPU 20000000UL
#include <util/delay.h>

#define S1 PINB5
#define S2 PINC0
#define S3 PINC1
#define S4 PIND3
#define S5 PIND7
#define S6 PINB4
#define S7 PINB3
#define S8 PINB2

uint16_t qtr_values[10];
volatile uint16_t internal_counter = 0;
#define MAX_VAL 300
#define SCALE 100


void QTR_init()
{

	DDRA = 0x0;
	DDRB &= 0xFC;

	PORTA = 0xFF;
	PORTB = 0x3;

}


uint8_t valueA = 0;
uint8_t valueB = 0;
uint16_t value = 0;

uint16_t QTR_reading(uint16_t *values){
	
	// Put the seonsor as output

	internal_counter = 0;
	uint16_t sens_val = 1;

	DDRA = 0xFF;
	DDRB |= 0x3;

	PORTA = 0xFF;
	PORTB |= 0x3;

	_delay_us(10);


	// Put sensors as input
	DDRA = 0x0;
	PORTA = 0x0;

	DDRB &= 0xFC;
	PORTB &= 0xFC;

	for(uint8_t idx = 0; idx < 10; idx++){
		qtr_values[idx] = 0;
	}

	while(internal_counter < MAX_VAL){
		
		valueA = PINA;
		valueB = PINB;
		
		for(uint8_t s_idx = 2; s_idx < 10; s_idx++){
			if( (valueA >> (s_idx - 2))  & 0x1){
				qtr_values[s_idx]++;
			}
		}

		if(valueB & 0x1){
			qtr_values[1]++;
		}
		if( (valueB >> 1) & 0x1){
			qtr_values[0]++;
		}	
		

		internal_counter++;
		
	//	_delay_us(100);

	}

	for(uint8_t idx = 0; idx < 10; idx++){
		qtr_values[idx] *= 10;
		*(values + idx) = *(qtr_values + idx);

		if( qtr_values[idx] > 3 ){
			sens_val = 0;
		}
	}
	

	return sens_val;
}


uint16_t QTR_compute()
{
	


	valueA = PINA;
	valueB = PINB;

	value = (uint16_t)valueA;
	value = (uint16_t)(value << 2);
	value |= (uint16_t)( (uint16_t)valueB & 1) << 1;
	value |= (uint16_t)(valueB & 0x2) >> 1;

	return value;
}


