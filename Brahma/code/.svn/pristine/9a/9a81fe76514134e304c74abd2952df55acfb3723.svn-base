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


// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
//#include <f32file.h>
//#include <s32file.h>

#include <scrshotsautomatic.rsg>

#include "ScrShotsAutomatic.hrh"
#include "ScrShotsAutomatic.pan"
#include "ScrShotsAutomaticApplication.h"
#include "ScrShotsAutomaticAppUi.h"
#include "ScrShotsAutomaticAppView.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsStringPool.h"
#include "ScrShotsMainControl.h"
#include "ScrShotsInfoScreen.h"
#include "ScrShotsAppFileList.h"
#include "ScrShotsCommonDefs.h"

#ifdef ENABLE_ENGINE
//#include "ScrShotsProducer.h"
#include "ScrShotsConvertor.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsStrLoader.h"

TScrShotsTheme ScrShotsThemeUtils::iCurrentTheme = TGrayTheme;

#endif

_LIT(KCustomFontName,"C:\\system\\apps\\brahma\\octin_stencil_free.gdr");

#include "EventLogger.h"

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif
// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppUi::ConstructL()
{
    
	// Initialise app UI with standard value.
	BaseConstructL(EAknEnableSkin);
    
	SLOGF(_L("BaseConstructL done"))
    //get the string pool ready
    ScrShotsStringPool::GetPool();
    
	//create the vector loader class
    iVectorLoader = CScrShotsVectorImgLoader::NewL();
    iVectorLoader->LoadAllVectorsL();
    iStrLoader = CScrShotsStrLoader::NewL();
    
    LoadCustomFontL();
	
	//first to create is the settings :)
#ifdef ENABLE_SETTINGS  
    SLOGF(_L("Initialising CScrShotsSettings"))
    iScrShotsSettings = CScrShotsSettings::NewL(this);
    
    //settings sanity check
    //check if the drive and folder exists, else fallback to the default ones
    TBuf<2> drv;
    iScrShotsSettings->OutputFolderDrive(drv);
    SLOGF(drv)
    if(!ScrShotsGenUtils::DriveExists(drv[0])){
        SLOGF(_L("Selected drive does not exist falling back to c:"))
        //set c: as the default drive
        iScrShotsSettings->SetOutputFolderDriveL(TChar('C'));
        iScrShotsSettings->SetOutputFolderNameL(KDefaultFileFolderName);
        iScrShotsSettings->SetDiskFillThresholdL(KDefaultThresholdPercent);
    }
    //selected application existence check
    TBuf<256> appName;
    iScrShotsSettings->AppNameForCaptureL(appName);
    TUint32 appUid = iScrShotsSettings->AppUidForCapture();
    if(!ScrShotsGenUtils::CheckIfAppExists(appName, TUid::Uid(appUid))){
        iScrShotsSettings->SetAppNameForCaptureL(KEmptyStr());
        iScrShotsSettings->SetAppUidForCaptureL(0);
    }
#endif
#ifdef ENABLE_ENGINE    
    SLOGF(_L("Creating CScrShotsDeviceWatcher obj"))
    iDeviceWatcher = CScrShotsDeviceWatcher::NewL(*iScrShotsSettings, this);
#endif
    SLOGF(_L("Calling ScrShotsGenUtils::DumpJsFileL"));
    //we need to dump the js file for loading in the info screen
    ScrShotsGenUtils::DumpJsFileL(iScrShotsSettings);

	// Create view object
#ifdef ENABLE_DEBUG_UI
	iAppView = CScrShotsAutomaticAppView::NewL(ClientRect(),*this);
#elif defined(ENABLE_THREE_SCREEN_UI)
	TRect noMenuRect(ClientRect().iTl,ApplicationRect().iBr);
	iAppView = CScrShotsAutomaticAppView::NewL(noMenuRect,*this, *iVectorLoader);
#endif
	SLOGF(_L("CScrShotsAutomaticAppView::NewL done"))
	StatusPane()->SwitchLayoutL( R_AVKON_STATUS_PANE_LAYOUT_USUAL_FLAT );
	UpdateScreenOrientation();
#ifdef ENABLE_ENGINE	
	SLOGF(_L("Initialising CScrShotsProducer"))
	iScrShotsProducer = CScrShotsProducer::NewL(iScrShotsSettings, this, this);
	SLOGF(_L("Initialising CScrShotsConvertor"))
	iScrShotsConvertor = CScrShotsConvertor::NewL(iScrShotsSettings, iScrShotsProducer);
	SLOGF(_L("Initialising engines done"))
#endif
	
	//now check the profile setting and make the sound off accordingly
	if(iDeviceWatcher->IsProfileSilentL()){
	    iScrShotsSettings->SetSoundL(EFalse);
	}
	
	iAppView->GetMainControl()->UpdateSelectedAppIconL();
	iAppView->StartAppMonitoring();
	//start watching the disk
	TUint32 diskThreshold = iScrShotsSettings->DiskFillThreshold();
	iScrShotsSettings->OutputFolderDrive(drv);
	iDeviceWatcher->StartWatchingDriveThresholdL(drv[0],diskThreshold);
	
}
// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppUi::CScrShotsAutomaticAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticAppUi::CScrShotsAutomaticAppUi()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppUi::~CScrShotsAutomaticAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticAppUi::~CScrShotsAutomaticAppUi()
{
    iAppView->StopAppMonitoring();
    delete iAppView;
    
    if(iVectorLoader)
        delete iVectorLoader;
#ifdef ENABLE_ENGINE
    if(iDeviceWatcher)
        iDeviceWatcher->StopWatchingDeviceParamsL();
    if(iScrShotsProducer)
        iScrShotsProducer->StopTakingScrShotsL();
    if(iScrShotsConvertor)
        iScrShotsConvertor->StopImageConsumerL();
    if(iScrShotsConvertor)
        delete iScrShotsConvertor;
    if(iScrShotsProducer)
        delete iScrShotsProducer;
    if(iDeviceWatcher)
        delete iDeviceWatcher;
#endif
#ifdef ENABLE_SETTINGS
    delete iScrShotsSettings;
#endif    
    
    FreeCustomFontL();
    
    if(iStrLoader)
        delete iStrLoader;
    
    ScrShotsStringPool::DestroyPool();
}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
 
