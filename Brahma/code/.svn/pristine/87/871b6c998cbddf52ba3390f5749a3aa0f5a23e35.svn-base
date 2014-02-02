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

#ifndef __TIMERX_H__
#define __TIMERX_H__

#include <e32base.h>

// To overcome the RTimer limit of 35 mins, here comes CTimerX.
// Theoretically the limit here 2^31 mins, which is approx 1491308 days.
// This should suffice for most uses.
// There are two mode one is the second mode, the other is minute mode.
// If something needs second specific granularity, use the second mode.
// The limit of the second mode is 2^ 31 seconds / 1000000 = 35 mins again :)


class MTimerXCallback 
{
public:
    virtual void HandleTimerExpireL(TInt aTimerId) = 0;
};

class CTimerX : public CActive
{
    public:
        static CTimerX* NewL(MTimerXCallback* aTimerCallback);
        ~CTimerX();
        void StartSecondTimerL(TInt aTimerSeconds, TBool aRepeatTimer);
        void StartMinTimerL(TInt aTimerMins, TBool aRepeatTimer);
        void StartMicroSecTimerL(TInt aTimerMicroSeconds, TBool aRepeatTimer);
        void StopTimerL();
        void SetRepeat(TBool aRepeatEnabled);
        TUint32 GetTimerId(){ return iTimerId; }
    private:
        explicit CTimerX(MTimerXCallback* aTimerCallback);
        void ConstructL();
        void SetTimer();
        
    public:
        void RunL();
        void DoCancel();
    private:
        //for internal state
        enum TTimerMode{
        	EModeNone,
        	ESecondMode,
        	EMinuteMode,
        	EMicroSecMode
        };
    private:
        TTimerMode	iTimerMode;
        RTimer  	iSystemTimer;
        TInt   		iOrigTime;
        TInt   		iRemTime;
        MTimerXCallback* iTimerCallback;
        TBool   	iRepeatTimer;
        TUint32		iTimerId;
};

#endif //__TIMERX_H__
