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
#include "brake.h"
#define ENCODER_LEFT    BIT2
#define ENCODER_RIGHT   BIT1
#define ENCODER_OK      BIT0

typedef enum
{
    left=1,
    right=2,
    OK=4
}encoderStates;

extern int8 tmpCounter_encoder;
extern bool manual_Test_Started;
extern uint8 encoderState;
void Encoder_Check();
void initEncoder();

#endif /* ENCODER_H_ */
