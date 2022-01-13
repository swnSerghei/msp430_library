#include "commontypes.h"
#include "adc.h"
#include "MPPT_function.h"
#define BOOST_R1 1000000
#define BOOST_R2 10400
#define BOOST_VOLTAGE 210000
#define BOOST_VOLTAGE_ADCs (int32)(((uint32)(BOOST_VOLTAGE*(uint32)BOOST_R2)/((uint32)BOOST_R1+(uint32)BOOST_R2))*(uint32)1000/(uint32)uVs_per_1ADC)
#define BOOST_VOLTAGE_TREESHOLD 5000
#define BOOST_VOLTAGE_TREESHOLD_ADCs (int32)(((uint32)(BOOST_VOLTAGE_TREESHOLD*(uint32)BOOST_R2)/((uint32)BOOST_R1+(uint32)BOOST_R2))*(uint32)1000/(uint32)uVs_per_1ADC)


#define ACTIVATE_LOAD1 BIT4
#define ACTIVATE_LOAD2 BIT3
#define PWM_BOOST BIT6
#define SD_BOOST BIT7

extern uint32 boost_voltage;
extern volatile int16 timer_Correction_Boost;
extern uint8 timer_Boost_state;
extern void Boost_voltage();
extern void boost_Voltage_test();
extern void stop_timer_Boost();
extern void start_timer_Boost();
