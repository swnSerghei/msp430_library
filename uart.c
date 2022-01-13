/*
 * uart.c
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */
#include "uart.h"
/*
Clock			Baudrate		UCBRx		UCBRSx		UCBRFx		UCOS16
1,048,576 Hz	9600			109			2			0			0
8MHz			115200			69			4			0			1
16MHz			115200			8			0			11			1
*/
int16 tmpValue = 0;
void Init_Uart()
{
    UCA0CTL1 |= UCSWRST;                                // Enable SW reset
//	P1SEL0 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
//	P1SEL1 &= ~(RXD + TXD) ; // P1.1 = RXD, P1.2=TXD
//
//	UCA0CTL1 |= UCSSEL_2; // SMCLK
////	UCA0BR0 = 160; // 1MHz 115200
////	UCA0BR1 = 1; // 1MHz 115200
////	UCA0BR0 = 34; // 4MHz 115200
//	    UCA0BR0   = 104;// 9600 baud at 16mhz
//	    UCA0BR1   = 0;
//	    UCA0MCTLW = UCBRS0 + UCBRF_2 + UCOS16;
//
//
//
//	UCA0IFG &= ~UCRXIFG;
//	UCA0IFG &= ~UCTXIFG;
//	UCA0IE |= UCTXIE; // Enable USCI_A0 RX interrupt
//	UCA0IE |= UCRXIE; // Enable USCI_A0 RX interrupt
#ifdef __MSP430G2553__
    P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2; // SMCLK
//    UCA0BR0 = 8;
//    UCA0BR1 = 0;
//    UCA0MCTL = 176 ;
        UCA0BR0   = 104;// 9600 baud at 16mhz
        UCA0BR1   = 0;
        UCA0MCTL = UCBRS0 + UCBRF_2 + UCOS16;
        //UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
        //UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt

#endif
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    UC0IFG &= ~UCA0RXIFG;
    UC0IFG &= ~UCA0TXIFG;
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
    UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt
	TxBuffer_Uart_Head = 0;
	TxBuffer_Uart_Tail = 0;

}
void Uart_RecalculateTail_Buffer()
{
	TxBuffer_Uart_Tail++;
	if (TxBuffer_Uart_Tail == (SizeOfBuffer) ) TxBuffer_Uart_Tail=0;//in case if TxBuffer_Uart_Tail is uint8 need to remove this line
	else if (TxBuffer_Uart_Tail == TxBuffer_Uart_Head) {}//overflow
}
void Uart_command(uint8 NrOfElements)
{

}
void Uart_RX_Interrupt(void)
{
//    if ( UCA0RXBUF == '1' ){P2DIR |= BIT2 + BIT0; if (P2OUT&BIT0) P2OUT &= ~(BIT0 + BIT2);else P2OUT |= (BIT0 + BIT2); }
//    else if ( UCA0RXBUF == '2' ) {P1OUT ^= BIT6;}
//    if ( UCA0RXBUF == '+' )
//    {
//        tmpValue = 1;
//    }
//    else if ( UCA0RXBUF == '-' )
//    {
//        tmpValue = -1;
//    }
    tmpValue = UCA0RXBUF;
}
void Uart_TX_Interrupt(void)
{
	if ( TxBuffer_Uart_Head != TxBuffer_Uart_Tail) // TX over?
	{
		UCA0TXBUF = TxBuffer_Uart[TxBuffer_Uart_Head];	//send next byte
		if (TxBuffer_Uart_Head == (SizeOfBuffer-1) ) TxBuffer_Uart_Head = 0;
		else TxBuffer_Uart_Head++;
	}
}
