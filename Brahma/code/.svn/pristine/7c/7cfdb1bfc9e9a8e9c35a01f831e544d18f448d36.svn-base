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

#include "ScrShotsProducer.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsStringPool.h"

#include "EventLogger.h"

#include <eikenv.h>
#include <bautils.h>
#include <eikapp.h>
#include <eikappui.h>
#include <APGWGNAM.H>

#ifdef ENABLE_ENGINE


//this is stored in private path on c: drive
_LIT(KSoundFileName,"tick.wav");

CScrShotsSound* CScrShotsSound::NewL()
{
    CScrShotsSound* self = new (ELeave) CScrShotsSound();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;

}

CScrShotsSound::~CScrShotsSound()
{
    iPlayer->Stop();
    delete iPlayer;
}

CScrShotsSound::CScrShotsSound():
        iFs(CEikonEnv::Static()->FsSession())
{

}

void CScrShotsSound::ConstructL()
{
    TFileName soundFile;
    User::LeaveIfError(iFs.PrivatePath(soundFile));
    soundFile.Append( KSoundFileName );
    TParsePtrC parse((CEikonEnv::Static()->EikAppUi()->Application())->AppFullName());
    soundFile.Insert(0, KCDriveWithColon);
    iPlayer = CMdaAudioPlayerUtility::NewL(*this);
    iPlayer->OpenFileL(soundFile);
    iIsInitDone = EFalse;
}

void CScrShotsSound::PlaySoundL()
{
    if(iIsInitDone){
        iPlayer->Play();
        iIsBeingPlayed = ETrue;
    }
}

void CScrShotsSound::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration)
{
    if(aError == KErrNone)
        iIsInitDone = ETrue;
}

void CScrShotsSound::MapcPlayComplete(TInt aError)
{
    if(aError == KErrNone)
        iIsBeingPlayed = ETrue;
}

//---------------CScrShotsProducer impl starts here------------------------------

CScrShotsProducer* CScrShotsProducer::NewL(CScrShotsSettings* aScrShotsSettings, MScrShotProgressObserver* aObserver, MOrientationNotifier* aOrientationNotifier)
{
	CScrShotsProducer* self = new (ELeave) CScrShotsProducer(aScrShotsSettings, aObserver, aOrientationNotifier);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
}

CScrShotsProducer::~CScrShotsProducer()
{
    delete iSystemTool;
	delete iIntervalTimer;
	//forget about the rest of the images!!!
	iImgList.ResetAndDestroy();
	iImgList.Close();
	delete iLBitmap;
	delete iPBitmap;
	delete iScreenDevice;
	iWs.Close();
	iAppName.Close();
	delete iSoundPlayer;
#ifdef ENABLE_WATERMARK	
	iWatermark.Close();
#endif //ENABLE_WATERMARK
	delete iSaveFolderPath;
	delete iBmpFileName;
}

CScrShotsProducer::CScrShotsProducer(CScrShotsSettings* aScrShotsSettings, MScrShotProgressObserver* aObserver, MOrientationNotifier* aOrientationNotifier):
						iScrShotsSettings(aScrShotsSettings)
						,iObserver(aObserver)
                        ,iOrientationNotifier(aOrientationNotifier)
                        ,iCaptureMode(ECaptureNone) 
#ifdef ENABLE_WATERMARK
                        ,iWatermarkFont(CEikonEnv::Static()->LegendFont())
#endif //ENABLE_WATERMARK
{
	
}

