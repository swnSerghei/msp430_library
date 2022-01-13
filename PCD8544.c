/*
 * PCD8544.c
 *
 *  Created on: May 17, 2019
 *      Author: uidg6243
 */
// LCD functions implementation
#include "PCD8544.h"
#include <msp430.h>

void setAddr(unsigned char xAddr, unsigned char yAddr) {
    writeToLCD(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void writeToLCD(unsigned char dataCommand, unsigned char data) {
//    static uint8 preview_state_display = CD5110_NONE;
    while((UCB0STAT & UCBUSY));
//    if (preview_state_display != new_state_display)
//    {
//        if      (new_state_display == CD5110_PANEL)         {LCD5110_PANEL_SELECT;LCD5110_GENERATOR_DESELECT;}
//        else if (new_state_display == CD5110_GENERATOR)     {LCD5110_GENERATOR_SELECT;LCD5110_PANEL_DESELECT;}
//        else if (new_state_display == CD5110_ALL_DISPLAYS)  {LCD5110_PANEL_SELECT;LCD5110_GENERATOR_SELECT;}
//        else                                                {LCD5110_GENERATOR_DESELECT;LCD5110_PANEL_DESELECT;}
//        preview_state_display = new_state_display;
//    }
//    LCD5110_PANEL_SELECT;//have just one display

    if(dataCommand) {
        LCD5110_SET_DATA;
    } else {
        LCD5110_SET_COMMAND;
    }
    //UCTXIFG &= ~UCB0IFG;
    UCB0TXBUF = data;
    //LCD5110_DESELECT;
}

void initLCD() {
//    LCD5110_GENERATOR_SELECT_DIR;           //For CS of Generator
//    LCD5110_PANEL_SELECT_DIR;               //For CS of SolarPanel
    LCD5110_SET_COMMAND_DIR;                //For Data/Code
    LCD5110_RESET_DIR;                      //For Reset

    LCD5110_RESET;
//    LCD5110_GENERATOR_SELECT;
//    LCD5110_PANEL_SELECT;
//    __delay_cycles(2);//820nS without any delay
    LCD5110_UNRESET;
//    LCD5110_GENERATOR_DESELECT;
//    LCD5110_PANEL_DESELECT;

//    LCD5110_GENERATOR_SELECT;
//    LCD5110_PANEL_SELECT;

    new_state_display = CD5110_ALL_DISPLAYS;
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLCD(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

//    LCD5110_GENERATOR_DESELECT;
//    LCD5110_PANEL_DESELECT;
}

void writeCharToLCD(char c) {
    unsigned char i;
    //LCD5110_SELECT;
    for(i = 0; i < 5; i++) {
        writeToLCD(LCD5110_DATA, font[c - 0x20][i]);
    }
    writeToLCD(LCD5110_DATA, 0);//need to review
    //LCD5110_DESELECT;
}

void writeStringToLCD(const char *string) {
    while(*string) {
        writeCharToLCD(*string++);
    }
}

void clearLCD() {
    setAddr(0, 0);
    int i = 0;
    new_state_display = CD5110_ALL_DISPLAYS;
    while(i < PCD8544_MAXBYTES) {
        writeToLCD(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, 0);
}

void clearBank(unsigned char bank) {
    setAddr(0, bank);
    int i = 0;
    while(i < PCD8544_HPIXELS) {
        writeToLCD(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, bank);
}

void PrintfToLCD(uint8 whitchDisplay,uint8 *string, ...)
{
         va_list vl;
         uint8 i;   //parsing the string
         uint8 HowManyDigits;
         uint16 sizeOfVector = 0;
         int32 powValue=1;
         int32 tmpValue;   //pentru a calcula partea intreaga
         //  szTypes is the last argument specified; you must access
         //  all others using the variable-argument macros.
         new_state_display = whitchDisplay;
         va_start( vl, string );
         // Step through the list.
         for(i = 0; string[i] != '\0'; ++i )
         {
             union Printable_t
             {
             int16  d;
             int32  ld;
             uint16  u;
             uint32 lu;
             float   f;
             char    c;
             char   *s;
             char   *v;
             } Printable;


             if (string[i] == '%' && string[i + 1] != '\0' )
             {
                 switch (string[i + 1])
                 {
                 case 'd'://decimal
                     Printable.d = va_arg(vl, int16);
                     if ( Printable.d != 0)
                     {
                         if (Printable.d < 0)
                         {
                             writeCharToLCD('-');
                             powValue = (-1);
                         }
                         else  powValue = 1;
                         for (HowManyDigits = 1; ((Printable.d / 10) / powValue) != 0; HowManyDigits++)
                         {
                             powValue *= 10;
                         }
                         while (HowManyDigits)
                         {
                             writeCharToLCD((uint8)((Printable.d / powValue) + '0'));
                             Printable.d %= powValue;
                             powValue /= 10;
                             HowManyDigits--;
                         }
                     }
                     else writeCharToLCD('0');
                     break;
                 case 'u'://decimal
                     Printable.u = va_arg(vl, uint16);
                     if ( Printable.u != 0)
                     {
                         powValue = 1;
                         for (HowManyDigits = 1; ((Printable.u / 10) / powValue) != 0; HowManyDigits++)
                         {
                             powValue *= 10;
                         }
                         while (HowManyDigits)
                         {
                             writeCharToLCD( (uint8)((Printable.u / powValue) + '0') );
                             Printable.u %= powValue;
                             powValue /= 10;
                             HowManyDigits--;
                         }
                     }
                     else writeCharToLCD( '0' );
                     break;
                 case 'l'://decimal
                     if ((string[i + 2]) == 'd')
                     {
                         Printable.ld = va_arg(vl, int32);
                         if ( Printable.ld != 0 )
                         {
                             if (Printable.ld < 0)
                             {
                                 writeCharToLCD('-');
                                 powValue = (-1);
                             }
                             else powValue = 1;
                             for (HowManyDigits = 1; ((Printable.ld / 10) / powValue) != 0; HowManyDigits++)
                             {
                                 powValue *= 10;
                             }
                             while (HowManyDigits)
                             {
                                 writeCharToLCD( (Printable.ld / powValue) + '0') ;
                                 Printable.ld %= powValue;
                                 powValue /= 10;
                                 HowManyDigits--;
                             }
                         }
                         else  writeCharToLCD( '0' );
                         i++;//for 'd'
                     }
                     else if ((string[i + 2]) == 'u')//uint32
                     {
                         Printable.lu = va_arg(vl, uint32);
                         if ( Printable.lu != 0 )
                         {
                             powValue = 1;
                             for (HowManyDigits = 1; ((Printable.lu / 10) / powValue) != 0; HowManyDigits++)
                             {
                                 powValue *= 10;
                             }
                             while (HowManyDigits)
                             {
                                 writeCharToLCD( (Printable.lu / powValue) + '0' );
                                 Printable.lu %= powValue;
                                 powValue /= 10;
                                 HowManyDigits--;
                             }
                         }
                         else writeCharToLCD( '0' );
                         i++;//for 'u'
                     }
                     else
                     {
                         writeCharToLCD( string[i] );//for %
                         writeCharToLCD( string[i + 1] );//for l
                     }
                     break;
                     //case 'f':
                     //  f = (float *)vl;
                     //  vl += sizeof(float);
                     //  tmpValue = (uint32)*f;

                     //  for (HowManyDigits = 1; (tmpValue / (10 * powValue)) != 0; HowManyDigits++)
                     //  {
                     //      powValue *= 10;
                     //  }
                     //  if (f < 0) { DLT_Buffer_For_PNC_Adapter[tail++] = '-'; }
                     //  while (HowManyDigits)
                     //  {
                     //      if (HowManyDigits == 1)
                     //      {
                     //          DLT_Buffer_For_PNC_Adapter[tail++] = (uint8)tmpValue + '0';
                     //      } //in case if only 1 digit
                     //      else
                     //      {
                     //          DLT_Buffer_For_PNC_Adapter[tail++] = (tmpValue / powValue) + '0';
                     //          tmpValue %= powValue;
                     //          powValue /= 10;
                     //      }
                     //      HowManyDigits--;
                     //  }
                     //  tmpValue = (uint32)*f * 100;
                     //  *f *= 100;
                     //  tmpValue = (uint32)*f%tmpValue;
                     //  DLT_Buffer_For_PNC_Adapter[tail++] = '.';
                     //  DLT_Buffer_For_PNC_Adapter[tail++] = ((tmpValue / 10) + '0');
                     //  DLT_Buffer_For_PNC_Adapter[tail++] = (tmpValue % 10) + '0';
                     //  break;

                 case 'c':
                     Printable.c = (char)va_arg(vl, uint16);
                     writeCharToLCD( Printable.c );
                     break;

                 case 's':
                     Printable.s = (char*)va_arg(vl, uint16);
                     while (*Printable.s != '\0')
                     {
                         writeCharToLCD( *Printable.s );
                         Printable.s+=sizeof(*Printable.s);
                     }
                     break;
                 //case 'x':
                 //  Printable.x = va_arg(vl, uint32);
                 //  //tmpValue = (uint32)Printable.ld;
                 //  DLT_Buffer_For_PNC_Adapter[tail++] = '0';
                 //  DLT_Buffer_For_PNC_Adapter[tail++] = 'x';
                 //  tmpValue = 1;
                 //  //while (Printable.x >>( 4 * (sizeof(Printable.x) * 2 - tmpValue)) )
                 //  //{
                 //  //  hexa_digits[Printable.x >> (4 * (sizeof(Printable.x) * 2 - 1))]; tmpValue++;
                 //  //}
                 //  //TxBuffer_Uart[TxBuffer_Uart_Tail] = Printable.ld / 16;
                 //  break;
                 case 'v':
                     sizeOfVector = va_arg(vl, uint16);
                     Printable.v = (char*)va_arg(vl, uint16);

                     while ( sizeOfVector-- )
                     {
 //                        if (string[i + 2] == 1) {tmpValue = *Printable.v1; Printable.v1 += sizeof(*Printable.v1);}
 //                        else if (string[i + 2] == 2){tmpValue = *Printable.v2; Printable.v2 += sizeof(*Printable.v2);}
 //                        else if (string[i + 2] == 4){tmpValue = *Printable.v4; Printable.v4 += sizeof(*Printable.v4);}
 //                        else
                         {tmpValue = *Printable.v; Printable.v += sizeof(*Printable.v);}

                         if ( tmpValue != 0)
                         {
                             if (tmpValue < 0)
                             {
                                 writeCharToLCD('-');
                                 powValue = (-1);
                             }
                             else powValue = 1;
                             for (HowManyDigits = 1; ((tmpValue / 10) / powValue) != 0; HowManyDigits++)
                             {
                                 powValue *= 10;
                             }
                             while ( HowManyDigits )
                             {
                                 writeCharToLCD( (tmpValue / powValue) + '0' );
                                 tmpValue %= powValue;
                                 powValue /= 10;
                                 HowManyDigits--;
                             }
                         }
                         else writeCharToLCD( '0' );
                         if ( sizeOfVector ) writeCharToLCD( ' ' );
                     }
                     break;
                 default:
                 {
                     writeCharToLCD( string[i] );
                     writeCharToLCD( string[i+1] );
                 }
                 break;
                 }
                 i++;
             }
             else
             {
                 writeCharToLCD( string[i] );
             }
         }
}


