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

#include "config.h"
#include "Arduino.h"
#include <IPAddress.h>
#include <Time.h>
#include <EEPROM.h>

#ifndef MEMORY_DEVICE_NAME_MAX_LENGTH
#define MEMORY_DEVICE_NAME_MAX_LENGTH 12 // Maximum length of the device name
#endif

#ifndef MEMORY_RFID_LENGTH
#define MEMORY_RFID_LENGTH  sizeof(long) + sizeof(time_t) // RFID Tag plus expiry time
#endif

#define MEMORY_HEADER_LEN 50

struct DeviceInfo {
  IPAddress ip;        // IP address of this device
  IPAddress gateway;   // Gateway to the Internet
  IPAddress subnet;    // Subnet Mask
  IPAddress server;    // IP address of the authentication server
  byte      mac[6];    // Mac address of this device
  char      deviceName[MEMORY_DEVICE_NAME_MAX_LENGTH]; // Name/Location of this device
};

// Due to the preproccesor unable to calculate the sizeof(DeviceInfo) we have to do it manualy
// Sizeof(IPAddress) = 6, 6*5 + MEMORY_DEVICE_NAME_MAX_LENGTH = 30 + MEMORY_DEVICE_NAME_MAX_LENGTH

#if MEMORY_HEADER_LEN < (6*5 + MEMORY_DEVICE_NAME_MAX_LENGTH)
  #error "Memory Header is larger than space allocated"
#endif

class MEMORY_EEPROM
{
    boolean defaultCalled;
  
    public:
        void init(void);
        bool store_network_info(DeviceInfo *device);
        bool get_network_info(DeviceInfo *device);
        
        bool store_access(long rfid, time_t expiration); 
        bool grant_access(long rfid);
        bool expire_access(long rfid);
        
        void print_access_list(void);
        bool erase_access_list(void);
        
    private:
        void defaultOutput(boolean isStart);
};

extern MEMORY_EEPROM eepromMemory;

#endif /* __MEMORY_EEPROM_H__ */
