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

#include "ScrShotsSettings.h"
#include <bautils.h>

#include "EventLogger.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsCommonDefs.h"

#ifdef ENABLE_SETTINGS
_LIT(KSettingsFileName, "ScrShotsSettings.db");
//table schema
_LIT(KSettingsTableName,"ScrShotsSettings");
_LIT(KOutputImageFormat,"OutputImageFormat");
_LIT(KOutputFileNameSeries,"OutputFileName");
_LIT(KOutputFolderName,"OutputFolderName");
_LIT(KOutputDrive,"OutputDrive");
_LIT(KSnapForApp,"AppName");
_LIT(KCaptureInterval,"CaptureInterval");
_LIT(KCaptureDuration,"CaptureDuration");
_LIT(KDiskFillThreshold,"DiskFillThreshold");
_LIT(KEnableSound,"EnableSound");
_LIT(KEnableLight,"EnableLight");
_LIT(KTargetAppUid,"AppUid");
_LIT(KTotalScrShotCount,"TotalScrShotCount");
#ifdef ENABLE_WATERMARK 
_LIT(KWatermarkText,"WatermarkText");
_LIT(KWatermarkTextPosition,"WatermarkTextPosition");
#endif


const TUint KOutputImageFormatColNum = 1;
const TUint KOutputFileNameSeriesColNum = 2;
const TUint KOutputFolderNameColNum = 3;
const TUint KOutputDriveColNum = 4;
const TUint KSnapForAppColNum = 5;
const TUint KCaptureIntervalColNum = 6;
const TUint KCaptureDurationColNum = 7;
const TUint KDiskFillThresholdColNum = 8;
const TUint KEnableSoundColNum = 9;
const TUint KEnableLightColNum = 10;
const TUint KTargetAppUidColNum = 11;
const TUint KTotalScrShotCountNum = 12;

#ifdef ENABLE_WATERMARK
const TUint KWatermarkTextColNum = 10;
const TUint KWatermarkTextPositionColNum = 11;
#endif


_LIT(KGeneralSelectAllQuery,"SELECT * FROM %S");

const TUint32 KDefaultCaptureInterval = 1;//1 second
const TUint32 KDefaultCaptureDuration = 10;//10 seconds
const TUint32 KDefaultBufferSize = 256;
const TUint32 KDefaultDiskThreshold = 98;//98% percent full, then disk threshold alarm will trigger by default
const TUint32 KDefaultEnableSound = 1;//Sound ON
const TUint32 KDefaultEnableLight = 1;//Sound OFF
const TUint32 KDefaultTotalScrShotCount = 0;
#ifdef ENABLE_WATERMARK 
const TUint32 KDefaultWatermarkTextPosition = ECenterPos;
_LIT(KDefaultWatermarkText,"ScrShots :)");
#endif

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif


