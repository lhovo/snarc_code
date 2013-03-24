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
#define RFID_TIMEOUT_COUNT 20 // 9 seems to be the min, lets double it to be safe..

/******************************************************************************
 * Constructors
 ******************************************************************************/
long hex2dec(String hexCode);

/******************************************************************************
 * User API
 ******************************************************************************/
 
SoftwareSerial RFID_SEED_125_Serial(RFID_RX_PIN, RFID_TX_PIN);
 
void RFID_SEEED_125::init(void)
{
    RFID_SEED_125_Serial.begin(RFID_BAUD_RATE);
    reading_card = NULL;
}

boolean RFID_SEEED_125::read(unsigned long *last_code)
{
  RFIDTag tag;
  int bytesRead, timeout;
  char in;

  tag.valid = false;
      
  if (RFID_SEED_125_Serial.available()) // Starts with a 0x02 Ends with 0x03
  {
    if((in = RFID_SEED_125_Serial.read()) == 0x02)
    {
        bytesRead = 0;
        tag.mfr   = 0;
        tag.id    = 0;
        tag.chk   = 0;
        
        if(reading_card != NULL)
        {
          reading_card();
        }
        
        timeout = 0;
        while(!RFID_SEED_125_Serial.available() && (timeout++ < RFID_TIMEOUT_COUNT)){}
        if(timeout >= RFID_TIMEOUT_COUNT) { return false; }
        
        
        while((in = RFID_SEED_125_Serial.read()) != 0x03)
        {
          tag.raw[bytesRead++] = in;
          
          timeout = 0;
          while(!RFID_SEED_125_Serial.available() && (timeout++ < RFID_TIMEOUT_COUNT)){}
          if(timeout >= RFID_TIMEOUT_COUNT) { return false; }
        }
        // ID completely read
        byte checksum = 0;
        String id = tag.raw;
    
        tag.mfr = hex2dec(id.substring(0,4));
        tag.id  = hex2dec(id.substring(4,10));
        tag.chk = hex2dec(id.substring(10,12));
    
        // Do checksum calculation
        int i2;		
        for(int i = 0; i < 5; i++) {
          i2 = 2*i;
    	checksum ^= hex2dec(id.substring(i2,i2+2));
        }
    
        if (checksum == tag.chk)
        {
          tag.valid = true;
          //Serial.println("Recived");
          //Serial.println(tag.id);
          *last_code = tag.id;
        }
    //      else
    //      {
    //        Serial.println("Bad CRC");
    //      }
    }
  }
  return tag.valid;
}

void RFID_SEEED_125::reading_callback(card_callback_t card_read)
{
  reading_card = card_read;
}

// Convert a HEX String to a decimal value (up to 8 bytes (16 hex characters))
long RFID_SEEED_125::hex2dec(String hexCode) {
  char buf[19] = "";
  hexCode = "0x" + hexCode;
  hexCode.toCharArray(buf, 18);
  
  return strtol(buf, NULL, 0);
}

void RFID_SEEED_125::clear(void)
{
    while (RFID_SEED_125_Serial.available() > 0) {
        RFID_SEED_125_Serial.read();
    }
    
    RFID_SEED_125_Serial.flush(); 
}

RFID_SEEED_125 RFIDseed125;
