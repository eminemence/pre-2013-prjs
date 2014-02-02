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

#ifndef __SCRSHOTSSYSTEMEVENTOBSERVER_H__
#define __SCRSHOTSSYSTEMEVENTOBSERVER_H__

#include <e32base.h>
#include <mw/ProEngFactory.h> 
#include <mw/MProEngNotifyHandler.h> 
#include <mw/MProEngProfileActivationObserver.h> 

enum TSystemEventsList{
    EAppLaunched,
    EAppNotFound,
    EAppClosed,
    ESilentModeActivated,
    ESilentModeDeactivated
};

const TInt KGeneralProfielId = 0;// General profile (default value) 
const TInt KSilentProfileId = 1;//1 = Silent profile 
const TInt KMeetingProfileId = 2;//2 = Meeting profile 
const TInt KOutdoorProfileId = 3;//3 = Outdoor profile 
const TInt KPagerProfileId = 4;//4 = Pager profile 
const TInt KOfflineProfileId = 5;//5 = Off-line profile 
const TInt KDriveProfileId = 6;//6 = Drive profile 
const TInt KUserCreatedLowerRangeProfileId = 30;//30-49 = User-created profiles 
const TInt KUserCreatedUpperRangeProfileId = 49;//30-49 = User-created profiles



//monitor launching of apps
class MSystemEventObserver
{
public:
	virtual void HandleSystemEventL(TSystemEventsList aEvent) = 0;
};

class CScrShotsSystemEventObserver : public CActive, MProEngProfileActivationObserver
{
public:
	static CScrShotsSystemEventObserver* NewL(MSystemEventObserver* aObserver);
	~CScrShotsSystemEventObserver();
private:
	explicit CScrShotsSystemEventObserver(MSystemEventObserver* aObserver);
	void ConstructL();
	void RunL();
	void DoCancel();
public:
	TBool LaunchAppAndStartAppMonitorL(const TDesC& aAppName);
	void StopWatchingForExit(TBool aCalledOnExit = EFalse);
	TInt GetSelectedAppWindowGroupId(){
	    return iSelectedAppWgId;
	}
	
public:
	//from MProEngProfileActivationObserver
	void HandleProfileActivatedL(TInt aProfileId);
	void HandleProfileActivationNotificationError(TInt aErrorCode) ;
private:
	enum TSystemMode{
	    EWaitingForExit,
	    EExitFired
	};
	MSystemEventObserver* iObserver;
	RProcess    iStartAppProc;
	RThread     iStartAppThread;
	TBool       iAppLaunched;
	TSystemMode iMode;
	TBool       iOpenedExistingApp;
	MProEngNotifyHandler* iProfileWatcher;
    TInt        iSelectedAppWgId;
};

#endif //__SCRSHOTSSYSTEMEVENTOBSERVER_H__
