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

#ifndef __SCRSHOTSAPPFILELIST_H__
#define __SCRSHOTSAPPFILELIST_H__

#ifdef ENABLE_THREE_SCREEN_UI
#include <coecntrl.h>
#include <mw/eikbctrl.h> 
#include "ScrShotsGradientBitmapTool.h"
#include <eiklbo.h>
#include <mw/eiklabel.h> 
#include "ScrShotsAutomaticAppView.h"
#ifdef ENABLE_CUSTOM_LISTBOX
#include "ScrShotsCustomList.h"
#else
class CEikColumnListBox;
#endif
#include "ScrShotsNumberEditControl.h"

class CScrShotsDriveGrid;
class CScrShotsTextLabel;


typedef struct {
    TBuf<256> iAppName;
    TUid        iAppUid;
}TAppListInfo;

class CScrShotsVectorImgLoader;

// CLASS DECLARATION
class CScrShotsAppFileList : public CEikBorderedControl
#if 0
,MEikListBoxObserver
#endif
, MEikScrollBarObserver, public MActiveScreenNotifier
{
    public:
        static CScrShotsAppFileList* NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView
                ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        virtual ~CScrShotsAppFileList();
    public:
        enum TListMode{
            EAppsListMode = 0,
            EFileListMode,
            EDriveListMode            
        };
    
    public:
        // Functions from base classes
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();
        virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
        virtual CCoeControl* ComponentControl(TInt aIndex) const;
        virtual TInt CountComponentControls() const; 
        void SetListMode(TListMode aViewMode);
        CScrShotsAutomaticAppView* GetAppView(){
            return iAppView;
        }
        TRect GetAppsFileListRect(){
            return iAppsFileListRect;
        }
//        TRect GetDrivesListRect(){
//            return iDriveListRect;
//        }
        CScrShotsVectorImgLoader& GetVectorImgLoader(){
            return iImgLoader;
        }
        CScrShotsActionFeedback& GetTouchFeedback(){
            return iButtonVibra;
        }
        
#if 0        
        //from MEikListBoxObserver
        void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
#endif        
        //from MEikScrollBarObserver
        virtual void HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);
        void UpdateIconSizes();
        void UpdateSelectionLabelL(const TDesC& aNewSelection);
        void HandleAppSelectionDoneL(const TDesC& aSelectedAppName, const TUid& aSelectedAppUid);
        TBool IsCurrentScreen();
        TInt CalculatedItemHeight(){
            return iCalculatedItemHeight;
        }
        CScrShotsCustomListbox* GetCustomListbox() {
            return iListBox;
        }
        void UpdateAppFileListScreenL();
        
    private:
        void ConstructL(const TRect& aRect);
        CScrShotsAppFileList(CScrShotsAutomaticAppView* aAppView
                ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        void ComputeRects(const TRect& aScreenRect, TBool aSizeChanged = EFalse);
#ifdef ENABLE_CUSTOM_LISTBOX
        void CreateCustomListboxL(TListMode aViewMode);
#else
        void CreateListboxL();
#endif
        void CreateOptionsL();
#ifdef ENABLE_CAPTION
        void LoadListAndChangeCaptionL(const TDesC& aScreenCaption, 
                TListMode aListMode, const TDesC& aSelectionCaption);
        void SetScreenCaptionL(const TDesC& aTitle);
#else
        void LoadListAndChangeCaptionL(TListMode aListMode, const TDesC& aSelectionCaption);
#endif
        

        
    private:        
        void LoadFolderList();
#ifdef ENABLE_DRIVE_LIST        
        void LoadDrivesList();
#endif
        void GoToFolder();
        void MoveUpToFolder();
        void SetCurrentSelectionNameL(const TDesC& aSelectionName);
        void ComputeListboxRects(TBool aSizeChanged = EFalse);
#ifdef ENABLE_DRIVE_LIST        
        void UpdateDiskThresholdRect();
#endif        
        void PromptAndCreateFolderNameL();
        void LoadDriveGridL();
        CScrShotsTextLabel* CreateAndSetCustomLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect);
        void UpdateDriveLabelsL();
        
    private:
        CScrShotsGradientBitmapSkin* iSkinDrawer;
#ifdef ENABLE_CAPTION        
        TRect iScreenCaptionRect;
#endif
        TRect iAppsFileListRect, iOptionsRect;
#ifdef ENABLE_DRIVE_LIST        
        TRect iDriveListRect, iDiskThresholdRect;
        TRect iDiskThresholdLabelRect, iDiskThresholdSliderRect, iDiskThresholdValueLabelRect;
#endif        
        TRect iSelectNameLabelRect, iCurrentNameLabelRect;
        TRect iBackButtonRect, iGoOneUpOptionRect, iCreateNewFolderRect, iExitButtonRect ;
        //this will appear in app selection and drive selection mode only
        TRect iRefreshButtonRect;
        CScrShotsTextLabel*  iSelectionName;
        CScrShotsTextLabel*  iCurrentName;
#ifdef ENABLE_CAPTION        
        CEikLabel*  iScreenCaption;
#endif
#ifdef ENABLE_DRIVE_LIST
        CEikLabel *iDiskThresholdLabel,  *iDiskThresholdValueLabel;
#endif        
        TListMode iCurrentListMode;
        //file list seq
        RArray<TFileName>       iCurrentFileList;
        RBuf                    iCurrentFolder;
        TChar                   iCurrentDrive;
        HBufC*          iMultiLineText; //dont delete
        CScrShotsAutomaticAppView* iAppView;//dont own
        CScrShotsActionFeedback& iButtonVibra;
#ifdef ENABLE_CUSTOM_LISTBOX        
        CScrShotsCustomListbox* iListBox;
#else
        CEikColumnListBox* iListBox;
#endif        
        CScrShotsVectorImgLoader& iImgLoader;
        TInt iPrevPos;
        //strings
        CScrShotsStrLoader* iStrLoader;//dont delete
        CScrShotsDriveGrid* iDriveGrid;
        TInt iCalculatedItemHeight;
        
};
#endif //ENABLE_THREE_SCREEN_UI

#endif //__SCRSHOTSAPPFILELIST_H__
