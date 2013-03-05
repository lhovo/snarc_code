/*
 *   door.h - actuate the door (Open/Close)
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
 
#ifndef __MEMORY_EEPROM_H__
#define __MEMORY_EEPROM_H__

#include "Arduino.h"
#include "config.h"
#include <IPAddress.h>
#include <Time.h>

#ifndef MEMORY_DEVICE_NAME_MAX_LENGTH
#define MEMORY_DEVICE_NAME_MAX_LENGTH 12 // Maximum length of the device name
#endif

#ifndef MEMORY_RFID_LENGTH
#define MEMORY_RFID_LENGTH            10 // RFID Tag
#endif

class MEMORY_EEPROM
{
    public:
        void init(void);
        bool store_network_info(IPAddress *ip, IPAddress *gateway, IPAddress *subnet);
        bool get_network_info(IPAddress *ip, IPAddress *gateway, IPAddress *subnet);
        
        bool store_access(char *rfid, time_t expiration); 
        bool find_access(char *rfid);
        bool expire_access(char *rfid);
        
        bool set_device_name(char *device);
        bool get_device_name(char *device);
        
        void print_access_list(void);
        bool erase_access_list(void);
        
    private:
};

extern MEMORY_EEPROM eepromMemory;

#endif /* __MEMORY_EEPROM_H__ */
