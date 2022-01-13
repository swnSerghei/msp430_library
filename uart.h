/*
 * uart.h
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */

#ifndef UART_H_
#define UART_H_

#include "commontypes.h"
#include <msp430.h>

#define RXD BIT1
#define TXD BIT2

#define SizeOfBuffer 100
uint8 TxBuffer_Uart[SizeOfBuffer];
uint8 TxBuffer_Uart_Head;
uint8 TxBuffer_Uart_Tail;
uint8 RxBuffer_Uart[SizeOfBuffer];
extern int16 tmpValue;

void Init_Uart();
void Uart_RX_Interrupt();
void Uart_TX_Interrupt();

void Uart_RecalculateTail_Buffer();
#endif /* UART_H_ */
