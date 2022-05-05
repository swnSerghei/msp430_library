/*
 * battery_functions.h
 *
 *  Created on: Aug 1, 2018
 *      Author: uidg6243
 */

#ifndef BATTERY_FUNCTIONS_H_
#define BATTERY_FUNCTIONS_H_

#include "commontypes.h"
#include "adc.h"
#include "interrupts.h"


#define Battery_R1 62000
#define Battery_R2  2310

#define maximum_V_Battery_charging  54500    //2.4V/cell,14400 for 12V and 57600 for 48V
#define maximum_V_Battery_charging_In_ADCs  (int32)(((int32)(maximum_V_Battery_charging*(int32)Battery_R2)/((int32)Battery_R1+(int32)Battery_R2))*(int32)1000/(int32)uVs_per_1ADC)
#define V_Battery_floating          13620    //2.27V/cell
#define BatteryCapacity             400000

#define Midle_ADCs_of_zero_Amps     512
#define maximum_A_Battery_charging  20000    //60A
#define maximum_A_Battery_charging_IN_ADCs  (int32)maximum_A_Battery_charging/(int32)mAs_per1ADC_battery    //60A
#define HISTEREZIS_BATTERY_VOLTAGE  500     //0.5V
#define HISTEREZIS_BATTERY_VOLTAGE_ADCs  (((int32)HISTEREZIS_BATTERY_VOLTAGE*(int32)Battery_R2)/((int32)Battery_R1+(int32)Battery_R2))*(int32)1000/(int32)uVs_per_1ADC    //0.5V
#define HISTEREZIS_BATTERY_CHARGING 1000    //1A
#define HISTEREZIS_BATTERY_CHARGING_ADCs (((int32)HISTEREZIS_BATTERY_CHARGING*(int32)Battery_R2)/((int32)Battery_R1+(int32)Battery_R2))*(int32)1000/(int16)uVs_per_1ADC

extern uint32 Battery_V;
extern int32 Battery_A, Battery_A_ODO;
extern int64 Battery_A_Sum_everySeconds;
extern uint32 countsForAmpsBattery;
extern Batt_states battery_check();

void battery_V();
void battery_A();

extern void SetBatteryCapacity_1s (int32 batteryAmps);
extern int32 GetBatteryCapacity();

#endif /* LIBRARY_BATTERY_FUNCTIONS_H_ */
