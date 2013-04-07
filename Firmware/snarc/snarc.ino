#include "config.h"
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Time.h>

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
    DOOR.init();
    
    attachInterrupt(INT_USER, userInterupt, CHANGE);
}

void loop()
{   
    LEDS.toggle(LEDS_YELLOW, 2000);
    MENU.check();
    
    if(RFID.read(&rfidTag))
    {
        MEMORY.getNetworkInfo(&mySettings);
        
        if(MEMORY.accessAllowed(&rfidTag))
        {
            Serial.println(rfidTag);
            LEDS.off(LEDS_YELLOW);
            LEDS.on(LEDS_GREEN);
            DOOR.unlockDoor(2000, &rfidTag, &mySettings.id); // open door for 2 seconds
            LEDS.off(LEDS_GREEN);
        }
        else if (ETHERNET.check_tag(&rfidTag, &mySettings.id) > 0)
        {    
             DOOR.unlockDoor(2000); // open door for 2 seconds
             
             // Record Card for next time
             RFID_info newCard = {rfidTag, now()+SECS_PER_WEEK};
             MEMORY.storeAccess(&newCard);
        }
        else
        {
            LEDS.blink(LEDS_RED);
        }
    }
    ETHERNET.listen();
}

void userInterupt()
{
  
}

/*
void led_test()
{
    int i, j;
    int led[4] = {LEDS_RED, LEDS_YELLOW, LEDS_GREEN, LEDS_ALL};
    
    for(j=0;j<4;j++)
    {
      for(i=0;i<=255;i+=3)
      {
        LEDS.pwm(led[j], i);
        delay(30);
      }
      for(i=255;i>=0;i-=3)
      {
        LEDS.pwm(led[j], i);
        delay(30);
      }
    }
    
    LEDS.blink(LEDS_RED);
    delay(400);
    LEDS.blink(LEDS_YELLOW);
    delay(400);
    LEDS.blink(LEDS_GREEN);
    delay(400);
    LEDS.blink(LEDS_ALL);
    delay(1000);
    LEDS.off(LEDS_ALL);
}
*/
