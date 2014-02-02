/*
 * Copyright (C) 2011  Mayur Kankanwadi(eminemence@gmail.com)
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


#ifndef __S60UPTIME_PAN__
#define __S60UPTIME_PAN__

/** S60Uptime application panic codes */
enum TS60UptimePanics
	{
	ES60UptimeUi = 1
	// add further panics here
	};

inline void Panic(TS60UptimePanics aReason)
	{
	_LIT(applicationName, "S60Uptime");
	User::Panic(applicationName, aReason);
	}

#endif // __S60UPTIME_PAN__
