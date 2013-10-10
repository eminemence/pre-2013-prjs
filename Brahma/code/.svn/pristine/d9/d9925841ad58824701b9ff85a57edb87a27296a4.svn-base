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

#include "ScrShotsGenUtils.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsStringPool.h"
#include "ScrShotsSettings.h"
#include <e32cmn.h>
#include <e32std.h>
#include <f32file.h>
#include <bautils.h>
#include <hwrmlight.h>
#include <eikenv.h>
#include <bautils.h>
#include <eikappui.h>
#include <eikapp.h>
#include <bafl/sysutil.h> 
#include <hal.h>
#include <Etel3rdParty.h>
#include <versioninfo.h>  // VersionInfo
#include <apgcli.h>
#include <apgicnfl.h>

#include "EventLogger.h"

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif


#ifdef ENABLE_ENGINE
RDriveList ScrShotsGenUtils::GetDriveListL()
{
    RDriveList drvs;
    return drvs;
}

void ScrShotsGenUtils::GenerateUniqueFileNameL(const TDesC& aNameSeries, TDes& aOutFileName)
{
    TStringPoolObj fileName(128);
    fileName.Des().Format(_L("%S%d"),&aNameSeries,User::TickCount());
    if(fileName.Des().Length() > aOutFileName.MaxLength())
        User::Leave(KErrArgument);
    else{
        aOutFileName.Copy(fileName.Des());
    }
}

// returns ETrue on creating aFolderName on aTargetDrive
// return EFalse on createing default folder (ScrShots) on aTargetDrive
TBool ScrShotsGenUtils::ChkExistenceAndCreateFolderL(const TDesC& aFolderName,const TDesC& aTargetDrive)
{
    TBool result = ETrue;
    TStringPoolObj fullPath(1024);
    fullPath.Des().Format(KPathFormatStringWithColon,&aTargetDrive, &aFolderName);
    if(!BaflUtils::FileExists(CEikonEnv::Static()->FsSession(), fullPath.Des())){
		TInt msgLength = fullPath.Des().Length() + 40; //40 is the len of the static string in the message;
        HBufC* msg = HBufC::NewLC(msgLength);
        msg->Des().Format(_L("%S not present on drive. Creating now!"),&(fullPath.Des()));
        CEikonEnv::Static()->AlertWin(msg->Des());
        CEikonEnv::Static()->FsSession().MkDirAll(fullPath.Des());
        //not giving user any option in this version!!
//        else{
//            fullPath.Des().Format(KPathFormatStringWithColon, &aTargetDrive, &KDefaultFileFolderName);
//            fs.MkDirAll(fullPath.Des());
//            result = EFalse;
//        }
        CleanupStack::PopAndDestroy(1);
    }
    return result;
}

void ScrShotsGenUtils::BlinkKeypadLight()
{
    TInt lightBlinkDuration = 500;
    TInt errCode = KErrNone;
    CHWRMLight* keypadLight = NULL;
    TRAP(errCode,keypadLight = CHWRMLight::NewL());
    if(!errCode){
        TRAP(errCode,keypadLight->LightBlinkL(CHWRMLight::ESecondaryKeyboard, lightBlinkDuration));
    delete keypadLight;
    }
}

TBool ScrShotsGenUtils::DriveExists(TChar aDriveChar)
{
    SLOGF(_L("ScrShotsGenUtils::DriveExists <<"));
    RFs fs;
    TDriveList dList;
    TBool drvExists = EFalse;
    if(fs.Connect() == KErrNone){
        fs.DriveList(dList);
        aDriveChar.UpperCase();
        unsigned int intval = aDriveChar.operator unsigned int(); 
        if(dList[intval - 65]){
            SLOGF(_L("Drive found"));
            drvExists = ETrue;
        }
        fs.Close();
    }
    SLOGF(_L("ScrShotsGenUtils::DriveExists >>"));
    return drvExists;
}

CFont* ScrShotsGenUtils::InitFontL(TInt aFontSize)
{
    CFont* font = NULL;
    _LIT( KMyFontName,"Swiss") ;
    const TInt KMyFontHeightInTwips = aFontSize ;
    TFontSpec myFontSpec( KMyFontName, KMyFontHeightInTwips ) ; 
    CEikonEnv::Static()->ScreenDevice()->GetNearestFontInPixels(font, myFontSpec);
    return font;
}

void ScrShotsGenUtils::FreeFontL(CFont* aFont)
{
    if(aFont){
        CEikonEnv::Static()->ScreenDevice()->ReleaseFont(aFont);
        aFont = NULL;
    }
}

