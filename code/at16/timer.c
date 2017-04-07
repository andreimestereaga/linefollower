/*
 * timer.c
 *
 * Created: 14.05.2015 15:31:06
 *  Author: Andrei
 */ 

#include <avr/io.h>

#define BIT8_REG_RESET		(uint8_t)0x00		/*8 BIT reset value for registers*/
#define PWM_MODE1			0x1u << WGM00		/*PWM Phase correct mode based on dual slope operation */
#define INT_CLK_DIV1		0x1u << CS00		/*Internal clock divided by 1*/
#define INT_CLK_DIV8		0x1u << CS01		/*Internal clock divided by 8*/
#define INT_CLK_DIV256		0x1u << CS02		/*Internal clock divided by 256*/
#define INT_CLK_DIV64		INT_CLK_DIV8 | INT_CLK_DIV1			/*Internal clock divided by 64*/
#define INT_CLK_DIV1024		INT_CLK_DIV64 | INT_CLK_DIV256		/*Internal clock divided by 1024*/

#define SET_COMPARE		(0x1u << COM01) | (0x1u << COM00)
#define CLEAR_COMPARE	0x1u << COM01

#define CTC_MODE 0x1u << WGM01

#define T0_OFVENABLE	0x1


void timer_init()
{
		uint8_t tccr0_reg = BIT8_REG_RESET;
		

		tccr0_reg = INT_CLK_DIV64 | CTC_MODE ; /*1 ms*/
		
		TCCR0 = tccr0_reg;
		TCNT0 = (uint8_t)0x00;
		OCR0 = (uint8_t)0x00;
		
		DDRB |= (0x1u << PINB3);
		
		if(T0_OFVENABLE)
		{
			TIMSK |= (0x1u << OCIE0);
		}
}

void timer_setValue(uint8_t timer_value)
{
	OCR0 = (uint8_t)timer_value;
}
