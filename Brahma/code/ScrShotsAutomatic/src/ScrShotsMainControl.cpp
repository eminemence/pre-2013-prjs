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

#include "ScrShotsMainControl.h"
#include "ScrShotsAppFileList.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsProducer.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsDeviceWatcher.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsStrLoader.h"
#include "ScrShotsTextLabel.h"
#include "ScrShotsStringPool.h"
#include "TimerX.h"

#ifdef ENABLE_THREE_SCREEN_UI
//#include <ScrShotsScreenIcons.mbg>
#include <ScrShotsAutomatic.rsg>
#include <eikapp.h>
#include <eikappui.h>

#include <mw/eikclbd.h> 
#include <eiklabel.h>
#include <aknutils.h>
#include <gulcolor.h>
#include <gdi.h>
#include <barsread.h> 
#include <gulicon.h>
#include <bautils.h>

//potrait config
#define KPotraitSaveOptionsRectHeightFactor 0.4
#define KPotraitTimingOptionsRectHeightFactor 0.3
#define KPotraitActionsRectHeightFactor 0.18
#define KPotraitOptionsRectHeightFactor 0.12
//landscape configs
#define KLandscapeSaveOptionsRectHeightFactor 0.8
#define KLandscapeTimingOptionsRectHeightFactor 0.6
#define KLandscapeActionsRectHeightFactor 0.4
#define KLandscapeOptionsRectHeightFactor 0.2

#if 1
#define KOptionsInfoButtonRectWidth 0.33
#define KOptionsSoundButtonRectWidth 0.33
#define KOptionsExitButtonRectWidth 0.33
#else // light and color implementation has been deferred
#define KOptionsInfoButtonRectWidth 0.15
#define KOptionsSoundButtonRectWidth 0.15
#define KOptionsLightButtonRectWidth 0.15
#define KOptionsColorsButtonRectWidth 0.40
#define KOptionsExitButtonRectWidth 0.15
#endif

#ifdef ENABLE_WATERMARK
#define KSaveOptionsDriveSelectHeight 0.20
#define KSaveOptionsFolderSelectHeight  0.20
#define KSaveOptionsNameSeriesHeight  0.20
#define KSaveOptionsFormatSelectHeight  0.20
#define KSaveOptionsWatermarkSelectHeight 0.20
#else
#define KSaveOptionsDriveSelectHeight 0.30
#define KSaveOptionsFolderSelectHeight  0.30
#define KSaveOptionsNameSeriesHeight  0.20
#define KSaveOptionsFormatSelectHeight  0.20
#endif

#define KFolderSelectLabelRect 0.25
#define KFolderSelectValueRect 0.55
#define KFolderSelectGotoSelectionRect 0.20 

#define KDriveSelectLabelRect 0.25
#define KDriveNameRectWidth 0.15
#define KDriveFreeSpaceRectWidth 0.45
#define KDriveFreeSpaceRectHeight 0.50
#define KDriveSelectActionRectWidth 0.15

#define KFormatSelectLabelWidth 0.25
#define KFormatSelectBmpWidth 0.1875
#define KFormatSelectJpegWidth 0.1875 
#define KFormatSelectPngWidth 0.1875
#define KFormatSelectGifWidth 0.1875

#define KNameSeriesLabelWidth 0.25
#define KNameSeriesEditWidth 0.75
#ifdef ENABLE_WATERMARK
#define KWatermarkSelectLabelRect 0.25
#define KWatermarkSelectValueRect 0.55
#define KWatermarkSelectGotoSelectionRect 0.20 
#endif
#define KTimingsIntervalRectWidth 0.45
#define KTimingsDurationRectWidth 0.55

#define KTimingsTitleRectHeight 0.20
#define KTimingsTimeControlsRectHeight 0.80
#define KIntervalNumberWidth 0.50
#define KDurationNumberWidth 0.33

#define KActionControlWidth 0.20
#define KActionForAppLabelHeight 0.30
#define KActionForAppActionHeight 0.70

#define KStopActionControlWidth 0.25
#define KModeLabelControlWidth 0.75
#define KModeLabelControlHeight 0.40
#define KScrShotCountLabelControlHeight 0.24
#define KSelectedAppNameLabelControlHeight 0.36

#define KMinInterValue 1
#define KMinDurationValue 10


#define KRoundRect4PixelBorder TSize(4,4)

#ifdef ENABLE_WATERMARK
#define KWatermarkControlCount 2
#else
#define KWatermarkControlCount 0
#endif

#define KBaseControlCount 13
#define KActionFiredLabelCount 3
#define KMainControlsComponentCount (KBaseControlCount + KActionFiredLabelCount + KWatermarkControlCount)


#ifdef ENABLE_WATERMARK
_LIT(KWatermarkSelectLabelString,"WATER\nMARK");
#endif 
//_LIT(KScrShotsCountLabelString,"Screen shots taken.");
//_LIT(KAppNameLabelString,"App selected.");
//_LIT(KIntervalTitleString,"INTERVAL(SECS)");
//_LIT(KDurationTitleString,"DURATION(SECS)");

//custom macros defn
#define _SET_NEXT_RECT_WITH_WIDTH(SrcRect, DestRect, ContainerRect, SizeFactor)     DestRect.iTl.iX = SrcRect.iBr.iX; \
DestRect.SetWidth(ContainerRect.Width() * SizeFactor);

#define _TEST_EDWIN_TO_LOSE_FOCUS_ if(iNameSeriesEdwin->IsFocused()){ \
    SaveNameSeriesTextL(); \
    iNameSeriesEdwin->SetFocus(EFalse); \
}

//--------------------------------------------------------------------------
//---------------------- CScrShotsEdwin impl--------------------------


void CScrShotsEdwin::Draw(const TRect& aRect) const 
{
    CWindowGc& gc = SystemGc();
    
    //now i will draw the border
    gc.SetPenStyle(CGraphicsContext::ESolidPen);
    gc.SetPenColor(ScrShotsThemeUtils::MainScreenLineColor());
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(ScrShotsThemeUtils::MainScreenGradientEndColor());
    gc.DrawRoundRect(aRect,TSize(4,4));

    //let edwin draw its things
    CEikEdwin::Draw(aRect);
}



//--------------------------------------------------------------------------
//---------------------- CScrShotsMainControl impl--------------------------