void CScrShotsAutomaticAppUi::HandleCommandL(TInt aCommand)
	{
   
	switch (aCommand)
		{
		case EEikCmdExit:
#ifdef ENABLE_DEBUG_UI		    
		case EAknSoftkeyExit:
#endif		    
		    {
			    Exit();
		    }
			break;
#ifdef ENABLE_DEBUG_UI			
		case EWatermarkTopLeftCommand:
#ifdef ENABLE_SETTINGS          
            iScrShotsSettings->SetWatermarkPositionL(ETopLeft);
#endif		    
		    break;
		case EWatermarkTopRightCommand:
#ifdef ENABLE_SETTINGS          
            iScrShotsSettings->SetWatermarkPositionL(ETopRight);
#endif		    
            break;
		case EWatermarkBottomLeftCommand:
#ifdef ENABLE_SETTINGS          
            iScrShotsSettings->SetWatermarkPositionL(EBottomLeft);
#endif		    
            break;
		case EWatermarkBottomRightCommand:
#ifdef ENABLE_SETTINGS          
            iScrShotsSettings->SetWatermarkPositionL(EBottomRight);
#endif		    
            break;
		case ETakeOneScrShotCommand:
			{
#ifdef ENABLE_ENGINE			
			iScrShotsProducer->TakeOneScrShotL(5);
#endif			
			}
			break;
		case EMultipleScrShotsFor60sCommand:
			{
#ifdef ENABLE_ENGINE			
			iScrShotsProducer->TakeMultipleScrShotL(5,60);
#endif			
			}
			break;
		case EStartNotesAppTillExit:
			{
#ifdef ENABLE_ENGINE			
			iScrShotsProducer->TakeMultipleScrShotOfAppL(_L("Calculator"),2);
#endif			
			}
			break;
		case ESaveSettingsCommand:
		    {
#ifdef ENABLE_SETTINGS		    
		    iScrShotsSettings->SaveSettingsL();
#endif		    
		    }
		    break;
		case EAbout:
			{

			CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(title); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(msg); //msg
			dlg->RunLD();
			}
			break;
		default:
			Panic( EScrShotsAutomaticUi);
			break;
#endif//ENABLE_DEBUG_UI			
		}
	}
 
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppUi::HandleStatusPaneSizeChange()
{    
#ifdef ENABLE_DEBUG_UI
    iAppView->SetRect(ClientRect());
#elif defined(ENABLE_THREE_SCREEN_UI)    
    TRect noMenuRect(ClientRect().iTl,ApplicationRect().iBr);
	iAppView->SetRect(noMenuRect);
#endif
}

