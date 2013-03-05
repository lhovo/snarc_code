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
    close();
}

void DOOR_ACTUATOR::open(void)
{
#ifdef DOOR_INVERT_PIN
    digitalWrite(DOOR_PIN, LOW);
#else
    digitalWrite(DOOR_PIN, HIGH);
#endif
}

void DOOR_ACTUATOR::close(void)
{
#ifdef DOOR_INVERT_PIN
    digitalWrite(DOOR_PIN, HIGH);
#else
    digitalWrite(DOOR_PIN, LOW);
#endif
}

void DOOR_ACTUATOR::unlockDoor(int timeMs)
{
    open();
    delay(timeMs);
    close();
}

DOOR_ACTUATOR DOOR;
