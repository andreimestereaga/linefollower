#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include "scheduler.h"
#include "pwm.h"



#define UART_IDLE 		0x0
#define UART_SEND 		0x1
#define UART_RECEIVE 	0x2
#define UART_OK			0x4
#define UART_NOK		0x5



#define UART_RXCIE UCSRB |= (0x1 << RXCIE);
#define UART_TXCIE UCSRB |= (0x1 << TXCIE);


uint8_t uart_tx_state;
uint8_t uart_rx_state;
uint8_t uart_rx_buffer[UART_RX_MAX];
uint8_t uart_rx_index;


ISR(USART_TXC_vect)
{
	uart_tx_state = UART_IDLE;
}

ISR(USART_RXC_vect)
{
	sch_setTaskStatus(TASK_1,DISABLE_TASK);
	pwm_setMotors(MOTOR_FORWARD,0,MOTOR_FORWARD,0);

	uart_rx_state = UART_RECEIVE;
	uart_rx_buffer[uart_rx_index++] = UDR;

	if(uart_rx_index >= UART_RX_MAX)
	{
		uart_rx_index = 0;
	}
}

uint8_t uart_rx_flush(uint8_t *buffer)
{
	cli();
	uint8_t uart_available = 0;
	if(uart_rx_state == UART_RECEIVE)
	{
		for(uint8_t idx = 0; idx < uart_rx_index; idx++)
		{
			*(buffer + idx) = *(uart_rx_buffer + idx);

		}
		uart_available = uart_rx_index;
		uart_rx_index = 0;
		uart_rx_state = UART_IDLE;
	}
	sei();

	return uart_available;
}



void uart_init(uint16_t baud) // 1Mhz baud, 8 data, 1 stop, none parity
{

	uart_rx_index = 0;
	uart_tx_state = UART_IDLE;
	uart_rx_state = UART_IDLE;

    UCSRA  = (0x1 << U2X);
	UCSRB  = (0x1 << TXEN) | (0x1 << RXEN) | (0x1 << RXCIE) | (0x1 << TXCIE);
	UCSRC  = (0x1 << URSEL) | (0x1 << UCSZ0) | (0x1 << UCSZ1);
	UBRRL =  3;
	UBRRH =  1;


	sei();
}

void uart_sendByte(uint8_t byte)
{
	UDR = byte;

	while( ( (UCSRA >> UDRE) & 0x1) == 0x0 )
	{
		
	}
}



uint8_t uart_sendByteNotBlocking(uint8_t byte)
{
	uint8_t retValue = UART_NOK;
	if( uart_tx_state == UART_IDLE)
	{
		retValue = UART_OK;
		
		UDR = byte;

	}

	return retValue;
}
