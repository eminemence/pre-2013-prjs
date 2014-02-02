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

#ifndef __EXTENDEDTIMER_PAN__
#define __EXTENDEDTIMER_PAN__

/** ExtendedTimer application panic codes */
enum TExtendedTimerPanics
    {
    EExtendedTimerUi = 1
    // add further panics here
    };

inline void Panic(TExtendedTimerPanics aReason)
    {
    _LIT(applicationName, "ExtendedTimer");
    User::Panic(applicationName, aReason);
    }

#endif // __EXTENDEDTIMER_PAN__