void ScrShotsGenUtils::SetFreeSizeText(CScrShotsStrLoader* strLoader, TDes& aFreeSizeText, TInt64 aDriveFreeSize)
{
    TPtr formatStrFreeSizeCell = strLoader->GetStringHBuf(EDriveListFreeSizeCellFormatString)->Des();
    if((aDriveFreeSize / (KOneGB)) > 0)
        aFreeSizeText.Format(formatStrFreeSizeCell, &KGBStr,aDriveFreeSize / (KOneGB));
    else if((aDriveFreeSize / (KOneMB)) > 0)
        aFreeSizeText.Format(formatStrFreeSizeCell, &KMBStr,aDriveFreeSize / (KOneMB));
    else if((aDriveFreeSize / (KOneKB)) > 0)
        aFreeSizeText.Format(formatStrFreeSizeCell,&KKBStr, aDriveFreeSize / (KOneKB));
    else
        aFreeSizeText.Format(formatStrFreeSizeCell, &KBytesStr,aDriveFreeSize );

}

/**
 * Get the following information from the device
 * 1. Device model name
 * 2. Device firmware version
 * 3. Device RAM information
 * 4. Device uptime 
 * 5. Number of screen shots taken
 * 6. OS version
 * 7. S60 version
 * 8. Locale information
 * This information will be dumped into a js file, so that it can be submitted to the scrshots server.
 * The following scenarios will require a submit to server
 * 1. Register usage with server
 * 2. Register anonymous issue report ,feedback or suggestion about the app.
 */

//_LIT8(KFunctionBlockStart,"function params_init(){ \r\n");
_LIT8(KStringVariableDeclaration, "var %S= \"%S\";\r\n");
//_LIT8(KFunctionBlockEnd,"} \r\n");

const TInt KDeviceParamsCount = 9;
static const char * const DeviceParams [] =
{
"device_model",
"firmware_version",
"ram_info",
"uptime",
"scrshot_count",
"symbian_version",
"s60_version",
"country_dialing_code",
"imei_hash",
};

_LIT(KDeviceJSFile,"device_info.js");
//manufacturer name - model name - model code
_LIT8(KModelNameFormatString, "%S %S [%S]");

static void GetWhirlPoolHash(TDes8& aWhirlPoolHash, TDes8& aImei)
{
#ifdef WHIRLPOOL_SYMBIAN 
    struct NESSIEstruct w;
    u8 digest[DIGESTBYTES];
    NESSIEinit(&w);
    NESSIEadd(aImei.Ptr(), 8*aImei.Length(), &w);
    NESSIEfinalize(&w, digest);    
    for(int  i = 0; i < DIGESTBYTES; i++)
        aWhirlPoolHash.AppendNumFixedWidth(digest[i],EHex, 2);
#endif    
}

class CGetImei: public CActive
{
public: 
    static CGetImei* NewL(){
        CGetImei* self = new (ELeave) CGetImei;
        CleanupStack::PushL(self);
        self->ConstructL();
        CleanupStack::Pop(self);
        return self;
    }
    
    ~CGetImei(){
        Cancel();
        
        delete iTelephony;
        iTelephony = NULL;
    }
    
    void GetImei(TDes8& aImei){
        aImei.Copy(iImei);
    }
    
    TBool IsImeiFetched() const{
        return iGotImei;
    }
protected:
    void RunL(){
        if(iStatus.Int() == KErrNone){
            iImei.Copy(iId.iSerialNumber);
            iGotImei = ETrue;
            iSchedulerWait->AsyncStop();
        }
    }
    void DoCancel(){
        iSchedulerWait->AsyncStop();
        iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
    }
private: 
    CGetImei():
    CActive(EPriorityStandard){
    
        CActiveScheduler::Add(this);
    }
    void ConstructL(){
        iSchedulerWait = new (ELeave)CActiveSchedulerWait;
        iTelephony = CTelephony::NewL();
        CTelephony::TPhoneIdV1Pckg pkg(iId);
        iTelephony->GetPhoneId(iStatus, pkg);
        SLOGF(_L("CAlling SetActive in  CGetImei::ConstructL"));
        SetActive();
        iSchedulerWait->Start();
    }

private:
    CTelephony*                iTelephony;
    CTelephony::TPhoneIdV1     iId;
    TBool                      iGotImei;
    TBuf<64>                    iImei;
    CActiveSchedulerWait*        iSchedulerWait;
};


