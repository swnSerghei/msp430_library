/*
 * solar_panel_functions.c
 *
 *  Created on: Aug 1, 2018
 *      Author: uidg6243
 */
#include "commontypes.h"
#include "solar_panel_functions.h"
#include "battery_functions.h"

uint32 SolarPanel_V,SolarPanel_V_Raw,SolarPanel_A,SolarPanel_A_Raw;

int32 get_MPPT_mWats()
{
    return SolarPanel_V * SolarPanel_A;
}
void solar_panel_V()
{
    uint32 tmpValue;
    tmpValue = ADC_rawData_filtered[V_IN];
    tmpValue = ( (tmpValue*uVs_per_1ADC)/1000 );

    tmpValue = ((uint32)IN_R1+(uint32)IN_R2)*(uint32)tmpValue/(uint32)IN_R2;//Vin mV

    if (!SolarPanel_V) SolarPanel_V+=tmpValue;
    else
    {
        SolarPanel_V+=tmpValue;
        SolarPanel_V/=2;
    }
}
void calibrateZeroADC_panel()
{
}
void solar_panel_A()
{
    uint32 tmpValue;
    tmpValue = ADC_rawData_filtered[A_IN];

    tmpValue = ( (tmpValue*mAs_per1ADC_panel));

    if (!SolarPanel_A)SolarPanel_A += tmpValue;
    else
    {
        SolarPanel_A += tmpValue;
        SolarPanel_A/=2;
    }
}
