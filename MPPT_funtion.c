/*
 * MPPT_funtion.c
 *
 *  Created on: Aug 2, 2018
 *      Author: uidg6243
 */
#include "MPPT_function.h"
#include "PCD8544.h"

#define HowManySteepsCheckMPPT 400
#define BOOST_implementation 0
int16 best_steep_MPPT = 0;

uint8  CheckMPPT_timer = 0;

volatile Buck_state timer_Buck_state = stop;
void start_timer_Buck()
{
#ifdef __MSP430G2955__
    TA1CCR1 = DeadTime;//minimum frequency start
//    TA1CCR1 = TA0CCR0 - DeadTime;
    P1SEL |= PWM_BUCK;
    P1OUT |= SD_BUCK;

    TA0CCTL2 = OUTMOD_7;//Set/Rese
    TA0CTL &= ~TAIFG;
    //        SMCLK      /1   UP_CCR0
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
//                TA0CTL = TASSEL_2 + ID_0 + MC_1;
#endif
#ifdef __MSP430G2553__
    MPPT_correction = 0;
    TA1CCR1 = DeadTime;//minimum frequency start
    activate_Buck;
    P3SEL |= PWM_BUCK;
    TA1CCTL1 = OUTMOD_7+CCIE;//Set/Reset
    TA1CTL &= ~TAIE;
#endif
    timer_Buck_state = start;
}
void configure_timer_Buck()
{
#ifdef __MSP430G2553__
    TA1CCR0 = FREQUENCY;
    P3DIR |= (PWM_BUCK + SD_BUCK);
    P3SEL &= ~PWM_BUCK;
    //        SMCLK      /1   Up to CCR0   clr
    TA1CTL = TASSEL_2 + ID_0 + MC_1 + TACLR + TAIE;

    TA1CCTL1  = OUTMOD_0;
    P3SEL &= ~PWM_BUCK;
    P3OUT &= ~PWM_BUCK;
    MPPT_correction=0;
    deactivate_Buck;
    timer_Buck_state = stop;

#endif
}
void stop_timer_Buck()
{
#ifdef __MSP430G2955__
    TA0CCTL2 = OUTMOD_5;

    TA0CCR0 = FREQUENCY;
    TA1CCR1 = DeadTime;//minimum frequency start

    P1OUT &= ~(PWM_BUCK + SD_BUCK);
    P1DIR |= PWM_BUCK + SD_BUCK;

    P1SEL |= PWM_BUCK;

    TA0CTL &= ~TAIFG;
    //        SMCLK      /1   stop   clr
    TA0CTL = TASSEL_2 + ID_0 + MC_0 + TACLR;

#endif
#ifdef __MSP430G2553__
    if ( (BUCK_timer <= DeadTime) && (timer_Buck_state == stop_in_progress) )
    {
        TA1CCTL1  = OUTMOD_0;
        P3SEL &= ~PWM_BUCK;
        P3OUT &= ~PWM_BUCK;
        //deactivate_Buck;
        timer_Buck_state = stop;
        TA1CTL |= TAIE;
    }
    else if( timer_Buck_state == start )
    {
        timer_Buck_state = stop_in_progress;
    }
#endif
}

