/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"
//31, highest

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)//29 priority
{
    __no_operation();
}
volatile uint16 counterTimer = 0;
#pragma vector=TIMER1_A1_VECTOR //28 priority
__interrupt void Timer1_A1 (void)
{
    volatile uint16 TA1IV_local = TA1IV;
//    if (TA1IV_local & TA1IV_TACCR1)    // CCR1
//     {
//        TA1CCTL1 &= ~CCIFG;
//     }
//    if (TA1IV_local & TA1IV_TACCR2)    // CCR2
//    {
//        if ( MPPT_correction < 0 )//decrease
//        {
//            if ( TA1CCR1_local > DeadTime )
//            {
//                TA1CCR1--;
//            }
////            start_adc();
//            MPPT_correction++;
//        }
//        else if ( MPPT_correction > 0 )//increase
//        {
//            if ( TA1CCR1_local < (FREQUENCY - DeadTime) )
//            {
//                TA1CCR1++;
//            }
//            //else if ( timer_Boost_state ) timer_Correction_Boost = (-1);
////            start_adc();
//            MPPT_correction--;
//        }
//        else
//        {
//        }//nothing to do
//        if (TA1CCR1_local > (FREQUENCY>>1)) TA1CCR2 = FREQUENCY>>4;
//        else TA1CCR2 = FREQUENCY-(FREQUENCY>>4);
//        TA1CCTL2 &= ~CCIFG;
//    }

    if (TA1IV_local == TA1IV_TACCR1)
    {
        if ( MPPT_correction < 0 || timer_Buck_state == stop_in_progress)//decrease
                {
                    if (BUCK_timer > DeadTime)
                    {
                        BUCK_timer--;
                        MPPT_correction++;
                    }
                    else
                    {
                        MPPT_correction = 0;
                        if ( timer_Buck_state == stop_in_progress ) stop_timer_Buck();
                    }
                }
                else if ( MPPT_correction > 0 )//increase
                {
                    if (BUCK_timer < (FREQUENCY-DeadTime))
                    {
                        BUCK_timer++;
                        MPPT_correction--;
                    }
                    else MPPT_correction = 0;
                }
                else
                {
                }//nothing to do

                TA1CCTL1 &= ~TA1IV_TACCR1;
    }
    else if(TA1IV_local == TA1IV_TACCR2)
    {
        if( timer_Boost_state == start)
       {
           if ( timer_Correction_Boost < 0 )//decrease
           {
               if ( BOOST_timer > DeadTime )
               {
                   BOOST_timer--;timer_Correction_Boost++;
               }
               else
               {
                   stop_timer_Boost();
                   timer_Correction_Boost = 0;
               }
           }
           else if ( timer_Correction_Boost > 0 )//increase
           {
               if ( BOOST_timer < (FREQUENCY - DeadTime))
               {
                   BOOST_timer++;
                   timer_Correction_Boost--;
               }
               else timer_Correction_Boost = 0;
           }
           else
           {
               //nothing to do, Misra
           }
       }
       TA1CCTL1 &= ~TA1IV_TACCR2;
    }
//    else if (TA1IV & TA1IV_TAIFG)
//     {
//         counterTimer++;
//     }
    TA1CCTL0 &= ~CCIFG;
}
bool ms10TaskOcured = 0,ms100TaskOcured = 0,secTaskOcured = 0,minTaskOcured = 0, hourTaskOcured = 0 ;
volatile uint16 counterWd=0;
#pragma vector = WDT_VECTOR  //priority 26; Interval timer vector location 1 744 186 times 1 h, 29 070 - 1 min, 485 - 1s
__interrupt void WatchDogTimer(void)//2.064ms Task
{
    counterWd++;
}
#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer0_A0 (void)//25 priority
{

}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)//24 priority
{
//    /*uint16*/TA0CCR1_local = TA0CCR1;
//    uint16 TA0CCR2_local = TA0CCR2;
//    uint16 TA0IV_local = TA0IV;
//    if(TA0IV_local & TA0IV_TACCR1)// CCR1
//    {
//        TA0CCTL1 &= ~CCIFG;
//    }
//    if (TA0IV_local & TA0IV_TACCR2)    // CCR2
//    {   if ( timer_Correction_Boost < 0 )//decrease
//        {
//            if ( TA0CCR1_local > DeadTime )
//            {
//                TA0CCR1--;
//            }
//            else if (timer_Boost_state)
//            {
//                stop_timer_Boost();
//                best_steep_MPPT = TA1CCR1;
//            }
////            start_adc();
//            timer_Correction_Boost++;
//        }
//        else if ( timer_Correction_Boost > 0 )//increase
//        {
//            if ( TA0CCR1_local < (FREQUENCY - DeadTime) )
//            {
//                TA0CCR1++;
//            }
////            start_adc();
//            timer_Correction_Boost--;
//        }
//        else
//        {
//        }//nothing to do
//        if (TA0CCR2_local > (FREQUENCY>>1)) TA0CCR2 = FREQUENCY>>4;
//        else TA0CCR2 = FREQUENCY-(FREQUENCY>>4);
//        TA0CCTL2 &= ~CCIFG;
//    }
    if (TA0IV & TA0IV_TACCR1)
     {
        if (brake_state == remove_brake_in_progress || brake_state == brake_in_progress)
            {
                BRAKE_TIMER_PWM += brake_correction;
                brake_correction -= brake_correction;
            }
            TA0CCTL0 &= ~CCIFG;
     }
     counterTimer++;
//    TA0CTL &= ~TAIFG;
//    __enable_interrupt();
}
#ifdef __MSP430G2955__

