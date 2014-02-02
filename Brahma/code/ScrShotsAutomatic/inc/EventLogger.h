/*
 * Copyright (C) 2011-2012  Mayur Kankanwadi(eminemence@gmail.com)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__


#include <e32std.h>
#include <e32base.h>
#include <flogger.h>
//#define ENABLE_LOGGING 1

#ifdef ENABLE_LOGGING
#define SLOGF(msg) CEventLogger::SLog(msg,__PRETTY_FUNCTION__); 

class CEventLogger:public CBase
{
public:
    //lots of cleanup and reimplementation required!!!!
    //static void SLog(TPtrC aString);
    static void SLog(TPtrC aString,const char* aFunctionName);
};
#else
#define SLOGF(msg) 
#endif

#endif