CScrShotsSettings* CScrShotsSettings::NewL(MSettingsObserver* aSettingObserver)
{
    CScrShotsSettings* self = new (ELeave) CScrShotsSettings(aSettingObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CScrShotsSettings::~CScrShotsSettings()
{
    TRAPD(errCode,SaveSettingsL());
    
    iOutputFileName.Close();
    iOutputFolderName.Close();
    iOutputDrive.Close();
    iAppName.Close();
#ifdef ENABLE_WATERMARK     
    iWatermarkText.Close();
#endif
    iSettingsDb.Close();
    iSettingsFs.Close();
}

void CScrShotsSettings::ConstructL()
{
    TInt errCode = KErrNone;
    iOutputFileName.CreateL(KDefaultBufferSize);
    iOutputFolderName.CreateL(KDefaultBufferSize);
    iOutputDrive.CreateL(2);
    iAppName.CreateL(KDefaultBufferSize);
#ifdef ENABLE_WATERMARK     
    iWatermarkText.CreateL(KDefaultBufferSize);
#endif
    TBuf<4> dbDrive;
    TBuf<1024> prvPath,dbFilePath;
    
    if(iSettingsFs.Connect() == KErrNone){
        //create private path if it does not exist
        if(iSettingsFs.CreatePrivatePath(RFs::GetSystemDrive()) == KErrNone){
            iSettingsFs.PrivatePath(prvPath);
            dbDrive.Append(TChar(RFs::GetSystemDriveChar()));//this works
            dbFilePath.Format(_L("%S:%S%S"),&dbDrive,&prvPath,&KSettingsFileName);
            if(!BaflUtils::FileExists(iSettingsFs,dbFilePath)){
                errCode = iSettingsDb.Create(iSettingsFs,dbFilePath);
                if(errCode != KErrNone){
                    User::LeaveIfError(errCode);
                }                
                TRAPD(errCode,InitSettingsDbL());
            }
            else{
                errCode = iSettingsDb.Open(iSettingsFs,dbFilePath);
                if(errCode != KErrNone){
                    User::LeaveIfError(errCode);
                }
                iDbInitDone = ETrue;
            }
            LoadSettingsL();        
        }
    }
}

CScrShotsSettings::CScrShotsSettings(MSettingsObserver* aSettingObserver):
        iSettingObserver(aSettingObserver)
{

}

void CScrShotsSettings::SaveSettingsL()
{
    if(!iDbInitDone)
        return;
    iSettingsDb.Begin();
    TBuf<256> qryBuf;
    qryBuf.Format(KGeneralSelectAllQuery, &KSettingsTableName); 
    RDbView settingsView;
    settingsView.Prepare(iSettingsDb, TDbQuery(qryBuf));
    CleanupClosePushL(settingsView);
    if(settingsView.EvaluateAll() == KErrNone){
        settingsView.FirstL();     
        settingsView.UpdateL();
        settingsView.SetColL(KOutputImageFormatColNum, iOutputImageFormat);
        settingsView.SetColL(KOutputFileNameSeriesColNum, iOutputFileName);
        settingsView.SetColL(KOutputFolderNameColNum, iOutputFolderName);
        settingsView.SetColL(KOutputDriveColNum, iOutputDrive);
        settingsView.SetColL(KSnapForAppColNum, iAppName);
        settingsView.SetColL(KCaptureIntervalColNum, iCaptureInterval);
        settingsView.SetColL(KCaptureDurationColNum, iCaptureDuration);
        settingsView.SetColL(KDiskFillThresholdColNum, iDiskFillThreshold);
        TUint32 enableSoundU32 = iEnableSound;
        settingsView.SetColL(KEnableSoundColNum, enableSoundU32);
        TUint32 enableLightU32 = iEnableLight;
        settingsView.SetColL(KEnableLightColNum, enableLightU32);
        settingsView.SetColL(KTargetAppUidColNum, iAppUid);
        settingsView.SetColL(KTotalScrShotCountNum, iTotalScrShotCount);
        
#ifdef ENABLE_WATERMARK         
        settingsView.SetColL(KWatermarkTextColNum, iWatermarkText);
        settingsView.SetColL(KWatermarkTextPositionColNum, iWatermarkPositionOnImage);
#endif        
        settingsView.PutL();
            
    }
    
    CleanupStack::PopAndDestroy(1); // settingsView
    //view destroyed , now commit the db
    iSettingsDb.Commit();
}

void CScrShotsSettings::InitSettingsDbL()
{
    if(iDbInitDone)
        return;
    
    CDbColSet* col = CDbColSet::NewLC();
    col->AddL(TDbCol(KOutputImageFormat, EDbColUint32));
    col->AddL(TDbCol(KOutputFileNameSeries, EDbColText, 255));
    col->AddL(TDbCol(KOutputFolderName, EDbColText, 255));
    col->AddL(TDbCol(KOutputDrive, EDbColText, 1));
    col->AddL(TDbCol(KSnapForApp, EDbColText, 255));
    col->AddL(TDbCol(KCaptureInterval, EDbColUint32));
    col->AddL(TDbCol(KCaptureDuration, EDbColUint32));
    col->AddL(TDbCol(KDiskFillThreshold, EDbColUint32));
    col->AddL(TDbCol(KEnableSound, EDbColUint32));
    col->AddL(TDbCol(KEnableLight, EDbColUint32));
    col->AddL(TDbCol(KTargetAppUid, EDbColUint32));
    col->AddL(TDbCol(KTotalScrShotCount, EDbColUint32));
#ifdef ENABLE_WATERMARK     
    col->AddL(TDbCol(KWatermarkText, EDbColText, 255));
    col->AddL(TDbCol(KWatermarkTextPosition, EDbColUint32));
#endif
    if(iSettingsDb.CreateTable(KSettingsTableName, *col) == KErrNone){
        CleanupStack::PopAndDestroy(col);
        
        // now set default values for some fields
        TBuf<256> qryBuf;
        qryBuf.Format(KGeneralSelectAllQuery, &KSettingsTableName);
        TRACE_ON(RDebug::Print(qryBuf));
        if(iSettingsDb.Begin() == KErrNone){
            RDbView settingsView;
            if(settingsView.Prepare(iSettingsDb, TDbQuery(qryBuf)) == KErrNone){
                CleanupClosePushL(settingsView);
        
                settingsView.InsertL();  
                settingsView.SetColL(KOutputImageFormatColNum, EGif);
                settingsView.SetColL(KOutputFileNameSeriesColNum, KDefaultFileFolderName);
                settingsView.SetColL(KOutputFolderNameColNum, KDefaultFileFolderName);
                settingsView.SetColL(KOutputDriveColNum, KCDriveWithoutColon);
            //    settingsView.SetColL(KSnapForAppColNum, 0);
                settingsView.SetColL(KCaptureIntervalColNum, KDefaultCaptureInterval);
                settingsView.SetColL(KCaptureDurationColNum, KDefaultCaptureDuration);
                settingsView.SetColL(KDiskFillThresholdColNum, KDefaultDiskThreshold);
                settingsView.SetColL(KEnableSoundColNum, KDefaultEnableSound);
                settingsView.SetColL(KEnableLightColNum, KDefaultEnableLight);
                settingsView.SetColL(KTargetAppUidColNum, 0);
                settingsView.SetColL(KTotalScrShotCountNum, KDefaultTotalScrShotCount);
#ifdef ENABLE_WATERMARK                 
                settingsView.SetColL(KWatermarkTextColNum, KDefaultWatermarkText);
                settingsView.SetColL(KWatermarkTextPositionColNum, KDefaultWatermarkTextPosition);
#endif
                
                settingsView.PutL();  
                CleanupStack::PopAndDestroy(1); // settingsView
                if(iSettingsDb.Commit() == KErrNone)
                    iDbInitDone = ETrue;
            }
        }
    }
    else{
        CleanupStack::PopAndDestroy(col);
    }
        
}

void CScrShotsSettings::LoadSettingsL()
{
    if(!iDbInitDone)
        return;
    
    TBuf<256> qryBuf;
    qryBuf.Format(KGeneralSelectAllQuery, &KSettingsTableName); 
    if(iSettingsDb.Begin() == KErrNone){
        RDbView settingsView;
        if(settingsView.Prepare(iSettingsDb, TDbQuery(qryBuf)) == KErrNone){
            CleanupClosePushL(settingsView);
            if(settingsView.EvaluateAll() == KErrNone){
                settingsView.FirstL();     
                settingsView.GetL();      
                iOutputImageFormat = settingsView.ColUint32(KOutputImageFormatColNum);
                iOutputFileName.Copy(settingsView.ColDes(KOutputFileNameSeriesColNum));
                iOutputFolderName.Copy(settingsView.ColDes(KOutputFolderNameColNum));
                iOutputDrive.Copy(settingsView.ColDes(KOutputDriveColNum));
                iAppName.Copy(settingsView.ColDes(KSnapForAppColNum));
                iCaptureInterval = settingsView.ColUint32(KCaptureIntervalColNum);
                iCaptureDuration = settingsView.ColUint32(KCaptureDurationColNum);
                iDiskFillThreshold = settingsView.ColUint32(KDiskFillThresholdColNum);
                iEnableSound = settingsView.ColUint32(KEnableSoundColNum);
                iEnableLight = settingsView.ColUint32(KEnableLightColNum);
                iAppUid = settingsView.ColUint32(KTargetAppUidColNum);
                iTotalScrShotCount = settingsView.ColUint32(KTotalScrShotCountNum);
#ifdef ENABLE_WATERMARK                 
                iWatermarkText.Copy(settingsView.ColDes(KWatermarkTextColNum));
                iWatermarkPositionOnImage = settingsView.ColUint32(KWatermarkTextPositionColNum);
#endif                
            }
            CleanupStack::PopAndDestroy(1); // settingsView
        }
        iSettingsDb.Commit();
    }
}

void CScrShotsSettings::SetSaveImageFormatL(TSaveImageFormat aSaveImgFormat)
{
    iOutputImageFormat = aSaveImgFormat;
    
    SaveSettingsL();
    
    //notify interested ppl
    TChangedSettingData settingsData;
    settingsData.iSettingName = ESaveImgFormat;
    settingsData.TChangedSettingValue.iChangedInt = iOutputImageFormat;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
}

void CScrShotsSettings::SetOutputFileNameL(const TDesC& aOutputFileName)
{
    if(aOutputFileName.Length() > iOutputFileName.MaxLength()){
        User::Leave(KErrArgument);
    }
    iOutputFileName.Copy(aOutputFileName);
    
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EOutputFileName;
    settingsData.TChangedSettingValue.iChangedString = iOutputFileName.AllocLC(); 
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString
    
}

void CScrShotsSettings::OutputFileNameL(TDes& aOutputFileName)
{
    if(aOutputFileName.MaxLength() < iOutputFileName.Length()){
         User::Leave(KErrArgument);
     }
    aOutputFileName.Copy(iOutputFileName);
}

void CScrShotsSettings::SetOutputFolderNameL(const TDesC& aOutputFolderName)
{
    if(aOutputFolderName.Length() > iOutputFolderName.MaxLength()){
        User::Leave(KErrArgument);
    }
    iOutputFolderName.Copy(aOutputFolderName);
    if(!ScrShotsGenUtils::ChkExistenceAndCreateFolderL(iOutputFolderName,iOutputDrive)){
        iOutputFolderName.Copy(KDefaultFileFolderName);
    }
    SaveSettingsL();

    TChangedSettingData settingsData;
    settingsData.iSettingName = EOutputFolderName;
    settingsData.TChangedSettingValue.iChangedString = iOutputFolderName.AllocLC(); 
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString

}

void CScrShotsSettings::OutputFolderNameL(TDes& aOutputFolderName)
{
    
    if(aOutputFolderName.MaxLength() < iOutputFolderName.Length()){
         User::Leave(KErrArgument);
     }
    aOutputFolderName.Copy(iOutputFolderName);
}

void CScrShotsSettings::SetOutputFolderDriveL(TChar aDriveChar)
{
    iOutputDrive.Copy(KEmptyStr());
    iOutputDrive.Append(aDriveChar);
    
    ScrShotsGenUtils::ChkExistenceAndCreateFolderL(iOutputFolderName,iOutputDrive);
    
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EOutputFolderDrive;
    settingsData.TChangedSettingValue.iChangedString = iOutputDrive.AllocLC(); 
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString    
}

void CScrShotsSettings::OutputFolderDrive(TDes& aOutputFolderDrive)
{
    aOutputFolderDrive.Copy(iOutputDrive);
}

void CScrShotsSettings::SetAppNameForCaptureL(const TDesC& aAppName)
{
    if(aAppName.Length() > iAppName.MaxLength()){
        User::Leave(KErrArgument);
    }
    iAppName.Copy(aAppName);
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EAppNameForCapture;
    settingsData.TChangedSettingValue.iChangedString = iAppName.AllocLC();
    SLOGF(_L("Calling CScrShotsSettings::SetAppNameForCaptureL"))
    SLOGF(aAppName)
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString    
    
}

void CScrShotsSettings::AppNameForCaptureL(TDes& aAppName)
{
    if(aAppName.MaxLength() < iAppName.Length()){
         User::Leave(KErrArgument);
     }    
    aAppName.Copy(iAppName);
}

void CScrShotsSettings::SetCaptureIntervalL(TUint32 aCaptureIntervalInSeconds)
{
    iCaptureInterval = aCaptureIntervalInSeconds;
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = ECaptureInterval;
    settingsData.TChangedSettingValue.iChangedInt = iCaptureInterval;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);    
}

void CScrShotsSettings::SetCaptureDurationL(TUint32 aCaptureDurationInSeconds)
{
    iCaptureDuration = aCaptureDurationInSeconds;
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = ECaptureDuration;
    settingsData.TChangedSettingValue.iChangedInt = iCaptureDuration;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);    
    
}

