/*
 * battery_functions.c
 *
 *  Created on: Aug 1, 2018
 *      Author: uidg6243
 */
#include "battery_functions.h"

uint32 Battery_V =0;
int32  Battery_A,Battery_A_ODO = 0;

Batt_states battery_check()
{
    Batt_states current_state = under_Voltage_or_current;
    static Batt_states precedent_state = under_Voltage_or_current;
    if (Battery_V > (maximum_V_Battery_charging + HISTEREZIS_BATTERY_VOLTAGE) )
    {
        current_state = OverVoltage;
        precedent_state =  current_state;
    }
    else if (Battery_A >= (maximum_A_Battery_charging + HISTEREZIS_BATTERY_CHARGING) )
    {
        current_state = OverCurrent;
        precedent_state =  current_state;
    }
    else if ( (precedent_state ==  OverVoltage) && (Battery_V > maximum_V_Battery_charging) ||
              (precedent_state ==  OverCurrent) && (Battery_A > maximum_A_Battery_charging) )
    {
        current_state = Hysteresis_zone;
    }
    return current_state;
}
void battery_V()
{
    uint32 tmpValue;
    tmpValue = ADC_rawData_filtered[1];  //ADC
    tmpValue= ( (tmpValue*uVs_per_1ADC)/1000 );  //mV

    tmpValue = ((uint32)Battery_R1+(uint32)Battery_R2)*(uint32)tmpValue/(uint32)Battery_R2;//Vin mV

    if (!Battery_V)Battery_V += tmpValue;
    else
    {
        Battery_V += tmpValue;
        Battery_V/=2;
    }
}
void battery_A()
{
    int32 tmpValue;
    tmpValue = ADC_rawData_filtered[A_battery];
//    tmpValue = tmpValue*(-1);

//    tmpValue*=mAs_per1ADC_battery;
    if (!Battery_A) Battery_A += tmpValue;
    else
    {
        Battery_A += tmpValue;
        Battery_A/=2;
    }

}

uint32 countsForAmpsBattery=0;
int64 Battery_A_Sum_everySeconds;
void SetBatteryCapacity_1s(int32 batteryAmps)
{
    if( (Battery_A_ODO + Battery_A_Sum_everySeconds/3600) < BatteryCapacity )
    {
        Battery_A_Sum_everySeconds += batteryAmps;
        countsForAmpsBattery++;
        if (countsForAmpsBattery == 3600)//1h
        {
            Battery_A_ODO =  Battery_A_ODO + Battery_A_Sum_everySeconds/3600;
            Battery_A_Sum_everySeconds = 0;
            countsForAmpsBattery=0;
        }
    }
}
int32 GetBatteryCapacity()
{
    return Battery_A_ODO + Battery_A_Sum_everySeconds/3600;
}