CScrShotsMainControl* CScrShotsMainControl::NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra, CScrShotsVectorImgLoader& aImgLoader)
{
    CScrShotsMainControl* self = new (ELeave) CScrShotsMainControl(aAppView, aButtonVibra, aImgLoader);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CScrShotsMainControl::CScrShotsMainControl(CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra, CScrShotsVectorImgLoader& aImgLoader):
        iAppView(aAppView)
,iButtonVibra(aButtonVibra)
,iVectorLoader(aImgLoader)
{

}

CScrShotsMainControl::~CScrShotsMainControl()
{
    delete iDriveUpdateTimer;
    iDriveUpdateTimer = NULL;
    
    if(iSelectedAppIcon){
        delete iSelectedAppIcon;
        iSelectedAppIcon = NULL;
    }

    iOutputFolderName.Close();
     
    delete iFolderSelectLabel;
    delete iFolderSelectValueLabel;
    delete iDriveSelectLabel;
    delete iNameSeriesSelectLabel;
    delete iFormatSelectLabel;
    delete iNameSeriesEdwin;
    delete iModeLabel;
    delete iScrShotsCountLabel;
    delete iAppNameLabel;
    delete iIntervalTitleLabel;
    delete iDurationTitleLabel;
    delete iIntervalEditCtrl;
    delete iDurationEditCtrl;
    delete iDriveFreeSpaceLabel;
    delete iDriveThresholdLabel;
    delete iAppNameSelectedLabel; 
    ScrShotsGenUtils::FreeFontL(iLabelFont);
//    ScrShotsGenUtils::FreeFontL(iFolderSelectValueFont);
    
    if(iSkinDrawer){
        delete iSkinDrawer;
        iSkinDrawer = NULL;
    }
}

void CScrShotsMainControl::Draw(const TRect& aRect) const
{
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect(Rect());
    iSkinDrawer->DrawSkin(gc,aRect);
    DrawMainScreen(gc,aRect);
    
    CEikBorderedControl::Draw(aRect);
}

void CScrShotsMainControl::SizeChanged()
{
    ComputeGroupRects(Rect());
    ComputeIconsSize();

    if(iIntervalEditCtrl)
        iIntervalEditCtrl->SetRect(iIntervalSelectSecRect);
    if(iDurationEditCtrl)
        iDurationEditCtrl->SetRect(iDurationSelectSecRect);

    if(iSkinDrawer){
        delete iSkinDrawer;
        iSkinDrawer = NULL;
    }
    iSkinDrawer = CScrShotsGradientBitmapSkin::NewL(Rect()
    		,ScrShotsThemeUtils::BackgroundSkinGradientStartColor() 
		, ScrShotsThemeUtils::BackgroundSkinGradientEndColor());
    DrawNow();
}

void CScrShotsMainControl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{
    CScrShotsSettings* appSettings = iAppView->GetAppSettings();
    
    TPoint touchPoint = aPointerEvent.iPosition;
    if(iSaveOptionsRect.Contains(touchPoint) && !iIsActionFired){ 
        if(iDriveSelectRect.Contains(touchPoint)){
            _TEST_EDWIN_TO_LOSE_FOCUS_
            if(aPointerEvent.iType == TPointerEvent::EButton1Up){
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
                iAppView->GetAppsFileListControl()->SetListMode(CScrShotsAppFileList::EDriveListMode);                
                iAppView->StartSwitchToListScreenL();
            }
        }else if(iFolderSelectRect.Contains(touchPoint)){
            _TEST_EDWIN_TO_LOSE_FOCUS_
            // if its a touch on anything other than the folder target 
            // label we will go to the next screen
            if(aPointerEvent.iType == TPointerEvent::EButton1Down){
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
                iAppView->GetAppsFileListControl()->SetListMode(CScrShotsAppFileList::EFileListMode);
                iAppView->StartSwitchToListScreenL();
            }
        }else if(iNameSeriesRect.Contains(touchPoint)){
            if(iNameSeriesEditRect.Contains(touchPoint)){
                if(aPointerEvent.iType == TPointerEvent::EButton1Down)
                    iNameSeriesEdwin->SetFocus( ETrue );
                iNameSeriesEdwin->HandlePointerEventL(aPointerEvent);
                TPointerEvent evt = aPointerEvent;
                //trying to force the vkb to be launched
                evt.iType = TPointerEvent::EButton1Up;
                iNameSeriesEdwin->HandlePointerEventL(evt);
            }
        }else if(iFormatSelectRect.Contains(touchPoint)){
            _TEST_EDWIN_TO_LOSE_FOCUS_
            if(!iFormatSelectLabelRect.Contains(touchPoint)  && aPointerEvent.iType == TPointerEvent::EButton1Down)
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            if(aPointerEvent.iType == TPointerEvent::EButton1Up){                
                if(iFormatSelectBmpRect.Contains(touchPoint)){
                    appSettings->SetSaveImageFormatL(EBmp);
                }else if(iFormatSelectJpegRect.Contains(touchPoint)){
                    appSettings->SetSaveImageFormatL(EJpeg);
                }else if(iFormatSelectPngRect.Contains(touchPoint)){
                    appSettings->SetSaveImageFormatL(EPng);
                }else if(iFormatSelectGifRect.Contains(touchPoint)){
                    appSettings->SetSaveImageFormatL(EGif);
                }
            }
        }
#ifdef ENABLE_WATERMARK         
        else if(iWatermarkSelectRect.Contains(touchPoint)){
            if(iWatermarkSelectEditRect.Contains(touchPoint)  && aPointerEvent.iType == TPointerEvent::EButton1Down){
                iWatermarkEdwin->SetFocus( ETrue );
                iWatermarkEdwin->HandlePointerEventL(aPointerEvent);
            }
        }
#endif
    }else if (iTimingOptionsRect.Contains(touchPoint) && !iIsActionFired){
        _TEST_EDWIN_TO_LOSE_FOCUS_
        if(iIntervalSelectSecRect.Contains(touchPoint)){
            iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            iIntervalEditCtrl->HandlePointerEventL(aPointerEvent);
        }else if(iDurationSelectSecRect.Contains(touchPoint)){
            iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            iDurationEditCtrl->HandlePointerEventL(aPointerEvent);
        }
    }else if (iActionsRect.Contains(touchPoint)){
        _TEST_EDWIN_TO_LOSE_FOCUS_
        if(aPointerEvent.iType != TPointerEvent::EButton1Down)
            return;
        if(iIsActionFired){
            if(iStopActionRect.Contains(touchPoint)){
                iAppView->GetAppUi().GetScrShotsProducer()->StopTakingScrShotsL();
                iDriveUpdateTimer->StopTimerL();
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
                iIsActionFired = EFalse;
                iCurrentAction = EModeNone;
            }
        } else{
            iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            CScrShotsStrLoader* strLoader = iAppView->GetAppUi().GetStrLoader();
            TPtr singlemsg          = strLoader->GetStringHBuf(ESingleScrShotActionString)->Des();
            TPtr multiplemsg        = strLoader->GetStringHBuf(EMultipleScrShotActionString)->Des();
            TPtr apptillexitmsg     = strLoader->GetStringHBuf(EMultipleScrShotActionFoAppTillExitString)->Des();
            TPtr appfordurationmsg  = strLoader->GetStringHBuf(EMultipleScrShotActionFoAppTillDurationString)->Des();
            TPtr appselectmsg       = strLoader->GetStringHBuf(EAppNotSelectedMsgString)->Des();
            TPtr applauncherrormsg       = strLoader->GetStringHBuf(EAppLaunchErrorMsgString)->Des();
            
            TInt interval = appSettings->CaptureInterval();
            TInt duration = appSettings->CaptureDuration();
            
            if(iAppSelectLabelRect.Contains(touchPoint) || iSelectedAppIconRect.Contains(touchPoint)){
                iAppView->GetAppsFileListControl()->SetListMode(CScrShotsAppFileList::EAppsListMode);
                iAppView->StartSwitchToListScreenL();
            }
            else if(interval > duration){
                CEikonEnv::Static()->InfoWinL(_L("Interval cannot be greater than duration.Please decrement interval value."), KNullDesC);
            }
            else{
                //just check if the touch is for a action and make sure that the name entered in the edwin is correct
                if(iSingleScrShotRect.Contains(touchPoint) || iMultipleScrShotRect.Contains(touchPoint) 
                        || iAppMultipleScrShotTillExitRect.Contains(touchPoint) || iAppMultipleScrShotForDurationRect.Contains(touchPoint)){
                    if(!IsFileNameValid()){
                        CEikonEnv::Static()->InfoWinL(_L("Please enter a valid file name for target file name series."), KNullDesC);
                        iNameSeriesEdwin->SetFocus(ETrue);
                        return;
                    }
                }
                if(iSingleScrShotRect.Contains(touchPoint)){
                    if(iAppView->GetAppUi().DiskHasSpace() && CEikonEnv::Static()->QueryWinL(singlemsg,KNullDesC)){
                        iCurrentAction = ESingleScrShot;
                        iAppView->GetAppUi().GetScrShotsProducer()->TakeOneScrShotL(interval);
                        iDriveUpdateTimer->StartSecondTimerL(5,ETrue);
                        iIsActionFired = ETrue;
                    }
                }else if(iMultipleScrShotRect.Contains(touchPoint)){
                    if(iAppView->GetAppUi().DiskHasSpace() && CEikonEnv::Static()->QueryWinL(multiplemsg,KNullDesC)){
                        iCurrentAction = EMultipleScrShot;
                        iAppView->GetAppUi().GetScrShotsProducer()->TakeMultipleScrShotL(interval, duration);
                        iDriveUpdateTimer->StartSecondTimerL(5,ETrue);
                        iIsActionFired = ETrue;
                    }
                }else if(iAppMultipleScrShotTillExitRect.Contains(touchPoint)){
                    TStringPoolObj appName(256);
                    appSettings->AppNameForCaptureL(appName.Des());
                    if(appSettings->AppUidForCapture() == 0){
                        CEikonEnv::Static()->AlertWin(appselectmsg);
                    }
                    else if(!ScrShotsGenUtils::CheckIfAppExists(appName.Des(), TUid::Uid(appSettings->AppUidForCapture()))){
                        CEikonEnv::Static()->AlertWin(_L("App not accessible. Please select another app!!"));
                    }
                    else if(iAppView->GetAppUi().DiskHasSpace() && CEikonEnv::Static()->QueryWinL(apptillexitmsg,KNullDesC)){
                        iCurrentAction = EAppSingleScrShot;
                        if(iAppView->GetAppUi().GetScrShotsProducer()->TakeMultipleScrShotOfAppL(appName.Des(),interval)){
                            iDriveUpdateTimer->StartSecondTimerL(5,ETrue);
                            iIsActionFired = ETrue;
                        }
                        else{
                            //show error dialog
                            CEikonEnv::Static()->AlertWin(applauncherrormsg);
                        }
                    }
                }else if(iAppMultipleScrShotForDurationRect.Contains(touchPoint)){
                    TStringPoolObj appName(256);
                    appSettings->AppNameForCaptureL(appName.Des());
                    if(appSettings->AppUidForCapture() == 0){
                        CEikonEnv::Static()->AlertWin(appselectmsg);
                    }
                    else if(!ScrShotsGenUtils::CheckIfAppExists(appName.Des(), TUid::Uid(appSettings->AppUidForCapture()))){
                        CEikonEnv::Static()->AlertWin(_L("App not accessible. Please select another app!!"));
                    }
                    else if(iAppView->GetAppUi().DiskHasSpace() && CEikonEnv::Static()->QueryWinL(appfordurationmsg,KNullDesC)){
                        iCurrentAction = EAppMultipleScrShot;
                        TStringPoolObj appName(256);
                        appSettings->AppNameForCaptureL(appName.Des());
                        if(iAppView->GetAppUi().GetScrShotsProducer()->TakeMultipleScrShotOfAppForDurationL(appName.Des(),interval, duration)){
                            iDriveUpdateTimer->StartSecondTimerL(5,ETrue);
                            iIsActionFired = ETrue;
                        }
                        else{
                            //show error dialog
                            CEikonEnv::Static()->AlertWin(applauncherrormsg);
                        }
                    }
                }
            }
            
            //action has been fired now change the icons and labels immediately.
            if(iIsActionFired){
                TAppStrList str = ENoneString;
                switch(iCurrentAction){
                    case ESingleScrShot:    str = ESingleScrShotModeString; break;
                    case EMultipleScrShot:  str = EMultipleScrShotModeString; break;
                    case EAppSingleScrShot: str = EAppScrShotTillExitModeString; break;
                    case EAppMultipleScrShot:   str = EAppScrShotForDurationModeString; break;                
                }
                //mode name
                CScrShotsStrLoader* strLoader = iAppView->GetAppUi().GetStrLoader();
                TStringPoolObj mode(256);
                 
                TPtr mptr1 = strLoader->GetStringHBuf(ECurrentModeLabelString)->Des();
                TPtr mptr2 = strLoader->GetStringHBuf(str)->Des();
                mode.Des().Format(mptr1,&mptr2);
                iModeLabel->SetTextL(mode.Des());
                
                //scr shot count
                UpdateScrShotCountLabelL();
//                TUint scrShotCount = iAppView->GetAppUi().GetScrShotsProducer()->GetScreenShotCount();
//                TPtr scrshotformat = strLoader->GetStringHBuf(EScrShotsCountLabelString)->Des();
//                mode.Des().Format(scrshotformat,scrShotCount);
//                iScrShotsCountLabel->SetTextL(mode.Des());
                
                //selected app name
                TPtr appNamePtr = strLoader->GetStringHBuf(EAppNameLabelString)->Des();
                if(iCurrentAction == EAppSingleScrShot || iCurrentAction == EAppMultipleScrShot){
                    //get the app selected from settings
                    TStringPoolObj appName(128);
                    appSettings->AppNameForCaptureL(appName.Des());
                    mode.Des().Format(appNamePtr,&(appName.Des()));
                }
                else{
                    TPtr noneStr = strLoader->GetStringHBuf(ENoneString)->Des();
                    mode.Des().Format(appNamePtr,&noneStr);
                }
                iAppNameLabel->SetTextL(mode.Des());
            }
        }
    }else if (iOptionsRect.Contains(touchPoint)){
        _TEST_EDWIN_TO_LOSE_FOCUS_
        if(aPointerEvent.iType != TPointerEvent::EButton1Down)
            return;        
        iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
        if (iInfoButtonRect.Contains(touchPoint)  && !iIsActionFired){
            iAppView->StartSwitchToInfoScreenL();
        }else if (iSoundButtonRect.Contains(touchPoint)){
            if(iAppView->GetAppUi().GetDeviceWatcher()->IsProfileSilentL() && !appSettings->IsSoundOn()){
                //show msg that sound cannot be made on in when phone is in silent mode
                HBufC* msg = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(ENoSoundInSilentProfileString);
                CEikonEnv::Static()->AlertWin(msg->Des());
            }
            else
            {
                iAppView->GetAppSettings()->SetSoundL(!appSettings->IsSoundOn());
                //priya::to check sound
                //iAppView->GetAppUi().GetScrShotsProducer()->PlayTickSoundL();
            }
        }
#if 0        
        else if (iLightButtonRect.Contains(touchPoint)){
            iAppView->GetAppSettings()->SetLightL(!iAppView->GetAppSettings()->IsLightOn());            
        }else if (iColorsButtonRect.Contains(touchPoint)){
            
        }
#endif        
        else if (iExitButtonRect.Contains(touchPoint)  && !iIsActionFired){
            HBufC* msg = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EExitPromptString);
            if(iAppView->GetAppUi().IsForeground() && CEikonEnv::Static()->QueryWinL(msg->Des(),KNullDesC))
                iAppView->GetAppUi().Exit();
        }
    }
    DrawNow();
}