void CScrShotsSettings::GetSaveImageFormatExtStringL(TDes& aExtString)
{
    switch(iOutputImageFormat){
        case EBmp:            aExtString.Copy(_L(".bmp"));            break;
        case EJpeg:            aExtString.Copy(_L(".jpeg"));            break;
        case EGif:            aExtString.Copy(_L(".gif"));            break;
        case EPng:            aExtString.Copy(_L(".png"));            break;
    }
}

void CScrShotsSettings::GetSaveImageFormatMimeTypeStringL(TDes8& aMimeType)
{
    switch(iOutputImageFormat){
        case EBmp:            aMimeType.Copy(_L8("image/bmp"));            break;
        case EJpeg:            aMimeType.Copy(_L8("image/jpeg"));            break;
        case EGif:            aMimeType.Copy(_L8("image/gif"));            break;
        case EPng:            aMimeType.Copy(_L8("image/png"));            break;
    }
}

void CScrShotsSettings::SetDiskFillThresholdL(TUint32 aDiskFillThresholdInPercent)
{
    iDiskFillThreshold = aDiskFillThresholdInPercent; 
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EDiskFillThreshold;
    settingsData.TChangedSettingValue.iChangedInt = iDiskFillThreshold;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     
}

void CScrShotsSettings::SetSoundL(TBool aEnableSound)
{
    iEnableSound = aEnableSound; 
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EIsSoundOn;
    settingsData.TChangedSettingValue.iChangedInt = iEnableSound;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     
    
}

