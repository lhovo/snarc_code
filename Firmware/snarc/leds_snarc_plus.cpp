/*
 *   snarc_plus_leds.cpp - Controlls the onboard leds on the snarc+ board 
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

#include "leds_snarc_plus.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

void SNARC_PLUS_LEDS::init(void)
{
  pinMode(LED_PIN_RED,    OUTPUT);
  pinMode(LED_PIN_GREEN,  OUTPUT);
  pinMode(LED_PIN_YELLOW, OUTPUT);
  digitalWrite(LED_PIN_RED,    LOW);
  digitalWrite(LED_PIN_GREEN,  LOW);
  digitalWrite(LED_PIN_YELLOW, LOW);
  leds = invert = 0;
}

void SNARC_PLUS_LEDS::show_leds(unsigned char changed, int intensity)
{
  if(changed & LEDS_GREEN) {
    /* Green did change */
    if((invert ^ leds) & LEDS_GREEN) {
      analogWrite(LED_PIN_GREEN,    intensity);
    } else {
      digitalWrite(LED_PIN_GREEN,    LOW);
    }
  }
  if(changed & LEDS_YELLOW) {
    if((invert ^ leds) & LEDS_YELLOW) {
      analogWrite(LED_PIN_YELLOW,    intensity);
    } else {
      digitalWrite(LED_PIN_YELLOW,    LOW);
    }
  }
  if(changed & LEDS_RED) {
    if((invert ^ leds) & LEDS_RED) {
      analogWrite(LED_PIN_RED,    intensity);
    } else {
      digitalWrite(LED_PIN_RED,    LOW);
    }
  }
}

void SNARC_PLUS_LEDS::on(unsigned char ledv)
{
  unsigned char changed;
  changed = (~leds) & ledv;
  leds |= ledv;
  show_leds(changed, 0xff);
}

void SNARC_PLUS_LEDS::off(unsigned char ledv)
{
  unsigned char changed;
  changed = leds & ledv;
  leds &= ~ledv;
  show_leds(changed, 0x00);
}

void SNARC_PLUS_LEDS::toggle(unsigned char ledv)
{
  invert = invert ^ ledv;
  show_leds(ledv, 0xff);
}

void SNARC_PLUS_LEDS::toggle(unsigned char ledv, unsigned int miliseconds)
{
    if (millis() > ledToggle + miliseconds)
    {
        ledToggle = millis();
        toggle(ledv);
    }
}

void SNARC_PLUS_LEDS::blink(unsigned char ledv)
{
  toggle(ledv);
  delay(400);
  toggle(ledv);
}

void SNARC_PLUS_LEDS::pwm(unsigned char ledv, int value)
{  
  if (value > 0)
  {
    leds |= ledv;
  }
  else
  {
    leds &= ~ledv;
  }

  if (value < 0xff)
  {
    show_leds(ledv, value);
  }
  else
  {
    show_leds(ledv, 0xff);
  }
}

SNARC_PLUS_LEDS SNARCPlusLEDS;
