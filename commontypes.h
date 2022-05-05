/*
 * commontypes.h
 *
 *  Created on: 04 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_
typedef unsigned char                  	uint8;	//0-255
typedef char						  	int8;	//-128 - 127
typedef unsigned short                 	uint16;	//0-65535
typedef short                 			int16;	//-32768 - 32767
typedef unsigned long                   uint32;	//0 - 4 294 967 295
typedef long                   			int32;	//-2 147 483 648 - 2 147 483 647
typedef unsigned long long int         	uint64;
typedef long long int         			int64;	//-9 223 372 036 854 775 808 - 9 223 372 036 854 775 807
typedef uint8							bool;

#define false 0
#define true 1
typedef enum
{
    start=0,
    stop_in_progress,
    stop
}Buck_state;
typedef enum
{
    no_any_limit=0,
    undefine,
    startTimer,
    Over_current_limit,
    Over_voltage_limit,
    Histirezis_zone,
    MPPT_limit,
    NO_PWM
}MPPT_state;

typedef enum
{
    OverCurrent = 0,
    OverVoltage,
    floating_state,
    Hysteresis_zone,
    under_Voltage_or_current
}Batt_states;
typedef enum
{
    no_brake=0,
    remove_brake_in_progress,
    brake_in_progress,
    braked
}Brake_state;
#endif /* COMMONTYPES_H_ */
