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

#include "TimerX.h"
#include <e32math.h>
#include "EventLogger.h"

const TInt KMaxMinLimit = 30;
const TInt KSecondsLimit = 30 * 60;
const TInt KMinToMicroSec = 60 * 1000000;
const TInt KSecToMicroSec = 1000000;

CTimerX* CTimerX::NewL(MTimerXCallback* aTimerCallback)
{
    CTimerX* self = new (ELeave) CTimerX(aTimerCallback);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CTimerX::~CTimerX()
{
    Cancel();
    iSystemTimer.Close();
}

void CTimerX::StartSecondTimerL(TInt aTimerSeconds, TBool aRepeatTimer)
{
    if(IsActive())
        StopTimerL();
	iTimerMode = ESecondMode;
	iOrigTime = iRemTime = aTimerSeconds;
	iRepeatTimer = aRepeatTimer;
	SetTimer();
}

void CTimerX::StartMinTimerL(TInt aTimerMins, TBool aRepeatTimer)
{
    if(IsActive())
        StopTimerL();
    iTimerMode = EMinuteMode;
    iRepeatTimer = aRepeatTimer;
    iOrigTime = iRemTime = aTimerMins;
    SetTimer();
}

void CTimerX::StartMicroSecTimerL(TInt aTimerMicroSeconds, TBool aRepeatTimer)
{
    if(IsActive())
        StopTimerL();
    iTimerMode = EMicroSecMode;
    iRepeatTimer = aRepeatTimer;
    iOrigTime = iRemTime = aTimerMicroSeconds;
    SetTimer();
}

void CTimerX::StopTimerL()
{
    Cancel();
}

CTimerX::CTimerX(MTimerXCallback* aTimerCallback):
    CActive(EPriorityStandard),
    iTimerCallback(aTimerCallback)
{
    iTimerId = Math::Random() + 1;
}

void CTimerX::ConstructL()
{
    iSystemTimer.CreateLocal();
    CActiveScheduler::Add(this);
}

void CTimerX::RunL()
{
	switch(iTimerMode){
		case ESecondMode:
		case EMinuteMode:
		case EMicroSecMode:{
			if(!iRemTime && iTimerCallback){
				iTimerCallback->HandleTimerExpireL(iTimerId);
				if(iRepeatTimer){
					iRemTime = iOrigTime;
					SetTimer();
				}
			}
			else {
				//set the next timer
				SetTimer();
			}		
		}
		break;
		default:
		case EModeNone:
			break;
	}
}

void CTimerX::DoCancel()
{
    iSystemTimer.Cancel();
    iOrigTime = iRemTime = 0;
}

void CTimerX::SetTimer()
{
	TInt limit = 0;

	switch(iTimerMode){
		case ESecondMode:{ 	limit = KSecondsLimit;	}	break;
		case EMinuteMode:{	limit = KMaxMinLimit;  }   break;
		case EMicroSecMode:{  limit = iRemTime;  }   break;
	}
	
	TInt finalTime = 0;
	if((iRemTime - limit) < limit){
		finalTime = iRemTime;
		iRemTime -= iRemTime;//0
	}
	else{
		finalTime = iRemTime - limit;
		iRemTime -= limit;
	}
	
	switch(iTimerMode){
		case ESecondMode:{	iSystemTimer.After(iStatus,finalTime  * KSecToMicroSec); }	break;
		case EMinuteMode:{  iSystemTimer.After(iStatus,finalTime * KMinToMicroSec);  }	break;
		case EMicroSecMode:{iSystemTimer.After(iStatus,finalTime );   } break;
	}
	SetActive();
}

void CTimerX::SetRepeat(TBool aRepeatEnabled)
{
    iRepeatTimer =  aRepeatEnabled;
}
