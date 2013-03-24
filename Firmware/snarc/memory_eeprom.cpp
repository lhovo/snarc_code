/*
 *   rfid_seeedstudio_125.cpp - 125Khz RFID Card reader by seeedstuidio (Electronic brick version) though this may work with others
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

#include "memory_eeprom.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/
 
void MEMORY_EEPROM::init(void)
{
    defaultCalled = false;
}

bool MEMORY_EEPROM::store_network_info(DeviceInfo *device)
{
    unsigned int i;
    for(i=0;i<sizeof(DeviceInfo);i++)
    {
       EEPROM.write(i, ((byte*) device)[i]);
    }
    return true;
}

bool MEMORY_EEPROM::get_network_info(DeviceInfo *device)
{
    unsigned int i;
    boolean valid = false;
    
    // Read EEPROM data
    for(i=0;i<sizeof(DeviceInfo);i++)
    {
      ((byte*) device)[i] = EEPROM.read(i);
    }

    // Ip Default
    if (device->ip == IPAddress(0xFFFFFFFF))
    {
       defaultOutput(true);
       Serial.println(F("Using Default IP - 192.168.1.200"));
       device->ip = IPAddress(192,168,1,200);
    }

    // Gateway Default
    if (device->gateway == IPAddress(0xFFFFFFFF))
    {
       defaultOutput(true);
       Serial.println(F("Using Default Gateway - 192.168.1.1"));
       device->gateway = IPAddress(192,168,1,1);
    }
    
    // Subnet Default
    if (device->subnet == IPAddress(0xFFFFFFFF))
    {
       defaultOutput(true);
       Serial.println(F("Using Default Subnet IP - 255.255.255.0"));
       device->subnet = IPAddress(255,255,255,0);
    }
    
    // Server Default
    if (device->server == IPAddress(0xFFFFFFFF))
    {
       defaultOutput(true);
       Serial.println(F("Using Default Server - 192.168.1.2"));
       device->server = IPAddress(192,168,1,2);
    }
    
    // Mac Address Default
    valid = false;
    for(i=0;i<6 && !valid;i++)
    {
        if(device->mac[i] != 0xff)
        {
            valid = true;
        }
    }
    if(!valid)
    {
       defaultOutput(true);
       Serial.println(F("Using Default Mac Address - DE:AD:BE:EF:FE:ED"));
       device->mac[0] = 0xDE;
       device->mac[1] = 0xAD;
       device->mac[2] = 0xBE;
       device->mac[3] = 0xEF;
       device->mac[4] = 0xFE;
       device->mac[5] = 0xED;
    }
    
    // Device Name Default
    valid = false;
    for(i=0;i<MEMORY_DEVICE_NAME_MAX_LENGTH && !valid;i++)
    {
        if((byte) device->deviceName[i] != 0xff)
        {
            valid = true;
        }
    }
    if(!valid)
    {
       defaultOutput(true);
       Serial.println(F("Using Default Name - default"));
       device->deviceName[0] = 'd';
       device->deviceName[1] = 'e';
       device->deviceName[2] = 'f';
       device->deviceName[3] = 'a';
       device->deviceName[4] = 'u';
       device->deviceName[5] = 'l';
       device->deviceName[6] = 't';
       device->deviceName[7] = '\0'; // End char
    }
    
    defaultOutput(false);
    return true;
}

void MEMORY_EEPROM::defaultOutput(boolean isStart)
{
    if(isStart && !defaultCalled)
    {
       defaultCalled = true;
       Serial.println(F("-- No config found in memory using defaults --"));
    }
    else if(!isStart && defaultCalled)
    {
       Serial.println(F("----------------------------------------------"));
    }
}

bool MEMORY_EEPROM::store_access(long rfid, time_t expiration)
{
    return true;
}

bool MEMORY_EEPROM::grant_access(long rfid)
{
    return true;
}
        
void MEMORY_EEPROM::print_access_list(void)
{
    
}

bool MEMORY_EEPROM::erase(void)
{
    for (int i = 0; i < 512; i++)
      EEPROM.write(i, 0xff);
    
    return true;   
}

MEMORY_EEPROM eepromMemory;
