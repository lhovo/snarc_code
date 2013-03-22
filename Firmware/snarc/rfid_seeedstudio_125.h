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

struct RFIDTag {
	int mfr;         // Manufacturer (?) Code (2 bytes), only useful in UART Mode
	long id;         // Tag ID (3 bytes)
	byte chk;        // Checksum (1 byte), only useful in UART Mode
	boolean valid;   // Validity of the Tag, based on the Checksum (UART Mode) or the parity bits (Wiegand Mode)
	char raw[13];    // The whole tag as a raw string, only useful in UART Mode
};

class RFID_SEEED_125
{
    public:
        void init(void);
        boolean read(unsigned long *last_code);
    private:
        long hex2dec(String hexCode);
        void clear(void);
};

extern RFID_SEEED_125 RFIDseed125;

#endif /* __RFID_125_SEEDSTUDIO_H__ */
