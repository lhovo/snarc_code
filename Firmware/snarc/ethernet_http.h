/*
 *   ethernet_http.h - Ethernet libary using calls to a http server for information
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
 
#ifndef __ETHERNET_HTTP_H__
#define __ETHERNET_HTTP_H__

#include "config.h"

#ifndef ETHERNET_CS
    #define ETHERNET_CS 10
#endif

#include "Arduino.h"
#include <Dns.h>
#include <IPAddress.h>
#include <Ethernet.h>

class ETHERNET_HTTP
{
    IPAddress serverIP;
    
    public:
        void init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server);
        void wiznet_reset(); // reset the hardware wiznet module 
        void print_settings(void);
        int  check_tag(unsigned long *tag, unsigned int *door);
        void listen(void);
    private:
};

class ETHERNET_WIZNET_CHECKER
{
    //IPAddress serverIP;
    unsigned long pollingInterval;// = 60;  // maximum time between network checks, in seconds
    unsigned long lastConnectionTime;// = 0;          // last time you connected to the server, in milliseconds

    public:
        void init(void);
        void listen(void);
    private:
     void wiznet_reset(); // reset the hardware wiznet module 
};


extern ETHERNET_HTTP ethernetHttp;

extern ETHERNET_WIZNET_CHECKER ethernetWiznetChecker;

#endif /* __ETHERNET_HTTP_H__ */