void CScrShotsMainControl::ConstructL(const TRect& aRect)
{
    iOutputFolderName.Create(1024);
    iLabelFont = ScrShotsGenUtils::InitFontL(15);
//    iFolderSelectValueFont = ScrShotsGenUtils::InitFontL(12);
    
    CreateWindowL();
    SetRect(aRect);
    ComputeGroupRects(aRect);
    iSkinDrawer = CScrShotsGradientBitmapSkin::NewL(aRect, ScrShotsThemeUtils::BackgroundSkinGradientStartColor(), ScrShotsThemeUtils::BackgroundSkinGradientEndColor());
    
    LoadVectorsL();
    iIntervalEditCtrl = CScrShotsNumberEditCtrl::NewL(CScrShotsNumberEditCtrl::ETwoPane, iVectorLoader, this, this);
    iDurationEditCtrl = CScrShotsNumberEditCtrl::NewL(CScrShotsNumberEditCtrl::EThreePane, iVectorLoader, this, this);
    iIntervalEditCtrl->SetRect(iIntervalSelectSecRect);
    //load default values from the db
    iIntervalEditCtrl->SetValue(iAppView->GetAppSettings()->CaptureInterval());
    iIntervalEditCtrl->SetRanges(KMinInterValue,99);
    
    iDurationEditCtrl->SetRect(iDurationSelectSecRect);
    iDurationEditCtrl->SetValue(iAppView->GetAppSettings()->CaptureDuration());
    iDurationEditCtrl->SetRanges(KMinDurationValue,999);
    
    LoadLabelControlsL();
    LoadEdwinControlsL();
    
    //get the selection app bitmap as well
    UpdateSelectedAppIconL();

    SetBorder(TGulBorder::ERaised | TGulBorder::EThreeStep 
            //| TGulBorder::EWithInline 
            | TGulBorder::EWithOutline 
            | TGulBorder::EAddTwoPixels 
            | TGulBorder::EAddFourRoundingPixels);    
    
    iDriveUpdateTimer = CTimerX::NewL(this);
    //ActivateL();

}

void CScrShotsMainControl::DrawMainScreen(CWindowGc& aGc,const TRect /*aDrawRect*/) const 
{
    TInt centerX = 0;
    aGc.SetPenColor(ScrShotsThemeUtils::MainScreenLineColor());
    
    if(Rect().Width() > Rect().Height()){//we are in landscape mode so draw a line
        aGc.DrawLine(TPoint(iSaveOptionsRect.iBr.iX + 2, iSaveOptionsRect.iTl.iY), TPoint(iActionsRect.iTl.iX - 2,iActionsRect.iBr.iY));
    }
    
    //draw the main screen in order
    //---draw the drive selection controls---
    //aGc.DrawRect(iDriveSelectRect);
    aGc.DrawLine(TPoint(iDriveSelectRect.iTl.iX + 8, iDriveSelectRect.iBr.iY), TPoint(iDriveSelectRect.iBr.iX - 8, iDriveSelectRect.iBr.iY));
    aGc.DrawLine(TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iTl.iY + 8), TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iDriveNameRect.iBr.iX, iDriveNameRect.iTl.iY + 8), TPoint(iDriveNameRect.iBr.iX, iDriveNameRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iDriveSelectActionRect.iTl.iX, iDriveSelectActionRect.iTl.iY + 8), TPoint(iDriveSelectActionRect.iTl.iX, iDriveSelectActionRect.iBr.iY - 8));
//    aGc.DrawRoundRect(iDriveNameRect,KRoundRect4PixelBorder);
//    aGc.DrawRoundRect(iDriveFreeSpaceRect,KRoundRect4PixelBorder);
//    aGc.DrawRoundRect(iDriveThresholdRect,KRoundRect4PixelBorder);
//    aGc.DrawRoundRect(iDriveSelectActionRect,KRoundRect4PixelBorder);    
    TBuf<4> drvName;
    iAppView->GetAppSettings()->OutputFolderDrive(drvName);
    if(drvName.Find(KCDriveWithoutColon) == 0)
        aGc.BitBlt(TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iTl.iY),iVectorLoader.GetBitmap(ECDrvSelected));
    else if(drvName.Find(KDDriveWithoutColon) == 0)
        aGc.BitBlt(TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iTl.iY),iVectorLoader.GetBitmap(EDDrvSelected));
    else if(drvName.Find(KEDriveWithoutColon) == 0)
        aGc.BitBlt(TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iTl.iY),iVectorLoader.GetBitmap(EEDrvSelected));
    else if(drvName.Find(KFDriveWithoutColon) == 0)
        aGc.BitBlt(TPoint(iDriveNameRect.iTl.iX, iDriveNameRect.iTl.iY),iVectorLoader.GetBitmap(EFDrvSelected));
    aGc.BitBlt(TPoint(iDriveSelectActionRect.iTl.iX, iDriveSelectActionRect.iTl.iY),iVectorLoader.GetBitmap(EDrvSelectAction));
    

    //---draw the folder selection controls---
    //    aGc.DrawRoundRect(iFolderSelectRect,KRoundRect4PixelBorder);
    //aGc.DrawRect(iFolderSelectLabelRect);
