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
// #define DOOR_INVERT_PIN // Invert the pin logic to the output Relay/Mosfet

// ------ Ethernet Protocol ------
#define USE_ETHERNET_HTTP
//#define USE_ETHERNET_SOCKET
//#define USE_ETHERNET_HTTP_SERVER

/* -------------------------------------
 *      END OF USER CONFIG OPTIONS
 * -------------------------------------
 */

#include <Ethernet.h>
#include <Time.h>

#define MEMORY_DEVICE_NAME_MAX_LENGTH 12
#define GLOBAL_BUFFER_LEN             40

extern char globalBuffer[GLOBAL_BUFFER_LEN];

// Declare Client info to be stored
struct DeviceInfo {
  IPAddress     ip;        // IP address of this device
  IPAddress     gateway;   // Gateway to the Internet
  IPAddress     subnet;    // Subnet Mask
  IPAddress     server;    // IP address of the authentication server
  byte          mac[6];    // Mac address of this device
  char          deviceName[MEMORY_DEVICE_NAME_MAX_LENGTH]; // Name/Location of this device
  unsigned int  id;        // Device unique id
};

struct RFID_info {
    unsigned long  card;
    time_t         expiration;
};

// --- Include the interactive serial menu ---
#include "serial_menu.h"
#define MENU SNARC_SERIAL_MENU

// --- Setup LED Defines ---
// Though you may have less colours define them anyway
#define LEDS_RED    1<<0
#define LEDS_YELLOW 1<<1
#define LEDS_GREEN  1<<2
#define LEDS_ALL    (LEDS_RED | LEDS_YELLOW | LEDS_GREEN)

// --- SNARC_PLUS Board Config ---
#ifdef SNARC_PLUS

    #include "leds_generic.h"
    #define LEDS         generic_leds

    #define DOOR_PIN           8 //5
    #define ETHERNET_CS        4
    #define ETHERNET_RESET_PIN 7
    #define AT45DB_CS          17

    #define INT_ETHERNET       0
    #define INT_USER           1

    #define LED_PIN_RED    5
    #define LED_PIN_YELLOW 6
    #define LED_PIN_GREEN  9
    
    #define LED_PINS    {LED_PIN_RED, LED_PIN_YELLOW, LED_PIN_GREEN}
    #define LED_DEFINED 3

#elif defined SNARC

    #include "leds_generic.h"
    #define LEDS         generic_leds

    #define DOOR_PIN            5
    #define ETHERNET_CS         10
    #define ETHERNET_RESET_PIN  8
    
    #define LED_PIN_RED    17
    #define LED_PIN_YELLOW 18
    #define LED_PIN_GREEN  19
    
    #define LED_PINS    {LED_PIN_RED, LED_PIN_YELLOW, LED_PIN_GREEN}
    #define LED_DEFINED 3
    
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
    #error NO ETHERNET DEFINED
#endif


// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#endif /* __CONFIGURE_H__ */
