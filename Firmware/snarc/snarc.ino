#include <SoftwareSerial.h>
#include "config.h"
#include <Time.h>

void setup()
{
    LEDS.init();
    RFID.init();
    MEMORY.init();
    
    Serial.begin(19200);
    
    MENU.display();

    attachInterrupt(INT_USER, blink, CHANGE);
}

void loop()
{
    char RFID_Tag[MEMORY_RFID_LENGTH];
//    int i;
//    
//    for(i=0;i<255;i+=5)
//    {
//      LEDS.pwm(LEDS_RED, i);
//      delay(30);
//    }
//    for(i=255;i>0;i-=5)
//    {
//      LEDS.pwm(LEDS_RED, i);
//      delay(30);
//    }
    LEDS.blink(LEDS_RED);
    delay(400);
    LEDS.blink(LEDS_YELLOW);
    delay(400);
    LEDS.blink(LEDS_ALL);
    delay(1000);
    LEDS.off(LEDS_ALL);
    
    if(RFID.read(RFID_Tag))
    {
        Serial.print("Found tag:");
        Serial.println(RFID_Tag);
    }
}

void blink()
{
    //state = !state;
}
