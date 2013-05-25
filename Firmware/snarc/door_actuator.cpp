/*
 *   door_actuator.cpp - actuate the door (Open/Close)
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

#include "door_actuator.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/
 
void DOOR_ACTUATOR::init(void)
{
    pinMode(DOOR_PIN, OUTPUT);
    lock();
}

void DOOR_ACTUATOR::open(void)
{
#ifdef DOOR_INVERT_PIN
    digitalWrite(DOOR_PIN, LOW);
#else
    digitalWrite(DOOR_PIN, HIGH);
#endif
}

void DOOR_ACTUATOR::lock(void)
{
#ifdef DOOR_INVERT_PIN
    digitalWrite(DOOR_PIN, HIGH);
#else
    digitalWrite(DOOR_PIN, LOW);
#endif
}

void DOOR_ACTUATOR::unlockDoor(int timeMs)
{
   unlockDoor(timeMs, 0, 0);
}

void DOOR_ACTUATOR::unlockDoor(int timeMs, unsigned long *card_no, unsigned long int *deviceID)
{
    // Open the door
    open();
    
    if( card_no > 0 )
    {
        if(ETHERNET.check_tag(card_no, deviceID) == 0)
        {
           MEMORY.expireAccess(card_no);
        }     
    }

    delay(timeMs); 
    
    // Lock the door again
    lock();
}

door_status DOOR_ACTUATOR::status(void)
{
    if (digitalRead(DOOR_PIN) == LOW)
    {
#ifdef DOOR_INVERT_PIN
      return DOOR_OPEN;
#else
      return DOOR_LOCKED;
#endif
    }
    else
    {
#ifdef DOOR_INVERT_PIN
      return DOOR_LOCKED;
#else
      return DOOR_OPEN;
#endif
    }
}

DOOR_ACTUATOR DOOR;