//    aGc.DrawRoundRect(iFolderSelectValueRect,KRoundRect4PixelBorder);
//    aGc.DrawRoundRect(iFolderSelectGoToSelectionRect,KRoundRect4PixelBorder);
    aGc.DrawLine(TPoint(iFolderSelectLabelRect.iBr.iX, iFolderSelectLabelRect.iTl.iY + 8), TPoint(iFolderSelectLabelRect.iBr.iX, iFolderSelectLabelRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iFolderSelectRect.iTl.iX + 8, iFolderSelectRect.iBr.iY), TPoint(iFolderSelectRect.iBr.iX - 8, iFolderSelectRect.iBr.iY));
    aGc.DrawLine(TPoint(iFolderSelectGoToSelectionRect.iTl.iX , iFolderSelectGoToSelectionRect.iTl.iY + 8), TPoint(iFolderSelectGoToSelectionRect.iTl.iX, iFolderSelectGoToSelectionRect.iBr.iY - 8));
    
    
    //---draw the name series controls---
    //aGc.DrawRect(iNameSeriesRect);
    //aGc.DrawRect(iFormatSelectRect);
    //    aGc.DrawRoundRect(iNameSeriesLabelRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iNameSeriesEditRect,KRoundRect4PixelBorder);    
    aGc.DrawLine(TPoint(iNameSeriesRect.iTl.iX + 8, iNameSeriesRect.iBr.iY), TPoint(iNameSeriesRect.iBr.iX - 8, iNameSeriesRect.iBr.iY));
    aGc.DrawLine(TPoint(iNameSeriesLabelRect.iBr.iX, iNameSeriesLabelRect.iTl.iY + 8), TPoint(iNameSeriesLabelRect.iBr.iX, iNameSeriesLabelRect.iBr.iY - 8));
    
    
    
    //---draw the format selection conrols---
    aGc.DrawLine(TPoint(iFormatSelectBmpRect.iTl.iX, iFormatSelectBmpRect.iTl.iY + 8), TPoint(iFormatSelectBmpRect.iTl.iX, iFormatSelectBmpRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iFormatSelectBmpRect.iBr.iX, iFormatSelectBmpRect.iTl.iY + 8), TPoint(iFormatSelectBmpRect.iBr.iX, iFormatSelectBmpRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iFormatSelectJpegRect.iBr.iX, iFormatSelectJpegRect.iTl.iY + 8), TPoint(iFormatSelectJpegRect.iBr.iX, iFormatSelectJpegRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iFormatSelectPngRect.iBr.iX, iFormatSelectPngRect.iTl.iY + 8), TPoint(iFormatSelectPngRect.iBr.iX, iFormatSelectPngRect.iBr.iY - 8));
    if(iAppView->GetAppSettings()->SaveImageFormat() == EBmp)
        aGc.BitBlt(TPoint(iFormatSelectBmpRect.iTl.iX, iFormatSelectBmpRect.iTl.iY),iVectorLoader.GetBitmap(EBmpSelectedIcon));
    else
        aGc.BitBlt(TPoint(iFormatSelectBmpRect.iTl.iX, iFormatSelectBmpRect.iTl.iY),iVectorLoader.GetBitmap(EBmpUnselectedIcon));
    
    if(iAppView->GetAppSettings()->SaveImageFormat() == EJpeg)
        aGc.BitBlt(TPoint(iFormatSelectJpegRect.iTl.iX, iFormatSelectJpegRect.iTl.iY),iVectorLoader.GetBitmap(EJpegSelectedIcon));
    else
        aGc.BitBlt(TPoint(iFormatSelectJpegRect.iTl.iX, iFormatSelectJpegRect.iTl.iY),iVectorLoader.GetBitmap(EJpegUnselectedIcon)); 
    
    if(iAppView->GetAppSettings()->SaveImageFormat() == EPng)
        aGc.BitBlt(TPoint(iFormatSelectPngRect.iTl.iX, iFormatSelectPngRect.iTl.iY),iVectorLoader.GetBitmap(EPngSelectedIcon));
    else
        aGc.BitBlt(TPoint(iFormatSelectPngRect.iTl.iX, iFormatSelectPngRect.iTl.iY),iVectorLoader.GetBitmap(EPngUnselectedIcon));
        
    if(iAppView->GetAppSettings()->SaveImageFormat() == EGif)
        aGc.BitBlt(TPoint(iFormatSelectGifRect.iTl.iX, iFormatSelectGifRect.iTl.iY),iVectorLoader.GetBitmap(EGifSelectedIcon));
    else
        aGc.BitBlt(TPoint(iFormatSelectGifRect.iTl.iX, iFormatSelectGifRect.iTl.iY),iVectorLoader.GetBitmap(EGifUnselectedIcon));
        
    aGc.BitBlt(TPoint(iFolderSelectGoToSelectionRect.iTl.iX, iFolderSelectGoToSelectionRect.iTl.iY),iVectorLoader.GetBitmap(EFolderSelectAction));
    
    
    
    
    //---draw the interval selection controls---
    //    aGc.DrawRoundRect(iIntervalSelectRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iDurationSelectRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iIntervalTitleRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iDurationTitleRect,KRoundRect4PixelBorder);    
      aGc.DrawLine(TPoint(iIntervalTitleRect.iTl.iX + 8, iIntervalTitleRect.iTl.iY), TPoint(iDurationTitleRect.iBr.iX - 8, iDurationTitleRect.iTl.iY));
      aGc.DrawLine(TPoint(iDurationTitleRect.iTl.iX , iDurationTitleRect.iTl.iY + 8), TPoint(iDurationTitleRect.iTl.iX, iDurationTitleRect.iBr.iY - 8));
      aGc.DrawLine(TPoint(iIntervalSelectSecRect.iTl.iX + 8, iIntervalSelectSecRect.iTl.iY),TPoint(iDurationSelectSecRect.iBr.iX - 8,iIntervalSelectSecRect.iTl.iY));
      aGc.DrawLine(TPoint(iDurationSelectSecRect.iTl.iX , iDurationSelectSecRect.iTl.iY + 8),TPoint(iDurationSelectSecRect.iTl.iX,iDurationSelectSecRect.iBr.iY - 8));
      
      
    //---draw the action selection controls---
    //    aGc.DrawRoundRect(iSingleScrShotRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iMultipleScrShotRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iAppMultipleScrShotTillExitRect,KRoundRect4PixelBorder);
    //    aGc.DrawRoundRect(iAppMultipleScrShotForDurationRect,KRoundRect4PixelBorder);
    aGc.DrawLine(TPoint(iSingleScrShotRect.iTl.iX + 8, iSingleScrShotRect.iTl.iY), TPoint(iAppSelectLabelRect.iBr.iX - 8, iSingleScrShotRect.iTl.iY));
    if(iIsActionFired){
        //aGc.DrawRoundRect(iStopActionRect, TSize(4,4));
        aGc.BitBlt(TPoint(iStopActionRect.iTl.iX, iStopActionRect.iTl.iY),iVectorLoader.GetBitmap(EStopAction));
        //vertical line
        aGc.DrawLine(TPoint(iStopActionRect.iBr.iX, iStopActionRect.iTl.iY + 8), TPoint(iStopActionRect.iBr.iX, iStopActionRect.iBr.iY - 8));
        //horizontal line
        aGc.DrawLine(TPoint(iCurrentModeLabelRect.iTl.iX + 8, iCurrentModeLabelRect.iBr.iY), TPoint(iCurrentModeLabelRect.iBr.iX - 8, iCurrentModeLabelRect.iBr.iY));
        aGc.DrawLine(TPoint(iScrShotsCountLabelRect.iTl.iX + 8, iScrShotsCountLabelRect.iBr.iY), TPoint(iScrShotsCountLabelRect.iBr.iX - 8, iScrShotsCountLabelRect.iBr.iY));
        
//        aGc.DrawRoundRect(iCurrentModeLabelRect, TSize(4,4));
//        aGc.DrawRoundRect(iScrShotsCountLabelRect, TSize(4,4));
//        aGc.DrawRoundRect(iAppNameLabelRect, TSize(4,4));
    }else{
        //aGc.DrawRoundRect(iAppSelectLabelRect,KRoundRect4PixelBorder);
        //aGc.DrawRoundRect(iSelectedAppIconRect,KRoundRect4PixelBorder);
        //draw the box for drawing the app icon and the app name
        CArrayFix<TPoint>* ptArray = new (ELeave)CArrayFixFlat<TPoint>(5) ;
        ptArray->AppendL(iAppSelectLabelRect.iTl);
        ptArray->AppendL(TPoint(iSelectedAppIconRect.iTl.iX, iSelectedAppIconRect.iBr.iY));
        ptArray->AppendL(iSelectedAppIconRect.iBr);
        ptArray->AppendL(TPoint(iSelectedAppIconRect.iBr.iX, iSelectedAppIconRect.iTl.iY));
        ptArray->AppendL(iAppSelectLabelRect.iBr);
        ptArray->AppendL(TPoint(iAppSelectLabelRect.iBr.iX, iAppSelectLabelRect.iTl.iY));
        ptArray->AppendL(iAppSelectLabelRect.iTl);
        aGc.SetBrushColor(ScrShotsThemeUtils::MainScreenLineColor());
        aGc.DrawPolyLine(ptArray);
        delete ptArray;
        TRect appIconRect = iSelectedAppIconRect;
        appIconRect.Move(2,2);
        if(Rect().Width() > Rect().Height())
            appIconRect.Shrink(4,4);
        else
            appIconRect.Shrink(8,8);

        if(iSelectedAppIcon && iSelectedAppIcon->Bitmap()){
            aGc.DrawBitmap(appIconRect, iSelectedAppIcon->Bitmap());
        }
        else{
            aGc.DrawBitmap(appIconRect, iVectorLoader.GetBitmap(EAppDefaultIcon));
        }
        
        aGc.BitBlt(TPoint(iSingleScrShotRect.iTl.iX, iSingleScrShotRect.iTl.iY),iVectorLoader.GetBitmap(ESingleScrshot));
        aGc.BitBlt(TPoint(iMultipleScrShotRect.iTl.iX, iMultipleScrShotRect.iTl.iY),iVectorLoader.GetBitmap(EMultiScrshot));
        aGc.BitBlt(TPoint(iAppMultipleScrShotTillExitRect.iTl.iX, iAppMultipleScrShotTillExitRect.iTl.iY),iVectorLoader.GetBitmap(EAppScrshotTillExit));
        aGc.BitBlt(TPoint(iAppMultipleScrShotForDurationRect.iTl.iX, iAppMultipleScrShotForDurationRect.iTl.iY),iVectorLoader.GetBitmap(EAppMultiScrshotForDuration));    
    }
    
    
    //---draw the options selection controls---
    //draw lines on the right of each rect
    aGc.DrawLine(TPoint(iInfoButtonRect.iBr.iX, iInfoButtonRect.iTl.iY + 8), TPoint(iInfoButtonRect.iBr.iX, iInfoButtonRect.iBr.iY - 8));
    aGc.DrawLine(TPoint(iSoundButtonRect.iBr.iX, iSoundButtonRect.iTl.iY + 8), TPoint(iSoundButtonRect.iBr.iX, iSoundButtonRect.iBr.iY - 8));
    //for the final rect draw the line on the left
    aGc.DrawLine(TPoint(iExitButtonRect.iTl.iX, iExitButtonRect.iTl.iY + 8), TPoint(iExitButtonRect.iTl.iX, iExitButtonRect.iBr.iY - 8));
//    aGc.DrawRect(iLightButtonRect);
//    aGc.DrawRect(iColorsButtonRect);
    aGc.DrawLine(TPoint(iInfoButtonRect.iTl.iX + 8,iInfoButtonRect.iTl.iY), TPoint(iExitButtonRect.iBr.iX - 8,iExitButtonRect.iTl.iY));
    centerX = iInfoButtonRect.Width() / 2 - iVectorLoader.GetBitmap(EInfoIcon)->SizeInPixels().iWidth / 2;
    aGc.BitBlt(TPoint(iInfoButtonRect.iTl.iX + centerX, iInfoButtonRect.iTl.iY),iVectorLoader.GetBitmap(EInfoIcon));    
    
    if(iAppView->GetAppSettings()->IsSoundOn()){
        centerX = iSoundButtonRect.Width() / 2 - iVectorLoader.GetBitmap(ESoundOnIcon)->SizeInPixels().iWidth / 2;
        aGc.BitBlt(TPoint(iSoundButtonRect.iTl.iX + centerX, iSoundButtonRect.iTl.iY),iVectorLoader.GetBitmap(ESoundOnIcon));
    }
    else{
        centerX = iSoundButtonRect.Width() / 2 - iVectorLoader.GetBitmap(ESoundOffIcon)->SizeInPixels().iWidth / 2;
        aGc.BitBlt(TPoint(iSoundButtonRect.iTl.iX + centerX, iSoundButtonRect.iTl.iY),iVectorLoader.GetBitmap(ESoundOffIcon));
    }
    
    //todo: draw the light off icon
