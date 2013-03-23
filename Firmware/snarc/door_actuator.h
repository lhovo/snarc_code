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

#include "Arduino.h"
#include "config.h"

#ifndef DOOR_PIN
#define DOOR_PIN 5
#endif

class DOOR_ACTUATOR
{
    public:
        void init(void);
        void open(void);
        void close(void);
        void unlockDoor(int timeMs); // Open door for timeMs milliseconds
        
    private:
};

extern DOOR_ACTUATOR DOOR;

#endif /* __DOOR_ACTUATE_H__ */