void CScrShotsSettings::SetLightL(TBool aEnableLight)
{
    iEnableLight = aEnableLight; 
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EIsLightOn;
    settingsData.TChangedSettingValue.iChangedInt = iEnableLight;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     
    
}

void CScrShotsSettings::GetOutputFolderCompletePathL(TDes& aOutputFolderFullPath)
{
    if(aOutputFolderFullPath.MaxLength() < (iOutputFolderName.Length() + iOutputDrive.Length() + 5)){
         User::Leave(KErrArgument);
     }    
    aOutputFolderFullPath.Format(_L("%S:\\%S\\"),&iOutputDrive,&iOutputFolderName);

}

void CScrShotsSettings::SetAppUidForCaptureL(const TUint32& aAppUid)
{
    iAppUid = aAppUid;
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EAppUidForCapture;
    settingsData.TChangedSettingValue.iChangedInt = iAppUid;  
    SLOGF(_L("Calling CScrShotsSettings::SetAppUidForCaptureL"))
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     

}

TUint32 CScrShotsSettings::AppUidForCapture()
{
    return iAppUid;
}

void CScrShotsSettings::SetAppInfoForCaptureL(const TDesC& aAppName, const TUint32& aAppUid)
{
    iAppUid = aAppUid;
    if(aAppName.Length() > iAppName.MaxLength()){
        User::Leave(KErrArgument);
    }
    iAppName.Copy(aAppName);
    
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EAppUidForCapture;
    settingsData.TChangedSettingValue.iChangedInt = iAppUid;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    
    settingsData.iSettingName = EAppNameForCapture;
    settingsData.TChangedSettingValue.iChangedString = iAppName.AllocLC(); 
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString    
}