#ifdef ENABLE_SETTINGS
void CScrShotsAutomaticAppUi::HandleSettingChangeL(const TChangedSettingData& aChangedSetting)
{
    switch(aChangedSetting.iSettingName){
        case ESaveImgFormat:
        case EOutputFileName:
        case EOutputFolderName:
        case EOutputFolderDrive:
        case EAppNameForCapture:
        case ECaptureInterval:
        case ECaptureDuration:
        case EDiskFillThreshold:
        case EIsSoundOn:
        case EIsLightOn:
            if(iAppView)
                iAppView->SettingsUpdatedL(aChangedSetting);
            break;
            
#ifdef ENABLE_WATERMARK            
        case EWatermarkPosition:
            iScrShotsProducer->UpdateWatermarkL(KNullDesC,aChangedSetting.TChangedSettingValue.iChangedInt);
            break;
        case EWatermarkText:
            iScrShotsProducer->UpdateWatermarkL(aChangedSetting.TChangedSettingValue.iChangedString->Des(),EMaxPosition);
        break;
#endif        
    }
}
#endif


void DumpTypefaces(CWsScreenDevice* dev){
#if BRAHMA_DEBUG
    TInt numFaces = 0;
    numFaces = dev->NumTypefaces();
    TRACE_ON(RDebug::Printf(">>>No of numtypefaces : %d<<<",numFaces));
    for(int  i = 0;i < numFaces ; i++){
        TTypefaceSupport tfs;
        dev->TypefaceSupport(tfs, i);
        TRACE_ON(RDebug::Print(_L("Got typeface : %S!!"),&(tfs.iTypeface.iName)));
    }    
#endif
}

void CScrShotsAutomaticAppUi::LoadCustomFontL()
{
    CWsScreenDevice* dev = CEikonEnv::Static()->ScreenDevice();
    
    DumpTypefaces(dev);
    TInt errCode = dev->AddFile( KCustomFontName(), iCustomFontFileID );
    TRACE_ON(RDebug::Printf("Added Font loaded : %d!!!",iCustomFontFileID));
    DumpTypefaces(dev);
    
    TFontSpec fontSpec;
    fontSpec.iHeight = 13;
    TFontSpec myFontSpec( _L("LatinBold13"), fontSpec.iHeight );
    //errCode = dev->GetNearestFontToDesignHeightInTwips( iCustomFont, myFontSpec );
    errCode = dev->GetNearestFontInPixels(iCustomFont, myFontSpec);
    
    TFontSpec smallfontSpec;
    smallfontSpec.iHeight = 9;
    TFontSpec mysmallFontSpec( _L("LatinPlain12"), fontSpec.iHeight );
    errCode = dev->GetNearestFontInPixels(iCustomSmallFont, mysmallFontSpec);
    
#if 0 //other style of loading font, which did not work for me     
    TAlgStyle algStyle;
    algStyle.SetHeightFactor(10);
    algStyle.SetWidthFactor(10);
    algStyle.SetIsMono(ETrue);
    TUid fontUid;
    fontUid.iUid = iCustomFontFileID;
    errCode = dev->GetFontById(iCustomFont, fontUid, algStyle);
    
#endif    
    if(iCustomFont && iCustomSmallFont){
        TRACE_ON(RDebug::Printf("Custom Font loaded!!!"));
    }
}

void CScrShotsAutomaticAppUi::FreeCustomFontL()
{
    CEikonEnv::Static()->ScreenDevice()->ReleaseFont(iCustomSmallFont);
    CEikonEnv::Static()->ScreenDevice()->ReleaseFont(iCustomFont);
    TRACE_ON(RDebug::Printf("Removing custom font: %d!!!",iCustomFontFileID));
    CEikonEnv::Static()->ScreenDevice()->RemoveFile(iCustomFontFileID);
}

