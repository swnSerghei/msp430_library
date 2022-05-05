/*
 * solar_panel_functions.h
 *
 *  Created on: Aug 1, 2018
 *      Author: uidg6243
 */

#ifndef LIBRARY_SOLAR_PANEL_FUNCTIONS_H_
#define LIBRARY_SOLAR_PANEL_FUNCTIONS_H_
#include "adc.h"


extern uint32 SolarPanel_V,SolarPanel_V_Raw,SolarPanel_A,SolarPanel_A_Raw;

#define IN_R1 120000
#define IN_R2 2000  //decrease for increase voltage

#define HIGH_treeshold_to_activate_PWM 3000        //start to do PWM more 3000mV
#define HIGH_treeshold_to_activate_PWM_ADCs   ((uint32)HIGH_treeshold_to_activate_PWM*(uint32)IN_R2)/((uint32)IN_R1+(uint32)IN_R2)*(uint32)1000/(uint32)uVs_per_1ADC
#define LOW_treeshold_to_deactivate_PWM 1000        //stop to do PWM less 1000mV
#define LOW_treeshold_to_deactivate_PWM_ADCs ((uint32)LOW_treeshold_to_deactivate_PWM*(uint32)IN_R2)/((uint32)IN_R1+(uint32)IN_R2)*(uint32)1000/(uint32)uVs_per_1ADC

//#define ACTIVATE_GND_PANEL_SOLAR    BIT0//from port 2
#define HIGH_SIDE_MPPT              BIT2//port 2
#define LOW_SIDE_MPPT               BIT1//port 2

void solar_panel_V();
void solar_panel_A();
void calibrateZeroADC_panel();

#endif /* LIBRARY_SOLAR_PANEL_FUNCTIONS_H_ */
