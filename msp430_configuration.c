/*
 * msp430g2955_configuration.c
 *
 *  Created on: Jan 29, 2020
 *      Author: uidg6243
 */
#include "msp430_configuration.h"
void configurationClock ()
{
    #ifdef __MSP430G2955__

    P1DIR = 0;P2DIR = 0;P3DIR = 0;P4DIR = 0;
    P1OUT = 0;P2OUT = 0;P3OUT = 0;P4OUT = 0;

    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulatio
    BCSCTL1 = CALBC1_16MHZ; // Set range
    BCSCTL2 |= DIVS_0;

    //WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    WDTCTL = WDTPW | WDTNMI | WDTTMSEL | WDTCNTCL ;//Watchdog timer+ password, NMI function, Interval timer mode, WDTCNT = 0000h
    IE1   |=  WDTIE;          //Enable the WDTIE bit

    #endif
    #ifdef __MSP430G2553__
    IFG1=0;
    P1DIR = 0;P2DIR = 0;P3DIR = 0;
    P1OUT = 0;P2OUT = 0;P3OUT = 0;
    P1SEL = 0;P2SEL = 0;P3SEL = 0;
    P1SEL2= 0;P2SEL2= 0;P3SEL2= 0;
    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulatio
    BCSCTL1 = CALBC1_16MHZ; // Set range
    BCSCTL2 |= DIVS_0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL;//Watchdog timer+ password, NMI function, Interval timer mode, WDTCNT = 0000h
    //IE1   =  WDTIE;          //Enable the WDTIE bit
    //IE1   = WDTIE | OFIE | NMIIE | ACCVIE;//for reset debug
    #endif
}

void test_SMCLK_configuration()
{
#ifdef __MSP430G2955__

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    //for monitoring SMCLK
    P1DIR   |= BIT4;
    P1SEL   |= BIT4;
    P1SEL2  &=~BIT4;

    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulatio

//    BCSCTL1 = CALBC1_16MHZ;//Basic Clock System Control Register 1
//    BCSCTL2 = 0;//Basic Clock System Control Register 2
//    BCSCTL3 = 0;//Basic Clock System Control Register 3
//
//    IFG1 = 0;
//    IFG2 = 0;
//
//    BCSCTL3 |= XT2S_0 + XCAP_3;//32768-Hz crystal on LFXT1;~12.5 pF
//    do{
//        IFG1 &= ~OFIFG;
//    }while(OFIFG&IFG1);
//    IFG1 &= ~OFIFG;
//    BCSCTL2 |= SELM1+SELS;
    while(1)
    {

    };
#endif
}
#pragma vector=NMI_VECTOR//NMI, oscillator fault, flash memory access violation; NMIIFG;OFIFG;ACCVIFG; address 0FFFCh;Priority 30
__interrupt void NMI_Interrupt_function(void)
{
   IFG1 = 0;
}
//#pragma vector=RESET_VECTOR//Power-up, external reset, watchdog, flash password, illegal instruction fetch; PORIFG;RSTIFG;WDTIFG;KEYV; address 0FFFEh;Priority 31
//__interrupt void Reset_Interrupt_function(void)
//{
//    IFG1 = 0;
//}



