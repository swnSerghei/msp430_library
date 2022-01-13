/*
 * WindTurbine.h
 *
 *  Created on: Mar 11, 2020
 *      Author: uidg6243
 */

#ifndef WINDTURBINE_H_
#define WINDTURBINE_H_

#include "commontypes.h"
#include "adc.h"
#include "interrupts.h"

#define treeshold_to_activate_PWM_For_Battery   1000        //start to do PWM more 3000mV
#define activate_PWM_For_Boost     57000        //start to do PWM more
#define activate_Break 59000
extern void windTurbine();
extern void windTurbine2();

#endif /* WINDTURBINE_H_ */