//    if(iAppView->GetAppSettings()->IsLightOn())
//        aGc.BitBlt(TPoint(iLightButtonRect.iTl.iX, iLightButtonRect.iTl.iY),iVectorLoader.GetBitmap(ESoundOnIcon));
//    else 
//        aGc.BitBlt(TPoint(iLightButtonRect.iTl.iX, iLightButtonRect.iTl.iY),iVectorLoader.GetBitmap(ESoundOffIcon));
    
    //aGc.BitBlt(TPoint(curX, curY),iSoundOffIcon);
    centerX = iExitButtonRect.Width() / 2 - iVectorLoader.GetBitmap(EExitIcon)->SizeInPixels().iWidth / 2;
    aGc.BitBlt(TPoint(iExitButtonRect.iTl.iX + centerX, iExitButtonRect.iTl.iY),iVectorLoader.GetBitmap(EExitIcon));

    
    //aGc.DrawRect(iWatermarkSelectRect);
#ifdef ENABLE_WATERMARK     
    aGc.DrawRoundRect(iWatermarkSelectEditRect,KRoundRect4PixelBorder);
    aGc.DrawRoundRect(iWatermarkSelectGotoSelectionRect,KRoundRect4PixelBorder);
#endif
}

void CScrShotsMainControl::LoadVectorsL()
{
    ComputeIconsSize();
}

void CScrShotsMainControl::UpdateIconSizes()
{
    ComputeIconsSize();
}