void CScrShotsAutomaticAppUi::HandleScrShotEventL(TScrShotEvent aEvent)
{
    switch(aEvent){
        case EScrShotTaken:
            iAppView->GetMainControl()->UpdateScrShotCountLabelL(ETrue);
            break;
        case EAppClosedByUser:
        case EScrShotTaskEnded:{
        //as this is called on exit so we need to place the trap harness to catch any 
        //leaves.
            TRAP_IGNORE(iAppView->GetMainControl()->HandleScrShotTaskEndedL());
            //dump the js file again so that we have the updated stats
            TRAP_IGNORE(ScrShotsGenUtils::DumpJsFileL(iScrShotsSettings));
            
            iAppView->GetInfoScreenControl()->ReloadHtmlContent();
            
        }
            break;
    }
}

CAknAppUi::TAppUiOrientation CScrShotsAutomaticAppUi::GetCurrentOrientation()
{
    return iChangedOrientation;
}

void CScrShotsAutomaticAppUi::HandleScreenDeviceChangedL()
{
    UpdateScreenOrientation();
    CAknAppUi::HandleScreenDeviceChangedL();
}

void CScrShotsAutomaticAppUi::UpdateScreenOrientation()
{
    TInt screenMode = CEikonEnv::Static()->ScreenDevice()->CurrentScreenMode();
    TPixelsTwipsAndRotation sizeAndRotation;
    CEikonEnv::Static()->ScreenDevice()->GetScreenModeSizeAndRotation(screenMode, sizeAndRotation);
    if(sizeAndRotation.iPixelSize.iWidth > sizeAndRotation.iPixelSize.iHeight)
        iChangedOrientation =  EAppUiOrientationLandscape;
    else
        iChangedOrientation = EAppUiOrientationPortrait;

}

void CScrShotsAutomaticAppUi::HandleWsEventL(const TWsEvent & aEvent, CCoeControl * aCtrl)
{
    switch(aEvent.Type())
    {
        case EEventWindowGroupsChanged:{
            iScrShotsProducer->CheckAndCloseLaunchedHiddenApp();
        }
        break;
    }
    CAknAppUi::HandleWsEventL(aEvent, aCtrl);
}

void CScrShotsAutomaticAppUi::HandleDiskThresholdReachedEventL()
{
    if(iScrShotsProducer && iScrShotsProducer->IsActive()){
        iScrShotsProducer->StopTakingScrShotsL();
        //show a dialog telling the user that the disk threshold 
        //has been reached so stopping taking screenshots
        CEikonEnv::Static()->AlertWin(_L("Stopped taking screenshots.Disk fill threshold has been reached."));
        CEikonEnv::Static()->AlertWin(_L("Empty disk or increase disk fill threshold."));
    }
}

TBool CScrShotsAutomaticAppUi::DiskHasSpace()
{
    TBool res = iDeviceWatcher->GetDiskWatcher()->DoesDiskHaveSpace();
    if(!res){
        CEikonEnv::Static()->AlertWin(_L("Cannot take screenshots."));
        CEikonEnv::Static()->AlertWin(_L("Empty disk or increase disk fill threshold."));
    }
    return res;
}

void CScrShotsAutomaticAppUi::HandleMemCardEventL(TBool aMemCardAdded)
{
    SLOGF(_L("CScrShotsAutomaticAppUi::HandleMemCardEventL"));
    if(iScrShotsProducer->IsActive() && !aMemCardAdded){
        iScrShotsProducer->StopTakingScrShotsL();
        CEikonEnv::Static()->AlertWin(_L("Stopped capturing screenshots! Target drive has been removed!"));       
    }
    iAppView->GetAppsFileListControl()->UpdateAppFileListScreenL();
}

void CScrShotsAutomaticAppUi::HandleAppInstallerEventL(TBool aAppInstalled)
{
    if(!aAppInstalled){
        //get the uid of the app
        //and check if it has been uninstalled
        TInt appUid;
        User::LeaveIfError(RProperty::Get(KUidSystemCategory, KUidSwiLatestInstallation, appUid));
        if(appUid == iScrShotsSettings->AppUidForCapture()){
            //reset the app name & uid
            iScrShotsSettings->SetAppNameForCaptureL(_L(""));
            iScrShotsSettings->SetAppUidForCaptureL(0);
        }
    }

    if(iAppView && iAppView->GetAppsFileListControl()){
        SLOGF(_L("Calling iAppView->GetAppsFileListControl()->UpdateAppFileListScreenL()"));
        iAppView->GetAppsFileListControl()->UpdateAppFileListScreenL();
    }
}


// End of File
