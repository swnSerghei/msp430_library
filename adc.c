/*
 * adc.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include "adc.h"

//#include "solar_panel_functions.h"
//#include "battery_functions.h"

bool ADC10_interrupt_Ocured = 0;

int32  last_good_MEM_panel_V,last_good_MEM_panel_A,last_good_MEM_battery_V,last_good_MEM_battery_A,last_good_MEM_Boost_V;
int32 eroor_SD24MEM_battery_A,eroor_SD24MEM_panel_A;

int ADC_rawData[ADC_CHANNELS];
int ADC_rawData_sum[ADC_CHANNELS];
int ADC_rawData_filtered[ADC_CHANNELS];
bool filtered_ADCvalue_available = 0;

void init_adc()
{
////    //             group     Preload      LSB toggle           2s complement
////	SD24CCTL0   = SD24GRP + SD24OSR_256 + SD24LSBTOG + SD24IE + SD24DF;//Panel Solar Voltage
////	SD24CCTL1   = SD24GRP + SD24OSR_256 + SD24LSBTOG + SD24IE + SD24DF;
////	SD24CCTL2   = SD24GRP + SD24OSR_256 + SD24LSBTOG + SD24IE + SD24DF;
////	SD24CCTL3   = SD24GRP + SD24OSR_256 + SD24LSBTOG + SD24IE + SD24DF;
//
//    //             group     Preload               2s complement
//    SD24CCTL0   = SD24GRP + SD24OSR_256 + SD24IE + SD24DF;//Panel Solar Voltage
//    SD24CCTL1   = SD24GRP + SD24OSR_256 + SD24IE + SD24DF;
//    SD24CCTL2   = SD24GRP + SD24OSR_256 + SD24IE + SD24DF;
//    SD24CCTL3   = SD24GRP + SD24OSR_256 + SD24IE + SD24DF;
//
//	SD24INCTL0  = SD24INCH_0; //SD24 channel differential pair input
//	SD24INCTL1  = SD24INCH_1; //Battery Voltage
//	SD24INCTL2  = SD24INCH_2 + SD24GAIN_16;//Solar Panel Amps
//	SD24INCTL3  = SD24INCH_3 + SD24GAIN_8;//Battery Amps
//
//	SD24PRE0    = 0x08;	//delay before first conversion after x cycles
//	SD24PRE1    = 0x08;
//	SD24PRE2    = 0x08;
//	SD24PRE3    = 0x08;
//
//	SD24CTL   |= SD24REFS;	//INTERNAL REFERINCE
//
//	SolarPanel_V = 0;
//	SolarPanel_A = 0;
//
//	Battery_V = 0;
//	Battery_A = 0;
//	Battery_A_ODO = 0;
//
//	start_adc();
#ifdef __MSP430G2955__
    ADC10CTL1 = 0;
    ADC10CTL0 = 0;
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);
    ADC10DTC1 = ADC_CHANNELS;
    ADC10DTC0 |=  ADC10CT;           //ADC10 continuous transfer
    ADC10SA    = (unsigned int)&ADC_rawData;
    ADC10CTL1 |= INCH_6 ;           //Multi-channel repeated conversion starting from channel 6,With INCH_5 we select the starting channel (in this case we start from channel 6, then we sample channel 5 and so on all the way down to channel 0).
    ADC10CTL1 |= CONSEQ_3;          /* Sequence of channels */
    ADC10CTL1 |= ADC10DIV_7;
    ADC10CTL0 |= ADC10SHT_0;          //ADC10 sample-and-hold time
    ADC10CTL0 |= MSC;               //Multiple sample and conversion. Valid only for sequence or repeated modes. The first rising edge of the SHI signal triggers the sampling timer, but further sample-andconversions are performed automatically as soon as the prior conversion is completed
    ADC10CTL0 |= ADC10ON;            //ADC10 on
    ADC10CTL0 |= REFON;             //Reference generator on
    ADC10CTL0 |= REF2_5V;            //Reference-generator voltage.e. REFON must also be set. 2.5 V
    //ADC10CTL0 |= ADC10SR ;              /* ADC10 Sampling Rate 0:200ksps / 1:50ksps */
    ADC10CTL0 |= SREF_0;                /* VR+ = AVCC and VR- = AVSS */
    ADC10AE0   = BIT6 + BIT5 + BIT4 + BIT3 + BIT2 + BIT1 + BIT0;              //Analog input enabled A0..A6
    ADC10CTL0 &=~ADC10IFG;              //Cleare Interrupt Flag
    ADC10CTL0 |= ADC10IE;               //ADC10 interrupt enable
    ADC10CTL0 |= ENC + ADC10SC;         //Enable conversion + Start conversion.

