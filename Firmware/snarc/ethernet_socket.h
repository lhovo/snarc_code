/*
 *   ethernet_socket.h - Ethernet libary using socket connection to comunicate to servers
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
 
#ifndef __ETHERNET_SOCKET_H__
#define __ETHERNET_SOCKET_H__

#include "Arduino.h"
#include "config.h"
#include <IPAddress.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#define SERVERIP_1 10
#define SERVERIP_2 0
#define SERVERIP_3 1
#define SERVERIP_4 253

class ETHERNET_SOCKET
{
    public:
        void init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server);
        boolean check_connection();
        int check_tag(long tag, int door);
    private:
};

extern ETHERNET_SOCKET ethernetSocket;

#endif /* __ETHERNET_SOCKET_H__ */
