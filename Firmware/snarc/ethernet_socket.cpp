/*
 *   ethernet_socket.cpp - Ethernet libary using socket connection to comunicate to servers
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

#include "ethernet_socket.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_PACKET_SIZE 30
#define PORTN 8001
#define POLL_INTERVAL 60ul
#define VERSION_NO 1

#ifdef USE_ETHERNET_SOCKET
/******************************************************************************
 * Constructors
 ******************************************************************************/
EthernetClient client;
byte packetBuffer[ BUFFER_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets
byte packetReturn[ BUFFER_PACKET_SIZE ];

/******************************************************************************
 * User API
 ******************************************************************************/        
 
void ETHERNET_SOCKET::init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server)
{
#ifdef ETHERNET_RESET_PIN
    pinMode(ETHERNET_RESET_PIN, OUTPUT);
    digitalWrite(ETHERNET_RESET_PIN, HIGH);
#endif
    delay(1000);   // delay boot by precautionary 1sec 
    Ethernet.begin(mac,ip,gateway,subnet); //, ETHERNET_CS);
    client.connect(server, PORTN);
    serverIP = server;
}

int ETHERNET_SOCKET::check_tag(unsigned long *tag, unsigned long int *door, char *name)
{
   return 0;
}

void ETHERNET_SOCKET::print_settings(void)
{
    byte macAdd[6];
    int i;
    
    Serial.println(F("--------- Ethernet Setting from chip ---------"));
    
    Serial.print(F("IPAddress:"));
    Serial.println(Ethernet.localIP());
    
    Serial.print(F("Gateway:  "));
    Serial.println(Ethernet.gatewayIP());
    
    Serial.print(F("Subnet:   "));
    Serial.println(Ethernet.subnetMask());

    Serial.print(F("DNS:      "));
    Serial.println(Ethernet.dnsServerIP());

//    Ethernet.macAddress(macAdd);
//    Serial.print(F("Mac:      "));
//    for(i=0;i<5;i++)
//    {
//      Serial.print(macAdd[i],16);
//      Serial.print(F(":"));
//    }
//    Serial.print(macAdd[5],16);

    Serial.print(F("Server:   "));
    Serial.println(serverIP);

    Serial.println(F("----------------------------------------------"));   
}

void ETHERNET_SOCKET::listen(void)
{ 
  // listen for incoming clients
  // Nothing to do here as all requests are done by us!
}

void ETHERNET_SOCKET::pack(byte *ptr, byte *data, int *start, int size)
{
  // http://msgpack.org/ formatted packets
  int i;
  ptr[(*start)++] = 0xC4;
  ptr[(*start)++] = size;
  for(i=0;i<size;i++)
  {
    ptr[(*start)++] = data[i];
  }
}

void ETHERNET_SOCKET_CHECKER::init(void)
{
   lastConnectionTime = 0;
}

int ETHERNET_SOCKET_CHECKER::getToken(void)
{
  int i;
  
  if(!client.connected())
  {
    client.stop();
    client.connect(mySettings.server, PORTN);
  }
  
  // Start the paket with a bit telling that 1 lot of data "pack" are being sent
   packetBuffer[0] = 0x91;
   packetReturn[0] = 'T';
   i=1;
   ETHERNET.pack(packetBuffer, packetReturn, &i, 1);
   client.write(packetBuffer,i);
  
   delay(100);
   packetBuffer[3] = '0';
  
   i=0;
   while(client.available())
   {
      packetBuffer[i++] = client.read();
   }
  
   if(packetBuffer[3] == 'T')
   {
     packetReturn[0] = packetBuffer[3];
     packetReturn[1] = packetBuffer[6];
     packetReturn[2] = packetBuffer[7];
     packetReturn[3] = packetBuffer[10];
     packetReturn[4] = packetBuffer[11];
    
     if(((uint16_t*)(packetReturn+3))[0] == calc_CRC(packetReturn, 3))
     {
       token = (packetReturn[2] << 8) | packetReturn[1];
       return true;
     }
   }
   return false;
}

// If we've not had a successful http request in the last 60 seconds, then try to make one
// if we've still not had a sucessful http request in hte last 60 seconds, then reset the wiznet module, wait 60 secs and try again.

