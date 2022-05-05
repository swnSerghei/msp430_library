#include "boost.h"
uint32 boost_voltage = 0;
void Boost_voltage()
{
    uint32 tmpValue;
    tmpValue = ADC_rawData_filtered[0];
    tmpValue = ( (tmpValue*uVs_per_1ADC)/1000 );

    tmpValue = ((uint32)BOOST_R1+(uint32)BOOST_R2)*(uint32)tmpValue/(uint32)BOOST_R2;//Vin mV

    if (!boost_voltage) boost_voltage+=tmpValue;
    else
    {
        boost_voltage+=tmpValue;
        boost_voltage/=2;
    }
}
volatile int16 timer_Correction_Boost = 0;
void boost_Voltage_test()
{
//    if ( start_timer_Boost == start )
//    {
//        if (boost_voltage > (BOOST_VOLTAGE + BOOST_VOLTAGE_TREESHOLD) )
//        {
//            timer_Correction_Boost = (-1);
//        }
////        else if (boost_voltage < (BOOST_VOLTAGE - BOOST_VOLTAGE_TREESHOLD))
////        {
////            timer_Correction_Boost = 1;
////        }
//        else {/*stay here, nothing to do*/}
//    }
}
void configure_timer_Boost()
{
#ifdef __MSP430G2553__
    P3DIR |= (PWM_BOOST + ACTIVATE_LOAD1 + ACTIVATE_LOAD2) ;
    P2DIR &= ~SD_BOOST;//input
    stop_timer_Boost();
#endif
}
uint8 timer_Boost_state = stop;
void stop_timer_Boost()
{
#ifdef __MSP430G2955__
    TA0CCTL2 = OUTMOD_5;

    TA0CCR0 = FREQUENCY;
    TA1CCR1 = DeadTime;//minimum frequency start

    P1OUT &= ~(PWM_PANEL + SD_PANEL);
    P1DIR |= PWM_PANEL + SD_PANEL;

    P1SEL |= PWM_PANEL;

    TA0CTL &= ~TAIFG;
    //        SMCLK      /1   stop   clr
    TA0CTL = TASSEL_2 + ID_0 + MC_0 + TACLR;

#endif
#ifdef __MSP430G2553__
    TA1CCTL2 = OUTMOD_0;
    P3SEL &= ~PWM_BOOST;
    P3OUT &= ~PWM_BOOST;
    P3OUT &= ~ACTIVATE_LOAD1;
    P3OUT &= ~ACTIVATE_LOAD2;
    if ( !(TA1CCTL2 & CCIE) ) TA1CTL |= TAIE;
#endif
    timer_Boost_state = stop;
}
void start_timer_Boost()
{
#ifdef __MSP430G2955__
    TA1CCR1 = DeadTime;//minimum frequency start
//    TA1CCR1 = TA0CCR0 - DeadTime;
    P1SEL |= PWM_PANEL;
    P1OUT |= SD_PANEL;

    TA0CCTL2 = OUTMOD_7;//Set/Rese
    TA0CTL &= ~TAIFG;
    //        SMCLK      /1   UP_CCR0
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
//                TA0CTL = TASSEL_2 + ID_0 + MC_1;
#endif
#ifdef __MSP430G2553__
    timer_Correction_Boost = 0;
    P3OUT |= ACTIVATE_LOAD1;
    P3SEL |= PWM_BOOST;
    TA1CCR2 = DeadTime;//minimum frequency start
    TA1CCTL2 = OUTMOD_7+CCIE;//Set/Rese
    if (TA1CTL & TAIE) TA1CTL &= ~TAIE;
#endif
    timer_Boost_state = start;
}