#endif
#ifdef __MSP430G2553__
    ADC10CTL1 = 0;
    ADC10CTL0 = 0;
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);
    ADC10DTC1 = ADC_CHANNELS;
    ADC10DTC0 |=  ADC10CT;           //ADC10 continuous transfer
    ADC10SA    = (unsigned int)ADC_rawData;
    ADC10CTL1 |= INCH_4;           //Multi-channel repeated conversion starting from channel 6,With INCH_5 we select the starting channel (in this case we start from channel 6, then we sample channel 5 and so on all the way down to channel 0).
    ADC10CTL1 |= CONSEQ_3;          /* Sequence of channels */
    ADC10CTL1 |= ADC10DIV_1;
    ADC10CTL0 |= ADC10SHT_3;          //ADC10 sample-and-hold time
    ADC10CTL0 |= MSC;               //Multiple sample and conversion. Valid only for sequence or repeated modes. The first rising edge of the SHI signal triggers the sampling timer, but further sample-andconversions are performed automatically as soon as the prior conversion is completed
    ADC10CTL0 |= ADC10ON;            //ADC10 on
    ADC10CTL0 |= REFON;             //Reference generator on
    ADC10CTL0 |= REF2_5V;            //Reference-generator voltage.e. REFON must also be set. 2.5 V
    //ADC10CTL0 |= ADC10SR ;              /* ADC10 Sampling Rate 0:200ksps / 1:50ksps */
    ADC10CTL0 |= SREF_1;                // SREF_0 >>> /* VR+ = AVCC and VR- = AVSS */, SREF_1 >>>> /* VR+ = VREF+ and VR- = AVSS */
    ADC10AE0   = BIT4 + BIT3 + BIT2 + BIT1 + BIT0;              //Analog input enabled A0..A6
    ADC10CTL0 |= ENC + ADC10SC;         //Enable conversion + Start conversion.
    ADC10CTL0 &=~ADC10IFG;              //Cleare Interrupt Flag
    ADC10CTL0 |= ADC10IE;               //ADC10 interrupt enable
    //ADC10CTL0 |= ENC + ADC10SC;         //Enable conversion + Start conversion.
//    start_adc();
#endif
}

void start_adc()
{
    ADC10CTL0 |= ENC + ADC10SC;         //Enable conversion + Start conversion.
//    ADC10CTL0 |= ADC10SC;         //Enable conversion + Start conversion.
}
uint8 counter_ADC_samples = 0;
void interrupt_ADC()
{
    uint8 i;
    if (counter_ADC_samples < HowManyAVG_samples)
    {
        for ( i=0; i < ADC_CHANNELS; i++ )
        {
            if (i == A_IN)
            {
                if (ADC_rawData[i]>256)ADC_rawData[i]-=256;
                else ADC_rawData[i]=0;
            }
            else if (i == A_battery)
            {
                ADC_rawData[i]-= Midle_ADCs_of_zero_Amps;
            }
            ADC_rawData_sum[i] += ADC_rawData[i];
        }
        counter_ADC_samples++;
    }
    else
    {
        for (i=0;i<ADC_CHANNELS;i++)
        {
            ADC_rawData_sum[i] >>= 3;
            ADC_rawData_filtered[i] += ADC_rawData_sum[i];
            ADC_rawData_filtered[i] >>= 1;
            ADC_rawData_sum[i] = 0;
        }
        filtered_ADCvalue_available = 1;
        counter_ADC_samples = 0;
    }
//    if ( timer_Boost_state ==0 && timer_Buck_state == 0 )
//    {
//        start_adc();
//    }
//    start_adc();
}
