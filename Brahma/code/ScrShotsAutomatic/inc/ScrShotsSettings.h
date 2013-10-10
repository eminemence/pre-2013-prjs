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

#ifndef __SCRSHOTSSETTINGS_H__
#define __SCRSHOTSSETTINGS_H__

#include <e32base.h>
#include <f32file.h> //RFs
#include <d32dbms.h>  //RDbStoreDatabase,RDbNamedDatabase

#ifdef ENABLE_SETTINGS

enum TSettingName
{
    ESaveImgFormat,
    EOutputFileName,
    EOutputFolderName,
    EOutputFolderDrive,
    EAppNameForCapture,
    EAppUidForCapture,
    ECaptureInterval,
    ECaptureDuration,
    EDiskFillThreshold,
    EIsSoundOn,
    EIsLightOn,
    ETotalScrShotCount,
#ifdef ENABLE_WATERMARK    
    EWatermarkPosition,
    EWatermarkText
#endif
};

typedef struct TChangedSetting
{
    TSettingName iSettingName;
    union TSettingValue{
        HBufC* iChangedString;
        TUint32 iChangedInt;
    }TChangedSettingValue;
}TChangedSettingData;

class MSettingsObserver{
public:
    virtual void HandleSettingChangeL(const TChangedSettingData& aChangedSetting) = 0;
};



enum TSaveImageFormat
{
	EBmp,
	EJpeg,
	EGif,
	EPng
};
#ifdef ENABLE_WATERMARK 
enum TWatermarkPosition
{
    ETopLeft,
    ETopRight,
    ECenterPos,
    EBottomLeft,
    EBottomRight,
    EMaxPosition
};
#endif
//stores app specific settings which will be used throughout the application
class CScrShotsSettings : public CBase
{

public:
	static CScrShotsSettings* NewL(MSettingsObserver* aSettingObserver);
	~CScrShotsSettings();
private:
	void ConstructL();
	explicit CScrShotsSettings(MSettingsObserver* aSettingObserver);
public:
	void SaveSettingsL();
	
	//helper functions
    void GetSaveImageFormatExtStringL(TDes& aExtString);
    void GetSaveImageFormatMimeTypeStringL(TDes8& aMimeType);
    
    //settings getters & setters
	TSaveImageFormat SaveImageFormat(){ return TSaveImageFormat(iOutputImageFormat); }
	void SetSaveImageFormatL(TSaveImageFormat aSaveImgFormat);
	
	void SetOutputFileNameL(const TDesC& aOutputFileName);//max length 256,if greater leaves
	void OutputFileNameL(TDes& aOutputFileName);
	
    //should not have starting and trailing '\' chars are they are not stored in settings
	void SetOutputFolderNameL(const TDesC& aOutputFolderName);//max length 256,if greater leaves
    void OutputFolderNameL(TDes& aOutputFolderName);
	
    void SetOutputFolderDriveL(TChar aDriveChar);
	void OutputFolderDrive(TDes& aOutputFolderDrive);
	
	void SetAppNameForCaptureL(const TDesC& aAppName);//max length 256,if greater leaves
	void AppNameForCaptureL(TDes& aAppName);
	
	void SetCaptureIntervalL(TUint32 aCaptureIntervalInSeconds);
	TUint32 CaptureInterval(){ return iCaptureInterval; }
	
	void SetCaptureDurationL(TUint32 aCaptureDurationInSeconds);
	TUint32 CaptureDuration(){ return iCaptureDuration; }
	
	//this will depend on the storage media currently selected
	void SetDiskFillThresholdL(TUint32 aDiskFillThresholdInPercent);
    TUint32 DiskFillThreshold(){ return iDiskFillThreshold; }
    
    void SetSoundL(TBool aEnableSound);
    TBool IsSoundOn(){ return iEnableSound;}
    
    void SetLightL(TBool aEnableLight);
    TBool IsLightOn(){ return iEnableLight;}    
    
    void SetAppUidForCaptureL(const TUint32& aAppUid);
    TUint32 AppUidForCapture();
    void SetAppInfoForCaptureL(const TDesC& aAppName, const TUint32& aAppUid);
    
    void GetOutputFolderCompletePathL(TDes& aOutputFolderFullPath);
    
    void SetTotalScrShotCount(TUint32 aScrShotCount);
    TUint32 TotalScrShotCount(){
        return iTotalScrShotCount;
    }
#ifdef ENABLE_WATERMARK     
    TWatermarkPosition WatermarkPosition(){ return TWatermarkPosition(iWatermarkPositionOnImage);}
    void SetWatermarkPositionL(TWatermarkPosition aWatermarkPosition);
    void SetWatermarkTextL(const TDesC& aWatermarkText);
    void WatermarkText(TDes& aWatermarkText);
#endif
private:
	void InitSettingsDbL();
	void LoadSettingsL();
	
private:
	TSaveImageFormat iSaveImgFormat;
#ifdef ENABLE_WATERMARK 	
	TWatermarkPosition  iWatermarkPosition;
#endif 	
	//RBuf	iSaveFolderPath;
    RFs                 iSettingsFs;
    RDbNamedDatabase    iSettingsDb;
    //settings values
    TUint32             iOutputImageFormat;
    RBuf                iOutputFileName;
    RBuf                iOutputFolderName;
    RBuf                iOutputDrive;
    RBuf                iAppName;
    TUint32             iAppUid;
    TUint32             iCaptureInterval;
    TUint32             iCaptureDuration;    
	TUint32             iDiskFillThreshold;
	TBool               iEnableSound;
	TBool               iEnableLight;
	TUint32             iTotalScrShotCount;
#ifdef ENABLE_WATERMARK 	
	RBuf                iWatermarkText;
	TUint32             iWatermarkPositionOnImage;
#endif
	//class member
	TBool               iDbInitDone;
	MSettingsObserver* iSettingObserver;
};
#endif//ENABLE_SETTINGS
#endif //__SCRSHOTSSETTINGS_H__
