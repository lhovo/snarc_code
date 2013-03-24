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


/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/
 
void ETHERNET_SOCKET::init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server)
{
    delay(1000);   // delay boot by another precautionary 1sec to allow power rail time to stabilise, etc ( ethernet module draws mucho powero ) 
    Ethernet.begin(mac,ip,gateway,subnet);
    serverIP = server;
}

boolean ETHERNET_SOCKET::check_connection()
{
   return false; 
}

int ETHERNET_SOCKET::check_tag(long tag, int door)
{
   EthernetClient client;
   return 0;
}

ETHERNET_SOCKET ethernetSocket;
