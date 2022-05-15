/*
 * rotation.h
 *
 *  Created on: Nov 5, 2021
 *      Author: uidg6243
 */

#ifndef ROTATION_H_
#define ROTATION_H_

#include "commontypes.h"
#include <msp430.h>

#define rotation_read_pin BIT4

#define how_manyTrigers_per_seconds 1

void setup_rotation();
void  interrupt_routin();
void trigeredFunctionEvery_2_SecondToCalculateRotatiom();
uint16 read_rotation();

#endif /* ROTATION_H_ */
