/*
 *   config.h - Setup the board with all the options avaliable 
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
 
#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

// ------ Define Board ------
#define SNARC_PLUS
//#define SNARC

// ------ Set RFID Input ------
#define RFID_125MHZ_SOFTSERIAL
#define RFID_RX_PIN 15
#define RFID_TX_PIN 14

// ------ Memory Storage ------
#define USE_EEPROM
//#define USE_FLASH_AT45DB

// ------ Door Settings ------
// #define DOOR_INVERT_PIN

// ------ Ethernet Protocol ------
#define USE_ETHERNET_HTTP
//#define USE_ETHERNET_SOCKET
//#define USE_ETHERNET_HTTP_SERVER

#define MEMORY_DEVICE_NAME_MAX_LENGTH 12   // Maximum length of the device name
#define MEMORY_RFID_LENGTH            10+1 // +1 for the string end

/* -------------------------------------
 *      END OF USER CONFIG OPTIONS
 * -------------------------------------
 */

// --- Setup LED Defines ---
// Though you may have less colours define them anyway
#define LEDS_RED    1
#define LEDS_YELLOW 2
#define LEDS_GREEN  4

// --- Include the interactive serial menu ---
#include "serial_menu.h"
#define MENU SNARC_SERIAL_MENU

// --- SNARC_PLUS Board Config ---
#ifdef SNARC_PLUS
    #include "leds_snarc_plus.h"
    #define LEDS         SNARCPlusLEDS
//    #define DISPLAY_LEDS SNARCPlusLEDS // Need to thiank about this somemore, thinking about I2C interface

    #define ETHERNET_CS        4  /* I need to find some way of passing this to the driver, presently includes arnt working */
    #define ETHERNET_RESET_PIN 7
    #define AT45DB_CS          17

    #define INT_ETHERNET       0
    #define INT_USER           1
    
    #define DOOR_PIN           8 //5

#elif defined SNARC
    #include "leds_snarc.h"
    #define LEDS         SNARC_LEDS
    
    #define DOOR_PIN    5
    
    #define ETHERNET_CS 10
#else
    #error No Board Defined!
#endif

// --- Include Door code ---
#include "door_actuator.h"

// --- Include RFID driver ---
#ifdef RFID_125MHZ_SOFTSERIAL
    #define  RFID_SEEED_RX RFID_RX_PIN
    #define  RFID_SEEED_TX RFID_TX_PIN
    #include "rfid_seeedstudio_125.h"
    #define  RFID RFIDseed125
#endif

// --- Specify storage ---
#ifdef USE_EEPROM
    #include "memory_eeprom.h"
    #define MEMORY eepromMemory
#elif defined USE_FLASH_AT45DB
    #define MEMORY_FLASH_AT45DB_CS 10
    #include "memory_flash_at45db.h"
    #define MEMORY MEMORY_FLASH_AT45DB
#else
    #error NO MEMORY DEFINED
#endif

// --- Specify Ethernet Protocol ---
#ifdef USE_ETHERNET_HTTP
    #include "ethernet_http.h"
    #define ETHERNET ethernetHttp
#elif defined USE_ETHERNET_SOCKET
    #include "ethernet_socket.h"
    #define ETHERNET ethernetSocket
//#elif defined USE_ETHERNET_HTTP_SERVER

#else
    #error NO MEMORY DEFINED
#endif


// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#endif /* __CONFIGURE_H__ */
