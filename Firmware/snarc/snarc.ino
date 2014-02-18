#include "config.h"
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <EEPROM.h>

#include <Time.h>
/*
 *   Simple NetworkAble RFID Controller firmware, for SNARC, SNARC+ and Arduino+Ethernet comptible hardware.
 *   Copyright (C) 2013 Luke Hovigton & David Bussenschutt. All right reserved.
 */
 
// TODO: 
// flash library ( external chip) not implemented
// socket library
// ntp
// time library not fully tested ( for onboard card expiry ) 
// wiznet hardware reset  ( take it from here:  https://github.com/davidbuzz/snarc/commit/f9e2aebe93cdc947fb6ceb957dfbb667a1da71f0 ) 
// http web interface ( crashes sometimes ) 
// watchdog reset
// password on web interface
// random mac address -on-the-fly- initialisation 
// "get current full list from server" menu option not implemented yet
// push full list of auth data from server ( to http interface ) 
// key revocation is tested and works ( by setting timestamp to zero ) 
// make the hardware "zero configuration" with DHCP and some sort of registration process.
// Piezo Micro cluster


DeviceInfo mySettings;
unsigned long rfidTag;
char globalBuffer[GLOBAL_BUFFER_LEN];

void setup()
{    
    unsigned int leds_init[] = LED_PINS;
    
    MENU.init(19200); // Set the TX/RX pins to 19200
    LEDS.init(leds_init, LED_DEFINED);
    RFID.init();
    MEMORY.init();
    MEMORY.getNetworkInfo(&mySettings);
    Serial.print(F("Device name: "));
    Serial.print(mySettings.deviceName);
    Serial.print(" id:");
    Serial.println(mySettings.id);
    ETHERNET.init(mySettings.mac, mySettings.ip, mySettings.gateway, mySettings.subnet, mySettings.server);
    NETWORKCHECKER.init();
    DOOR.init();
    attachInterrupt(INT_USER, userInterupt, HIGH);
}

void loop()
{   
    LEDS.toggle(LEDS_YELLOW, 2000);
    MENU.check();
    
    if(RFID.read(&rfidTag))
    {
        MEMORY.getNetworkInfo(&mySettings);
        Serial.print(F("RFID Tag:"));
        Serial.println(rfidTag);
        LEDS.off(LEDS_YELLOW);
        
        if(MEMORY.accessAllowed(&rfidTag)) // is tag in local EEPROM? 
        {
            LEDS.on(LEDS_GREEN);
            DOOR.unlockDoor(2000, &rfidTag, &mySettings.id, mySettings.deviceName); // open door for 2 seconds and log to HTTP remote
            LEDS.off(LEDS_GREEN);
        }
        else if (ETHERNET.check_tag(&rfidTag, &mySettings.id, mySettings.deviceName) > 0) // unknown key, check what remote server has to say ( server logs it) ? 
        {    
             LEDS.on(LEDS_GREEN | LEDS_RED);
             DOOR.unlockDoor(2000); // open door for 2 seconds , no logging
             
             // Record Card for next time
             RFID_info newCard = {rfidTag, now()+SECS_PER_WEEK};
             MEMORY.storeAccess(&newCard);
             LEDS.off(LEDS_GREEN | LEDS_RED);
        }
        else
        {
            LEDS.blink(LEDS_RED);
        }
    }
    ETHERNET.listen();   // local http server handler.
    
    NETWORKCHECKER.listen();
    DOOR.locktimeout();
    
}

void userInterupt()
{
#ifdef ENABLE_ESTOP_AS_SAFETY_DEVICE
  DOOR.lock();
#endif
#ifdef ENABLE_ESTOP_AS_EGRESS_BUTTON
  DOOR.unlockDoor(2000); // open door for 2 seconds
#endif
}
