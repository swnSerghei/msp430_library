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
enum ADC_mapping//mapping to A0..Ax
{
    A_IN=INCH_4,
    V_battery=INCH_3,
    V_boost = INCH_0,
    A_battery=1,
    V_IN=1
};

#define uVs_per_1ADC            2455
#define mAs_per1ADC_battery     191
#define mAs_per1ADC_panel       41//88
#define mAs_per1ADC_windTurbine 260//???? need to calibrate
#define ADC_CHANNELS 3

#define HowManyAVG_samples 16//change in code how many times to shift to right, if 2 move 1, 4 -> 2, 8 -> 3, 16 -> 4, 32 -> 5,      3mS For 16

extern int32  last_good_MEM_panel_V,last_good_MEM_panel_A,last_good_MEM_battery_V,last_good_MEM_battery_A,last_good_MEM_Boost_V;
extern int32 eroor_SD24MEM_battery_A,eroor_SD24MEM_panel_A;
extern bool ADC10_interrupt_Ocured;
extern uint8 counter_ADC_samples;
struct ADC_values
{
//    unsigned int currentSense_PanelSolar_raw; //ADC6
//    unsigned int temperature_Battery_raw;     //ADC5
//    unsigned int currentSense_Battery_raw;    //ADC4
//    unsigned int voltageBattery_raw;          //ADC3
//    unsigned int voltagePanelSolar_raw;       //ADC2
//    unsigned int currentSense_Generator_raw;  //ADC1
//    unsigned int voltageGenerator_raw;        //ADC0
};
extern int ADC_rawData[HowManyAVG_samples];
int ADC_rawData_filtered[ADC_CHANNELS];
extern bool filtered_ADCvalue_available;

void start_adc();
void init_adc();
void interrupt_ADC();

#endif /* ADC_H_ */