void CScrShotsProducer::ConstructL()
{
	iIntervalTimer = CTimerX::NewL(this);
	iLandscapeScreenShotAreaSize = TSize(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight,CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth);;
	iPotraitScreenShotAreaSize = CEikonEnv::Static()->ScreenDevice()->SizeInPixels();
	User::LeaveIfError(iWs.Connect());
	iScreenDevice = new (ELeave) CWsScreenDevice(iWs);
	iScreenDevice->Construct();
	iLBitmap = new(ELeave)CFbsBitmap;
	iLBitmap->Create(iLandscapeScreenShotAreaSize,CEikonEnv::Static()->ScreenDevice()->DisplayMode());
    iPBitmap = new(ELeave)CFbsBitmap;
    iPBitmap->Create(iPotraitScreenShotAreaSize,CEikonEnv::Static()->ScreenDevice()->DisplayMode());
	
	iSystemTool = CScrShotsSystemEventObserver::NewL(this);
	iSoundPlayer = CScrShotsSound::NewL();
#ifdef ENABLE_WATERMARK    
    iWatermark.CreateL(255);	
	iScrShotsSettings->WatermarkText(iWatermark);
	iWatermarkpos = TWatermarkPosition(iScrShotsSettings->WatermarkPosition());
	CalculateWatermarkPoint();
#endif //ENABLE_WATERMARK
	iBmpFileName = HBufC::NewL(KBufferSize * 4);
}

void CScrShotsProducer::DeleteHeadAndRemoveRawFileL()
{
    //remove the file name entry from the q    
    HBufC* fileName = iImgList[0];
    iImgList.Remove(0);

    //remove the file from the fs
    CEikonEnv::Static()->FsSession().Delete(fileName->Des());
    delete fileName;
}

void CScrShotsProducer::GetFileNameFromDB()
{
	TBuf<KBufferSize> saveFolderPath;
	TBuf<4> saveDrv;
	
	iScrShotsSettings->OutputFileNameL(iNameSeries);
	iScrShotsSettings->OutputFolderNameL(saveFolderPath);
	iScrShotsSettings->OutputFolderDrive(saveDrv);
	
	RFs& fsSession = CEikonEnv::Static()->FsSession();
	//TODO: Probably this delete is not required
	if(iSaveFolderPath)
		delete iSaveFolderPath;
	iSaveFolderPath = HBufC::NewL(1024);
	//check if the path exists
	iSaveFolderPath->Des().Format(KPathFormatStringWithColon,&saveDrv,&saveFolderPath);
	if(!BaflUtils::FileExists(fsSession,iSaveFolderPath->Des())){
		fsSession.MkDirAll(iSaveFolderPath->Des());
	}
}

void CScrShotsProducer::TakeOneScrShotL(TInt aAfterInterval)
{
    iCaptureMode = ECaptureOneShot;
	//just cancel to be on the safer side.
	iIntervalTimer->StopTimerL();
	
	iForDuration = 0;
	iAfterDuration = aAfterInterval;
	iScrShotsCount = 0;
	GetFileNameFromDB();
	iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
}

void CScrShotsProducer::TakeMultipleScrShotL(TInt aAfterInterval,TInt aForDuration)
{
    iCaptureMode = ECaptureMultipleShots;
	iForDuration = aForDuration;
	iAfterDuration = aAfterInterval;
	iScrShotsCount = 0;
	GetFileNameFromDB();
	iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
}

TBool CScrShotsProducer::TakeMultipleScrShotOfAppL(const TDesC& aAppName,TInt aAfterInterval)
{
    iCaptureMode = ECaptureAppTillExit;
    iForDuration = 0;
    iAfterDuration = aAfterInterval;
    iScrShotsCount = 0;
	//launch app
    TBool result = iSystemTool->LaunchAppAndStartAppMonitorL(aAppName);
    if(result)
    {
		GetFileNameFromDB();
        iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
    }
    return result; 
}

TBool CScrShotsProducer::TakeMultipleScrShotOfAppForDurationL(const TDesC& aAppName
		,TInt aAfterInterval,TInt aForDuration)
{
    iCaptureMode = ECaptureAppForDuration;
    iForDuration = aForDuration;
    iAfterDuration = aAfterInterval;   
    iScrShotsCount = 0;
    //launch app
    TBool result = iSystemTool->LaunchAppAndStartAppMonitorL(aAppName);
    //start taking screenshots
    if(result)
    {
		GetFileNameFromDB();
        iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
    }
    return result;
}


