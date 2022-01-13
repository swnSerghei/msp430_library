/*
 * nokia5110_display.c
 *
 *  Created on: May 17, 2019
 *      Author: uidg6243
 */
#include "commontypes.h"
#include "nokia5110_display.h"
//static uint8 textsize = 1;
//static int16 cursor_x,cursor_y;
/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
    @param  s  Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
/**************************************************************************/
void setTextSize(uint8 s) {
//    if (s > 0) textsize = s;
  //  else s = 1;
}
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Set text cursor location
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
*/
/**************************************************************************/
void setCursor(int16 x, int16 y) {
//    cursor_x = x;
//    cursor_y = y;
}

/**************************************************************************/
/*!
    @brief  Get text cursor X location
    @returns    X coordinate in pixels
*/
/**************************************************************************/
int16 getCursorX() {
//    return cursor_x;
    return 0;
}

/**************************************************************************/