void CScrShotsMainControl::ComputeIconsSize()
{
    TInt optionsRectHeight = iOptionsRect.Size().iHeight - 4;
    TSize iconSize (optionsRectHeight, optionsRectHeight);

    iVectorLoader.SetIconSize(EBack,iconSize);
    iVectorLoader.SetIconSize(EExitIcon ,iExitButtonRect.Size());
    iVectorLoader.SetIconSize(ESoundOnIcon ,iSoundButtonRect.Size());
    iVectorLoader.SetIconSize(ESoundOffIcon ,iSoundButtonRect.Size());
    iVectorLoader.SetIconSize(EInfoIcon ,iInfoButtonRect.Size());
    iVectorLoader.SetIconSize(EDrvSelectAction ,iDriveSelectActionRect.Size());

    iVectorLoader.SetIconSize(EBmpSelectedIcon ,iFormatSelectBmpRect.Size());
    iVectorLoader.SetIconSize(EPngSelectedIcon ,iFormatSelectPngRect.Size());
    iVectorLoader.SetIconSize(EJpegSelectedIcon ,iFormatSelectJpegRect.Size());
    iVectorLoader.SetIconSize(EGifSelectedIcon ,iFormatSelectGifRect.Size());
    iVectorLoader.SetIconSize(EBmpUnselectedIcon ,iFormatSelectBmpRect.Size());
    iVectorLoader.SetIconSize(EPngUnselectedIcon ,iFormatSelectPngRect.Size());
    iVectorLoader.SetIconSize(EJpegUnselectedIcon ,iFormatSelectJpegRect.Size());
    iVectorLoader.SetIconSize(EGifUnselectedIcon ,iFormatSelectGifRect.Size());

    iVectorLoader.SetIconSize(ESingleScrshot ,iSingleScrShotRect.Size());
    iVectorLoader.SetIconSize(EMultiScrshot ,iMultipleScrShotRect.Size());
    iVectorLoader.SetIconSize(EAppScrshotTillExit ,iAppMultipleScrShotTillExitRect.Size());
    iVectorLoader.SetIconSize(EAppMultiScrshotForDuration ,iAppMultipleScrShotForDurationRect.Size());
    iVectorLoader.SetIconSize(EFolderSelectAction ,iFolderSelectGoToSelectionRect.Size());
    iVectorLoader.SetIconSize(EStopAction ,iStopActionRect.Size());
    
    iVectorLoader.SetIconSize(ECDrvSelected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(EDDrvSelected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(ECDrvUnselected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(EEDrvSelected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(EEDrvUnselected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(EFDrvSelected ,iDriveNameRect.Size());
    iVectorLoader.SetIconSize(EFDrvUnselected ,iDriveNameRect.Size());
    if(IsCurrentScreen()){
        if(iIntervalEditCtrl)
            iIntervalEditCtrl->UpdateIconSizes();
        if(iDurationEditCtrl)
            iDurationEditCtrl->UpdateIconSizes();
    }
    
}

void CScrShotsMainControl::ComputeGroupRects(const TRect& aScreenRect)
{
    TInt scrWidth = aScreenRect.Width();
    TInt scrHeight = aScreenRect.Height();
    if(scrWidth > scrHeight){ //we are in landscape mode
        TInt landscapewidth = (scrWidth>>1) - 4;
        iSaveOptionsRect = TRect(TPoint(4,4),TSize(landscapewidth, scrHeight * KLandscapeSaveOptionsRectHeightFactor));
        iOptionsRect = TRect(TPoint(4,iSaveOptionsRect.iBr.iY),TSize(landscapewidth, scrHeight * KLandscapeOptionsRectHeightFactor - 6));
        iTimingOptionsRect = TRect(TPoint(iSaveOptionsRect.iBr.iX,4),TSize(landscapewidth, scrHeight * KLandscapeTimingOptionsRectHeightFactor));
        iActionsRect = TRect(TPoint(iSaveOptionsRect.iBr.iX,iTimingOptionsRect.iBr.iY),TSize(landscapewidth, (scrHeight * KLandscapeActionsRectHeightFactor) - 8));        
    }else { //we are in potrait mode
        iSaveOptionsRect = TRect(TPoint(4,4),TSize(scrWidth-8, (scrHeight * KPotraitSaveOptionsRectHeightFactor)));
        iTimingOptionsRect = TRect(TPoint(4,iSaveOptionsRect.iBr.iY),TSize(scrWidth-8, (scrHeight * KPotraitTimingOptionsRectHeightFactor)));
        iActionsRect = TRect(TPoint(4,iTimingOptionsRect.iBr.iY),TSize(scrWidth-8, (scrHeight * KPotraitActionsRectHeightFactor)));
        iOptionsRect = TRect(TPoint(4,iActionsRect.iBr.iY),TSize(scrWidth-8, ((scrHeight * KPotraitOptionsRectHeightFactor) - 6)));       
    }
    //save settings sub rects
    CalculateSaveOptionsSubrects();
    
    CalculateTimeOptionsSubrects();
    
    CalculateActionSubrects();
    
    //calculate sub rects now
    CalculateOptionsSubrects();
}

void CScrShotsMainControl::CalculateSaveOptionsSubrects()
{
#ifdef ENABLE_WATERMARK    
    iDriveSelectRect = iFolderSelectRect = iNameSeriesRect = iFormatSelectRect = iWatermarkSelectRect = iSaveOptionsRect;
#else
    iDriveSelectRect = iFolderSelectRect = iNameSeriesRect = iFormatSelectRect = iSaveOptionsRect;
#endif
    iDriveSelectRect.SetHeight(iSaveOptionsRect.Height() * KSaveOptionsDriveSelectHeight);
    iDriveSelectLabelRect = iDriveNameRect = iDriveFreeSpaceRect = iDriveThresholdRect = iDriveSelectActionRect = iDriveSelectRect;
        
    iDriveSelectLabelRect.SetWidth(iDriveSelectRect.Width() * KDriveSelectLabelRect);
    //_SET_NEXT_RECT_WITH_WIDTH(SrcRect, DestRect, ContainerRect, SizeFactor)
    _SET_NEXT_RECT_WITH_WIDTH(iDriveSelectLabelRect, iDriveNameRect, iDriveSelectRect, KDriveNameRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iDriveNameRect, iDriveFreeSpaceRect, iDriveSelectRect, KDriveFreeSpaceRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iDriveNameRect, iDriveThresholdRect, iDriveSelectRect, KDriveFreeSpaceRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iDriveFreeSpaceRect, iDriveSelectActionRect, iDriveSelectRect, KDriveSelectActionRectWidth)
    iDriveFreeSpaceRect.SetHeight(iDriveSelectRect.Height() * KDriveFreeSpaceRectHeight);
    iDriveThresholdRect.iTl.iY = iDriveFreeSpaceRect.iBr.iY;
    iDriveThresholdRect.SetHeight(iDriveSelectRect.Height() * KDriveFreeSpaceRectHeight);
    
    
    if(iDriveSelectLabel)
        iDriveSelectLabel->SetExtent(iDriveSelectLabelRect.iTl, iDriveSelectLabelRect.Size());
    if(iDriveFreeSpaceLabel)
        iDriveFreeSpaceLabel->SetExtent(iDriveFreeSpaceRect.iTl, iDriveFreeSpaceRect.Size());
    if(iDriveThresholdLabel)
        iDriveThresholdLabel->SetExtent(iDriveThresholdRect.iTl, iDriveThresholdRect.Size());
    
    iFolderSelectRect.iTl.iY = iDriveSelectRect.iBr.iY;
    iFolderSelectRect.SetHeight(iSaveOptionsRect.Height() * KSaveOptionsFolderSelectHeight);
    iFolderSelectLabelRect = iFolderSelectValueRect = iFolderSelectGoToSelectionRect = iFolderSelectRect;
    iFolderSelectLabelRect.SetWidth(iFolderSelectRect.Width() * KFolderSelectLabelRect);
    
    _SET_NEXT_RECT_WITH_WIDTH(iFolderSelectLabelRect, iFolderSelectValueRect, iFolderSelectRect, KFolderSelectValueRect)
    _SET_NEXT_RECT_WITH_WIDTH(iFolderSelectValueRect, iFolderSelectGoToSelectionRect, iFolderSelectRect, KFolderSelectGotoSelectionRect)
    
    if(iFolderSelectLabel)
        iFolderSelectLabel->SetExtent(iFolderSelectLabelRect.iTl, iFolderSelectLabelRect.Size());
    if(iFolderSelectValueLabel)
        iFolderSelectValueLabel->SetExtent(iFolderSelectValueRect.iTl, iFolderSelectValueRect.Size());
    
    iNameSeriesRect.iTl.iY = iFolderSelectRect.iBr.iY;
    iNameSeriesRect.SetHeight(iSaveOptionsRect.Height() * KSaveOptionsNameSeriesHeight);
    iNameSeriesLabelRect = iNameSeriesEditRect = iNameSeriesRect;
    iNameSeriesLabelRect.SetWidth(iNameSeriesRect.Width() * KNameSeriesLabelWidth );
    _SET_NEXT_RECT_WITH_WIDTH(iNameSeriesLabelRect,iNameSeriesEditRect, iNameSeriesRect, KNameSeriesEditWidth )
    //just shave off the pixels from the border
    iNameSeriesEditRect.SetWidth( iNameSeriesEditRect.Width() - 4);
    if(iNameSeriesEdwin){
        TSize edwinSize = iNameSeriesEditRect.Size();
        edwinSize -= TSize(8,8);
        TPoint newPt = iNameSeriesEditRect.iTl; 
        newPt += TPoint(4,4);
        iNameSeriesEdwin->SetExtent(newPt, edwinSize);
    }
    if(iNameSeriesSelectLabel)
        iNameSeriesSelectLabel->SetExtent(iNameSeriesLabelRect.iTl, iNameSeriesLabelRect.Size());
    
    iFormatSelectRect.iTl.iY = iNameSeriesRect.iBr.iY;
    iFormatSelectRect.SetHeight(iSaveOptionsRect.Height() * KSaveOptionsFormatSelectHeight);
    iFormatSelectLabelRect = iFormatSelectBmpRect = iFormatSelectJpegRect = iFormatSelectPngRect = iFormatSelectGifRect = iFormatSelectRect;
    iFormatSelectLabelRect.SetWidth(iFormatSelectRect.Width() * KFormatSelectLabelWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iFormatSelectLabelRect, iFormatSelectBmpRect, iFormatSelectRect, KFormatSelectBmpWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iFormatSelectBmpRect, iFormatSelectJpegRect, iFormatSelectRect, KFormatSelectJpegWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iFormatSelectJpegRect, iFormatSelectPngRect, iFormatSelectRect, KFormatSelectPngWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iFormatSelectPngRect, iFormatSelectGifRect, iFormatSelectRect, KFormatSelectGifWidth)
    
    if(iFormatSelectLabel)
        iFormatSelectLabel->SetExtent(iFormatSelectLabelRect.iTl, iFormatSelectLabelRect.Size());
#ifdef ENABLE_WATERMARK
    iWatermarkSelectRect.iTl.iY = iFormatSelectRect.iBr.iY;
    iWatermarkSelectRect.SetHeight(iSaveOptionsRect.Height() * KSaveOptionsWatermarkSelectHeight);
     
    iWatermarkSelectLabelRect = iWatermarkSelectEditRect = iWatermarkSelectGotoSelectionRect = iWatermarkSelectRect;
    iWatermarkSelectLabelRect.SetWidth(iWatermarkSelectRect.Width() * KWatermarkSelectLabelRect);    
    _SET_NEXT_RECT_WITH_WIDTH(iWatermarkSelectLabelRect, iWatermarkSelectEditRect, iWatermarkSelectRect, KWatermarkSelectValueRect)
    _SET_NEXT_RECT_WITH_WIDTH(iWatermarkSelectEditRect, iWatermarkSelectGotoSelectionRect, iWatermarkSelectRect, KWatermarkSelectGotoSelectionRect)
    if(iWatermarkSelectLabel)
        iWatermarkSelectLabel->SetExtent(iWatermarkSelectLabelRect.iTl, iWatermarkSelectLabelRect.Size());
    if(iWatermarkEdwin)
        iWatermarkEdwin->SetExtent(iWatermarkSelectEditRect.iTl,iWatermarkSelectEditRect.Size());
#endif
}

void CScrShotsMainControl::CalculateTimeOptionsSubrects()
{
    iIntervalSelectRect = iDurationSelectRect = iTimingOptionsRect;
    iIntervalSelectRect.SetWidth(iTimingOptionsRect.Width() * KTimingsIntervalRectWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iIntervalSelectRect, iDurationSelectRect, iTimingOptionsRect, KTimingsDurationRectWidth);
    
    iIntervalTitleRect = iIntervalSelectSecRect = iIntervalSelectRect;
    iIntervalTitleRect.SetHeight(iIntervalSelectRect.Height() * KTimingsTitleRectHeight);
    iIntervalSelectSecRect.iTl.iY = iIntervalTitleRect.iBr.iY;
    iIntervalSelectSecRect.SetHeight(iIntervalSelectRect.Height() * KTimingsTimeControlsRectHeight);
//    iIntervalSelectSec2Rect.SetHeight(iIntervalSelectRect.Height() * KTimingsTimeControlsRectHeight);
//    iIntervalSelectSec1Rect.SetWidth(iIntervalSelectRect.Width() * KIntervalNumberWidth);
//    _SET_NEXT_RECT_WITH_WIDTH(iIntervalSelectSec1Rect, iIntervalSelectSec2Rect, iIntervalSelectRect, KIntervalNumberWidth);
    
    iDurationTitleRect = iDurationSelectSecRect = iDurationSelectRect;
    iDurationTitleRect.SetHeight(iDurationSelectRect.Height() * KTimingsTitleRectHeight);
    iDurationSelectSecRect.iTl.iY = iDurationTitleRect.iBr.iY;
    iDurationSelectSecRect.SetHeight(iDurationSelectRect.Height() * KTimingsTimeControlsRectHeight);
    
    if(iIntervalTitleLabel)
        iIntervalTitleLabel->SetExtent(iIntervalTitleRect.iTl, iIntervalTitleRect.Size());
    if(iDurationTitleLabel)
        iDurationTitleLabel->SetExtent(iDurationTitleRect.iTl, iDurationTitleRect.Size());

//    iDurationSelectSec2Rect.SetHeight(iDurationSelectRect.Height() * KTimingsTimeControlsRectHeight);
//    iDurationSelectSec3Rect.SetHeight(iDurationSelectRect.Height() * KTimingsTimeControlsRectHeight);
//    
//    iDurationSelectSec1Rect.SetWidth(iDurationSelectRect.Width() * KDurationNumberWidth);
//    _SET_NEXT_RECT_WITH_WIDTH(iDurationSelectSec1Rect, iDurationSelectSec2Rect, iDurationSelectRect, KDurationNumberWidth);
//    _SET_NEXT_RECT_WITH_WIDTH(iDurationSelectSec2Rect, iDurationSelectSec3Rect, iDurationSelectRect, KDurationNumberWidth);
}

void CScrShotsMainControl::CalculateActionSubrects()
{
    iSingleScrShotRect = iMultipleScrShotRect = iAppMultipleScrShotTillExitRect = iAppMultipleScrShotForDurationRect = iAppSelectLabelRect = iSelectedAppIconRect = iActionsRect;
    iSingleScrShotRect.SetWidth(iActionsRect.Width() * KActionControlWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iSingleScrShotRect, iMultipleScrShotRect, iActionsRect, KActionControlWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iMultipleScrShotRect, iSelectedAppIconRect, iActionsRect, KActionControlWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iSelectedAppIconRect, iAppMultipleScrShotTillExitRect , iActionsRect, KActionControlWidth);
    _SET_NEXT_RECT_WITH_WIDTH(iAppMultipleScrShotTillExitRect, iAppMultipleScrShotForDurationRect, iActionsRect, KActionControlWidth);
    iAppSelectLabelRect.iTl.iX = iMultipleScrShotRect.iBr.iX;
    iAppSelectLabelRect.SetWidth(iActionsRect.Width() * (KActionControlWidth * 3));
    iAppSelectLabelRect.SetHeight(iActionsRect.Height() * KActionForAppLabelHeight);
    iSelectedAppIconRect.iTl.iY = iAppMultipleScrShotTillExitRect.iTl.iY = iAppMultipleScrShotForDurationRect.iTl.iY = iAppSelectLabelRect.iBr.iY;
    iSelectedAppIconRect.SetHeight(iActionsRect.Height() * KActionForAppActionHeight);
    iAppMultipleScrShotTillExitRect.SetHeight(iActionsRect.Height() * KActionForAppActionHeight);
    iAppMultipleScrShotForDurationRect.SetHeight(iActionsRect.Height() * KActionForAppActionHeight);
    
    iStopActionRect = iCurrentModeLabelRect = iScrShotsCountLabelRect = iAppNameLabelRect = iActionsRect;
    iStopActionRect.SetWidth(iActionsRect.Width() * KStopActionControlWidth);
    iCurrentModeLabelRect.iTl.iX = iStopActionRect.iBr.iX;
    iCurrentModeLabelRect.SetWidth(iActionsRect.Width() * KModeLabelControlWidth);
    iCurrentModeLabelRect.SetHeight(iActionsRect.Height() * KModeLabelControlHeight);
    iScrShotsCountLabelRect.iTl.iX = iStopActionRect.iBr.iX;
    iScrShotsCountLabelRect.iTl.iY = iCurrentModeLabelRect.iBr.iY;
    iScrShotsCountLabelRect.SetWidth(iActionsRect.Width() * KModeLabelControlWidth);
    iScrShotsCountLabelRect.SetHeight(iActionsRect.Height() * KScrShotCountLabelControlHeight);
    iAppNameLabelRect.iTl.iX = iStopActionRect.iBr.iX;
    iAppNameLabelRect.iTl.iY = iScrShotsCountLabelRect.iBr.iY;
    iAppNameLabelRect.SetWidth(iActionsRect.Width() * KModeLabelControlWidth);
    iAppNameLabelRect.SetHeight(iActionsRect.Height() * KSelectedAppNameLabelControlHeight);
    if(iModeLabel)
        iModeLabel->SetExtent(iCurrentModeLabelRect.iTl, iCurrentModeLabelRect.Size());
    if(iScrShotsCountLabel)
        iScrShotsCountLabel->SetExtent(iScrShotsCountLabelRect.iTl, iScrShotsCountLabelRect.Size());
    if(iAppNameLabel)
        iAppNameLabel->SetExtent(iAppNameLabelRect.iTl, iAppNameLabelRect.Size());
    if(iAppNameSelectedLabel)
        iAppNameSelectedLabel->SetExtent(iAppSelectLabelRect.iTl, iAppSelectLabelRect.Size());
}

void CScrShotsMainControl::CalculateOptionsSubrects()
{
    iInfoButtonRect = iSoundButtonRect = iLightButtonRect = iColorsButtonRect = iExitButtonRect = iOptionsRect;
    iInfoButtonRect.SetWidth(iOptionsRect.Width() * KOptionsInfoButtonRectWidth);

    
#if 1
    _SET_NEXT_RECT_WITH_WIDTH(iInfoButtonRect, iSoundButtonRect, iOptionsRect, KOptionsSoundButtonRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iSoundButtonRect, iExitButtonRect, iOptionsRect, KOptionsExitButtonRectWidth)
#else
    _SET_NEXT_RECT_WITH_WIDTH(iInfoButtonRect, iSoundButtonRect, iOptionsRect, KOptionsSoundButtonRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iSoundButtonRect, iLightButtonRect, iOptionsRect, KOptionsLightButtonRectWidth)    
    _SET_NEXT_RECT_WITH_WIDTH(iLightButtonRect, iColorsButtonRect, iOptionsRect, KOptionsColorsButtonRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iColorsButtonRect, iExitButtonRect, iOptionsRect, KOptionsExitButtonRectWidth)
    _SET_NEXT_RECT_WITH_WIDTH(iColorsButtonRect, iExitButtonRect, iOptionsRect, KOptionsExitButtonRectWidth)    
#endif
    
}

CCoeControl* CScrShotsMainControl::ComponentControl(TInt aIndex) const
{
    if(iIsActionFired){
        switch(aIndex){
            case 0:            return iDriveSelectLabel;
            case 1:            return iFolderSelectLabel;
            case 2:            return iFolderSelectValueLabel;
            case 3:            return iNameSeriesSelectLabel;
            case 4:            return iFormatSelectLabel;
            case 5:            return iNameSeriesEdwin;
            case 6:            return iIntervalEditCtrl;
            case 7:            return iDurationEditCtrl;
            case 8:            return iIntervalTitleLabel;
            case 9:            return iDurationTitleLabel;
            case 10:            return iDriveFreeSpaceLabel;
            case 11:            return iDriveThresholdLabel;
            case 12:            return iScrShotsCountLabel;
            case 13:            return iAppNameLabel;
            case 14:            return iModeLabel;        
        }
    }
    else{
        switch(aIndex){
            case 0:            return iDriveSelectLabel;
            case 1:            return iFolderSelectLabel;
            case 2:            return iFolderSelectValueLabel;
            case 3:            return iNameSeriesSelectLabel;
            case 4:            return iFormatSelectLabel;
            case 5:            return iNameSeriesEdwin;
            case 6:            return iIntervalEditCtrl;
            case 7:            return iDurationEditCtrl;
            case 8:            return iIntervalTitleLabel;
            case 9:            return iDurationTitleLabel;
            case 10:            return iDriveFreeSpaceLabel;
            case 11:            return iDriveThresholdLabel;
            case 12:            return iAppNameSelectedLabel;                
    #ifdef ENABLE_WATERMARK            
            case KBaseControlCount :
                return iWatermarkSelectLabel;
            case (KBaseControlCount + 1):
                return iWatermarkEdwin;
    #endif            
                
        }
    }
    return NULL;
}

TInt CScrShotsMainControl::CountComponentControls() const
{
    if(iIsActionFired)
        return KBaseControlCount + KActionFiredLabelCount - 1;//we dont want the appnameselected label control to be drawn, so subtracting one
    else
        return KBaseControlCount;
    
}

void CScrShotsMainControl::UpdateFolderSelectValueL()
{
    iAppView->GetAppSettings()->GetOutputFolderCompletePathL(iOutputFolderName);
    TStringPoolObj outDir(512);
    iFolderSelectValueLabel->TrimFolderNameToShowLatestFolderL(iOutputFolderName,outDir.Des());
    iFolderSelectValueLabel->SetTextL(iOutputFolderName);
}

void CScrShotsMainControl::UpdateDriveDataL()
{
    TStringPoolObj drv(2),msg(256);
    iAppView->GetAppSettings()->OutputFolderDrive(drv.Des());
    TUint32 dskFillLimit = iAppView->GetAppSettings()->DiskFillThreshold();
    const RArray<TDriveExtraInfo>& dvArray = iAppView->GetAppUi().GetDeviceWatcher()->GetDriveListWithInfo(EFalse);
    for(int i = 0; i < dvArray.Count(); i++){
        const TDriveExtraInfo& data = dvArray[i];
        if(data.iDriveChar == drv.Des()[0]){
            ScrShotsGenUtils::SetFreeSizeText(iAppView->GetAppUi().GetStrLoader(), msg.Des(), data.iDrvFreeSize);
            if(iDriveFreeSpaceLabel){
                iDriveFreeSpaceLabel->SetTextL(msg.Des());
            }
            TPtr desPtr = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EDriveListThresholdSizeCellFormatString)->Des();
            msg.Des().Format(desPtr,dskFillLimit);
            if(iDriveThresholdLabel){
                iDriveThresholdLabel->SetTextL(msg.Des());
            }
            break;
        }
    }
}

void CScrShotsMainControl::LoadLabelControlsL()
{
    HBufC* txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EDriveSelectLabelString);
    iDriveSelectLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iDriveSelectLabelRect);
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EFreeSpaceString);
    iDriveFreeSpaceLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iDriveFreeSpaceRect);
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EThresholdPercentString);
    iDriveThresholdLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iDriveThresholdRect);
    
    UpdateDriveDataL();
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EFolderSelectLabelString);
    iFolderSelectLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iFolderSelectLabelRect);
