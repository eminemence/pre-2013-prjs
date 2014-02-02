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

#ifndef __SCRSHOTSDEVICEWATHCER_H__
#define __SCRSHOTSDEVICEWATHCER_H__

#include <e32base.h>
#include <f32file.h>
#include <apgcli.h>
#include <apgicnfl.h>
#include <Etel3rdParty.h>  //CTelephony
#include <e32property.h>
#include <sacls.h>


class CScrShotsSettings;


struct TDriveExtraInfo{
    TInt    iDrvNumber;
    TInt64  iDrvTotalSize;
    TInt64  iDrvFreeSize;
    TInt64  iThresholdPercent;
    TChar   iDriveChar;
    TBool   iCurrentTargetDrive;
    TMediaType  iDriveType;
    TBool iDriveIsRemoveable;
};

struct TAppExtraInfo{
    TBuf<128> iExeName;
    TBuf<128> iAppCaption;
    TChar iInstallDrive;
};

struct TAppIconInfo{
    CApaMaskedBitmap* iBmp;
    TBool iDummyIcon;
    TInt iBmpIndex;
    TAppIconInfo(){
        iBmp = NULL;
        iDummyIcon = EFalse;
        iBmpIndex = -1;
    }
    ~TAppIconInfo(){
        delete iBmp;
    }
};

//--- App install/uninstall watcher class ---
class MInstallerWatcher{
public:
    //ETrue if app installed, EFalse app uninstalled
    virtual void HandleAppInstallerEventL(TBool aAppinstalled) = 0;
};

class CAppInstallerWatcher : public CActive
{
public:
    static CAppInstallerWatcher* NewL(MInstallerWatcher* aInstallerWatcher);
    ~CAppInstallerWatcher();
    void StartInstallerWatcherL();
    void StopInstallerWatcherL();
    enum TWatchMode{
        ENone,
        EWatchingEvent,
        EEventFired,
        ETimerFired
    };
private:
    explicit CAppInstallerWatcher(MInstallerWatcher* aInstallerWatcher);
    void ConstructL();
    void RunL();
    void DoCancel();
    void GetAppInstallerStatus();
    
private:
    RProperty iAppInstallWatcher;
    MInstallerWatcher*    iInstallerWatcher;
    TWatchMode iAppMode;
    RTimer iTimer;
};


//--- Mem card watcher class ---
class MMemCardWatcher{
public:
    //ETrue if mem card added, EFalse mem card removed
    virtual void HandleMemCardEventL(TBool aMemCardAdded) = 0;
};


class CMemCardWatcher : public CActive
{
public:
    static CMemCardWatcher* NewL(MMemCardWatcher* aMemCardWatcher);
    ~CMemCardWatcher();
    void StartWatchingMemCardL(TInt aDriveNumber);
    void StopWatchingMemCardL();
    TInt GetMemCardWatchDriveNumber(){
        return iDriveNumber;
    }

private:
    explicit CMemCardWatcher(MMemCardWatcher* aMemCardWatcher);
    void ConstructL();
    void RunL();
    void DoCancel();
    
private:
    RFs                 iFs;
    TInt                iDriveNumber;
    TBool               iIsDriveAvailable;
    MMemCardWatcher*    iMemCardWatcher;
};

//--- Disk watcher utility class ---

class MDiskWatcher{
public:
    virtual void HandleDiskThresholdEventL() = 0;
};

class CDiskWatcher : public CActive
{
public:
    static CDiskWatcher* NewL(MDiskWatcher* aDiskWatcher);
    ~CDiskWatcher();
    void StartWatchingDiskL(TInt aDriveNumber,TChar aDriveChar, TInt64 aThresholdLimit);
    void StopWatchingDiskL();
    TBool DoesDiskHaveSpace();
private:
    explicit CDiskWatcher(MDiskWatcher* aDiskWatcher);
    void ConstructL();
    void RunL();
    void DoCancel();
    
private:
    RFs iFs;
    RTimer iDiskWatchTimer;
    TChar iDriveToWatch;
    TInt iDriveNumber;
    TInt64 iThresholdLimit;
    MDiskWatcher* iDiskWatcher;
};

