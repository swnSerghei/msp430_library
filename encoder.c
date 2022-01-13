/*
 * encoder.c
 *
 *  Created on: Feb 5, 2020
 *      Author: uidg6243
 */
#include "encoder.h"
bool manual_Test_Started = 0;
void interruptEncoder(encoderStates encoderState)
{
//    if (encoderState == left)       {/*tmpCounter--;*/decreaseTimer_windTurbine();}
//    else if (encoderState == right) {/*tmpCounter++;*/increaseTimer_windTurbine();}
    //else if (encoderState == OK)    {/*tmpCounter = 0;*/if(timer_Boost_state)stop_timer_Boost(); else start_timer_Boost();}
    if (encoderState == left)       {if (manual_Test_Started) timer_Correction_Buck= -1;}
    else if (encoderState == right) {if (manual_Test_Started) timer_Correction_Buck=1;}
    else if (encoderState == OK)
    {
        if ( manual_Test_Started == 0 )
        {
            manual_Test_Started = 1;
            if( !timer_Buck_state ) start_timer_Buck();
        }
        else
        {
            manual_Test_Started = 0;
            if( timer_Buck_state ) stop_timer_Buck();
        }
    }
    else {/*MISRA*/}
}
void initEncoder()
{
    P1IFG &= ~ (ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK);
    P1IES &= ~ (ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK);
    P1IE |= ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK;
}
