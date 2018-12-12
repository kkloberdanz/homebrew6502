/*
 * uart.h
 *
 * This file contains the function declarations for uart.c
 *
 */

#ifndef UART_H
#define UART_H 1

void UART_init(void);
void UART_write(unsigned char);
void UART_sendvalue(uint8_t);
void UART_writestr(char *);


#endif