void ETHERNET_SOCKET_CHECKER::listen(void)
{  
      int serveraccess = -1;
      int i, j, k;
      if (millis() - lastConnectionTime > POLL_INTERVAL*1000ul ) {
         Serial.println(F("network poll checking now.... "));
         last_connection_time();

         if(getToken())
         {
           serveraccess = 1;
           // Send Heart Beat to say we are alive!
           Serial.println("Success!!");
           // Start the paket with a bit telling that 5 lots of data "packs" are being sent
           packetBuffer[0] = 0x95;
           j=1;
           k=1;
           
           packetReturn[0] = 'H';
           ETHERNET.pack(packetBuffer, packetReturn, &j, 1);
           
           // Insert Device Name
           for(int i=0; i<sizeof(mySettings.deviceName); i++){
             packetReturn[k++] = mySettings.deviceName[i];
           }
           ETHERNET.pack(packetBuffer, packetReturn+1, &j, sizeof(mySettings.deviceName));   
           
           // Insert Mac Address
           for(int i=0; i<6; i++){
             packetReturn[k++] = mySettings.mac[i];
           }
           ETHERNET.pack(packetBuffer, packetReturn+k-6, &j, 6);
           
           // Insert Version Number
           packetReturn[k++] = VERSION_NO;
           ETHERNET.pack(packetBuffer, packetReturn+k-1, &j, 1);
           
           // CRC
           packetReturn[k++] = 's';
           packetReturn[k++] = 'x';
           packetReturn[k++] = token & 0xFF;
           packetReturn[k++] = token >> 8;
           
           ((uint16_t*)(packetBuffer+j+5))[0] = calc_CRC(packetReturn, k);
           ETHERNET.pack(packetBuffer, packetBuffer+j+5, &j, 2);
           
           client.write(packetBuffer,j);
           
           delay(100);
           packetBuffer[3] = '0';
          
           i=0;
           while(client.available())
           {
              packetBuffer[i++] = client.read();
           }
           
           if(packetBuffer[3] == 'H' && packetBuffer[6] == 1)
           {
              // All ok.. 
              last_connection_time();
              Serial.println(F("network check appears OK. "));
              return;
           }
         }
       
        // Something went wrong lets reset the wiz chip
        Serial.println(F("network appears offline, forcing wiznet reset "));
        last_connection_time(); // to make this code block not run again for at least 60 seconds, which is enough time ot bring wiznet online.
        wiznet_reset();
        delay(2000);
        // re-init wiznet stackand whatnot.
        ETHERNET.init(mySettings.mac, mySettings.ip, mySettings.gateway, mySettings.subnet, mySettings.server);
      }
}

void ETHERNET_SOCKET_CHECKER::wiznet_reset(void)
{ 
#ifdef ETHERNET_RESET_PIN
  //WIZRESET - ~30-48 secs after this, it pings - this is probably overkill, but I'm not sure if it's leading or trailing edge trggered, so I do both
  digitalWrite(ETHERNET_RESET_PIN, LOW);
  delay(50);
  digitalWrite(ETHERNET_RESET_PIN, HIGH);
  delay(200);
#endif 
}

void ETHERNET_SOCKET_CHECKER::last_connection_time() { 
   lastConnectionTime = millis();
} 

uint16_t ETHERNET_SOCKET_CHECKER::calc_CRC(unsigned char *p, uint16_t packetLength)
{
    uint16_t crc = 0;
    static uint8_t i, j;
    for (i = 0; i < packetLength; i++) 
    {
//        Serial.print(p[i],HEX);
//        Serial.print(":");
        crc = crc ^ p[i] << 8; 
        for(j=0;j < 8; j++)
        {
            if (crc & 0x8000) 
                crc = crc << 1 ^ 0x1021; 
            else 
                crc = crc << 1; 
        }
    }
//    Serial.println("");
//    Serial.print("ret:");
//    Serial.print(crc & 0xFF,HEX);
//    Serial.print('-');
//    Serial.print(crc >> 8,HEX);
//    Serial.println(' ');
    return crc;
}

ETHERNET_SOCKET_CHECKER ethernetSocketChecker;
ETHERNET_SOCKET ethernetSocket;

#endif //USE_ETHERNET_SOCKET
