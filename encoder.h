/*
 * encoder.h
 *
 *  Created on: Feb 5, 2020
 *      Author: uidg6243
 */

#ifndef ENCODER_H_
#define ENCODER_H_
#include <msp430.h>
#include "commontypes.h"
#include "WindTurbine.h"

#define ENCODER_LEFT    BIT4
#define ENCODER_RIGHT   BIT5
#define ENCODER_OK      BIT3

typedef enum
{
    left=0,
    right,
    OK
}encoderStates;

extern int8 tmpCounter;
extern bool manual_Test_Started;
void interruptEncoder(encoderStates encoderState);
void initEncoder();

#endif /* ENCODER_H_ */
