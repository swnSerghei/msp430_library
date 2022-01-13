/*
 * spi.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include "spi.h"
void init_spi()
{
#ifdef msp430i
UCB0CTLW0 |= UCSWRST;

P1SEL0 |= SIMO + CLK;

UCB0CTLW0 |= UCMST;		//Master mode select
UCB0CTLW0 |= UCSYNC;	//syncron mode
UCB0CTLW0 |= UCMSB;		//MSB firs
UCB0CTLW0 |= UCCKPH;    //Clock Phase inactive state is low
//UCB0CTLW0 |= UCCKPL;  //The inactive state is high
UCB0CTLW0 |= UCSSEL__SMCLK;	//10b = SMCLK
UCB0BR0 |= UCSYNC;
UCB0IFG &= ~UCTXIFG;
//UCB0IE &= UCTXIE;
UCB0CTLW0 &= ~UCSWRST;
#endif

#ifdef __MSP430G2955__
    UCB0CTL1 |= UCSWRST;

    P3SEL |= SIMO + SOMI + CLK;

    UCB0CTL0 |= UCMST;     //Master mode select
    UCB0CTL0 |= UCSYNC;    //syncron mode
    UCB0CTL0 |= UCMSB;     //MSB firs
    UCB0CTL0 |= UCCKPH;    //Clock Phase inactive state is low
    //UCB0CTLW0 |= UCCKPL;  //The inactive state is high
    UCB0CTL1 |= UCSSEL_3;  //10b = SMCLK
    UCB0BR0  = 16;   //low byte
    UCB0BR1  = 0;   //high byte
    UCB0BR0 |= UCSYNC;
    IFG2 &= ~UCB0TXIFG;
    IFG2 &= ~UCB0RXIFG;
    UCB0CTL1 &= ~UCSWRST;

#endif

#ifdef __MSP430G2553__
    UCB0CTL1 |= UCSWRST;

    P1SEL |= SIMO + CLK;
    P1SEL2 |= SIMO + CLK;

    UCB0CTL0 |= UCMST;     //Master mode select
    UCB0CTL0 |= UCSYNC;    //syncron mode
    UCB0CTL0 |= UCMSB;     //MSB firs
    UCB0CTL0 |= UCCKPH;    //Clock Phase inactive state is low
    //UCB0CTLW0 |= UCCKPL;  //The inactive state is high
    UCB0CTL1 |= UCSSEL_3;  //10b = SMCLK
    UCB0BR0  = 16;   //low byte
    UCB0BR1  = 0;   //high byte
    UCB0BR0 |= UCSYNC;
    IFG2 &= ~UCB0TXIFG;
    IFG2 &= ~UCB0RXIFG;
    UCB0CTL1 &= ~UCSWRST;
#endif
}

void SPI_Interrupt()
{
#ifdef msp430i
    UCB0IFG &= ~UCTXIFG;
#endif
#ifdef __MSP430G2955__
    IFG2 &= ~UCB0TXIFG;
#endif
}
