/*
 * interrupts.h
 *
 *  Created on: 15.01.2016
 *      Author: uidg6243
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "commontypes.h"
#include <msp430.h>
#include "encoder.h"
#include "spi.h"
#include "adc.h"
#include "MPPT_function.h"
#include "boost.h"
#include "rotation.h"
#include "encoder.h"
#include "brake.h"
extern  volatile uint16 counterWd;
extern volatile uint16 counterTimer;

//#define ms100   48 for watchdog was 48
#define ms10 200//timer is on 20khz
#define ms100 10
#define second  100
#define second_2 200
#define minut   6000
#define hour    360000

extern uint16 tmpValue1,tmpValue2;
extern bool ms10TaskOcured,ms100TaskOcured,secTaskOcured ,minTaskOcured , hourTaskOcured ;
#endif /* INTERRUPTS_H_ */
