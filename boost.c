#include "boost.h"
uint32 boost_voltage = 0;
void Boost_voltage()
{
    uint32 tmpValue;
    tmpValue = ADC_rawData_filtered[V_boost];
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
    TA0CCTL1 = OUTMOD_5;

    TA0CCR0 = FREQUENCY;
    TA0CCR1 = DeadTime;//minimum frequency start

    P2DIR |= (PWM_BOOST + SD_BOOST + ACTIVATE_LOAD1 + ACTIVATE_LOAD2) ;

    TA0CTL &= ~TAIFG;
    //TA0CCTL1 &= ~CCIE;
    TA0CCTL2 &= ~CCIE;
    //        SMCLK      /1   stop   clr
    TA0CTL = TASSEL_2 + ID_0 + MC_0 + TACLR;
    P2SEL &= ~PWM_BOOST;
    P2OUT &= ~PWM_BOOST;
    P2OUT &= ~SD_BOOST;
    P2OUT &= ~ACTIVATE_LOAD1;
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
    P2OUT |= ACTIVATE_LOAD1;
    TA0CCR1 = DeadTime;//minimum frequency start
    TA0CCR2 = FREQUENCY-(FREQUENCY>>4);
    P2SEL |= PWM_BOOST;
    TA0CCTL1 = OUTMOD_7;//Set/Rese
    TA0CTL &= ~TAIFG;
    //        SMCLK      /1   UP_CCR0
    TA0CCTL1 |= CCIE;
    TA0CCTL2 |= CCIE;
    TA0CTL = TASSEL_2 + ID_0 + MC_1;// + TAIE;
    P2OUT |= SD_BOOST;
#endif
    timer_Boost_state = start;
}
