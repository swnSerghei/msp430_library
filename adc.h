/*
 * adc.h
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */

#ifndef ADC_H_
#define ADC_H_
#include "commontypes.h"
#include <msp430.h>
#include "battery_functions.h"

#define uVs_per_1ADC            2455
#define mAs_per1ADC_battery     191
#define mAs_per1ADC_panel       41//88
#define mAs_per1ADC_windTurbine 260//???? need to calibrate
#define ADC_CHANNELS 3
#define ADC_EnableConversion_StartConversion ADC10CTL0 |= ENC + ADC10SC

#define HowManyAVG_samples 16//change in code how many times to shift to right, if 2 move 1, 4 -> 2, 8 -> 3, 16 -> 4, 32 -> 5,      3mS For 16

extern int32  last_good_MEM_panel_V,last_good_MEM_panel_A,last_good_MEM_battery_V,last_good_MEM_battery_A,last_good_MEM_Boost_V;
extern int32 eroor_SD24MEM_battery_A,eroor_SD24MEM_panel_A;
int ADC_rawData_filtered[ADC_CHANNELS];
extern bool filtered_ADCvalue_available;

void start_adc();
void init_adc();
void check_interrupt_ADC();

#endif /* ADC_H_ */