#if 0    
    iFolderSelectValueLabel = new (ELeave) CEikLabel;
    iFolderSelectValueLabel->SetContainerWindowL( *this );
    iFolderSelectValueLabel->SetFont(iFolderSelectValueFont);
    //get the path from settings
    //iAppView->GetAppSettings()->GetOutputFolderCompletePathL(iOutputFolderName);
    //for testing
    iOutputFolderName.Copy(_L("C:\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\LONG\\"));
    //TODO: check if the string is to be broken across multiple lines add the logic to do it
    if(iFolderSelectValueFont->TextWidthInPixels(iOutputFolderName) > iFolderSelectValueRect.Size().iWidth - 4){
        //we need to break the text
        CArrayFix<TPtrC16> * wrappedLines = new (ELeave) CArrayFixFlat<TPtrC16>(5);
        AknTextUtils::WrapToArrayL(iOutputFolderName, iFolderSelectValueRect.Size().iWidth - 4, *iFolderSelectValueFont, *wrappedLines);
        iOutputFolderName.Zero();
        for(int i = 0; i < wrappedLines->Count() ; i++){
            iOutputFolderName.Append(wrappedLines->At(i));
            if(i != wrappedLines->Count() - 1)
                iOutputFolderName.Append(_L("\n"));
        }
        delete wrappedLines;
    }
    iFolderSelectValueLabel->SetTextL(iOutputFolderName);
    iFolderSelectValueLabel->SetAlignment(EHLeftVTop); 
    iFolderSelectValueLabel->OverrideColorL( EColorLabelText, ScrShotsThemeUtils::MainScreenTextColor() );
    iFolderSelectValueLabel->SetExtent(iFolderSelectValueRect.iTl, iFolderSelectValueRect.Size());
#else
    iFolderSelectValueLabel = CScrShotsTextLabel::NewL();
    iFolderSelectValueLabel->SetTextColor(ScrShotsThemeUtils::MainScreenTextColor());
    iFolderSelectValueLabel->SetExtent(iFolderSelectValueRect.iTl, iFolderSelectValueRect.Size());
    iFolderSelectValueLabel->SetLabelTextAlignment(CScrShotsTextLabel::ELeft);
    iFolderSelectValueLabel->SetFontL(iAppView->GetAppUi().GetCustomFont());
    //get the path from settings
    UpdateFolderSelectValueL();
#endif
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(ENameSeriesSelectLabelString);
    iNameSeriesSelectLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iNameSeriesLabelRect);
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(ETargetFormatSelectLabelString);
    iFormatSelectLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iFormatSelectLabelRect);
    
#ifdef ENABLE_WATERMARK    
    iWatermarkSelectLabel = CreateAndSetLabelL(KWatermarkSelectLabelString, ScrShotsThemeUtils::MainScreenTextColor(), iWatermarkSelectRect);
#endif
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(ECurrentModeLabelString);
    iModeLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iCurrentModeLabelRect);
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EScrShotsCountLabelString);
    iScrShotsCountLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iScrShotsCountLabelRect);
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EAppNameLabelString);
    iAppNameLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iAppNameLabelRect);
    //get the selected app name from the settings
    TStringPoolObj appName(256);
    iAppView->GetAppSettings()->AppNameForCaptureL(appName.Des());
    if(appName.Des().Length())
        iAppNameSelectedLabel = CreateAndSetCustomLabelL(appName.Des(), ScrShotsThemeUtils::MainScreenTextColor(), iAppSelectLabelRect);
    else{
        //get the label name from resource file
        TPtr msg = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EAppListCaption)->Des();
        iAppNameSelectedLabel = CreateAndSetCustomLabelL(msg, ScrShotsThemeUtils::MainScreenTextColor(), iAppSelectLabelRect);    
    }
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EIntervalTitleString);
    iIntervalTitleLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iIntervalTitleRect);
    
    txt1 = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EDurationTitleString);
    iDurationTitleLabel = CreateAndSetCustomLabelL(txt1->Des(), ScrShotsThemeUtils::MainScreenTextColor(), iDurationTitleRect);
}


CEikLabel* CScrShotsMainControl::CreateAndSetLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect)
{
    CEikLabel* label = new (ELeave) CEikLabel;
    label->SetContainerWindowL( *this );
    label->SetFont(iAppView->GetAppUi().GetCustomFont());
    label->SetTextL(aLabelText);
    label->SetAlignment(EHCenterVCenter);
    label->OverrideColorL( EColorLabelText, aLabelColor );
    label->SetExtent(aLabelRect.iTl, aLabelRect.Size());
    return label;
}

