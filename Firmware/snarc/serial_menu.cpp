/*
 *   serial_menu.c - Give the user a serial menu to program from
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

#include "serial_menu.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_TIMEOUT_COUNT 10

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

void SERIAL_MENU::init(int baud)
{
  Serial.begin(baud);
  Serial.println("To enter program mode type '+++'\n");
  state = START;
}

void SERIAL_MENU::display(void)
{
    char incomingByte = 0;
    DeviceInfo   mySettings;
    IPAddress    tempIP;
  
    MEMORY.getNetworkInfo(&mySettings);
    changesMade = false;
  
    LEDS.off(LEDS_ALL);
    clear_serial_buffer();
    Serial.println(F("Entered Programming Mode! "));
    prompt();
    
    while( incomingByte != -1 )
    {
        LEDS.toggle(LEDS_RED, 400); // RED FLASH FOR PROGRAMMING MODE
        
        if ( Serial.available() )
        {
            incomingByte = Serial.read();
            Serial.println((char)incomingByte);
            
            switch((char)incomingByte)
            {
                case 'r':  // 'r'ead cards
                    readCards();
                    break;
                
//                case 'n': // add 'new' card:
//                    newCard();
//                    break;
                
                case 'i': // Erase/Initialise all MEMORY
                    initMemory();
                    break;
                
                case 'x': // Expire all cards
                    xpireCards();
                    break;
                
                // --------------  Server update functions -------------- //
                // ---  These need to be saved after updating them ------ //
                
                case 'd': // Set Device Name
                    listen_for_device_name(mySettings.deviceName);
                    break;

                case 't': // Set Device Zone
                    listen_for_device_id(&mySettings.id);
                    break;
                
                case 'b': // Set Ip Address
                    tempIP = mySettings.ip;
                    listen_for_ipaddress(&tempIP);
                    mySettings.ip = tempIP;
                    break;
                
                case 'g': // Set Gateway Address
                    tempIP = mySettings.gateway;
                    listen_for_ipaddress(&tempIP);
                    mySettings.gateway = tempIP;
                    break;

                case 'k': // Set Subnet Address
                    tempIP = mySettings.subnet;
                    listen_for_ipaddress(&tempIP);
                    mySettings.subnet = tempIP;
                    break;
                
                case 'a': // Set Server Address
                    tempIP = mySettings.server;
                    listen_for_ipaddress(&tempIP);
                    mySettings.server = tempIP;
                    break;
                
                case 'm': // Set Mac Address
                    macAddr();
                    break;
                
                case 's': // Save Changed data
                    save();
                    break;

                case 'p':
                    print_node_config(&mySettings);
                    break;
                    
                case 'e':
                    ETHERNET.print_settings();
                    break;

                case 'q': // q mean quit programming mode, and resume normal behaviour
                    incomingByte = -1;
                    break;
                
                // ignore whitespace
                case '\r':
                case '\n':
                case ' ':
                    break;
                    
                // nothing
                case 'h':
                default:
                    prompt();
                    break;
            } //switch/case
            if(incomingByte != -1)
            {
                Serial.println(F("Command Done.."));
            }
            clear_serial_buffer();
        } // if
    } //while
    
    LEDS.off(LEDS_ALL);
    Serial.println("Exited Program Mode");
}


void SERIAL_MENU::prompt(void)
{
    Serial.println(F("\nPROGRAM MODE:"));
    //Serial.println(F("For Command List goto: https://github.com/HSBNE/snarc_code"));
    Serial.println(F("r - print card list"));
//    Serial.println(F("n - program new key to MEMORY"));
    Serial.println(F("z - expire all cards from MEMORY"));
    Serial.println(F("i - wipe and initialise EEPROM (dangerous!) \n"));
    
    Serial.println(F("-- Options below need to be saved after change --"));
    Serial.println(F("d - set device name"));
    Serial.println(F("t - set device zone"));
    //Serial.println(F("m - set MAC address"));
    Serial.println(F("b - set IP address"));
    Serial.println(F("g - set gateway address"));
    Serial.println(F("k - set subnet address"));
    //Serial.println(F("e - set DNS address")); // -- Not supported yet -- //
    Serial.println(F("a - set server address"));
    Serial.println(F("s - save changes"));
    Serial.println(F("p - print node config"));
    Serial.println(F("e - print loaded ethernet settings\n"));
    
    Serial.println(F("q - quit programming mode"));
}

// Check for +++, then enter program mode..
void SERIAL_MENU::check(void)
{
  if (Serial.available())
  {
    switch(state)
    {
      case START:
      case PLUS1:
        if (Serial.read() == '+')
        {
          state++;
        }
        else
        {
          state = START;
        }
        break;
      case PLUS2:
        state = START;
        if (Serial.read() == '+')
        {
          display();
        }
        break;
    }
  }
}

// Read current list from EEPROM cache
void SERIAL_MENU::readCards(void)
{
  Serial.println(F("Card list.."));
  MEMORY.printAccessList();
}

// Write new code to MEMORY
// the next key scanned will be saved
void SERIAL_MENU::newCard(void)
{
  RFID_info    newCard;
  
  Serial.println(F("Scan new card now"));
  
  // Wait for card to be read
  while(!RFID.read(&newCard.card)){}
  
  if(MEMORY.storeAccess(&newCard))
  {
    Serial.print(F("-- "));
    Serial.print(newCard.card);
    Serial.println(F(" STORED --"));
  }
  else
  {
    Serial.println(F("ERROR ADDING CARD!"));
  }
}

// Erase/Initialise all MEMORY
void SERIAL_MENU::initMemory(void)
{
  MEMORY.erase();
  Serial.println(F("Memory Erase complete.."));
}

// Expire all cards
void SERIAL_MENU::xpireCards(void)
{
  if(MEMORY.expireAccess())
  {
    Serial.println(F("-- ALL CARDS REMOVED --"));
  }
}

// Set Mac Address
void SERIAL_MENU::macAddr(void)
{
  Serial.println(F("disabled in implementation sorry ( buggy ) , use hardcode MACs only."));
  //// TODO: set MAC address
  ////listen_for_new_mac_address();
  //mySettings.mac[0] = 0x02;
  //mySettings.mac[1] = 0x08;
  //mySettings.mac[2] = 0xDC;
  //// NIC (Network Interface Controller)
  //mySettings.mac[3] = 0xEF;
  //mySettings.mac[4] = 0xFE;
  //mySettings.mac[5] = 0xED;
  //changesMade = true;
}

// Save Changed data
void SERIAL_MENU::save(void)
{
  if(changesMade)
  {
    MEMORY.storeNetworkInfo(&mySettings);
    changesMade = false;
    Serial.println(F("Changes Saved.."));
    Serial.println(F("Reset device for settings to take effect"));
  }
  else
  {
    Serial.println(F("No changes made.."));
  }
}

void SERIAL_MENU::print_node_config(DeviceInfo *settings)
{
    Serial.println(F("---------------- Local config ----------------"));
  
    Serial.print(F("Device Name: "));
    Serial.println((String) settings->deviceName);

    Serial.print(F("Device ID: "));
    Serial.println((int)settings->id);
    
    Serial.print(F("IPAddress:"));
    Serial.println((IPAddress) settings->ip);
    
    Serial.print(F("Gateway:  "));
    Serial.println((IPAddress) settings->gateway);
    
    Serial.print(F("Subnet:   "));
    Serial.println((IPAddress) settings->subnet);

    Serial.print(F("Server:   "));
    Serial.println((IPAddress) settings->server);

    // We know the first Half as it's Static, lets save some code space and just print it :)
    Serial.print(F("Mac:      02:08:DC:"));
    Serial.print(settings->mac[3],16);
    Serial.print(":");
    Serial.print(settings->mac[4],16);
    Serial.print(":");
    Serial.println(settings->mac[5],16);
    
    Serial.println(F("----------------------------------------------"));
    
}

void SERIAL_MENU::listen_for_ipaddress(IPAddress *change)
{
    Serial.print(F("Current IP is: "));
    Serial.println(*change);
    Serial.println(F("Enter new IP eg 192.168.1.1"));
    
    boolean keepReading = true;
    int     serial_recieve_index = 0;
    
    clear_serial_buffer();
    
    while (keepReading)
    {
        while (Serial.available())
        {
            if (Serial.peek() == 13 || Serial.peek() == 10)
            {
                // new line. End entry
                keepReading = false;
                break;
            }
            globalBuffer[serial_recieve_index++] = Serial.read();
            if (serial_recieve_index >= 15) // there is a max of 15 chars - 255.255.255.255
            {
                // max length. End entry
                keepReading = false;
                break;
            }
        }
    }
    clear_serial_buffer();
    
    if (serial_recieve_index == 0)
    {
        // Empty, do not save.
        Serial.println(F("No input detected. No changes made."));
    }
    else
    {
       // Proccess the ip address and make it a number 
       int part[4];

       serial_recieve_index = sscanf(globalBuffer, "%d.%d.%d.%d", part, part+1, part+2, part+3);
       if(serial_recieve_index == 4)
       {
          *change = IPAddress(part[0], part[1], part[2], part[3]);
          Serial.print(F("IP set to: "));
          Serial.println(*change);
          changesMade = true;
       }
    }
}

void SERIAL_MENU::listen_for_device_name(char *deviceName)
{
    boolean keepReading = true;
    int     serial_recieve_index = 0, i;
            
    Serial.print(F("Current device name is: "));
    Serial.println(deviceName);
    Serial.print(F("Enter new device name (max "));
    Serial.print(MEMORY_DEVICE_NAME_MAX_LENGTH-1); // We need a termination char
    Serial.println(F(" characters):"));
    
    clear_serial_buffer();
    
    while (keepReading)
    {
        while (Serial.available())
        {
            if (Serial.peek() == 13 || Serial.peek() == 10)
            {
                // new line. End entry
                keepReading = false;
                break;
            }
            deviceName[serial_recieve_index++] = Serial.read();
            if (serial_recieve_index >= MEMORY_DEVICE_NAME_MAX_LENGTH-1)
            {
                // max length. End entry
                keepReading = false;
                break;
            }
        }
    }
    clear_serial_buffer();
    
    if (serial_recieve_index == 0)
    {
        // Empty, do not save.
        Serial.println(F("No input detected. No changes made."));
    }
    else
    {
        // Fill rest of device name array with null chars
        for (i = serial_recieve_index; i <= MEMORY_DEVICE_NAME_MAX_LENGTH; i++)
        {
            deviceName[i] = '\0';
        }
        
        changesMade = true;
        // Echo & save new name
        Serial.print(F("Device name set to: "));
        Serial.println(deviceName);
    }
}

void SERIAL_MENU::listen_for_device_id(unsigned long int *deviceId)
{
    boolean keepReading = true;
    int     serial_recieve_index = 0;
    char    deviceChar[6];
            
    Serial.print(F("Current device id is: "));
    Serial.println(*deviceId);
    Serial.print(F("Enter new device Id"));
    
    clear_serial_buffer();
    
    while (keepReading)
    {
        while (Serial.available())
        {
            if (Serial.peek() == 13 || Serial.peek() == 10)
            {
                // new line. End entry
                keepReading = false;
                break;
            }
            deviceChar[serial_recieve_index++] = Serial.read();
            if (serial_recieve_index >= 5)
            {
                // max length. End entry
                keepReading = false;
                break;
            }
        }
    }
    clear_serial_buffer();
    
    if (serial_recieve_index == 0)
    {
        // Empty, do not save.
        Serial.println(F("No input detected. No changes made."));
    }
    else
    {
        deviceChar[serial_recieve_index] = '\0';
        sscanf(deviceChar, "%lu", deviceId);
        changesMade = true;
        
        Serial.print(F("Device ID set to: "));
        Serial.println(*deviceId);
    }
}

// There is lots of delay here cos we want to make sure nothing is left in the buffer..
// But still be responsive to the user.
void SERIAL_MENU::clear_serial_buffer(void)
{
    int timeout = 0;
    while(!Serial.available() && (timeout++ < SERIAL_TIMEOUT_COUNT)){delay(1);}
    while (Serial.available())
    {
        Serial.read();
        timeout = 0;
        while(!Serial.available() && (timeout++ < SERIAL_TIMEOUT_COUNT)){delay(1);}
    }
}

SERIAL_MENU SNARC_SERIAL_MENU;
