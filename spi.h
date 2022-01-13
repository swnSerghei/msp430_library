/*
 * spi.h
 *
 *  Created on: 04 февр. 2015 г.
 *      Author: Serghei
 */
#ifndef SPI_H_
#define SPI_H_
#include "commontypes.h"
#include "spi.h"
#include <msp430.h>
#ifdef __MSP430G2955__
#define SIMO BIT1   //from port 3
#define SOMI BIT2   //from port 3
#define CLK  BIT3   //from port 3
#endif
#ifdef __MSP430G2553__
#define SIMO BIT7   //from port 1
#define SOMI BIT6   //from port 1
#define CLK  BIT5   //from port 1
#endif
void init_spi();
void SPI_Interrupt();
#endif /* SPI_H_ */
