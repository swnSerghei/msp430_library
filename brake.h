#ifndef BRAKE_H_
#define BRAKE_H_

#include "commontypes.h"
#include <msp430.h>
#include "boost.h"
#include "MPPT_function.h"
#define BRAKE_PIN_PWM BIT6
#define BRAKE_PIN_SD BIT7
#define BRAKE_TIMER_PWM TA0CCR1

#define First_brake_pin BIT5
#define First_brake_Deactivate P2OUT |= First_brake_pin
#define First_brake_Activate P2OUT &= ~First_brake_pin

extern Brake_state brake_state;
extern int16 brake_correction;
extern void configure_break();
extern void put_brake();
extern void remove_breake();

#endif /* BRAKE_H_ */
