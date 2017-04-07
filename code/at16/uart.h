#ifndef _UART_H
#define _UART_H

#include "stdint.h"
#define BAUD9600 207
#define UART_RX_MAX 20

extern void uart_init(uint16_t baud);
extern void uart_sendByte(uint8_t byte);
extern uint8_t uart_sendByteNotBlocking(uint8_t byte);
extern uint8_t uart_rx_flush(uint8_t *buffer);

#endif//_UART_H