class MCallNotifier
{
public:
	virtual void NotifyCallL(TBool aEnableSound) = 0;
};

class CCallMonitor: public CActive
{
public: 
	static CCallMonitor* NewL(MCallNotifier* aCallNotifier);
	static CCallMonitor* NewLC(MCallNotifier* aCallNotifier);
	~CCallMonitor();
	void StartMonitor();
	void StopMonitor();
protected:
	void RunL();
	void DoCancel();
private: 
	CCallMonitor();
	void ConstructL(MCallNotifier* aCallNotifier);

private:
	CTelephony* 						iTelephony;
	MCallNotifier*  					iCallNotifier;
	CTelephony::TCallInfoV1Pckg		    iCallStatusPckg;
	
};

//--- Device watcher utility class ---
class MDeviceWatcher{
public:
    virtual void HandleDiskThresholdReachedEventL() = 0;
    virtual void HandleMemCardEventL(TBool aMemCardAdded) = 0;
    virtual void HandleAppInstallerEventL(TBool aAppInstalled) = 0;
};

class CScrShotsDeviceWatcher : public CBase,public MDiskWatcher,public MCallNotifier, public MMemCardWatcher, public MInstallerWatcher
{
    public:
        static CScrShotsDeviceWatcher* NewL(CScrShotsSettings& aAppSettings,MDeviceWatcher* aDeviceWatcher);
        ~CScrShotsDeviceWatcher();
        const RArray<TDriveExtraInfo>& GetDriveListWithInfo(TBool aGetCachedValue = EFalse);
        const RPointerArray<TApaAppInfo>& GetAppListWithInfoL(TBool aGetCachedValue = EFalse);
        const RPointerArray<TAppIconInfo>& GetAppIconArray(){
            return iAppsIconArray;
        }
        void StartWatchingDriveThresholdL(TChar aDriveChar, TInt aThresholdPercent);
        void StopWatchingDriveThresholdL();
        void HandleDiskThresholdEventL();
        void StopWatchingDeviceParamsL();
        RArray<TEntry>&  GetFolderList(const TDesC& aPathName, TInt& aErrorCode, TBool aForceRefresh = EFalse);
        RArray<TEntry>&  GoOneUpAndGetFolderListL(const TDesC& aPathName);
        RBuf& GetCurrentRootPath(){
            return iCurrentRootPath;
        }
        CDiskWatcher* GetDiskWatcher(){
            return iDiskWatcher;
        }
#if 0        
        CFont* GetCustomFont(){
            return iCustomFont;
        }
#endif
        TBool IsProfileSilentL();
        void StartCallMonitor();
        void StopCallMonitor();
        void NotifyCallL(TBool aEnableSound);
        
        void HandleMemCardEventL(TBool aMemCardAdded);
        void HandleAppInstallerEventL(TBool aAppinstalled);
        
    private:
        CScrShotsDeviceWatcher(CScrShotsSettings& aAppSettings,MDeviceWatcher* aDeviceWatcher);
        void ConstructL();
        void LoadCustomFontL();
        void UnloadCustomFontL();
    private:
        RFs iFs;
        CDiskWatcher* iDiskWatcher;
        RArray<TDriveExtraInfo>         iDrvsInfoArray;
        RPointerArray<TApaAppInfo>      iAppsInfoArray;
        RPointerArray<TAppIconInfo>     iAppsIconArray;
        RArray<TEntry>                  iFolderInfoArray;
        CDir*   iFolderArray;
        RBuf    iCurrentRootPath;
        CMemCardWatcher*                iMemCardWatcher;
#if 0        
        CFont*  iCustomFont;
        TInt iCustomFontFileID;
#endif        
        CScrShotsSettings& iAppSettings;
        CCallMonitor*		iCallMonitor;
        MDeviceWatcher*     iDeviceWatcher;
        CAppInstallerWatcher* iAppListWatcher;
};



#endif //__SCRSHOTSDEVICEWATHCER_H__