void CScrShotsProducer::StopTakingScrShotsL()
{
    
    iIntervalTimer->StopTimerL();
    switch(iCaptureMode){
        case ECaptureAppForDuration:
        case ECaptureAppTillExit:
            iSystemTool->StopWatchingForExit();
            break;
    }
    //add this count to the settings and reset the count
    iObserver->HandleScrShotEventL(EScrShotTaskEnded);
    iScrShotsCount = 0;
    iCaptureMode = ECaptureNone;
}

void CScrShotsProducer::HandleTimerExpireL(TInt /*aTimerId*/)
{
    TakeScreenShotL();	
	if(iForDuration > 0){
		iForDuration -= iAfterDuration;
		if(iForDuration > 0){
		    if(iForDuration < iAfterDuration)
		        iIntervalTimer->StartSecondTimerL(iForDuration,EFalse);
		    else
		        iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
		}
	}else if(iCaptureMode == ECaptureAppTillExit){
	    iIntervalTimer->StartSecondTimerL(iAfterDuration,EFalse);
	}else if(iCaptureMode == ECaptureAppForDuration){
	    iSystemTool->StopWatchingForExit();    
    }
	if(!iIntervalTimer->IsActive()){
	    iObserver->HandleScrShotEventL(EScrShotTaskEnded);
	    iCaptureMode = ECaptureNone;
	}
}

void CScrShotsProducer::TakeScreenShotL()
{
#ifdef BRAHMA_DEBUG    
    TUint start = User::TickCount();
#endif
    //HBufC* sbmpFileName = HBufC::NewLC(KBufferSize * 4);
    
    if(iScrShotsSettings->IsSoundOn())
        PlayTickSoundL();
    
    TRect rect;
    if(iOrientationNotifier->GetCurrentOrientation() == CAknAppUiBase::EAppUiOrientationPortrait){
        rect = TRect(TPoint(0,0),iPotraitScreenShotAreaSize);
        iScreenDevice->CopyScreenToBitmap(iPBitmap,rect);
    }else if(iOrientationNotifier->GetCurrentOrientation() == CAknAppUiBase::EAppUiOrientationLandscape){
        rect = TRect(TPoint(0,0),iLandscapeScreenShotAreaSize);
        iScreenDevice->CopyScreenToBitmap(iLBitmap,rect);
    }
	
	//increment screenshot count 
	iScrShotsCount++;
	GenerateFileNameL(iBmpFileName);
#ifdef ENABLE_WATERMARK	
	//adding the watermark.
    CFbsBitGc*  bitmapGc = NULL;	
    CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(iBitmap);
    User::LeaveIfError(bitmapDevice->CreateContext(bitmapGc));
    //do some mojo with the draw so that it looks more "blended"
    bitmapGc->SetPenStyle(CGraphicsContext::EDotDashPen);
    bitmapGc->SetPenColor(KRgbRed);
    bitmapGc->UseFont(iWatermarkFont);
    bitmapGc->SetShadowColor(TRgb(134,134,134));
    bitmapGc->SetShadowMode(ETrue);
    bitmapGc->DrawText(iWatermark, iWatermarkPoint);
    bitmapGc->DiscardFont();
#endif //ENABLE_WATERMARK
    
    if(iOrientationNotifier->GetCurrentOrientation() == CAknAppUiBase::EAppUiOrientationPortrait){
        iPBitmap->Save(iBmpFileName->Des());
    }else if(iOrientationNotifier->GetCurrentOrientation() == CAknAppUiBase::EAppUiOrientationLandscape){
        iLBitmap->Save(iBmpFileName->Des());
    }

#ifdef ENABLE_WATERMARK     
    //we dont need the context any more
    delete bitmapGc;
    delete bitmapDevice;
#endif //ENABLE_WATERMARK    
        
	//CleanupStack::Pop();
	//add to the queue
	iImgList.Append(iBmpFileName);
//	if(iBmpFileName)
//		delete iBmpFileName;
	iBmpFileName = HBufC::NewL(KBufferSize * 4);
#ifdef BRAHMA_DEBUG	
	TUint end = User::TickCount();
	TBuf<256> msg;
	msg.Format(_L("TakeScreenShotL time taken : %d"), end-start);
	SLOGF(msg);
#endif
	iObserver->HandleScrShotEventL(EScrShotTaken);
}

