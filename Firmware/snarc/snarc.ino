#include "config.h"
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Time.h>

void setup()
{    
    DeviceInfo mySettings;
    unsigned int leds_init[] = LED_PINS;
    
    MENU.init(19200); // Set the TX/RX pins to 19200
    LEDS.init(leds_init, LED_DEFINED);
    RFID.init();
    RFID.reading_callback(rfid_alert_user);
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
    unsigned long rfidTag;
    
    LEDS.toggle(LEDS_YELLOW, 2000);
    MENU.check();
    
    if(RFID.read(&rfidTag))
    {
        if(MEMORY.accessAllowed(rfidTag))
        {
            Serial.println(rfidTag);
            LEDS.off(LEDS_YELLOW);
            LEDS.on(LEDS_GREEN);
            DOOR.unlockDoor(2000); // open door for 2 seconds
            LEDS.off(LEDS_GREEN);
        }
        else
        {
            LEDS.blink(LEDS_RED);
        }
    }
}

void rfid_alert_user()
{
  LEDS.on(LEDS_YELLOW);
}

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

void userInterupt()
{
  
}
