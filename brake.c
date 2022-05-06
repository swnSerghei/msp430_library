#include "brake.h"
Brake_state brake_state;
int16 brake_correction = 0;
void configure_break()
{
    P2DIR |= BRAKE_PIN_PWM + First_brake_pin + BRAKE_PIN_SD;
    P2OUT &= BRAKE_PIN_PWM + BRAKE_PIN_SD;
    First_brake_Deactivate;
    //P2SEL |= BRAKE_PIN_PWM;
    TA0CCR0 = FREQUENCY;
    BRAKE_TIMER_PWM = DeadTime;
    //        SMCLK      /1   Up to CCR0   clr
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;
    TA0CCTL1 = OUTMOD_3+CCIE;
    brake_state = no_brake;
}
void put_brake()
{
    if ( (brake_state == remove_brake_in_progress) || (brake_state == brake_in_progress) )
    {
        if ( BRAKE_TIMER_PWM < (FREQUENCY-DeadTime) )
        {
            brake_correction = 10;
            brake_state = brake_in_progress;
        }
        else
        {
            P2SEL &= ~BRAKE_PIN_PWM;
            P2OUT &= ~BRAKE_PIN_PWM;
            brake_state = braked;
        }
    }
    else if ( brake_state ==  no_brake )
    {
        BRAKE_TIMER_PWM = DeadTime;
        P2SEL |= BRAKE_PIN_PWM;
        P2OUT |= BRAKE_PIN_SD;
        brake_state = brake_in_progress;
    }
}
void remove_breake()
{
    if ( (brake_state == remove_brake_in_progress) || (brake_state == brake_in_progress) )
    {
        if (BRAKE_TIMER_PWM > DeadTime)
        {
            brake_correction = (-10);
            brake_state = remove_brake_in_progress;
        }
        else
        {
            P2SEL &= ~BRAKE_PIN_PWM;
            P2OUT |= BRAKE_PIN_PWM;
            P2OUT &= ~BRAKE_PIN_SD;
            brake_state = no_brake;
        }
    }
    else if ( brake_state ==  braked )
    {
        BRAKE_TIMER_PWM = (FREQUENCY-DeadTime);
        P2SEL |= BRAKE_PIN_PWM;
        brake_state = remove_brake_in_progress;
    }
}
