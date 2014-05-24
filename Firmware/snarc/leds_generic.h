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

#include "config.h"
#include "Arduino.h" 
 
#ifndef __LEDS_GENERIC_H__
#define __LEDS_GENERIC_H__

/*
// Some of the Variables that should be defined in your config/program

#define LED_PIN_RED    5
#define LED_PIN_BLUE   6
#define LED_PIN_GREEN  9
#define LED_PINS       {LED_PIN_RED, LED_PIN_YELLOW, LED_PIN_GREEN}
#define LED_DEFINED 3

#ifndef LEDS_RED
#define LEDS_RED    1<<0
#endif

#ifndef LEDS_YELLOW
#define LEDS_YELLOW 1<<1
#endif

#ifndef LEDS_GREEN
#define LEDS_GREEN  1<<2
#endif

#ifndef LEDS_ALL
#define LEDS_ALL    (LEDS_RED | LEDS_YELLOW | LEDS_GREEN)
#endif
*/

class LEDS_GENERIC
{
    public:
        void init();
        
        void on(unsigned int ledv);
        void off(unsigned int ledv);
        void toggle(unsigned int ledv);
        void toggle(unsigned int ledv, unsigned int miliseconds);
        void blink(unsigned int ledv);
        void pwm(unsigned int ledv, int value);
    private:
        void show_leds(unsigned int changed, int intensity);
        
        unsigned int  leds; 
        unsigned long ledToggle;
};

extern LEDS_GENERIC generic_leds;

#endif /* __LEDS_GENERIC_H__ */
