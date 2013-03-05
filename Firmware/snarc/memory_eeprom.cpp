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

bool MEMORY_EEPROM::store_network_info(IPAddress *ip, IPAddress *gateway, IPAddress *subnet)
{
    return true;
}

bool MEMORY_EEPROM::get_network_info(IPAddress *ip, IPAddress *gateway, IPAddress *subnet)
{
    return true;
}

bool MEMORY_EEPROM::store_access(char *rfid, time_t expiration)
{
    return true;
}

bool MEMORY_EEPROM::find_access(char *rfid)
{
    return true;
}

bool MEMORY_EEPROM::set_device_name(char *name)
{
    return true;
}

bool MEMORY_EEPROM::get_device_name(char *device)
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