volatile int16 MPPT_correction = 0;
int16 DebugValue=0;
uint16 steepsForTrack_MPPT=0;
bool inWichDirectionCheckingMPPT = 1;
int32 mWats_Input_best,mWats_Input;
Batt_states batteryStatus;
volatile uint8 Boost_max_Voltage = 0;
void calculate_MPPT()
{
    const int16 delta_mW = 20000;//20W

    if ( timer_Buck_state == stop && ( SolarPanel_V > (HIGH_treeshold_to_activate_PWM + Battery_V)) )
    {
        start_timer_Buck();
        best_steep_MPPT = 0;
        steepsForTrack_MPPT = HowManySteepsCheckMPPT;
        inWichDirectionCheckingMPPT = 1;
        MPPT_correction = 0;
        CheckMPPT_timer = 0;
        mWats_Input_best = 0;
    }
    else if ( timer_Buck_state == start && (SolarPanel_V < (LOW_treeshold_to_deactivate_PWM + Battery_V)) )
    {
        stop_timer_Buck();
        stop_timer_Boost();
    }
    else if (timer_Buck_state == start)
    {
            mWats_Input = (SolarPanel_V * SolarPanel_A)/1000;
            batteryStatus = battery_check();
            if ( batteryStatus == OverVoltage || batteryStatus == OverCurrent
#if BOOST_implementation == 1
                    || boost_voltage > BOOST_VOLTAGE
#endif
                )
            {
#if BOOST_implementation == 1
                if ( boost_voltage > BOOST_VOLTAGE ) timer_Correction_Boost = (-1);
                if ( batteryStatus == OverVoltage || batteryStatus == OverCurrent )
#endif
                MPPT_correction  = (-1);
                mWats_Input_best = mWats_Input;
                inWichDirectionCheckingMPPT = 1;
                steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                CheckMPPT_timer = 0;
                DebugValue = 30;
            }
            else if ( CheckMPPT_timer == 0 && MPPT_correction == 0 && timer_Correction_Boost == 0)   //battery Is OK and if timer expired for check MPPT, MPPT_correction/Boost means the timer update the correction ==> DO IT!!
            {
                mWats_Input = (SolarPanel_V * SolarPanel_A)/1000;
#if BOOST_implementation == 1
                if ( (batteryStatus == Hysteresis_zone) && (timer_Boost_state == stop) )
                    {
                        start_timer_Boost();
                        best_steep_MPPT = TA0CCR1;
                    }
#endif
//                mWats_Input = (SolarPanel_V * SolarPanel_A)/1000;

                if ( inWichDirectionCheckingMPPT )//increasing check
                {
                    if ( (mWats_Input - mWats_Input_best) >= delta_mW )
                    {
                        mWats_Input_best = mWats_Input;
                        if ( batteryStatus == Hysteresis_zone )
                        {
#if BOOST_implementation == 1
                            if (timer_Boost_state)
                            {
                                if ( boost_voltage < (BOOST_VOLTAGE - BOOST_VOLTAGE_TREESHOLD) )
                                {
                                    timer_Correction_Boost = 1;
                                }
                                best_steep_MPPT = TA0CCR1;
                            }
#endif
                        }
                        else
                        {
#if BOOST_implementation == 1
                            if ( timer_Boost_state )
                            {

                                best_steep_MPPT = TA0CCR1;
                            }
                            else
#endif
                            {
                                best_steep_MPPT = TA1CCR1;

                            }
                            MPPT_correction = 1;
                        }
                        steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                        DebugValue = 11;
                    }
                    else if ( (mWats_Input_best - mWats_Input) >= delta_mW )
                    {
                        if ( timer_Boost_state )
                        {
#if BOOST_implementation == 1
                            timer_Correction_Boost = best_steep_MPPT - TA0CCR1;
#endif
                        }
                        else
                        {
                            MPPT_correction = best_steep_MPPT - TA1CCR1;
                        }
                    mWats_Input_best = mWats_Input;
                    inWichDirectionCheckingMPPT = 0;
                    steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                    DebugValue = 12;
                    }
                    else
                    {
                        steepsForTrack_MPPT--;
                        if ( steepsForTrack_MPPT == 0 )
                        {
                            if ( timer_Boost_state )
                            {
    #if BOOST_implementation == 1

                                timer_Correction_Boost = best_steep_MPPT - TA0CCR1;
    #endif
                            }
                            else
                            {
                                MPPT_correction = best_steep_MPPT - TA1CCR1;
                            }
                            inWichDirectionCheckingMPPT = 0;
                            steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                        }
                        else
                        {
                            if ( batteryStatus == Hysteresis_zone )
                            {
#if BOOST_implementation == 1
                                if ( boost_voltage < (BOOST_VOLTAGE - BOOST_VOLTAGE_TREESHOLD) )
                                {
                                    timer_Correction_Boost = 1;
                                }
#endif
                            }
                            else
                            {
                                MPPT_correction = 1;
                                //if (timer_Boost_state) timer_Correction_Boost = -1;
                            }
                        }
                        DebugValue = 13;
                    }
                }
                else //decreasing check
                {
                    if ( (mWats_Input - mWats_Input_best) >= delta_mW )
                    {
                        mWats_Input_best = mWats_Input;
                        if ( timer_Boost_state )
                        {
#if BOOST_implementation == 1
                            best_steep_MPPT = TA0CCR1;
                            timer_Correction_Boost = (-1);
#endif
                        }
                        else
                        {
                            best_steep_MPPT = TA1CCR1;
                            MPPT_correction = (-1);
                        }
//                       steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                       DebugValue = 21;
                    }
                    else if ( (mWats_Input_best - mWats_Input) >= delta_mW )//no sense to continue to decrement
                    {
                        if ( timer_Boost_state )
                        {
#if BOOST_implementation == 1
                            timer_Correction_Boost = best_steep_MPPT - TA0CCR1;
#endif
                        }
                        else
                        {
                           // MPPT_correction = TA1CCR1 - best_steep_MPPT;
                            MPPT_correction = best_steep_MPPT - TA1CCR1;
                        }
                        inWichDirectionCheckingMPPT = 1;
                        CheckMPPT_timer = CheckMPPT_How_Offen;                                     //just after down side to reset timer for track MPPT
                        steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                        mWats_Input_best = 0;
                        DebugValue = 22;
                    }
                    else
                    {
                        steepsForTrack_MPPT--;
                        if ( steepsForTrack_MPPT == 0 )
                        {
#if BOOST_implementation == 1
                            if ( timer_Boost_state )
                           {
                                timer_Correction_Boost = best_steep_MPPT - TA0CCR1;
                           }
                            else
#endif
                            {
                                MPPT_correction = best_steep_MPPT - TA1CCR1;
                            }
                            inWichDirectionCheckingMPPT = 0;
                            steepsForTrack_MPPT = HowManySteepsCheckMPPT;
                            CheckMPPT_timer = CheckMPPT_How_Offen;                                     //just after down side to reset timer for track MPPT
                        }
                        else
                        {
                            if ( batteryStatus == Hysteresis_zone )
                            {
#if BOOST_implementation == 1
                                if (timer_Boost_state) timer_Correction_Boost = (-1);
#endif
                            }
                            else
                            {
                                if (timer_Boost_state)
                                {
                                    MPPT_correction = 1;
                                    timer_Correction_Boost = (-1);
                                }
                                else
                                {
                                    MPPT_correction = (-1);
                                }
                            }
                        }
                       DebugValue = 23;
                    }
                }
            }
        }
}
//void calculate_MPPT_boost()
//{
//    const int16 delta_mW = 5000;
//    static uint16 steepsForTrack_MPPT_Boost=0;
//    static bool inWichDirectionCheckingMPPT_Boost = 1;
//    static int32 mWats_Input_best_ADCs = 0,mWats_Input_ADCs;
//
//
//    if ( SolarPanel_V > 55000 )
//    {
//        if ( timer_Boost_state == stop)
//            {
//                start_timer_Boost();
//                inWichDirectionCheckingMPPT_Boost = 1;
//                timer_Correction_Boost = 0;
//                CheckMPPT_timer = 0;
//                mWats_Input_best_ADCs = 0;
//            }
//    }
//    else if (timer_Boost_state == start ) stop_timer_Boost();
//
//    if (timer_Boost_state == start && CheckMPPT_timer == 0 && timer_Correction_Boost == 0 )   //battery Is OK and if timer expired for check MPPT, timer_Correction_Boost means the timer update the correction ==> DO IT!!
//    {
//        mWats_Input_ADCs = ADC_rawData_filtered[V_IN] * ADC_rawData_filtered[A_IN];    //mW
//
//        if ((WindTurbine_V > MAX_BOOST_VOLTAGE + Hysteresis_boost))
//        {
//            mWats_Input_best_ADCs = mWats_Input_ADCs;
//            inWichDirectionCheckingMPPT_Boost = 1;
//            steepsForTrack_MPPT_Boost = HowManySteepsCheckMPPT;
//            MPPT_correction  = (-1);
//            CheckMPPT_timer = 0;
//        }
//        else
//        {
//            if (steepsForTrack_MPPT_Boost == HowManySteepsCheckMPPT) mWats_Input_best_ADCs = mWats_Input_ADCs;
//
//            if ( inWichDirectionCheckingMPPT_Boost )//increasing check
//            {
//                if ( mWats_Input_ADCs > mWats_Input_best_ADCs )
//                {
//                    best_steep_MPPT = TA1CCR2;
//                    mWats_Input_best_ADCs = mWats_Input_ADCs;
//                    timer_Correction_Boost = 1;
//                    steepsForTrack_MPPT_Boost = (HowManySteepsCheckMPPT - 1);
//                    DebugValue = 11;
//                }
//                else if ( mWats_Input_ADCs < (mWats_Input_best_ADCs - delta_mW ) )
//                {
//                    timer_Correction_Boost = best_steep_MPPT - TA1CCR2;
//                    inWichDirectionCheckingMPPT_Boost = 0;
//                    steepsForTrack_MPPT_Boost = HowManySteepsCheckMPPT;
//                    DebugValue = 12;
//                }
//                else
//                {
//                    steepsForTrack_MPPT_Boost--;
//                    if (steepsForTrack_MPPT_Boost==0)
//                    {
//                        timer_Correction_Boost = best_steep_MPPT - TA1CCR2;
//                        inWichDirectionCheckingMPPT_Boost = 0;
//                        steepsForTrack_MPPT_Boost = HowManySteepsCheckMPPT;
//                    }
//                    else
//                    {
//                        timer_Correction_Boost = 1;
//                    }
//                    DebugValue = 13;
//                }
//            }
//            else //decreasing check
//            {
//                if ( mWats_Input_ADCs > mWats_Input_best_ADCs )
//                {
//                   best_steep_MPPT = TA1CCR2;
//                   mWats_Input_best_ADCs = mWats_Input_ADCs;
//                   timer_Correction_Boost = (-1);
//                   steepsForTrack_MPPT_Boost = (HowManySteepsCheckMPPT - 1);
//                   DebugValue = 21;
//                }
//                else if ( (mWats_Input_ADCs < (mWats_Input_best_ADCs - delta_mW )) || (TA1CCR2 <= DeadTime) )//no sense to continue to decrement
//                {
//                    timer_Correction_Boost = TA1CCR2 - best_steep_MPPT;
//                    inWichDirectionCheckingMPPT_Boost = 1;
//                    steepsForTrack_MPPT_Boost = HowManySteepsCheckMPPT;
//                    DebugValue = 22;
//                }
//                else
//                {
//                    steepsForTrack_MPPT_Boost--;
//                   if (steepsForTrack_MPPT_Boost==0)
//                   {
//                       timer_Correction_Boost = TA1CCR2 - best_steep_MPPT;
//                       inWichDirectionCheckingMPPT_Boost = 1;
//                       CheckMPPT_timer = CheckMPPT_How_Offen;                                     //just after down side to reset timer for track MPPT
//                       steepsForTrack_MPPT_Boost = HowManySteepsCheckMPPT;
//                   }
//                   else
//                   {
//                       timer_Correction_Boost = (-1);
//                   }
//                   DebugValue = 23;
//                }
//
//            }
//        }
//    }
//}
