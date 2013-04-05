/*
 *   ethernet_http.cpp - Ethernet libary using calls to a http server for information
 *   Copyright (C) 2013 Luke Hovigton, David Bussenschutt. All right reserved.
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

#include "ethernet_http.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

void ETHERNET_HTTP::init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server)
{
    delay(1000);   // delay boot by another precautionary 1sec to allow power rail time to stabilise, etc ( ethernet module draws mucho powero ) 
    Ethernet.begin(mac,ip,gateway,subnet, ETHERNET_CS);    
    serverIP = server;
}

void ETHERNET_HTTP::print_settings(void)
{
    byte macAdd[6];
    
    Serial.println(F("--------- Ethernet Setting from chip ---------"));
    
    Serial.print(F("IPAddress:"));
    Serial.println(Ethernet.localIP());
    
    Serial.print(F("Gateway:  "));
    Serial.println(Ethernet.gatewayIP());
    
    Serial.print(F("Subnet:   "));
    Serial.println(Ethernet.subnetMask());

    Serial.print(F("DNS:      "));
    Serial.println(Ethernet.dnsServerIP());

    Ethernet.macAddress(macAdd);
    Serial.print(F("Mac:      "));
    Serial.print(macAdd[0],16);
    Serial.print(":");
    Serial.print(macAdd[1],16);
    Serial.print(":");
    Serial.print(macAdd[2],16);
    Serial.print(":");
    Serial.print(macAdd[3],16);
    Serial.print(":");
    Serial.print(macAdd[4],16);
    Serial.print(":");
    Serial.println(macAdd[5],16);

    Serial.print(F("Server:   "));
    Serial.println(serverIP);

    Serial.println(F("----------------------------------------------"));   
}

boolean ETHERNET_HTTP::check_connection()
{
   return false; 
}

int ETHERNET_HTTP::check_tag(long tag, int door)
{
   EthernetClient client;
   int client_recieve_pointer = 0, x = 0;
   char client_recieve_data[32];

   if (client.connect(serverIP,80))
   {
  	Serial.println(F("http client connected"));
  	client.print("GET /logger.php?secret=asecret&q=");
  	client.print(tag);
  	client.print("&d=");
  	client.println(door);
  	client.println();
  	Serial.println(F("http client finished"));
    }
    else
    {
	Serial.println(F("http connection failed"));
	return -1; // error code to say server offline, which is different to "0" , which means deny access.
    }
    
    // delay some arbitrary amount for the server to respond to the client. say, 1 sec. ?
    delay(3000);
    
    for (x=0;x<=32;x++)  //client_recieve_data is only 32 bytes long.
    {
        if (client.available())
        {
            char c = client.read();
            client_recieve_data[client_recieve_pointer++] = c; //client.read();
        }

        // if the server's disconnected, stop the client:
        if (!client.connected())
        {
            Serial.println(F("client.stop"));
            client.stop();
       	    break;
        }
    }
    
    if ( x >= 32 )
    {
        Serial.println(F("too much HTTP data, error! ( do you have an auth.php on the server? ) "));
        return -1;
    }

    client_recieve_pointer = 0;
    // if the server's disconnected, stop the client:
    Serial.print(F("http data:"));
    // recieved data is now in the string: client_recieve_data
    Serial.println(client_recieve_data);
    
    // we expect the permissions string to look like 'access:3' ( for permit ), or 'access:0' (for deny )
    String Permissions =  String(client_recieve_data);
    
    int colonPosition = Permissions.indexOf(':');

    String scs = Permissions.substring(colonPosition + 1);  // as a "String" "object" starting from after the colon to the end!
    char cs[10];
    scs.toCharArray(cs,10); // same this as a char array "string" ( lower case)
    Serial.print(F("perms from server:"));
    Serial.println(cs);
    int ci = atoi(cs); // as an int!  if this fails, it returns zero, which means no-access, so that's OK.
    
    // basic bound check,  return -1 on error .
    if ( ci < 0 || ci > 255 ) {
       return -1;   
    }
    return ci;
}

ETHERNET_HTTP ethernetHttp;
