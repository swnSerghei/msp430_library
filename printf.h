/*
 * printf.h
 *
 *  Created on: 02 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_PRINTF_H_
#define LIBRARY_PRINTF_H_
#include <stdarg.h>
//#include <stdio.h>
#include "commontypes.h"
#include "uart.h"

void print(uint8 *string, ... );
//uint32 _power( uint32 val, uint32 _pow );

#endif /* LIBRARY_PRINTF_H_ */
