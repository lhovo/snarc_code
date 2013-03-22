#include <SoftwareSerial.h>
#include "config.h"
#include <Time.h>

void setup()
{
    LEDS.init();
    RFID.init();
    MEMORY.init();
    
    Serial.begin(19200);
    
//    MENU.display();

    attachInterrupt(INT_USER, userInterupt, CHANGE);
}

void loop()
{
    unsigned long rfidTag;
    
    if(RFID.read(&rfidTag))
    {
        Serial.println(rfidTag);
        LEDS.blink(LEDS_GREEN);
    }
//    else
//    {
//     Serial.println("nope :("); 
//    }
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
