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

#include "rfid_seeedstudio_125.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/
 
SoftwareSerial RFID_SEED_125_Serial(RFID_SEEED_RX, 21); // RX, TX
 
void RFID_SEEED_125::init(void)
{
    // We don't care about sending data on this device so give a random TX value
    RFID_SEED_125_Serial.begin(2400);
    clear();
}

void RFID_SEEED_125::clear(void)
{
    while (RFID_SEED_125_Serial.available() > 0) {
        RFID_SEED_125_Serial.read();
    }
    
    RFID_SEED_125_Serial.flush(); 
}


int RFID_SEEED_125::read(char *last_code)
{
    int tagfound = 0, val = 0, bytesread = 0;
    if (RFID_SEED_125_Serial.available() > 0) // input waiting from internal rfid reader
    {
        if ((val = RFID_SEED_125_Serial.read()) == 10)
        {
            while (bytesread < 10)
            {   // read 10 digit code
                if (RFID_SEED_125_Serial.available() > 0)
                {
                    val = RFID_SEED_125_Serial.read();
                    if ((val == 10) || (val == 13))
                    {
                        break;
                    }
                    last_code[bytesread++] = val;
                }
            }
            if(bytesread == 10)
            {
                Serial.println(F("TAG detected!"));
                Serial.println(last_code);  //maybe we could not tell user the full tag number?
                
                // just in case.....
                Serial.flush();
                RFID_SEED_125_Serial.flush();
                
                tagfound = 1;
            }
            Serial.flush();
            bytesread = 0;
        }
    }
    return tagfound;
}


RFID_SEEED_125 RFIDseed125;
