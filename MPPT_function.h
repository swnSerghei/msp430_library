/*
 * MPPT_function.h
 *
 *  Created on: Aug 2, 2018
 *      Author: uidg6243
 */

#ifndef MPPT_FUNCTION_H_
#define MPPT_FUNCTION_H_

#include <msp430.h>
#include "adc.h"
#include "solar_panel_functions.h"
#include "battery_functions.h"
#include "commontypes.h"
#include "printf.h"

#define PWM_BUCK    BIT2
#define SD_BUCK    BIT1
#define deactivate_Buck P3OUT &= ~SD_BUCK
#define activate_Buck P3OUT |= SD_BUCK
#define BUCK_timer TA1CCR1

#define FREQUENCY 800 //20khz
#define DeadTime  15 //deadTime

#define CheckMPPT_How_Offen 3   //each x seconds start tracking MPPT

extern int16 best_steep_MPPT;

extern uint8 CheckMPPT_timer;
void calculate_MPPT();
void calculate_MPPT_boost();
extern void start_timer_Buck();
extern void stop_timer_Buck();
extern void configure_timer_Buck();
extern uint8 FlagToActivateLowSide;
extern volatile int16 MPPT_correction;
extern volatile Buck_state timer_Buck_state;
extern int16 DebugValue;
extern uint16 steepsForTrack_MPPT;
extern bool inWichDirectionCheckingMPPT;
extern int32 solarPanel_mWats_best,mWats_Input;
extern Batt_states batteryStatus;
extern volatile uint8 Boost_max_Voltage;
extern int32 mWats_Input_best,mWats_Input;
#endif /* MPPT_FUNCTION_H_ */

