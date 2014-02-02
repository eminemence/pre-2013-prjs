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

#include "ScrShotsDeviceWatcher.h"
#include "ScrShotsSettings.h"
#include <bautils.h>
#include <eikenv.h>
#include <mw/ProEngFactory.h>
#include <mw/MProEngEngine.h>
#include <mw/MProEngProfile.h> 
#include "ScrShotsStringPool.h"
#include "EventLogger.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsGenUtils.h"

const TUid KGUIAPPUID = { 0x100039CE};

//_LIT(KCustomTtfFontName,"c:\\system\\apps\\2006508E\\ScrShotsCustomFont.ttf");
//_LIT(KCustomGdrFontName,"c:\\system\\apps\\2006508E\\square_future.gdr");

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif

//-----------------------------------------------------------------------------
//----------------CAppInstallerWatcher impl starts here----------------------
//-----------------------------------------------------------------------------

enum TInstOp
 {
     EInstOpNone = 0x00000000,
     EInstOpInstall = 0x00000001,
     EInstOpUninstall = 0x00000002,
     EInstOpRestore = 0x00000004
 };

CAppInstallerWatcher* CAppInstallerWatcher::NewL(MInstallerWatcher* aInstallerWatcher)
    {
    CAppInstallerWatcher* self = new (ELeave) CAppInstallerWatcher(aInstallerWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    
    }

CAppInstallerWatcher::~CAppInstallerWatcher()
    {
        Cancel();
        iAppInstallWatcher.Close();
        iTimer.Close();
    }

void CAppInstallerWatcher::StartInstallerWatcherL()
    {
        iAppInstallWatcher.Subscribe(iStatus);
        SetActive();
        iAppMode = EWatchingEvent;
    }

void CAppInstallerWatcher::StopInstallerWatcherL()
    {
        Cancel();
    }

CAppInstallerWatcher::CAppInstallerWatcher(MInstallerWatcher* aInstallerWatcher):
        CActive(EPriorityStandard)
,iInstallerWatcher(aInstallerWatcher)
    {
        CActiveScheduler::Add(this);
    }

void CAppInstallerWatcher::ConstructL()
    {
    iTimer.CreateLocal();
    iAppInstallWatcher.Attach(KUidSystemCategory, KSAUidSoftwareInstallKeyValue);
    StartInstallerWatcherL();
    }

void CAppInstallerWatcher::RunL()
    {
        if(iStatus.Int() == KErrNone){
            SLOGF(_L("Got install\\uninstall event....."));
            switch(iAppMode){
                case EWatchingEvent:{
                    iTimer.After(iStatus, TTimeIntervalMicroSeconds32(15000000));
                    SetActive();
                    SLOGF(_L("Timer fired , so that we have fresh app list..."))
                    iAppMode = ETimerFired;
                }
                    break;
                case ETimerFired:{
                    GetAppInstallerStatus();
                }
                    break;
            }
        }
    }

void CAppInstallerWatcher::GetAppInstallerStatus(){
    TInt instStatus = 0;
    User::LeaveIfError( RProperty::Get( KUidSystemCategory, KSAUidSoftwareInstallKeyValue, instStatus ) );
    TBool appInstalled = (instStatus & EInstOpInstall); 
    //appInstalled = (!appInstalled) ? instStatus & EInstOpUninstall : appInstalled;
    
    if(iInstallerWatcher){
        SLOGF(_L("Got status now refreshing list"));
        iInstallerWatcher->HandleAppInstallerEventL(appInstalled);
    }
    StartInstallerWatcherL();

}

void CAppInstallerWatcher::DoCancel()
    {
        switch(iAppMode){
            case EWatchingEvent:{
                iAppInstallWatcher.Cancel();
            }
                break;
            case ETimerFired:{
                iTimer.Cancel();
            }
                break;
        }
    }
 

//-----------------------------------------------------------------------------
//----------------CMemCardWatcher impl starts here----------------------
//-----------------------------------------------------------------------------


CMemCardWatcher* CMemCardWatcher::NewL(MMemCardWatcher* aMemCardWatcher)
{
    CMemCardWatcher* self = new (ELeave) CMemCardWatcher(aMemCardWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CMemCardWatcher::~CMemCardWatcher()
{
    Cancel();
    iFs.Close();
}

void CMemCardWatcher::StartWatchingMemCardL(TInt aDriveNumber)
{
    if(IsActive())
        Cancel();
    //check the drives current status
    TVolumeInfo vInfo;
    if(KErrNone != iFs.Volume(vInfo, aDriveNumber))
        iIsDriveAvailable = EFalse;
    else
        iIsDriveAvailable = ETrue;
    
    iFs.NotifyChange(ENotifyDisk, iStatus);
    SetActive();
    iDriveNumber = aDriveNumber;
}

void CMemCardWatcher::StopWatchingMemCardL()
{
    Cancel();
}

CMemCardWatcher::CMemCardWatcher(MMemCardWatcher* aMemCardWatcher):
        CActive(EPriorityStandard)
    ,iMemCardWatcher(aMemCardWatcher)
    {
    CActiveScheduler::Add(this);
    }

void CMemCardWatcher::ConstructL()
{
    User::LeaveIfError(iFs.Connect());
}

void CMemCardWatcher::RunL()
{
    if(iStatus.Int() == KErrNone){
        TVolumeInfo vInfo;
        if(KErrNone != iFs.Volume(vInfo, iDriveNumber)){
            if(iIsDriveAvailable){
                iMemCardWatcher->HandleMemCardEventL(EFalse);
                iIsDriveAvailable = EFalse;
            }
        }
        else{
            if(!iIsDriveAvailable){
                //mem card was not available before
                iMemCardWatcher->HandleMemCardEventL(ETrue);
                iIsDriveAvailable = ETrue;
            }
        }
        if(!IsActive())
        {
           iFs.NotifyChange(ENotifyDisk, iStatus);
           SetActive();
        }
    }
}

void CMemCardWatcher::DoCancel()
{
    iFs.NotifyChangeCancel();
}

//-----------------------------------------------------------------------------
//----------------CDiskWatcher impl starts here----------------------
//-----------------------------------------------------------------------------

CDiskWatcher* CDiskWatcher::NewL(MDiskWatcher* aDiskWatcher)
{
    CDiskWatcher* self = new (ELeave) CDiskWatcher(aDiskWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CDiskWatcher::~CDiskWatcher()
{
    Cancel();
    iDiskWatchTimer.Close();
    iFs.Close();
}

void CDiskWatcher::StartWatchingDiskL(TInt aDriveNumber,TChar aDriveChar, TInt64 aThresholdLimit)
{
    if(IsActive())
        StopWatchingDiskL();
    iDriveToWatch = aDriveChar;
    iDriveNumber = aDriveNumber;
    iThresholdLimit = aThresholdLimit;
    iDiskWatchTimer.After(iStatus, TTimeIntervalMicroSeconds32(5000000));
    SetActive();
}

void CDiskWatcher::StopWatchingDiskL()
{
    Cancel();
}

CDiskWatcher::CDiskWatcher(MDiskWatcher* aDiskWatcher):
        CActive(EPriorityStandard),
        iDiskWatcher(aDiskWatcher)
{
    CActiveScheduler::Add(this);
}

void CDiskWatcher::ConstructL()
{
    User::LeaveIfError(iFs.Connect());
    User::LeaveIfError(iDiskWatchTimer.CreateLocal());
}

void CDiskWatcher::RunL()
{
    if(iStatus.Int() == KErrNone){
        //request has completed so need to do something abt it
        TVolumeInfo volInfo;
        iFs.Volume(volInfo, iDriveNumber);
        if(volInfo.iFree < iThresholdLimit)
            iDiskWatcher->HandleDiskThresholdEventL();
        //watch again
        iDiskWatchTimer.After(iStatus, TTimeIntervalMicroSeconds32(5000000));
        SetActive();
    }
}

void CDiskWatcher::DoCancel()
{
    iDiskWatchTimer.Cancel();
}

TBool CDiskWatcher::DoesDiskHaveSpace()
{
    TVolumeInfo volInfo;
    iFs.Volume(volInfo, iDriveNumber);
    if(volInfo.iFree < iThresholdLimit)
        return EFalse;
    return ETrue;
}


//-----------------------------------------------------------------------------
//----------------CScrShotsDeviceWatcher impl starts here----------------------
//-----------------------------------------------------------------------------
CScrShotsDeviceWatcher* CScrShotsDeviceWatcher::NewL(CScrShotsSettings& aAppSettings,MDeviceWatcher* aDeviceWatcher)
{
    CScrShotsDeviceWatcher* self = new (ELeave) CScrShotsDeviceWatcher(aAppSettings,aDeviceWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CScrShotsDeviceWatcher::~CScrShotsDeviceWatcher()
{
//    UnloadCustomFontL();
    delete iAppListWatcher;
    delete iMemCardWatcher;
    delete iDiskWatcher;
    iFs.Close();
    iAppsInfoArray.ResetAndDestroy();
    iAppsInfoArray.Close();
    iAppsIconArray.ResetAndDestroy();
    iAppsIconArray.Close();
    
    iDrvsInfoArray.Close();
    delete iFolderArray;
    iCurrentRootPath.Close();
    iFolderInfoArray.Close();
    delete iCallMonitor;
    
}

const RArray<TDriveExtraInfo>& CScrShotsDeviceWatcher::GetDriveListWithInfo(TBool aGetCachedValue)
{
    
    //user has requested a fresh copy or we do not have any entries in the list, just fill it up
    if(!aGetCachedValue || !iDrvsInfoArray.Count()){
        TBuf<2> drvName;
        iAppSettings.OutputFolderDrive(drvName);
        SLOGF(drvName);
        //just reset the entries
        iDrvsInfoArray.Reset();
        //add up the new entries
        TDriveList list;
        iFs.DriveList(list);
        for(int i = 0 ;i < list.Length(); i++){
            if(list[i]){
                TChar dChar;
                iFs.DriveToChar(i, dChar);
                TVolumeInfo dInfo;
                iFs.Volume(dInfo, i);
                if(dInfo.iDrive.iDriveAtt == KDriveAttRom || 
                        dInfo.iDrive.iDriveAtt == KDriveAttRedirected ||
                        dInfo.iDrive.iDriveAtt == KDriveAttSubsted )
                    continue;
                
                if(dInfo.iDrive.iMediaAtt == KMediaAttWriteProtected 
                        || dInfo.iDrive.iMediaAtt == KMediaAttLocked)
                    continue;
                
                if(dInfo.iDrive.iType == EMediaNotPresent || 
                        dInfo.iDrive.iType == EMediaUnknown ||
                        dInfo.iDrive.iType == EMediaFloppy ||
                        dInfo.iDrive.iType == EMediaCdRom ||
                        dInfo.iDrive.iType == EMediaRam ||
                        dInfo.iDrive.iType == EMediaRom)
                    continue;

                TDriveExtraInfo inf;
                inf.iDrvNumber = i;
                if(drvName[0] == dChar){
                    inf.iCurrentTargetDrive = ETrue;
                    inf.iThresholdPercent = iAppSettings.DiskFillThreshold();
                }
                else{
                    inf.iCurrentTargetDrive = EFalse;
                    inf.iThresholdPercent = KDefaultThresholdPercent;
                }

                inf.iDriveChar = dChar;
                inf.iDrvFreeSize = dInfo.iFree;
                inf.iDrvTotalSize = dInfo.iSize;
#if BRAHMA_DEBUG                
                TBuf<256> msg;
                msg.Format(_L("drive char : %c"), dChar.GetUpperCase());
                SLOGF(msg)
                msg.Format(_L("drive freesize : %ld"), dInfo.iFree);
                SLOGF(msg)
                msg.Format(_L("drive total size : %ld"), dInfo.iSize);
                SLOGF(msg)
#endif                
                inf.iDriveType = dInfo.iDrive.iType;
                //this is the check for e:(mass storage), as it is also reported as removable, so internal will help to differentiate with memcard
                if(dChar.GetUpperCase() > TChar('C')){
                    inf.iDriveIsRemoveable = ((dInfo.iDrive.iDriveAtt & KDriveAttRemovable) && (dInfo.iDrive.iDriveAtt & KDriveAttInternal)) ? EFalse : ETrue;
                    if(inf.iDriveIsRemoveable){
                        if(!iMemCardWatcher)
                            iMemCardWatcher = CMemCardWatcher::NewL(this);
                        else
                            iMemCardWatcher->StopWatchingMemCardL();
                        
                        iMemCardWatcher->StartWatchingMemCardL(inf.iDrvNumber);
                    }
                }
                else
                    inf.iDriveIsRemoveable = EFalse; 
                iDrvsInfoArray.Append(inf);
            }
        }
    }
    return iDrvsInfoArray;
}

void CScrShotsDeviceWatcher::StartWatchingDriveThresholdL(TChar aDriveChar, TInt aThresholdPercent)
{
    TBool driveFound = EFalse;
    TInt drvNumber = 0;
    TInt64 limit = 0;
    
    //get the latest drive stats
    GetDriveListWithInfo(EFalse);
    
    for(int i = 0;i < iDrvsInfoArray.Count() ; i++){
        TDriveExtraInfo dvInfo =iDrvsInfoArray[i];
        aDriveChar.UpperCase();
        TChar upcase(aDriveChar);
        aDriveChar.LowerCase();
        TChar locase(aDriveChar);
        if(dvInfo.iDriveChar == upcase || dvInfo.iDriveChar == locase){
            dvInfo.iCurrentTargetDrive = ETrue;
            dvInfo.iThresholdPercent = aThresholdPercent;
            drvNumber = dvInfo.iDrvNumber;
            driveFound = ETrue;
            limit = ((100.0-aThresholdPercent)/100.0) * dvInfo.iDrvTotalSize;
//            if(limit < (dvInfo.iDrvTotalSize - dvInfo.iDrvFreeSize)){
            if(limit > dvInfo.iDrvFreeSize){
                //this is probably the case when we cannot specify precision values
                //so just setting the current free size as the limit which should not be crossed.
                //probably this is what the user wants.
                limit = dvInfo.iDrvFreeSize;
                //TRACE_ON(RDebug::Printf("ThresholdPercent specified is out of bounds"));
                //User::Leave(KErrArgument);
            }
            break;
        }
    }
    if(!driveFound){
        TRACE_ON(RDebug::Printf("DriveChar specified was not found");)
        User::Leave(KErrArgument);
    }
    iDiskWatcher->StartWatchingDiskL(drvNumber,aDriveChar, limit);
}

void CScrShotsDeviceWatcher::StopWatchingDriveThresholdL()
{
    iDiskWatcher->StopWatchingDiskL();
}

void CScrShotsDeviceWatcher::HandleDiskThresholdEventL()
{
    if(iDeviceWatcher){
        //notify the ppl to stop producing the screenshots
        iDeviceWatcher->HandleDiskThresholdReachedEventL();
    }
}

CScrShotsDeviceWatcher::CScrShotsDeviceWatcher(CScrShotsSettings& aAppSettings,MDeviceWatcher* aDeviceWatcher)
:iAppSettings(aAppSettings)
,iDeviceWatcher(aDeviceWatcher)
{

}

void CScrShotsDeviceWatcher::ConstructL()
{
    
    User::LeaveIfError(iFs.Connect());
    iDiskWatcher = CDiskWatcher::NewL(this); 
    GetDriveListWithInfo(ETrue);
    GetAppListWithInfoL(ETrue);
    iCurrentRootPath.Create(1024);
    iCallMonitor = CCallMonitor::NewL(this);
    iAppListWatcher = CAppInstallerWatcher::NewL(this);
}

void CScrShotsDeviceWatcher::StopWatchingDeviceParamsL()
{
    StopWatchingDriveThresholdL();
}

const RPointerArray<TApaAppInfo>& CScrShotsDeviceWatcher::GetAppListWithInfoL(TBool aGetCachedValue)
{
    if(aGetCachedValue && iAppsInfoArray.Count())
        return iAppsInfoArray;
    iAppsInfoArray.ResetAndDestroy();
    iAppsIconArray.ResetAndDestroy();
    
    RApaLsSession appLs;
    User::LeaveIfError(appLs.Connect());
    //TApaEmbeddabilityFilter filter;
    //filter.AddEmbeddability(TApaAppCapability::ENotEmbeddable);
    //appLs.GetFilteredApps(filter);
    appLs.GetAllApps();
    TApaAppInfo dummyAppInfo;
    CArrayFixFlat<TSize>* iconArraySizes = new(ELeave)CArrayFixFlat<TSize>(3);
    CleanupStack::PushL(iconArraySizes);
    TRACE_ON(RDebug::Printf("Getting apps list now...");)
    TInt appCount = 0;
    while(appLs.GetNextApp(dummyAppInfo)!= RApaLsSession::ENoMoreAppsInList){
        appCount++;
#if BRAHMA_DEBUG        
        TRACE_ON(RDebug::Print(dummyAppInfo.iFullName);)
        TRACE_ON(RDebug::Print(dummyAppInfo.iShortCaption);)
#endif        
        if(dummyAppInfo.iShortCaption[0] == '!' || dummyAppInfo.iShortCaption.Length() == 0)
            continue;
        TStringPoolObj fullName(512);
        fullName.Des().Copy(dummyAppInfo.iFullName);
        fullName.Des().LowerCase();
        //we dont want to show Brahma in the app list.
        if(dummyAppInfo.iUid == KBrahmaUid)
            continue;
        //filtering out midlets (.fakeapp) & wrt widgets (.html)
        if(fullName.Des().Find(_L(".fakeapp")) != KErrNotFound || fullName.Des().Find(_L(".html")) != KErrNotFound)
            continue;
        TApaAppCapabilityBuf appCap;
        if(appLs.GetAppCapability(appCap, dummyAppInfo.iUid) == KErrNone){
            //if its hidden its a ui app 
            if(appCap().iAppIsHidden || appCap().iAttributes == TApaAppCapability::EControlPanelItem)
                continue;
        }
        //now get the app icon
        iconArraySizes->Reset();        
        TInt errcode = appLs.GetAppIconSizes(dummyAppInfo.iUid,*iconArraySizes);        
        TApaAppInfo* appInfo = new TApaAppInfo;
        //Mem::FillZ(appInfo, sizeof(TApaAppInfo));
        appInfo->iCaption.Copy(dummyAppInfo.iCaption);
        appInfo->iFullName.Copy(dummyAppInfo.iFullName);
        appInfo->iShortCaption.Copy(dummyAppInfo.iShortCaption);
        appInfo->iUid = dummyAppInfo.iUid;
        iAppsInfoArray.Append(appInfo);
        //adding the default icon info and then checking if there are any icons to add.
        TAppIconInfo* appIcon = new TAppIconInfo;
        appIcon->iBmpIndex = iAppsInfoArray.Count() - 1;
        appIcon->iDummyIcon = ETrue;
        TRACE_ON(RDebug::Printf("errCode : %d iconarraysize : %d", errcode, iconArraySizes->Count());)
        if((errcode == KErrNone) && iconArraySizes->Count())
        {
            CApaMaskedBitmap* iconOfSize = CApaMaskedBitmap::NewLC();
            TBool iconAdded = EFalse;
            for(TInt i=0;i < iconArraySizes->Count();i++)
            {
                TSize sz = (*iconArraySizes)[i];
                //not interested in small icons
                TRACE_ON(RDebug::Printf("Size w : %d h : %d",sz.iWidth, sz.iHeight);)
                if(sz.iWidth <= 16 && sz.iHeight <= 16)
                    continue;
               
                if(appLs.GetAppIcon(appInfo->iUid,(*iconArraySizes)[i],*iconOfSize) == KErrNone 
                        && sz.iWidth > 16 && sz.iHeight > 16){
                    appIcon->iBmp = iconOfSize;
                    //add the icon to the array
                    iconAdded = ETrue;
                    appIcon->iDummyIcon = EFalse;
                    break;
                }
            }
            CleanupStack::Pop(iconOfSize);
            if(!iconAdded)
                delete iconOfSize;
        }
        iAppsIconArray.Append(appIcon);
    }
    TRACE_ON(RDebug::Printf("Total app count : %d",appCount);)
    CleanupStack::PopAndDestroy(iconArraySizes);
    appLs.Close();
    return iAppsInfoArray;
}

RArray<TEntry>& CScrShotsDeviceWatcher::GetFolderList(const TDesC& aPathName, TInt& aErrorCode, TBool aForceRefresh)
{
    if(iCurrentRootPath.Compare(aPathName) == 0 && !aForceRefresh ){
        aErrorCode = KErrNone;
        return iFolderInfoArray;
    }
    //now check if we can go inside the folder or not
    CDir*   tmpFolderArray = NULL;
    TInt errCode = KErrNone;
    errCode = iFs.GetDir(aPathName, KEntryAttDir , ESortByName, tmpFolderArray);
    if(errCode != KErrNone){
        delete tmpFolderArray;
        //show a dialog informing user about the error!!
        aErrorCode = errCode;
        return iFolderInfoArray;
    }
    
    if(iCurrentRootPath.MaxLength() < aPathName.Length()){
        iCurrentRootPath.ReAlloc(aPathName.Length());
    }
    iCurrentRootPath.Copy(aPathName);
    if(iFolderArray){
        delete iFolderArray;
        iFolderArray = NULL;
    }
    iFolderInfoArray.Reset();
    if(!tmpFolderArray)
        errCode = iFs.GetDir(iCurrentRootPath, KEntryAttDir , ESortByName, tmpFolderArray);
    aErrorCode = errCode;
    iFolderArray = tmpFolderArray;
    if(errCode == KErrNone){
        int count = iFolderArray->Count();
        for(int  i = 0; i < count; i++){
            if(iFolderArray->operator [](i).IsDir()){
                TEntry entry = iFolderArray->operator [](i);
                iFolderInfoArray.Append(entry);
            }
        }
        return iFolderInfoArray;
    }
}

RArray<TEntry>& CScrShotsDeviceWatcher::GoOneUpAndGetFolderListL(const TDesC& aPathName)
{
    if(iCurrentRootPath.MaxLength() < aPathName.Length()){
        iCurrentRootPath.ReAlloc(aPathName.Length());
    }
    iCurrentRootPath.Copy(aPathName);
    
    TParse pathParse;
    pathParse.Set(iCurrentRootPath,NULL,NULL);
    TInt err = pathParse.PopDir();
    if(!err){
        iCurrentRootPath.Copy(pathParse.FullName());
        if(iFolderArray){
            delete iFolderArray;
            iFolderArray = NULL;
        }
        iFolderInfoArray.Reset();
        TInt errCode = KErrNone;
        errCode = iFs.GetDir(iCurrentRootPath, KEntryAttDir, ESortByName, iFolderArray); 
        if(errCode == KErrNone){
            int count = iFolderArray->Count();
            for(int  i = 0; i < count; i++){
                if(iFolderArray->operator [](i).IsDir()){
                    TEntry entry = iFolderArray->operator [](i);
                    iFolderInfoArray.Append(entry);
                }
            }            
            return iFolderInfoArray;
        }
        else
            User::Leave(errCode);
    }
    else //we are at root directory so cannot pop anymore, just return the old array
        return iFolderInfoArray;
}
#if 0
void CScrShotsDeviceWatcher::LoadCustomFontL()
{
    CWsScreenDevice* scrDevice = CEikonEnv::Static()->ScreenDevice();
    iCustomFontFileID = 0x100001;
    TInt errCode = scrDevice->AddFile(KCustomGdrFontName(),iCustomFontFileID);
    User::LeaveIfError(errCode);
    RDebug::Printf("ScrShotsAutomatic::Custom font has loaded");
    TUid fontUid;
    fontUid.iUid = iCustomFontFileID;
    TAlgStyle algStyle;
    algStyle.SetHeightFactor(180);
    algStyle.SetIsMono(ETrue);
    errCode = scrDevice->GetFontById(iCustomFont, fontUid, algStyle);
    if(errCode){//fallback font
        TFontSpec myFontSpec;
        myFontSpec.iHeight = 180;
        myFontSpec.iTypeface.iName=_L("M39_SQUAREFUTURE");
        errCode = scrDevice->GetNearestFontInTwips(iCustomFont,myFontSpec);
        User::LeaveIfError(errCode);
    }
}

void CScrShotsDeviceWatcher::UnloadCustomFontL()
{
    CEikonEnv::Static()->ScreenDevice()->ReleaseFont(iCustomFont);
    CEikonEnv::Static()->ScreenDevice()->RemoveFile(iCustomFontFileID);
}
#endif
TBool CScrShotsDeviceWatcher::IsProfileSilentL()
{
    MProEngEngine *proEng = ProEngFactory::NewEngineLC();
    MProEngProfile* profile = proEng->ActiveProfileL();
    CleanupReleasePushL(*profile);
    TBool issilent = profile->IsSilent();
    CleanupStack::PopAndDestroy(2);
    return issilent;
    
}
void CScrShotsDeviceWatcher::StartCallMonitor()
{
	iCallMonitor->StartMonitor();
}
void CScrShotsDeviceWatcher::StopCallMonitor()
{
	iCallMonitor->StopMonitor();
}
void CScrShotsDeviceWatcher::NotifyCallL(TBool aEnableSound)
{
	if(iAppSettings.IsSoundOn() && !aEnableSound)
		iAppSettings.SetSoundL(aEnableSound);
		
}

void CScrShotsDeviceWatcher::HandleMemCardEventL(TBool aMemCardAdded)
{
    TBuf<2> settingsDrv;
    TChar drvChar;
    iFs.DriveToChar(iMemCardWatcher->GetMemCardWatchDriveNumber(), drvChar);
    if(aMemCardAdded){
        SLOGF(_L("memory card added..."));
        //update drive grid
        if(iDeviceWatcher)
            iDeviceWatcher->HandleMemCardEventL(aMemCardAdded);
    }
    else {
        SLOGF(_L("memory card removed..."));
        //if mem card was in setting , then make sure that the default drive has been set
        iAppSettings.OutputFolderDrive(settingsDrv);
        SLOGF(_L("memory card removed1..."));
        SLOGF(settingsDrv);
        drvChar.LowerCase();
        TBool lowerCase = (settingsDrv[0] == drvChar);
        drvChar.UpperCase();
        TBool upperCase = (settingsDrv[0] == drvChar);
        TBuf<2> drchar;
        drchar.Append(drvChar);
        SLOGF(_L("memory card removed2..."));
        SLOGF(drchar);
        if(upperCase || lowerCase){
            SLOGF(_L("setting output drive to c:"));
            iAppSettings.SetOutputFolderDriveL(TChar('C'));
            iAppSettings.SetOutputFolderNameL(KDefaultFileFolderName);
            iAppSettings.SetDiskFillThresholdL(KDefaultThresholdPercent);
        }
        //we will fallback on the default threshold value
        StopWatchingDriveThresholdL();
        this->StartWatchingDriveThresholdL(TChar('C'), KDefaultThresholdPercent);
        
        if(iDeviceWatcher)
            iDeviceWatcher->HandleMemCardEventL(aMemCardAdded);
    }
}

void CScrShotsDeviceWatcher::HandleAppInstallerEventL(TBool aAppinstalled)
{
    if(!aAppinstalled){
        //something has been uninstalled, if its the one in settings
        //just make the settings fallback to empty values
        TStringPoolObj appName(256);
        iAppSettings.AppNameForCaptureL(appName.Des());
        if(!ScrShotsGenUtils::CheckIfAppExists(appName.Des(), TUid::Uid(iAppSettings.AppUidForCapture()))){
            iAppSettings.SetAppNameForCaptureL(KEmptyStr());
            iAppSettings.SetAppUidForCaptureL(0);
        }
    }
        
    if(iDeviceWatcher)
        iDeviceWatcher->HandleAppInstallerEventL(aAppinstalled);
}



//-----------------------------------------------------------------------------
//----------------CCallMonitor impl starts here----------------------
//-----------------------------------------------------------------------------

CCallMonitor* CCallMonitor::NewL(MCallNotifier* aCallNotifier)
{
	CCallMonitor* self = CCallMonitor::NewLC(aCallNotifier);
	CleanupStack::Pop(self);
	return self;
}
CCallMonitor* CCallMonitor::NewLC(MCallNotifier* aCallNotifier)
{
	CCallMonitor* self = new (ELeave) CCallMonitor();
	CleanupStack::PushL(self);
	self->ConstructL(aCallNotifier);
	return self;
}

CCallMonitor::~CCallMonitor()
{
	if(IsActive())
		Cancel();
	if(iTelephony)
	{
		delete iTelephony;
		iTelephony= NULL;
	}
}
void CCallMonitor::StartMonitor()
{
	if(IsActive())
		Cancel();
		
	if(iTelephony)
	{
		iTelephony->NotifyChange(iStatus,CTelephony::EVoiceLineStatusChange,iCallStatusPckg);
		SetActive();
	}
}
void CCallMonitor::StopMonitor()
{
	if(iTelephony)
		iTelephony->CancelAsync(CTelephony::EVoiceLineStatusChangeCancel);	
}
void CCallMonitor::RunL()
{
	TInt status = iStatus.Int();
	if(status == KErrNone)
	{
		CTelephony::TCallStatus callStatus = iCallStatusPckg().iStatus;
		switch(callStatus)
		{
			case CTelephony::EStatusRinging:
			case CTelephony::EStatusDialling:
			{
				iCallNotifier->NotifyCallL(EFalse);
				break;
			}
			case CTelephony::EStatusAnswering:
			case CTelephony::EStatusConnecting:
			case CTelephony::EStatusConnected:
			{
				break;
			}
			case CTelephony::EStatusDisconnecting:
			case CTelephony::EStatusIdle:
			{
				iCallNotifier->NotifyCallL(ETrue);
				break;
			}
			default:
			break;
			
		}
		iTelephony->NotifyChange(iStatus,CTelephony::EVoiceLineStatusChange,iCallStatusPckg);
		SetActive();

	}
}
void CCallMonitor::DoCancel()
{
	StopMonitor();
}
CCallMonitor::CCallMonitor()
: CActive(EPriorityStandard),
iCallStatusPckg(CTelephony::TCallInfoV1())
{
	CActiveScheduler::Add(this);
}
void CCallMonitor::ConstructL(MCallNotifier* aCallNotifier)
{
	iTelephony = CTelephony::NewL();
	iCallNotifier = aCallNotifier;
}