#pragma vector=TIMER0_B1_VECTOR //timer
__interrupt void Timer_B1 (void)
{
    switch(TA1IV)
    {
     case 2:    // CCR1
     break;
     case 4:    // CCR2
     break;
     case TA1IV_TAIFG:
     {
        if ( timer_Correction_Boost < 0 )//decrease
        {
          if ( TB0CCR2 > DeadTime )
          {
              TB0CCR2--;
          }
          timer_Correction_Boost++;
        }
        else if ( timer_Correction_Boost > 0 )//increase
        {
          if ( TB0CCR2 < (TB0CCR2 - DeadTime) )
          {
              TB0CCR2++;
          }
          timer_Correction_Boost--;
        }
        else
        {
        }//nothing to do
     }
     break;
     case 14:
     break;
     default:   //default :)
     break;
    }
    TA1CTL &= ~TAIFG;
    __enable_interrupt();
}
#endif

#pragma vector = PORT1_VECTOR//18
__interrupt void port1_isr(void)
{



//    __enable_interrupt();
}
#pragma vector = PORT2_VECTOR//19
__interrupt void port2_isr(void)
{
    if (P2IFG & ENCODER_LEFT)
    {
        if (!(P2IN & ENCODER_RIGHT))
        {
            encoderState |= right;
//            if (MPPT_precedent_state != NO_PWM)MPPT_set(MPPT_limit);
        }
        else
        {

            encoderState |= left;
        }
        P2IFG &= ~ENCODER_LEFT;
    }
    else if (P2IFG & ENCODER_OK)
    {
        encoderState |= OK;
//        if (MPPT_precedent_state != NO_PWM) MPPT_set(NO_PWM);
//        else MPPT_set(startTimer);
        P2IFG &= ~ENCODER_OK;
    }
    else if ( P2IFG & rotation_read_pin )
    {
        interrupt_routin();
        P2IFG &= ~rotation_read_pin;
    }
//	__enable_interrupt();
}

//#pragma vector = USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//{
//    switch (UCA0IV)
//    {
//    case 0x06:
//        {
//            Uart_TX_Interrupt();
//            UCA0IFG &= ~UCTXIFG; // Vector 6: UCTXIFG
//            break;
//        }
//    case 0x08:
//        {
//            Uart_RX_Interrupt();
//            UCA0IFG &= ~UCRXIFG; // Vector 8: UCRXIFG
//            break;
//        }
//    default: break;
//    }
//	if (UCA0IV & UCTXIFG) {I2C_RX_Interrupt();IFG2 &= ~UCB0RXIFG;}
//	if (IFG2 & UCB0TXIFG) {I2C_TX_Interrupt();IFG2 &= ~UCB0TXIFG;}
//	if (UCA0IFG & UCTXIFG)
//	{
//	    UCA0IFG &= ~UCTXIFG;
//	    Uart_TX_Interrupt();
//
//	}
//	if (UCA0IFG & UCRXIFG)
//	{
//	    UCA0IFG &= ~UCRXIFG;
//	    Uart_RX_Interrupt();
//	}
//	//__bis_SR_register(GIE);        // interrupts
//}
//#pragma vector = USCI_B0_VECTOR
//__interrupt void USCI_B0_ISR(void)
//{
////    if (UCB0IFG & UCTXIFG) SPI_Interrupt();
//#ifdef __MSP430G2955__
//    if (IFG2 &= ~UCB0TXIFG) SPI_Interrupt();
//#endif
//}
//#pragma vector=SD24_VECTOR
//  __interrupt void SD24AISR(void)
//{
////      volatile int16 tmpValue;
////      switch (SD24IV)
////        {
////        case 2:                                                 // SD24MEM Overflow
////            break;
////        case 4:                                                 // SD24MEM0 IFG
////            tmpValue = SD24MEM0;
////
////            if ( last_good_SD24MEM_panel_V ) {last_good_SD24MEM_panel_V += tmpValue;last_good_SD24MEM_panel_V =last_good_SD24MEM_panel_V>>1;}
////            else last_good_SD24MEM_panel_V = tmpValue;
////
////            NewAdcValueGot |= V_BATT_ADC_GOT;
////            break;
////        case 6:                                                 // SD24MEM1 IFG
////            tmpValue = SD24MEM1;
////
////            if ( last_good_SD24MEM_battery_V ) {last_good_SD24MEM_battery_V += tmpValue;last_good_SD24MEM_battery_V = last_good_SD24MEM_battery_V>>1;}
////            else last_good_SD24MEM_battery_V = tmpValue;
////
////            NewAdcValueGot |= V_PANEL_SOLAR_GOT;
////            break;
////        case 8:                                                 // SD24MEM2 IFG
////            tmpValue = SD24MEM2;
////
////            if (last_good_SD24MEM_panel_A) {last_good_SD24MEM_panel_A += tmpValue;last_good_SD24MEM_panel_A>>=1;}
////            else last_good_SD24MEM_panel_A = tmpValue;
////
////            NewAdcValueGot |= A_PANEL_SOLAR_GOT;
////            break;
////        case 10:                                                // SD24MEM3 IFG
////            tmpValue = SD24MEM3;
////            tmpValue*=(-1);
////            if ( last_good_SD24MEM_battery_A ) {last_good_SD24MEM_battery_A += tmpValue;last_good_SD24MEM_battery_A>>=1;}
////            else last_good_SD24MEM_battery_A = tmpValue;
////
////            NewAdcValueGot |= A_BATT_ADC_GOT;
////            break;
////        }
//}

