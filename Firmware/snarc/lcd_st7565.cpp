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
  // We are using the SPI bus but not in the normal way so disable the clock for now.
  SPI.end();
	glcd.begin(0x18);
	glcd.display(); // show splashscreen
	delay(2000);
	glcd.clear();
	glcd.drawstring(0,0,"Hello World");
  glcd.display();
  SPI.begin();
}

LCD_ST7565 LCD;
