/*
 *   ethernet_http.h - Ethernet libary using calls to a http server for information
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
 
#ifndef __ETHERNET_HTTP_H__
#define __ETHERNET_HTTP_H__

#include "Arduino.h"
#include "config.h"
#include <IPAddress.h>
#include <Ethernet.h>

class ETHERNET_HTTP
{
    public:
        void init(byte *mac, IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress server);
        boolean check_connection();
        int check_tag(long tag, int door);
    private:
};

extern ETHERNET_HTTP ethernetHttp;

#endif /* __ETHERNET_HTTP_H__ */