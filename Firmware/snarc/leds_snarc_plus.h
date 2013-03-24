/*
 *   snarc_plus_leds.h - Controlls the onboard leds on the snarc+ board 
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
 
#include "Arduino.h" 
 
#ifndef __LEDS_H__
#define __LEDS_H__

#define LED_PIN_RED    5
#define LED_PIN_YELLOW 6
#define LED_PIN_GREEN  9

#ifndef LEDS_RED
#define LEDS_RED    1
#endif

#ifndef LEDS_YELLOW
#define LEDS_YELLOW 2
#endif

#ifndef LEDS_GREEN
#define LEDS_GREEN 4
#endif

#ifndef LEDS_ALL
#define LEDS_ALL    (LEDS_RED | LEDS_YELLOW | LEDS_GREEN)
#endif

class SNARC_PLUS_LEDS
{
    unsigned char leds, invert; 
    
    public:
        void init(void);
        
        void on(unsigned char ledv);
        void off(unsigned char ledv);
        void toggle(unsigned char ledv);
        void blink(unsigned char ledv);
        void pwm(unsigned char ledv, int value);
    private:
        void show_leds(unsigned char changed, int intensity);
};

extern SNARC_PLUS_LEDS SNARCPlusLEDS;

#endif /* __LEDS_H__ */