void ScrShotsGenUtils::DumpJsFileL(CScrShotsSettings* scrShotsSettings)
{
    CGetImei* getImei = CGetImei::NewL();
    CleanupStack::PushL(getImei);
    RBuf8 formatBuf;
    formatBuf.Create(256);
    formatBuf.CleanupClosePushL();
    TFileName infoFile;
    
    User::LeaveIfError(CEikonEnv::Static()->FsSession().PrivatePath(infoFile));
    infoFile.Append( KDeviceJSFile );
    TParsePtrC parse((CEikonEnv::Static()->EikAppUi()->Application())->AppFullName());
    infoFile.Insert(0, KCDriveWithColon);
    
    RFs fsConn;
    User::LeaveIfError(fsConn.Connect());
    CleanupClosePushL(fsConn);
    RFile jsFile;
    if(BaflUtils::FileExists(fsConn, infoFile)){
        BaflUtils::DeleteFile(fsConn, infoFile);
    }
    //now create the file
    User::LeaveIfError(jsFile.Create(fsConn, infoFile, EFileWrite));
    CleanupClosePushL(jsFile);
    
//    User::LeaveIfError(jsFile.Write(KFunctionBlockStart));
    RBuf8 values;
    values.Create(256);
    for(int i = 0; i < KDeviceParamsCount ; i++){
        formatBuf.Zero();
        TInt sizeOfItem = User::StringLength((TText8 *)DeviceParams [i]);
        TPtr8 item((unsigned char*)DeviceParams [i],sizeOfItem ,sizeOfItem );
        switch(i){
            //device_model
            case 0:{
                CDeviceTypeInformation* devInfo = SysUtil::GetDeviceTypeInfoL();
                CleanupStack::PushL(devInfo);
                TPtrC manufName, modelCode, modelName;
                devInfo->GetManufacturerName(manufName);
                devInfo->GetModelCode(modelCode);
                devInfo->GetModelName(modelName);
                RBuf8 manufName8, modelName8, modelCode8;
                manufName8.Create(128); modelName8.Create(128); modelCode8.Create(128);
                manufName8.CleanupClosePushL(); modelName8.CleanupClosePushL(); modelCode8.CleanupClosePushL();
                manufName8.Copy(manufName);
                modelName8.Copy(modelName);
                modelCode8.Copy(modelCode);
                values.Format(KModelNameFormatString, &manufName8, &modelName8, &modelCode8);
                CleanupStack::PopAndDestroy(4);
                break;
            }
            //firmware_version
            case 1:{
                RBuf swVersion;
                swVersion.Create(128);
                SysUtil::GetSWVersion(swVersion);
                values.Copy(swVersion);
                TInt pos = 0;
                while((pos = values.Find(_L8("\n"))) != KErrNotFound){
                    //values.Delete(pos,1);
                    values.Replace(pos,1,_L8("_"));
                }
                swVersion.Close();
                break;
            }
            
            //ram_info
            case 2:{
                TInt totalram= 0;
                HAL::Get(HALData::EMemoryRAM, totalram);
                totalram /= 1024;
                values.Num(totalram);
                break;
            }
            
            //uptime
            case 3:{
                TTimeIntervalMicroSeconds32 iTickPeriod;
                UserHal::TickPeriod(iTickPeriod);
                TUint tickCount = User::TickCount();
                TUint noOfTicksPerSecond = (1000 * 1000) / iTickPeriod.Int();
                TInt noOfSecsSinceStartUp = tickCount / noOfTicksPerSecond;//noOfMicroSecsSinceStartup / (1000 * 1000);
                values.Num(noOfSecsSinceStartUp);
                break;
            }
            
            //scrshot count
            case 4:{
                values.Num(scrShotsSettings->TotalScrShotCount());
                break;
            }
            
            //symbian version
            case 5:{
                TVersion epocver = User::Version();
                values.Copy(epocver.Name());            
                break;
            }
            
            //series 60 version
            case 6:{
                VersionInfo::TPlatformVersion platformVersion;
                VersionInfo::GetVersion(platformVersion);
                TInt aMajor = platformVersion.iMajorVersion;
                TInt aMinor = platformVersion.iMinorVersion;
                values.Format(_L8("%d.%d"),aMajor, aMinor);
                break;
            }
            //country code
            case 7:{
            
                TLocale loc;
                int code = loc.CountryCode();
                values.Num(code);
                break;
            }
            //imei hash
            case 8:{
                TBuf8<256> inputimei;
                TBuf8<256> outimeihash;
                if(getImei->IsImeiFetched()){
                    getImei->GetImei(inputimei);
                    GetWhirlPoolHash(outimeihash,inputimei);
                    values.Copy(outimeihash);
                }
            }
            break;

        }
        
        formatBuf.Format(KStringVariableDeclaration, 
                &item, 
                &values);
        TRACE_ON(RDebug::Printf((const char*)formatBuf.PtrZ()));
        jsFile.Write(formatBuf);
    }
    values.Close();
    CleanupStack::PopAndDestroy(4);
    //TODO: This code should be commented in final release build
#if BRAHMA_DEBUG
    //code to copy the js file just created to the c:\ so that it can be analyzed
    _LIT(KTargetFileName, "c:\\device_info.js");
    BaflUtils::CopyFile(CEikonEnv::Static()->FsSession(),infoFile, KTargetFileName);
#endif
}

TBool ScrShotsGenUtils::CheckIfAppExists(const TDesC& aAppName, const TUid& aAppUid)
{
    TBool appFound = EFalse;
    RApaLsSession appLs;
    if(appLs.Connect()==KErrNone){
        appLs.GetAllApps();
        TApaAppInfo dummyAppInfo;
        while(appLs.GetNextApp(dummyAppInfo)!= RApaLsSession::ENoMoreAppsInList){
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
            if(dummyAppInfo.iCaption.Compare(aAppName) == 0 &&  dummyAppInfo.iUid == aAppUid){
                appFound = ETrue;
                break;
            }
        }
        appLs.Close();
    }
    return appFound;
}

#endif
