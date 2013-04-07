/*
 *   door_actuator.h - actuate the door (Open/Close)
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
 
#ifndef __DOOR_ACTUATE_H__
#define __DOOR_ACTUATE_H__

#include "config.h"
#include "Arduino.h"

#ifndef DOOR_PIN
#define DOOR_PIN 5
#endif

enum door_status {DOOR_LOCKED, DOOR_OPEN};

class DOOR_ACTUATOR
{
    public:
        void          init(void);
        void          open(void);
        void          lock(void);
        door_status   status(void);
        // Open door for timeMs milliseconds
        void          unlockDoor(int timeMs);
        void          unlockDoor(int timeMs, unsigned long *card_no, unsigned int *deviceID);
        
    private:
};

extern DOOR_ACTUATOR DOOR;

#endif /* __DOOR_ACTUATE_H__ */
