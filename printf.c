/*
 * printf.c
 *
 *  Created on: 01 џэт. 2016 у.
 *      Author: Serghei
 */

#include "printf.h"
void print(uint8 *string, ... )
{
	 va_list vl;
	 uint8 i;	//parsing the string
	 uint8 countIntegerNumber;
	 uint32 powValue=1;
	 uint32 tmpValue;	//pentru a calcula partea intreaga
	 //  szTypes is the last argument specified; you must access
	 //  all others using the variable-argument macros.
	 va_start( vl, string );
	 // Step through the list.
	 for(i = 0; string[i] != '\0'; ++i )
	 {
		 union Printable_t
	 	 {
		 int16  i;
		 uint16 u;
		 int32  ld;
		 uint32 lu;
		 float   f;
		 char    c;
		 char   *s;
	 	 } Printable;

	 	 if (string[i] == '%' && string[i+1] != '\0')
	 	 {
	 		 switch( string[i+1])
	 		 {
			 case 'd'://decimal
				Printable.i = va_arg(vl, int16);
				if (Printable.i < 0)
				{
				    Uart_Fill_TX_Buffer('-');
				    Printable.ld= ((-1)*Printable.ld);
				}
				for (countIntegerNumber=1;( (Printable.i/10) / powValue ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
					    Uart_Fill_TX_Buffer((uint8)Printable.i+48);
					} //in case if only 1 digit
					else
					{
					    Uart_Fill_TX_Buffer((Printable.i / powValue)+48);
                        Printable.i %= powValue;
                        powValue/=10;
					}
					countIntegerNumber--;
				}
             i++; //increment d, % incrementing by for()
			 break;
			 case 'u'://decimal
                 Printable.u = va_arg(vl, int16);
                 for (countIntegerNumber=1;( (Printable.u/10) / powValue ) != 0; countIntegerNumber++)
                 {
                     powValue*=10;
                 }
                 while(countIntegerNumber)
                 {
                     if (countIntegerNumber == 1)
                     {
                         Uart_Fill_TX_Buffer((uint8)Printable.u+48);
                     } //in case if only 1 digit
                     else
                     {
                         Uart_Fill_TX_Buffer((Printable.u / powValue)+48);
                         Printable.u %= powValue;
                         powValue/=10;
                     }
                     countIntegerNumber--;
                 }
                 i++; //increment d, % incrementing by for()
                 break;
			 case 'l'://long
			     if ( (string[i+2] != 'u') )
                 {
			         if ( string[i+2] == 'd' )  i++;//mean %ld; % increment by for, l by the end of case, d by i++
                     Printable.ld = va_arg( vl, int32);
                     if (Printable.ld < 0)
                     {
                         Uart_Fill_TX_Buffer('-');
                         Printable.ld= ((-1)*Printable.ld);
                     }
                     for (countIntegerNumber=1;( (Printable.ld/10) / powValue ) != 0; countIntegerNumber++)
                     {
                         powValue*=10;
                     }
                     while(countIntegerNumber)
                     {
                         if (countIntegerNumber == 1)
                         {
                             Uart_Fill_TX_Buffer( (uint8)Printable.ld+48);
                         } //in case if only 1 digit
                         else
                         {
                             Uart_Fill_TX_Buffer((Printable.ld / powValue)+48);
                             Printable.ld %= powValue;
                             powValue/=10;
                         }
                         countIntegerNumber--;
                     }
                 }
                     else//lu
                     {
                         i++;//mean %lu; % increment by for, l by the end of case, u by i++
                         Printable.lu = va_arg( vl, uint32);
                         for (countIntegerNumber=1;( (Printable.lu/10) / powValue ) != 0; countIntegerNumber++)
                          {
                              powValue*=10;
                          }
                          while(countIntegerNumber)
                          {
                              if (countIntegerNumber == 1)
                              {
                                  Uart_Fill_TX_Buffer( (uint8)Printable.lu+48);
                              } //in case if only 1 digit
                              else
                              {
                                  Uart_Fill_TX_Buffer((Printable.lu / powValue)+48);
                                  Printable.lu %= powValue;
                                  powValue/=10;
                              }
                              countIntegerNumber--;
                      }
                     }
			     i++; //increment l, % incrementing by for()
             break;
			 case 'f':
				Printable.f = va_arg( vl, double );
				tmpValue = (uint32)Printable.f;
				for (countIntegerNumber=1;( tmpValue / (10*powValue) ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				if (Printable.f < 0)
				{
				    Uart_Fill_TX_Buffer('-');
				    Printable.f= ((-1)*Printable.f);
				}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
					    Uart_Fill_TX_Buffer( (uint8)tmpValue+48 );
					} //in case if only 1 digit
					else
					{
                        Uart_Fill_TX_Buffer( (tmpValue / powValue)+48 );
                        tmpValue %= powValue;
                        powValue/=10;
					}
					countIntegerNumber--;
				}
				tmpValue = (uint32)Printable.f * 100;
				Printable.f *= 100;
				tmpValue = (uint32)Printable.f%tmpValue;
				Uart_Fill_TX_Buffer('.');
				Uart_Fill_TX_Buffer( (tmpValue / 10)+48 );
				Uart_Fill_TX_Buffer((tmpValue % 10)+48);
				i++; //increment f, % incrementing by for()
				 //printf_s( "%f\n", Printable.f );
			 break;

			 case 'c':
				 Printable.c = va_arg( vl, char );
				 Uart_Fill_TX_Buffer( Printable.c);
				 i++;
			 break;

			 case 's':
//				 Printable.s = va_arg( vl, char * );
//				 while( Printable.s != 0 )
//                 {
//                     Uart_Fill_TX_Buffer( Printable.s);
//                     Printable.s++;
//                 }
			 break;
			 case 'x':
//                 Printable.ld = va_arg( vl, uint32 );
//                 tmpValue = (uint32)Printable.ld;
//                 TxBuffer_Uart[TxBuffer_Uart_Tail] = '0';
//                 Uart_RecalculateTail_Buffer();
//                 TxBuffer_Uart[TxBuffer_Uart_Tail] = 'x';
//                 Uart_RecalculateTail_Buffer();
//                 TxBuffer_Uart[TxBuffer_Uart_Tail] = Printable.ld / 16;
             break;
			 default:
			 {
			     Uart_Fill_TX_Buffer( string[i]);
				 //Uart_Fill_TX_Buffer(string[i+1]);
			 }
			 break;
	 		 }

	 	 }
		 else
		 {
		    Uart_Fill_TX_Buffer( string[i] );
		 }
	 }
}
