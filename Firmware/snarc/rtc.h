/*
 *   rtc.h - Real Time Clock
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
 
#ifndef __RTC_H__
#define __RTC_H__

#include "config.h"
#include "Arduino.h"
#include <IPAddress.h>

class REAL_TIME_CLOCK
{
    public:
        void init(void);
        
    private:
        void sendNTPpacket(IPAddress& address);
};

extern REAL_TIME_CLOCK RTC;

#endif /* __RTC_H__ */
