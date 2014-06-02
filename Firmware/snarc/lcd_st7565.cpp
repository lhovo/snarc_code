/*
 *   lcd_st7565.cpp - Graphical display
 *   Copyright (C) 2013 Luke Hovigton. All right reserved.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 /******************************************************************************
 * Includes
 ******************************************************************************/

#include "lcd_st7565.h"
#include "ST7565.h"

// To get the libary to use the spi lines,
// initialise the first two pins to zero

// pin 11 - Serial data out (SID)
// pin 13 - Serial clock out (SCLK)
// pin 17 - Data/Command select (RS or A0)
// pin 18 - LCD reset (RST)
// pin 19 - LCD chip select (CS)
ST7565 glcd(0, 0, 17, 18, 19);

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/
 
void LCD_ST7565::init(void)
{
  glcd.begin(0x18);
  glcd.display(); // show splashscreen
}

void LCD_ST7565::start(void)
{
  glcd.clear();
  glcd.drawstring(0,0,"Hello World");
  glcd.display();
}

void LCD_ST7565::updateCounter(uint16_t time)
{
  glcd.clear();
  glcd.drawstring(0,0,"Operator:");
  glcd.drawstring(0,1,"Lord Clancy");
  formatHalfTime(time, 2, 0);
  formatFullTime(time, 2, 61); // 128 pixels - (11 chars * 6 pixels) = 62 - 1
  
  glcd.drawstring(0,6,"Machine Time:");
  formatFullTime(time, 7, 0);

  glcd.display();
}


void LCD_ST7565::formatFullTime(uint16_t time, uint8_t line, uint8_t offset)
{
  char timestring[12];
  uint16_t msToSecond = time/10;
  
  int2str(timestring,msToSecond/86400);
  timestring[2] = ':';
  int2str(timestring+3,(msToSecond/3600)%24);
  timestring[5] = ':';
  int2str(timestring+6,(msToSecond/60)%60);
  timestring[8] = ':';
  int2str(timestring+9,msToSecond%60);
  timestring[11] = 0;
  glcd.drawstring(0+offset,line,timestring);
}

void LCD_ST7565::formatHalfTime(uint16_t time, uint8_t line, uint8_t offset)
{
  char timestring[6];
  uint16_t msToSecond = time/10;
  
  int2str(timestring,(msToSecond/60)%60);
  timestring[2] = ':';
  int2str(timestring+3,msToSecond%60);
  timestring[5] = 0;
  glcd.drawstring(0+offset,line,timestring);
}

void LCD_ST7565::int2str(char* buffer, register int i ) {
  register unsigned char L = 0;
  register char c, b;  // lower-byte of i

  // decades (check on lower byte to optimize code)
  b = char( i );
  if( b > 9 ) {
    c = b < 50
    ? ( b < 30
       ? ( b < 20 ? 1 : 2 )
       :   b < 40 ? 3 : 4
       )
    : b < 80
    ? ( i < 60
       ? 5
       : i < 70 ? 6 : 7
       )
    : i < 90 ? 8 : 9;
    buffer[ L++ ] = c + 48;
    b -= c * 10;
  }
  else buffer[ L++ ] = 48;

  // last digit
  buffer[ L++ ] = b + 48;
}

LCD_ST7565 LCD;