void CScrShotsSettings::SetTotalScrShotCount(TUint32 aScrShotCount)
{
    iTotalScrShotCount = aScrShotCount;
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = ETotalScrShotCount;
    settingsData.TChangedSettingValue.iChangedInt = iTotalScrShotCount;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     

}


#ifdef ENABLE_WATERMARK 
void CScrShotsSettings::SetWatermarkPositionL(TWatermarkPosition aWatermarkPosition) 
{ 
    iWatermarkPositionOnImage = aWatermarkPosition; 
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EWatermarkPosition;
    settingsData.TChangedSettingValue.iChangedInt = iWatermarkPositionOnImage;    
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);     
    
}

void CScrShotsSettings::SetWatermarkTextL(const TDesC& aWatermarkText)
{
    if(aWatermarkText.Length() > iWatermarkText.MaxLength()){
        User::Leave(KErrArgument);
    }
    iWatermarkText.Copy(aWatermarkText);
    SaveSettingsL();
    
    TChangedSettingData settingsData;
    settingsData.iSettingName = EWatermarkText;
    settingsData.TChangedSettingValue.iChangedString = iWatermarkText.AllocLC(); 
    if(iSettingObserver)
        iSettingObserver->HandleSettingChangeL(settingsData);
    CleanupStack::PopAndDestroy(1);//settingsData.TChangedSettingValue.iChangedString   
}

void CScrShotsSettings::WatermarkText(TDes& aWatermarkText)
{
    if(aWatermarkText.MaxLength() < iWatermarkText.Length()){
         User::Leave(KErrArgument);
     }    
    aWatermarkText.Copy(iWatermarkText);
}
#endif //ENABLE_WATERMARK

#endif
