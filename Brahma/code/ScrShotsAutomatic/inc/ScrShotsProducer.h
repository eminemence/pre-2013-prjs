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

#ifndef __SCRSHOTSPRODUCER_H__
#define __SCRSHOTSPRODUCER_H__

#include <e32base.h>
#include <w32std.h>
#include <mdaaudiosampleplayer.h>
#include <aknappui.h> 
#include "TimerX.h"
#include "ScrShotsSystemEventObserver.h"

#ifdef ENABLE_ENGINE
#include "ScrShotsSettings.h"

class CScrShotsConvertor;
typedef RPointerArray<HBufC> RImgList;
const TUint KBufferSize = 256;

class CScrShotsSound : public CBase, MMdaAudioPlayerCallback
{
public:
    static CScrShotsSound* NewL();
    ~CScrShotsSound();
private:
    CScrShotsSound();
    void ConstructL();
public:
    void PlaySoundL();
    //from MMdaAudioPlayerCallback
    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
    void MapcPlayComplete(TInt aError);

private:
    CMdaAudioPlayerUtility* iPlayer;
    TBool iIsInitDone;
    TBool iIsBeingPlayed;
    RFs& iFs;
};

enum TScrShotEvent{
    EScrShotTaken,
    EAppClosedByUser,
    EScrShotTaskEnded,
};

class MScrShotProgressObserver{
public:
    virtual void HandleScrShotEventL(TScrShotEvent aEvent) = 0;
};

class MOrientationNotifier{
public:
    virtual CAknAppUiBase ::TAppUiOrientation GetCurrentOrientation() = 0;
};


    
class CScrShotsProducer : public CBase,MTimerXCallback,MSystemEventObserver
{
public:
	static CScrShotsProducer* NewL(CScrShotsSettings* aScrShotsSettings, MScrShotProgressObserver* aObserver, MOrientationNotifier* aOrientationNotifier);
	~CScrShotsProducer();
	
private:
	CScrShotsProducer(CScrShotsSettings* aScrShotsSettings, MScrShotProgressObserver* aObserver, MOrientationNotifier* aOrientationNotifier);
	void ConstructL();
	void DeleteHeadAndRemoveRawFileL();
	void TakeScreenShotL();
	void GenerateFileNameL(HBufC* aSbmpFileName);
	void HandleTimerExpireL(TInt aTimerId);
	void HandleSystemEventL(TSystemEventsList aEvent);
	void PlayTickSoundL();
	void GetFileNameFromDB();
#ifdef ENABLE_WATERMARK	
	void CalculateWatermarkPoint();
#endif
	void DiskIsFilled();
		
public:
	// all the durations are in seconds.
	void TakeOneScrShotL(TInt aAfterDuration);
	void TakeMultipleScrShotL(TInt aAfterInterval,TInt aForDuration);
	//this takes the screenshots till the app exits
	TBool TakeMultipleScrShotOfAppL(const TDesC& aAppName,TInt aAfterInterval);
	// this takes the screenshots for a fixed duration and will stop if the app exits before the duration.
	TBool TakeMultipleScrShotOfAppForDurationL(const TDesC& aAppName,TInt aAfterDuration,TInt aForDuration);
	void StopTakingScrShotsL();
#ifdef ENABLE_WATERMARK	
	void UpdateWatermarkL(const TDesC& aWatermark,const TUint32 aWatermarkPosition);
#endif //ENABLE_WATERMARK
	TUint GetScreenShotCount(){
	    return iScrShotsCount;
	}
	TBool IsActive(){
	    return (iCaptureMode != ECaptureNone);
	}
	
	void CheckAndCloseLaunchedHiddenApp();
private:
	enum TCaptureMode{
	    ECaptureNone,
	    ECaptureOneShot,
	    ECaptureMultipleShots,
	    ECaptureAppTillExit,
	    ECaptureAppForDuration
	};
	
	//list of scr shots taken in raw format
	RImgList iImgList;
	RWsSession iWs;
	CWsScreenDevice* iScreenDevice;
	CFbsBitmap* iLBitmap;
	CFbsBitmap* iPBitmap;
	TInt 	iForDuration;
	TInt 	iAfterDuration;
	RBuf	iAppName;
	TSize	iLandscapeScreenShotAreaSize;
	TSize   iPotraitScreenShotAreaSize;
	CTimerX* iIntervalTimer;
	TCaptureMode    iCaptureMode;
	CScrShotsSystemEventObserver *iSystemTool;
	//CScrShotsConvertor* iScrShotConvertor;//ref only, not owned
	CScrShotsSettings* 	iScrShotsSettings;//ref only, not owned
	// add audio player to play the clicking sound.
#ifdef ENABLE_WATERMARK	
	RBuf        iWatermark;
	TWatermarkPosition iWatermarkpos;
	TPoint      iWatermarkPoint;
	const CFont*      iWatermarkFont;
#endif //ENABLE_WATERMARK
	TUint iScrShotsCount;
	CScrShotsSound* iSoundPlayer;
	MScrShotProgressObserver* iObserver;
	HBufC*	 iSaveFolderPath;
	TBuf<KBufferSize> iNameSeries;
	HBufC* iBmpFileName;
	MOrientationNotifier* iOrientationNotifier;
	CAknAppUiBase::TAppUiOrientation   iCurrentOrientation;
	friend class CScrShotsConvertor;
	
};
#endif
#endif //__SCRSHOTSPRODUCER_H__
