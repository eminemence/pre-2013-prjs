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
#include <coemain.h>
#include "ScrShotsAutomaticAppView.h"
#ifdef ENABLE_THREE_SCREEN_UI
#include "ScrShotsMainControl.h"
#include "ScrShotsAppFileList.h"
#include "ScrShotsInfoScreen.h"
#include "ScrShotsButtonFeedback.h"
#include "EventLogger.h"

const TInt KComponentCount = 3;
#endif //ENABLE_THREE_SCREEN_UI



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticAppView* CScrShotsAutomaticAppView::NewL(const TRect& aRect,CScrShotsAutomaticAppUi& aAppUi, CScrShotsVectorImgLoader& aImgLoader)
	{
    CScrShotsAutomaticAppView* self = new (ELeave) CScrShotsAutomaticAppView(aAppUi, aImgLoader);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppView::ConstructL(const TRect& aRect)
	{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);
	
#ifdef ENABLE_THREE_SCREEN_UI	
	TSize scrSize = CEikonEnv::Static()->ScreenDevice()->SizeInPixels();
	TRect centerRect(aRect.iTl,aRect.Size());
	TRect leftRect(TPoint(-scrSize.iWidth,aRect.iTl.iY),aRect.Size());
	TRect rightRect(TPoint(scrSize.iWidth,aRect.iTl.iY),aRect.Size());
	
	iButtonVibra = CScrShotsActionFeedback::NewL();
	iMainControl = CScrShotsMainControl::NewL(centerRect,this,*iButtonVibra, iImgLoader);
	iInfoScreen = CScrShotsInfoScreen::NewL(leftRect,this,*iButtonVibra, iImgLoader);
	iAppFileList = CScrShotsAppFileList::NewL(rightRect,this, *iButtonVibra, iImgLoader);
	
	iSwitchTimer = CTimerX::NewL(this);
	iAppUi.AddToStackL(iMainControl); 
#endif //ENABLE_THREE_SCREEN_UI
	
	// Activate the window, which makes it ready to be drawn
	ActivateL();
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::CScrShotsAutomaticAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticAppView::CScrShotsAutomaticAppView(CScrShotsAutomaticAppUi& aAppUi, CScrShotsVectorImgLoader& aImgLoader):
        iAppUi(aAppUi)
,iImgLoader(aImgLoader)
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::~CScrShotsAutomaticAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticAppView::~CScrShotsAutomaticAppView()
	{
    switch(iCurrentScreen){
        case EMainScreen:
            iAppUi.RemoveFromStack(iMainControl);
            break;
        case EAppsListScreen:
        case EFileListScreen:
            iAppUi.RemoveFromStack(iAppFileList);
            break;
        case EInfoScreen:
            iAppUi.RemoveFromStack(iInfoScreen);
            break;
    }
#ifdef ENABLE_THREE_SCREEN_UI
    delete iSwitchTimer;
    delete iMainControl;
    delete iInfoScreen;
    delete iAppFileList;
#endif //ENABLE_THREE_SCREEN_UI
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppView::Draw(const TRect& /*aRect*/) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppView::SizeChanged()
{
#ifdef ENABLE_THREE_SCREEN_UI
    TSize scrSize = CEikonEnv::Static()->ScreenDevice()->SizeInPixels();
    TRect centerRect(Position(),Rect().Size());
    TRect leftRect(TPoint(-scrSize.iWidth,Position().iY),Rect().Size());
    TRect rightRect(TPoint(scrSize.iWidth,Position().iY),Rect().Size());
    
    switch(iCurrentScreen){
        case EMainScreen:{
            if(iMainControl)
                iMainControl->SetRect(centerRect);
            if(iInfoScreen)
                iInfoScreen->SetRect(leftRect);
            if(iAppFileList)
                iAppFileList->SetRect(rightRect);        
        }
            break;
        case EAppsListScreen:
        case EFileListScreen:{
        if(iMainControl)
            iMainControl->SetRect(rightRect);
        if(iInfoScreen)
            iInfoScreen->SetRect(leftRect);
        if(iAppFileList)
            iAppFileList->SetRect(centerRect);        
        
        }
            break;
        case EInfoScreen:{
        if(iMainControl)
            iMainControl->SetRect(leftRect);
        if(iInfoScreen)
            iInfoScreen->SetRect(centerRect);
        if(iAppFileList)
            iAppFileList->SetRect(rightRect);        
        
        }
            break;
    }
#endif //ENABLE_THREE_SCREEN_UI    
	DrawNow();
}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticAppView::HandlePointerEventL(
		const TPointerEvent& /*aPointerEvent*/)
{
    //StartSwitchToInfoScreen();
//    CCoeControl::HandlePointerEventL(aPointerEvent);
}

#ifdef ENABLE_THREE_SCREEN_UI
CCoeControl* CScrShotsAutomaticAppView::ComponentControl(TInt aIndex) const
{
    switch(aIndex){
        case 0:
            return iMainControl;
        case 1:
            return iInfoScreen;
        case 2:
            return iAppFileList;
    }
    return NULL;
}

TInt CScrShotsAutomaticAppView::CountComponentControls() const 
{
    if(iMainControl && iInfoScreen && iAppFileList)
        return KComponentCount;
    return 0;
}

void CScrShotsAutomaticAppView::StartSwitchToInfoScreenL()
{
    iInfoScreen->UpdateIconSizes();
    iInfoScreen->DrawNow();
    iSwitchTimer->StartMicroSecTimerL(50000,ETrue);
    iSeqNum = 7;
    iNextIncrement = GetFibonacciNo(iSeqNum);
    iUsageNum = 0;
    iCurrentTransition = ESwitchToInfoScreen;
}

void CScrShotsAutomaticAppView::StartSwitchToListScreenL()
{
    iAppFileList->UpdateIconSizes();
    iAppFileList->DrawNow();
    iSwitchTimer->StartMicroSecTimerL(50000,ETrue);
    iSeqNum = 7;
    iNextIncrement = GetFibonacciNo(iSeqNum);
    iUsageNum = 0;
    iCurrentTransition = ESwitchToListScreen;
}

void CScrShotsAutomaticAppView::AddControlToStackL(CCoeControl* aOldControl, CCoeControl* aNewControl)
{
    if(aOldControl)
        iAppUi.RemoveFromStack(aOldControl);
    iAppUi.AddToStackL(aNewControl);
}


void CScrShotsAutomaticAppView::HandleTimerExpireL(TInt /*aTimerId*/)
{
    TPoint mainControlPos = iMainControl->Position();
    switch(iCurrentTransition){
        case ESwitchToListScreen:{
            TPoint appsFileListPos = iAppFileList->Position(); 
            if(iUsageNum > 3){
                iSeqNum++;
                iUsageNum = 0;
                iNextIncrement = GetFibonacciNo(iSeqNum);
            }
            appsFileListPos.iX -= iNextIncrement;
            mainControlPos.iX -= iNextIncrement;
            iUsageNum++;
            iMainControl->SetPosition(mainControlPos);
            iAppFileList->SetPosition(appsFileListPos);
            if(appsFileListPos.iX < 0){
                appsFileListPos.iX = 0;
                iAppFileList->SetPosition(appsFileListPos);
                iMainControl->SetPosition(TPoint(-iAppFileList->Size().iWidth,appsFileListPos.iY));
                iSwitchTimer->SetRepeat(EFalse);
                AddControlToStackL(iMainControl, iAppFileList);
                iSeqNum = 0;
                iCurrentScreen = EAppsListScreen;
                iAppFileList->UpdateIconSizes();
            }            
        }
            break;
        case ESwitchToInfoScreen:{
            TPoint infoScreenPos = iInfoScreen->Position(); 
            if(iUsageNum > 3){
                iSeqNum++;
                iUsageNum = 0;
                iNextIncrement = GetFibonacciNo(iSeqNum);
            }
            infoScreenPos.iX += iNextIncrement;
            mainControlPos.iX += iNextIncrement;
            iUsageNum++;
            iMainControl->SetPosition(mainControlPos);
            iInfoScreen->SetPosition(infoScreenPos);
            if(infoScreenPos.iX > 0){
                infoScreenPos.iX = 0;
                iInfoScreen->SetPosition(infoScreenPos);
                iMainControl->SetPosition(TPoint(iInfoScreen->Size().iWidth,infoScreenPos.iY));
                iMainControl->MakeEditorLoseFocus();
                iSwitchTimer->SetRepeat(EFalse);
                AddControlToStackL(iMainControl, iInfoScreen);
                iSeqNum = 0;
                iCurrentScreen = EInfoScreen;
            }                    
        }
            break;
        case ESwitchToMainScreen:{
            switch(iCurrentScreen){
                case EInfoScreen:{
                    TPoint infoScreenPos = iInfoScreen->Position(); 
                    if(iUsageNum > 3){
                        iSeqNum++;
                        iUsageNum = 0;
                        iNextIncrement = GetFibonacciNo(iSeqNum);
                    }
                    infoScreenPos.iX -= iNextIncrement;
                    mainControlPos.iX -= iNextIncrement;
                    iUsageNum++;
                    iMainControl->SetPosition(mainControlPos);
                    iInfoScreen->SetPosition(infoScreenPos);
                    if(mainControlPos.iX < 0){
                        mainControlPos.iX = 0;
                        iMainControl->SetPosition(mainControlPos);
                        iInfoScreen->SetPosition(TPoint(-iMainControl->Size().iWidth,mainControlPos.iY));
                        iSwitchTimer->SetRepeat(EFalse);
                        iMainControl->UpdateIconSizes();
                        iMainControl->DrawNow();
                        iSeqNum = 0;
                        iCurrentScreen = EMainScreen;
                        AddControlToStackL(iInfoScreen, iMainControl);
                    }                                    
                }
                    break;
                case EAppsListScreen:
                case EFileListScreen:                    
                    {
                        TPoint appsFileListPos = iAppFileList->Position(); 
                        if(iUsageNum > 3){
                            iSeqNum++;
                            iUsageNum = 0;
                            iNextIncrement = GetFibonacciNo(iSeqNum);
                        }
                        appsFileListPos.iX += iNextIncrement;
                        mainControlPos.iX += iNextIncrement;
                        iUsageNum++;
                        iMainControl->SetPosition(mainControlPos);
                        iAppFileList->SetPosition(appsFileListPos);
                        if(mainControlPos.iX > 0){
                            mainControlPos.iX = 0;
                            iMainControl->SetPosition(mainControlPos);
                            iAppFileList->SetPosition(TPoint(iMainControl->Size().iWidth,mainControlPos.iY));
                            iSwitchTimer->SetRepeat(EFalse);
                            iSeqNum = 0;
                            iCurrentScreen = EMainScreen;
                            iMainControl->UpdateIconSizes();
                            iMainControl->DrawNow();
                            AddControlToStackL(iAppFileList, iMainControl);
                        }            
                    }
                    break;
            }
        }
        break;
    }
    //DrawNow();
}

TInt CScrShotsAutomaticAppView::GetFibonacciNo(TInt aSeqNum)
{
    if(aSeqNum == 0)
        return 0;
    else if(aSeqNum == 1)
        return 1;
    else
        return GetFibonacciNo(aSeqNum - 1) + GetFibonacciNo(aSeqNum - 2);  
}

void CScrShotsAutomaticAppView::StartSwitchToMainControl()
{
    iSwitchTimer->StartMicroSecTimerL(50000,ETrue);
    iSeqNum = 7;
    iNextIncrement = GetFibonacciNo(iSeqNum);
    iUsageNum = 0;
    iCurrentTransition = ESwitchToMainScreen;                
}

void CScrShotsAutomaticAppView::SettingsUpdatedL(const TChangedSettingData& aChangedSetting)
{
    if(iMainControl){
        iMainControl->UpdateFolderSelectValueL();
        iMainControl->UpdateDriveDataL();
    }
    
    switch(aChangedSetting.iSettingName){
        case EOutputFolderDrive:{
        
            SLOGF(_L("CScrShotsAutomaticAppView::SettingsUpdatedL in EOutputFolderDrive"));
            if(iAppFileList)
                iAppFileList->UpdateAppFileListScreenL();
        }
        break;
        case EAppNameForCapture: 
        case EAppUidForCapture:{
            SLOGF(_L("app name / uid has changed...."))
            if(iMainControl){
                TBuf<256> selectAppName;
                TUid selectedAppUid;
                selectedAppUid.iUid = GetAppUi().GetAppSettings()->AppUidForCapture();
                GetAppUi().GetAppSettings()->AppNameForCaptureL(selectAppName);
                SLOGF(selectAppName);
                iMainControl->HandleAppSelectionDoneL(selectAppName, selectedAppUid);                
                iMainControl->UpdateSelectedAppIconL();
            }
        }
            break;
    }
    
    DrawNow();
}

void CScrShotsAutomaticAppView::StartAppMonitoring()
{
    Window().EnableGroupChangeEvents();
    Window().EnableFocusChangeEvents();
    Window().EnableGroupListChangeEvents();
    Window().EnableVisibilityChangeEvents();
}

void CScrShotsAutomaticAppView::StopAppMonitoring()
{
    Window().DisableGroupChangeEvents();
    Window().DisableFocusChangeEvents();
    Window().DisableGroupListChangeEvents();
    Window().DisableVisibilityChangeEvents();
}


#endif 


// End of File
