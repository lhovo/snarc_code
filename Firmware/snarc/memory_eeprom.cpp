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
    for(i=0;i<sizeof(DeviceInfo);i++)
    {
      ((byte*) device)[i] = EEPROM.read(i);
    }
    return true;
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

bool MEMORY_EEPROM::erase_access_list(void)
{
 
    return true;   
}

MEMORY_EEPROM eepromMemory;
