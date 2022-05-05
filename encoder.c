/*
 * encoder.c
 *
 *  Created on: Feb 5, 2020
 *      Author: uidg6243
 */
#include "encoder.h"
bool manual_Test_Started = 0;
int8 tmpCounter_encoder = 0;
uint8 encoderState = 0;
void Encoder_Check()
{
    if (encoderState & left)
    {
        tmpCounter_encoder--;
        MPPT_correction-=10;;
        timer_Correction_Boost-=1;
        encoderState &= ~left;
        remove_breake();
    }
    if (encoderState & right)
    {
        tmpCounter_encoder++;
        MPPT_correction +=10;
        timer_Correction_Boost+=1;
        encoderState &= ~right;

        put_brake();
    }
    if (encoderState == OK)
    {
        tmpCounter_encoder ++;
        //if (timer_Buck_state == 0)
        if (timer_Boost_state == start)
        //if ( (brake_state == remove_brake_in_progress) || (brake_state == no_brake) )
        {
            //stop_timer_Buck();
            stop_timer_Boost();
            //put_brake();
        }
        else
        {
            //start_timer_Buck();
            start_timer_Boost();
            //remove_breake();
        }
        encoderState &= ~OK;
    }
//    if (encoderState == left)       {if (manual_Test_Started) timer_Correction_Buck= -1;}
//    else if (encoderState == right) {if (manual_Test_Started) timer_Correction_Buck=1;}
//    else if (encoderState == OK)
//    {
//        if ( manual_Test_Started == 0 )
//        {
//            manual_Test_Started = 1;
//            if( !timer_Buck_state ) start_timer_Buck();
//        }
//        else
//        {
//            manual_Test_Started = 0;
//            if( timer_Buck_state ) stop_timer_Buck();
//        }
//    }
    else {/*MISRA*/}
}
void initEncoder()
{
    P2DIR &= ~ (ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK);
    //pull up
//    P2OUT |=   (ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK);
//    P2REN |=   (ENCODER_LEFT + ENCODER_RIGHT + ENCODER_OK);
    //
    P2IFG &= ~ (ENCODER_LEFT + ENCODER_OK);
    P2IES |=   (ENCODER_LEFT + ENCODER_OK);
    P2IE |= ENCODER_LEFT + ENCODER_OK;
}
