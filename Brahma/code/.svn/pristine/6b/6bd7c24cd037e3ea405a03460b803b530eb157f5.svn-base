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

#ifndef __SCRSHOTSMAINCONTROL_H__
#define __SCRSHOTSMAINCONTROL_H__

#ifdef ENABLE_THREE_SCREEN_UI
#include <coecntrl.h>
#include <mw/eikbctrl.h>
#include "ScrShotsGradientBitmapTool.h"
#include <aknutils.h>

#include "ScrShotsAutomaticAppView.h"
#include <eikedwin.h>
#include "ScrShotsNumberEditControl.h"

class CScrShotsVectorImgLoader;
class CScrShotsTextLabel;

// CScrShotsEdwin CLASS DECLARATION
class CScrShotsEdwin : public CEikEdwin 
{
    public:
    void Draw(const TRect& aRect) const;
};

enum TCurrentAction{
    EModeNone = 0,
    ESingleScrShot,
    EMultipleScrShot,
    EAppSingleScrShot,
    EAppMultipleScrShot
};

// CScrShotsMainControl CLASS DECLARATION
class CScrShotsMainControl : public CEikBorderedControl, MEikEdwinObserver, MDataSaveClient, MActiveScreenNotifier,MTimerXCallback 
{
    public:
        static CScrShotsMainControl* NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        virtual ~CScrShotsMainControl();
    
    public:
        // Functions from base classes
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();
        virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
        virtual CCoeControl * ComponentControl(TInt aIndex) const;
        virtual TInt CountComponentControls() const;        
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType );
        //from MEikEdwinObserver
        void HandleEdwinEventL(CEikEdwin* aEdwin,TEdwinEvent aEventType);
        //from MDataSaveClient
        // will be notified when its time to save the value in the db
        void DoDataSaveL(CScrShotsNumberEditCtrl* aCtrl, TInt aChangedValue);
        void HandleThresholdReached(TBool aIsUpperRange);
        void UpdateIconSizes();
        void PrepareForFocusGainL();
        TBool IsCurrentScreen();
        void UpdateFolderSelectValueL();
        void UpdateDriveDataL();
        void HandleAppSelectionDoneL(const TDesC& aSelectedAppName, const TUid& aSelectedAppUid);
        void UpdateSelectedAppIconL();
        void HandleScrShotTaskEndedL();
        void UpdateScrShotCountLabelL(TBool aDrawNow = EFalse);
        void HandleTimerExpireL(TInt aTimerId);
        void MakeEditorLoseFocus();

    private:
        void ConstructL(const TRect& aRect);
        CScrShotsMainControl(CScrShotsAutomaticAppView* aAppView,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        void DrawMainScreen(CWindowGc& aGc,const TRect aDrawRect) const;
        void LoadVectorsL();
        void ComputeGroupRects(const TRect& aScreenRect);
        void ComputeIconsSize();
        void CalculateSaveOptionsSubrects();
        void CalculateTimeOptionsSubrects();
        void CalculateActionSubrects();
        void CalculateOptionsSubrects();
        void LoadLabelControlsL();
        void LoadEdwinControlsL();
        CEikLabel* CreateAndSetLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect);
        CScrShotsTextLabel* CreateAndSetCustomLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect);
        void SaveNameSeriesTextL();
        void SetSelectedAppIconL(const TUid& aSelectedAppUid);
        TBool IsFileNameValid();
        
    private:
        CScrShotsGradientBitmapSkin* iSkinDrawer;
        CScrShotsAutomaticAppView* iAppView;//dont own,dont delete
        
        //these rect will contain the options and will help in detecting the touch event
        TRect iSaveOptionsRect, iTimingOptionsRect, iActionsRect, iOptionsRect;
        
        //save options sub rect
        TRect iDriveSelectRect, iFolderSelectRect,iNameSeriesRect,iFormatSelectRect;
#ifdef ENABLE_WATERMARK        
        TRect iWatermarkSelectRect;
#endif
        TRect iDriveSelectLabelRect, iDriveNameRect, iDriveFreeSpaceRect, iDriveThresholdRect, iDriveSelectActionRect;
        TRect iFolderSelectLabelRect, iFolderSelectValueRect, iFolderSelectGoToSelectionRect;
        TRect iNameSeriesLabelRect, iNameSeriesEditRect;
        TRect iFormatSelectLabelRect, iFormatSelectBmpRect, iFormatSelectJpegRect, iFormatSelectPngRect, iFormatSelectGifRect;
#ifdef ENABLE_WATERMARK
        TRect iWatermarkSelectLabelRect, iWatermarkSelectEditRect, iWatermarkSelectGotoSelectionRect;
#endif //ENABLE_WATERMARK
        
        //timing options sub rect
        TRect iIntervalSelectRect, iDurationSelectRect;
        TRect iIntervalTitleRect, iDurationTitleRect;
        TRect iIntervalSelectSecRect, iDurationSelectSecRect;
        
        //actions sub rect
        TRect iSingleScrShotRect, iMultipleScrShotRect, iAppMultipleScrShotTillExitRect, iAppMultipleScrShotForDurationRect, iSelectedAppIconRect;
        TRect iAppSelectLabelRect;
        //this will be activated once an action is fired
        TRect iStopActionRect, iCurrentModeLabelRect, iScrShotsCountLabelRect, iAppNameLabelRect;
        
        
        //menu options sub rect
        TRect iInfoButtonRect, iSoundButtonRect, iLightButtonRect, iColorsButtonRect, iExitButtonRect;
        CScrShotsActionFeedback& iButtonVibra;
        CScrShotsTextLabel* iFolderSelectLabel;
        CScrShotsTextLabel* iFolderSelectValueLabel;
        CScrShotsTextLabel* iDriveSelectLabel;
        CScrShotsTextLabel* iNameSeriesSelectLabel;
        CScrShotsTextLabel* iDriveFreeSpaceLabel, * iDriveThresholdLabel;
        CScrShotsTextLabel* iAppNameSelectedLabel;
        CScrShotsTextLabel* iFormatSelectLabel, *iIntervalTitleLabel, *iDurationTitleLabel;
        CScrShotsTextLabel* iModeLabel, *iScrShotsCountLabel, *iAppNameLabel;
#ifdef ENABLE_WATERMARK        
        CEikLabel* iWatermarkSelectLabel;
        CEikEdwin* iWatermarkEdwin;
#endif
        CScrShotsEdwin* iNameSeriesEdwin;
        CFont* iLabelFont;
        //CFont* iFolderSelectValueFont;
        RBuf iOutputFolderName;
        TBool iEdWinIsInEditMode;
        TBool iIsActionFired;
        CScrShotsVectorImgLoader& iVectorLoader;
        CScrShotsNumberEditCtrl* iIntervalEditCtrl, *iDurationEditCtrl;
        CGulIcon* iSelectedAppIcon;
        TCurrentAction iCurrentAction;
        CTimerX* iDriveUpdateTimer;
};
#endif //ENABLE_THREE_SCREEN_UI

#endif //__SCRSHOTSMAINCONTROL_H__
