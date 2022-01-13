/*
 * rotation.c
 *
 *  Created on: Nov 5, 2021
 *      Author: uidg6243
 */

#include "rotation.h"

uint16 rotation_counter = 0;
uint16 rotationPeerSeconds = 0;

void setup_rotation()
{
    P2DIR &= ~rotation_read_pin;
    P2IES &= ~rotation_read_pin; //Bit = 0: The PxIFGx flag is set with a low-to-high transition
    P2IFG &= ~rotation_read_pin;
    P2IE |=   rotation_read_pin;
}

void interrupt_routin()
{
    rotation_counter++;
}
void trigeredFunctionEvery_2_SecondToCalculateRotatiom()
{
    rotationPeerSeconds = rotation_counter/how_manyTrigers_per_seconds/2;
    rotation_counter = 0;
}
uint16 read_rotation()
{
    return rotationPeerSeconds;
}