void CScrShotsProducer::GenerateFileNameL(HBufC* aSbmpFileName)
{
	TBuf<KBufferSize> uniqFileName;
	TStringPoolObj fileName(128);
	fileName.Des().Format(_L("%S%d"),&iNameSeries,User::TickCount());
	if(fileName.Des().Length() > uniqFileName.MaxLength())
		User::Leave(KErrArgument);
	else
		uniqFileName.Copy(fileName.Des());
    TPtr saveFolderPath = iSaveFolderPath->Des();
	aSbmpFileName->Des().Format(_L("%S%S.sbmp"),&saveFolderPath,&uniqFileName);
}

void CScrShotsProducer::HandleSystemEventL(TSystemEventsList aEvent)
{
    switch(aEvent){
        case EAppClosed:{
        
            //just stop the timer, everything else is taken care of
            //dont stop the timer if the mode is take scr shot till duration
            if(iCaptureMode == ECaptureAppTillExit){
                iIntervalTimer->StopTimerL();
                iObserver->HandleScrShotEventL(EAppClosedByUser);
            }
        }
        break;
        case ESilentModeActivated:
            iScrShotsSettings->SetLightL(EFalse);
            iScrShotsSettings->SetSoundL(EFalse);
            break;
        case ESilentModeDeactivated:
            iScrShotsSettings->SetLightL(ETrue);
            iScrShotsSettings->SetSoundL(ETrue);
            break;
    }
}

void CScrShotsProducer::PlayTickSoundL()
{
    iSoundPlayer->PlaySoundL();
}

void CScrShotsProducer::CheckAndCloseLaunchedHiddenApp()
{
    switch(iCaptureMode){
        case ECaptureAppTillExit:{
            TInt wgid = iSystemTool->GetSelectedAppWindowGroupId();
            RWsSession session = CEikonEnv::Static()->WsSession();
            CApaWindowGroupName* wgName = CApaWindowGroupName::NewL( session, wgid );
            if(wgName->Hidden()){
                StopTakingScrShotsL();
            }
            delete wgName;
        }
        break;
    }
}

#ifdef ENABLE_WATERMARK
void CScrShotsProducer::UpdateWatermarkL(const TDesC& aWatermark,const TUint32 aWatermarkPosition)
{
    if(aWatermark.Length())
        iWatermark.Copy(aWatermark);
    if(aWatermarkPosition < EMaxPosition)
        iWatermarkpos = TWatermarkPosition(aWatermarkPosition);
    CalculateWatermarkPoint();
}

void CScrShotsProducer::CalculateWatermarkPoint()
{
    TInt txtWidth = iWatermarkFont->TextWidthInPixels(iWatermark);
    //keep the position ready so that we draw the water mark quickly 
    switch(iWatermarkpos){
        case ETopLeft:
            iWatermarkPoint = TPoint(10,iWatermarkFont->HeightInPixels() + 5);
            break;
        case ETopRight:
            iWatermarkPoint = TPoint(iScreenShotAreaSize.iWidth - txtWidth ,iWatermarkFont->HeightInPixels() + 5);
            break;
        case ECenterPos:{
            iWatermarkPoint = TPoint((iScreenShotAreaSize.iWidth/2) - (txtWidth/2),iScreenShotAreaSize.iHeight/2);
        }
            break;
        case EBottomLeft:
            iWatermarkPoint = TPoint(10,iScreenShotAreaSize.iHeight - iWatermarkFont->HeightInPixels());
            break;
        case EBottomRight:
            iWatermarkPoint = TPoint(iScreenShotAreaSize.iWidth - txtWidth,iScreenShotAreaSize.iHeight - iWatermarkFont->HeightInPixels());
            break;        
    }

}
#endif //ENABLE_WATERMARK
#endif
