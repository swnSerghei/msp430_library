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
		 int32  ld;
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
				    TxBuffer_Uart[TxBuffer_Uart_Tail] = '-';Uart_RecalculateTail_Buffer();Printable.ld= ((-1)*Printable.ld);
				}
				for (countIntegerNumber=1;( (Printable.i/10) / powValue ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
						TxBuffer_Uart[TxBuffer_Uart_Tail] = (uint8)Printable.i+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[TxBuffer_Uart_Tail]=(Printable.i / powValue)+48;
					Printable.i %= powValue;
					powValue/=10;
					}
					Uart_RecalculateTail_Buffer();
					countIntegerNumber--;
				}
			 break;
			 case 'l'://decimal
                 Printable.ld = va_arg( vl, int32);
                 if (Printable.ld < 0) {TxBuffer_Uart[TxBuffer_Uart_Tail] = '-';Uart_RecalculateTail_Buffer();Printable.ld= ((-1)*Printable.ld);}
                 for (countIntegerNumber=1;( (Printable.ld/10) / powValue ) != 0; countIntegerNumber++)
                 {
                     powValue*=10;
                 }
                 while(countIntegerNumber)
                 {
                     if (countIntegerNumber == 1)
                     {
                         TxBuffer_Uart[TxBuffer_Uart_Tail] = (uint8)Printable.ld+48;
                     } //in case if only 1 digit
                     else
                     {
                     TxBuffer_Uart[TxBuffer_Uart_Tail]=(Printable.ld / powValue)+48;
                     Printable.ld %= powValue;
                     powValue/=10;
                     }
                     Uart_RecalculateTail_Buffer();
                     countIntegerNumber--;
                 }
             break;
			 case 'f':
				Printable.f = va_arg( vl, double );
				tmpValue = (uint32)Printable.f;

				for (countIntegerNumber=1;( tmpValue / (10*powValue) ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				if (Printable.f < 0) {TxBuffer_Uart[TxBuffer_Uart_Tail] = '-';Uart_RecalculateTail_Buffer();Printable.f= ((-1)*Printable.f);}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
						TxBuffer_Uart[TxBuffer_Uart_Tail] = (uint8)tmpValue+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[TxBuffer_Uart_Tail]=(tmpValue / powValue)+48;
					tmpValue %= powValue;
					powValue/=10;
					}
					Uart_RecalculateTail_Buffer();
					countIntegerNumber--;
				}
				tmpValue = (uint32)Printable.f * 100;
				Printable.f *= 100;

				tmpValue = (uint32)Printable.f%tmpValue;
				TxBuffer_Uart[TxBuffer_Uart_Tail]='.';
				Uart_RecalculateTail_Buffer();
				TxBuffer_Uart[TxBuffer_Uart_Tail]=( (tmpValue / 10)+48 );
				Uart_RecalculateTail_Buffer();
				TxBuffer_Uart[TxBuffer_Uart_Tail]=(tmpValue % 10)+48;
				Uart_RecalculateTail_Buffer();
				 //printf_s( "%f\n", Printable.f );
			 break;

			 case 'c':
				 Printable.c = va_arg( vl, char );
				 //printf_s( "%c\n", Printable.c );
			 break;

			 case 's':
				 Printable.s = va_arg( vl, char * );
				// printf_s( "%s\n", Printable.s );
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
				 TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i];
				 Uart_RecalculateTail_Buffer();
			 	 TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i+1];
			 	 Uart_RecalculateTail_Buffer();
			 }
			 break;
	 		 }
	 		 i++;
	 	 }
		 else
		 {
			TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i];
			Uart_RecalculateTail_Buffer();
		 }
	 }
	 if (!uartInProgress) Uart_TX_Interrupt();
}
