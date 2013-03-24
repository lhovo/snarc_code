/*
 *   rfid_seeedstudio_125.h - 125Khz RFID Card reader by seeedstuidio (Electronic brick version) though this may work with others
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
 
#ifndef __RFID_125_SEEDSTUDIO_H__
#define __RFID_125_SEEDSTUDIO_H__

#include "Arduino.h"
#include "config.h"
#include <SoftwareSerial.h>

#ifndef RFID_RX_PIN
#define RFID_RX_PIN 15
#endif

#ifndef RFID_TX_PIN
#define RFID_TX_PIN 14
#endif

#ifndef RFID_BAUD_RATE
#define RFID_BAUD_RATE 9600
#endif

struct RFIDTag {
	int mfr;         // Manufacturer (?) Code (2 bytes), only useful in UART Mode
	long id;         // Tag ID (3 bytes)
	byte chk;        // Checksum (1 byte), only useful in UART Mode
	boolean valid;   // Validity of the Tag, based on the Checksum (UART Mode) or the parity bits (Wiegand Mode)
	char raw[13];    // The whole tag as a raw string, only useful in UART Mode
};

typedef void (*card_callback_t)(void);

class RFID_SEEED_125
{
    card_callback_t reading_card;

    public:
        void init(void);
        
        // Try read a RFID Tag, last_code will be updated if tag found
        // Returns True on successful tag read, False otherwise
        boolean read(unsigned long *last_code);
        
        // Register a user callback function to allow us to blink some leds or something before the card has finished being read.
        // This is totaly optional and only saves a fraction of a second.
        void reading_callback(card_callback_t card_read);
    private:
        long hex2dec(String hexCode);
        void clear(void);
};

extern RFID_SEEED_125 RFIDseed125;

#endif /* __RFID_125_SEEDSTUDIO_H__ */
