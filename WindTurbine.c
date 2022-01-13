/*
 * WindTurbine.c
 *
 *  Created on: Mar 11, 2020
 *      Author: uidg6243
 */
#include "WindTurbine.h"

void windTurbine2()
{
    uint32 wind_V = SolarPanel_V;
//    if ( timer_Buck_state == stop && ( wind_V > (treeshold_to_activate_PWM_For_Battery + Battery_V)) ) start_timer_Buck();
//    else if ( timer_Buck_state == start && (wind_V < Battery_V) ) stop_timer_Buck();
    if ( timer_Buck_state == stop && ( wind_V > ((2*treeshold_to_activate_PWM_For_Battery) + Battery_V)) )
    {
        start_timer_Buck();
        DebugValue=1;
    }
    else if ( (timer_Buck_state == start || timer_Boost_state == start) && (wind_V < (Battery_V + treeshold_to_activate_PWM_For_Battery)) )
    {
        stop_timer_Buck();
        stop_timer_Boost();
        DebugValue=9;
    }
    if ( wind_V >= (activate_Break + treeshold_to_activate_PWM_For_Battery) )
    {
        P2OUT |= ACTIVATE_LOAD2;
    }
    else if ( (wind_V < (activate_Break)) && (P2OUT&ACTIVATE_LOAD2) )
    {
        start_timer_Boost();
        P2OUT &= ~ACTIVATE_LOAD2;
    }
}
void windTurbine()
{
    Batt_states batteryStatus;
    uint32 wind_V = SolarPanel_V;
//    if ( wind_V >= (activate_Break + treeshold_to_activate_PWM_For_Battery) )
//    {
//        P2OUT |= ACTIVATE_LOAD2;
//    }
//    else if ( (wind_V < (activate_Break)) && (P2OUT&ACTIVATE_LOAD2) )
//    {
//        start_timer_Boost();
//        P2OUT &= ~ACTIVATE_LOAD2;
//    }
    if ( timer_Buck_state == stop && ( wind_V > ((2*treeshold_to_activate_PWM_For_Battery) + Battery_V)) )
    {
        start_timer_Buck();
        DebugValue=1;
    }
    else if ( (timer_Buck_state == start || timer_Boost_state == start) && (wind_V < (Battery_V + treeshold_to_activate_PWM_For_Battery)) )
    {
        stop_timer_Buck();
        stop_timer_Boost();
        DebugValue=9;
    }
    else if (timer_Buck_state == start)
    {
        batteryStatus = battery_check();
        if ( batteryStatus == OverVoltage || batteryStatus == OverCurrent )
        {
            MPPT_correction  = (-1);
            DebugValue = 2;
        }
        else if (batteryStatus == under_Voltage_or_current)
        {
            MPPT_correction  = 1;
            DebugValue = 6;
        }
        else
        {
            //Hysteresis_zone, nothing to do
        }
        if ( (timer_Boost_state == start) && (boost_voltage > (uint32)BOOST_VOLTAGE) )
        {
            timer_Correction_Boost = (-1);
            DebugValue = 3;
        }
        else
        {
            if ( timer_Boost_state == start )
            {
              if ( wind_V >= activate_PWM_For_Boost ) timer_Correction_Boost = 1;
              else timer_Correction_Boost = (-1);
              DebugValue = 4;
            }
            else
            {
                if ( wind_V >= activate_PWM_For_Boost ) start_timer_Boost();
                DebugValue = 5;
            }
        }
    }
}
