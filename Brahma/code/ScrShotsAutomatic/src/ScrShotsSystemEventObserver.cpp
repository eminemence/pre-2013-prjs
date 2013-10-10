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

#include "ScrShotsSystemEventObserver.h"
#include <apgtask.h>
#include <eikenv.h>
#include <apgcli.h> // for RApaLsSession

#include "EventLogger.h"

CScrShotsSystemEventObserver* CScrShotsSystemEventObserver::NewL(MSystemEventObserver* aObserver)
{
	CScrShotsSystemEventObserver* self = new (ELeave) CScrShotsSystemEventObserver(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
}

CScrShotsSystemEventObserver::~CScrShotsSystemEventObserver()
{
    delete iProfileWatcher;
    StopWatchingForExit(ETrue);
    if(iAppLaunched){
        if(iOpenedExistingApp){
            iStartAppThread.Close();
            iOpenedExistingApp = EFalse;
        }
        else 
        iStartAppProc.Close();
    }
}

CScrShotsSystemEventObserver::CScrShotsSystemEventObserver(MSystemEventObserver* aObserver):
		CActive(EPriorityStandard)
		    ,iObserver(aObserver)
            ,iSelectedAppWgId(-1)
{
    CActiveScheduler::Add(this);
}

void CScrShotsSystemEventObserver::ConstructL()
{
    iProfileWatcher = ProEngFactory::NewNotifyHandlerL();
    iProfileWatcher->RequestProfileActivationNotificationsL(*this);
}

TBool CScrShotsSystemEventObserver::LaunchAppAndStartAppMonitorL(const TDesC& aAppName)
{
    if(IsActive()){
        StopWatchingForExit(EFalse);
    }
    SLOGF(aAppName);
    // NOTE::this function needs some serious tweaking.!!!
    TApaTaskList taskList( CEikonEnv::Static()->WsSession() );    
    RApaLsSession rs;
    if(rs.Connect()!= KErrNone)
        return EFalse;
    CleanupClosePushL(rs);
    TApaAppInfo appInfo;
    rs.GetFilteredApps(TApaAppCapability::ENonNative,TApaAppCapability::ENotEmbeddable);
    TInt appCount = 0;
    rs.AppCount(appCount);
    TApaTask task = taskList.FindApp( aAppName );
    for(TInt i = 0; i < appCount; i++){
        rs.GetNextApp(appInfo);
        
#if BRAHMA_DEBUG
        CApaAppViewArray* viewArr = new (ELeave) CApaAppViewArray(10); 
        rs.GetAppViews(*viewArr,appInfo.iUid);
        RDebug::Printf("Getting views for this app : %d",viewArr->Count());
        for(int i = 0 ;i < viewArr->Count() ; i++){
            TApaAppViewInfo& appviewinfo = viewArr->At(i);
            RDebug::Print(appviewinfo.iViewCaption);
        }
        delete viewArr;
#endif
        
//        TFindProcess proc;
//        proc.Find(aAppName);
        SLOGF(appInfo.iShortCaption);
        if(appInfo.iShortCaption.Compare(aAppName) == 0){
            if(task.Exists()){
                task.BringToForeground();
                iSelectedAppWgId = task.WgId();
                TInt eCode = iStartAppThread.Open(task.ThreadId());
#if BRAHMA_DEBUG                
                TBuf<256> msg;
                msg.Format(_L("eCode code : %d"), eCode);
                SLOGF(msg);
#endif                
                if(eCode == KErrNone){
                    iStartAppThread.Logon(iStatus);
                    SLOGF(_L("CAlling SetActive in  CScrShotsSystemEventObserver::LaunchAppAndStartAppMonitorL#1"));
                    SetActive();
                    iOpenedExistingApp = ETrue;
                    iAppLaunched = ETrue;
                    iMode = EWaitingForExit;
                }
            }
            else{
                TInt errCode = iStartAppProc.Create(appInfo.iFullName,KNullDesC);
#if BRAHMA_DEBUG                
                TBuf<256> msg;
                msg.Format(_L("error code : %d"), errCode);
                SLOGF(msg);
#endif                
                if(errCode == KErrNone){
                    iStartAppProc.Resume();
                    iStartAppProc.Logon(iStatus);
                    SLOGF(_L("CAlling SetActive in  CScrShotsSystemEventObserver::LaunchAppAndStartAppMonitorL#2"));
                    SetActive();
                    //get the task list again and find this app to get the window group id
                    TApaTaskList taskList2( CEikonEnv::Static()->WsSession() );
                    TApaTask task2 = taskList2.FindApp( aAppName );
                    iSelectedAppWgId = task2.WgId();
                    iOpenedExistingApp = EFalse;
                    iAppLaunched = ETrue;
                    iMode = EWaitingForExit;
                }
            }
            break;
        }
    }
    //start watcher till app exits
    CleanupStack::PopAndDestroy(1);//rs
    return iAppLaunched;
}

void CScrShotsSystemEventObserver::RunL()
{
    if(iStatus.Int() == KErrNone){
        switch(iMode){
            case EExitFired:
            case EWaitingForExit:
                if(iOpenedExistingApp){
                    iStartAppThread.Close();
                    iOpenedExistingApp = EFalse;
                }
                else
                    iStartAppProc.Close();
                iAppLaunched = EFalse;
                iObserver->HandleSystemEventL(EAppClosed);
                break;
        }
    }
}

void CScrShotsSystemEventObserver::DoCancel()
{
    StopWatchingForExit(ETrue);
}

void CScrShotsSystemEventObserver::StopWatchingForExit(TBool aCalledOnExit)
{
    if(iAppLaunched){
        if(iOpenedExistingApp){
            iStartAppThread.LogonCancel(iStatus);
            iOpenedExistingApp = EFalse;
        }
        else        
            iStartAppProc.LogonCancel(iStatus);
        
        if(aCalledOnExit){
            User::WaitForRequest(iStatus);
        }
        else{ 
            //Wrongly interpreted that LogonCancel requires a setactive to complete!! :)
            //so this being commented is corrected
            //SetActive();
            iMode = EExitFired;
        }
    }
    iAppLaunched = EFalse;
}

void CScrShotsSystemEventObserver::HandleProfileActivatedL(TInt aProfileId)
{
    const TInt KGeneralProfielId = 0;// General profile (default value) 
    const TInt KSilentProfileId = 1;//1 = Silent profile 
    const TInt KMeetingProfileId = 2;//2 = Meeting profile 
    const TInt KOutdoorProfileId = 3;//3 = Outdoor profile 
    const TInt KPagerProfileId = 4;//4 = Pager profile 
    const TInt KOfflineProfileId = 5;//5 = Off-line profile 
    const TInt KDriveProfileId = 6;//6 = Drive profile 
//    const TInt KUserCreatedLowerRangeProfileId = 30;//30-49 = User-created profiles 
//    const TInt KUserCreatedUpperRangeProfileId = 49;//30-49 = User-created profiles

    switch(aProfileId){
        case KGeneralProfielId:
        case KOfflineProfileId:
        case KOutdoorProfileId:
        case KDriveProfileId:
        default:
            iObserver->HandleSystemEventL(ESilentModeDeactivated);
            break;
        case KSilentProfileId:
        case KMeetingProfileId:
        case KPagerProfileId:
            //sound-light should be off
            iObserver->HandleSystemEventL(ESilentModeActivated);
            break;
    }
}

void CScrShotsSystemEventObserver::HandleProfileActivationNotificationError(TInt /*aErrorCode*/) 
{
    //can't get notification so turn off the sound-light
}