CScrShotsTextLabel* CScrShotsMainControl::CreateAndSetCustomLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect)
{
    CScrShotsTextLabel* label = CScrShotsTextLabel::NewL();
    label->SetTextColor(aLabelColor);
    label->SetExtent(aLabelRect.iTl, aLabelRect.Size());
    label->SetLabelTextAlignment(CScrShotsTextLabel::ECenter);
    label->SetFontL(iAppView->GetAppUi().GetCustomFont());
    //label->SetFontSize(19);
    label->SetTextL(aLabelText);
    return label;
}


void CScrShotsMainControl::LoadEdwinControlsL()
{
    iNameSeriesEdwin = new (ELeave) CScrShotsEdwin; 
    iNameSeriesEdwin->SetContainerWindowL(*this);
    {
        TResourceReader reader;
        CEikonEnv::Static()->CreateResourceReaderLC(reader, R_NAME_WATERMARK_EDIT);
        iNameSeriesEdwin->ConstructFromResourceL(reader);
        CleanupStack::PopAndDestroy(); // reader
    }
    
    
    iNameSeriesEdwin->SetBorder(TGulBorder::ESunken | TGulBorder::EAddTwoPixels | TGulBorder::EAddTwoRoundingPixels);
    iNameSeriesEdwin->ActivateL();
    iNameSeriesEdwin->SetEdwinObserver(this);
    iNameSeriesEdwin->SetAlignment(ELayoutAlignCenter);
    iNameSeriesEdwin->SetCursorVisible(ETrue);
    //this code set the edwin height to just about the font height
//    if(iNameSeriesEditRect.Height() > (iLabelFont->HeightInPixels() + 4 + 4)){
//        iNameSeriesEdwin->SetMaximumHeight(iLabelFont->HeightInPixels() + 4 + 4);
//        iNameSeriesEdwin->SetPosition(TPoint(iNameSeriesEditRect.iTl.iX, iNameSeriesEditRect.iTl.iY + (iNameSeriesEditRect.Height() / 2 - iNameSeriesEdwin->MaximumHeight() / 2)));
//    }
//    else
//        iNameSeriesEdwin->SetExtent(iNameSeriesEditRect.iTl, iNameSeriesEditRect.Size() - TSize(4,4));
    //this just sets the edwin height to the rect size
    iNameSeriesEdwin->SetExtent(iNameSeriesEditRect.iTl, iNameSeriesEditRect.Size() - TSize(4,4));
    
    TStringPoolObj msg(128);
    iAppView->GetAppSettings()->OutputFileNameL(msg.Des());
    iNameSeriesEdwin->SetTextL(&(msg.Des()));
#ifdef ENABLE_WATERMARK    
    iWatermarkEdwin = new (ELeave) CEikEdwin;
    iWatermarkEdwin->SetContainerWindowL(*this);
    {
        TResourceReader reader;
        CEikonEnv::Static()->CreateResourceReaderLC(reader, R_NAME_WATERMARK_EDIT);
        iWatermarkEdwin->ConstructFromResourceL(reader);    
        CleanupStack::PopAndDestroy(); // reader
    }
     
    iWatermarkEdwin->SetExtent(iWatermarkSelectEditRect.iTl,iWatermarkSelectEditRect.Size());
    iWatermarkEdwin->SetBorder(TGulBorder::ESunken | TGulBorder::EAddFourPixels | TGulBorder::EAddFourRoundingPixels);
    iWatermarkEdwin->ActivateL();
#endif
}

TKeyResponse CScrShotsMainControl::OfferKeyEventL( 
        const TKeyEvent& aKeyEvent, 
        TEventCode aType )
{
    if ( iNameSeriesEdwin != NULL
        && iNameSeriesEdwin->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
    {
        return EKeyWasConsumed;
    }
    return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
}

void CScrShotsMainControl::HandleEdwinEventL(CEikEdwin* aEdwin,TEdwinEvent aEventType)
{
    switch(aEventType){
        case EEventTextUpdate:{
            if(aEdwin->AknEdwinFlags() & EAknEditorFlagTouchInputModeOpened)
                iEdWinIsInEditMode = ETrue;
            //we dont want anything other than digits, alphabets and '_'
            TStringPoolObj edTxt(128);
            aEdwin->GetText(edTxt.Des());
            TInt curPos = aEdwin->CursorPos();
            TBool needReDraw = EFalse;
            for (TInt i = edTxt.Des().Length()-1; i > -1; i--)
                {
                TChar pchar = edTxt.Des()[i];
                if (!pchar.IsAlphaDigit() && (pchar != '_'))
                    {
                    edTxt.Des().Delete(i, 1);
                    needReDraw = ETrue;
                    }
                }
            if (needReDraw )
                {
                aEdwin->SetTextL(&edTxt.Des());
                aEdwin->HandleTextChangedL();
                aEdwin->SetCursorPosL(curPos, EFalse);
                }   
        }
            break;
    }
}

void CScrShotsMainControl::SaveNameSeriesTextL()
{
    if(iEdWinIsInEditMode){
        TStringPoolObj nameSeriesText(256);
        iNameSeriesEdwin->GetText(nameSeriesText.Des());
        iAppView->GetAppSettings()->SetOutputFileNameL(nameSeriesText.Des());
        iEdWinIsInEditMode = EFalse;
    }
}

void CScrShotsMainControl::DoDataSaveL(CScrShotsNumberEditCtrl* aCtrl, TInt aChangedValue)
{
    if(aCtrl == iIntervalEditCtrl && !aCtrl->IsValueSaved()){
        iAppView->GetAppSettings()->SetCaptureIntervalL(aChangedValue);
    }else if(aCtrl == iDurationEditCtrl && !aCtrl->IsValueSaved()){
        iAppView->GetAppSettings()->SetCaptureDurationL(aChangedValue);
    }    
}

void CScrShotsMainControl::PrepareForFocusGainL()
{
//    UpdateIconSizes();
//    iIntervalEditCtrl->UpdateIconSizes();
//    iDurationEditCtrl->UpdateIconSizes();
}

TBool CScrShotsMainControl::IsCurrentScreen()
{
    return (iAppView->GetCurrentScreen() == CScrShotsAutomaticAppView::EMainScreen);
}

void CScrShotsMainControl::HandleAppSelectionDoneL(const TDesC& aSelectedAppName, const TUid& aSelectedAppUid)
{
    if(aSelectedAppName.Length())
        iAppNameSelectedLabel->SetTextL(aSelectedAppName);
    else{
        TPtr msg = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EAppListCaption)->Des();
        iAppNameSelectedLabel->SetTextL(msg);
    }

    SetSelectedAppIconL(aSelectedAppUid);
    DrawNow();
}

void CScrShotsMainControl::SetSelectedAppIconL(const TUid& aSelectedAppUid)
{
    //delete the current icon
    if(iSelectedAppIcon){
        delete iSelectedAppIcon;
        iSelectedAppIcon = NULL;
    }
    
    if(!aSelectedAppUid.iUid){
        return;
    }    

    RPointerArray<TApaAppInfo> appdata = iAppView->GetAppUi().GetDeviceWatcher()->GetAppListWithInfoL(ETrue);
    if(iAppView->GetAppsFileListControl() && iAppView->GetAppsFileListControl()->GetCustomListbox()){
        //take the icon from the listbox and save it as selected icon
        CArrayPtr<CGulIcon>* appicons = iAppView->GetAppsFileListControl()->GetCustomListbox()->CustomIconArray();
        int ind = 0;
        int appCount = appdata.Count();
        for(TInt index = 2; index <  appCount + 2; index++){
            TApaAppInfo* app = appdata[ind++];
            if(app->iUid == aSelectedAppUid){
                CGulIcon* selectedappicon = appicons->At(index);
                CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
                bmp->Duplicate(selectedappicon->Bitmap()->Handle());
                AknIconUtils::SetSize(bmp, iSelectedAppIconRect.Size());
                iSelectedAppIcon = CGulIcon::NewL( bmp, bmp );
                break;
            }
        }
    }
}

void CScrShotsMainControl::UpdateSelectedAppIconL()
{
    TUid appUid;
    appUid.iUid = iAppView->GetAppSettings()->AppUidForCapture();
    SetSelectedAppIconL(appUid);
    DrawNow();
}

void CScrShotsMainControl::HandleScrShotTaskEndedL()
{
    //take the final count and update in db
    TUint scrShotCount = iAppView->GetAppUi().GetScrShotsProducer()->GetScreenShotCount();
    TUint currentSavedCount = iAppView->GetAppSettings()->TotalScrShotCount();
    iAppView->GetAppSettings()->SetTotalScrShotCount(currentSavedCount + scrShotCount);
    iIsActionFired = EFalse;
    DrawNow();
}

void CScrShotsMainControl::UpdateScrShotCountLabelL(TBool aDrawNow)
{
    TStringPoolObj mode(256);
    //get the scrshot count from db and update label
    TUint scrShotCount = iAppView->GetAppUi().GetScrShotsProducer()->GetScreenShotCount();
    TPtr scrshotformat = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EScrShotsCountLabelString)->Des();
    mode.Des().Format(scrshotformat,scrShotCount);
    iScrShotsCountLabel->SetTextL(mode.Des());
    if(aDrawNow)
        DrawNow();
}

void CScrShotsMainControl::HandleTimerExpireL(TInt aTimerId)
{
    UpdateDriveDataL();
    DrawNow();
}

void CScrShotsMainControl::HandleThresholdReached(TBool aIsUpperRange)
{
    if(aIsUpperRange){
        CEikonEnv::Static()->InfoWinL(_L("Upper limit has been reached.Cannot increment further."), KNullDesC);
    }
    else{
        CEikonEnv::Static()->InfoWinL(_L("Lower limit has been reached.Cannot decrement further."), KNullDesC);
    }
}

TBool CScrShotsMainControl::IsFileNameValid()
{
    TFileName fileName;
    iNameSeriesEdwin->GetText(fileName);
    //remove spaces if any 
    fileName.Trim();
    if(fileName.Length() && BaflUtils::Parse(fileName) == KErrNone)
        return ETrue;
    return EFalse;
}

void CScrShotsMainControl::MakeEditorLoseFocus()
{
    if(iNameSeriesEdwin)
        iNameSeriesEdwin->SetFocus(EFalse);
}

#endif //ENABLE_THREE_SCREEN_UI
