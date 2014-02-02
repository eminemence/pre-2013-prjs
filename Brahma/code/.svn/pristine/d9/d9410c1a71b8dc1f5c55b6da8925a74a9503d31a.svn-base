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

#include "ScrShotsAppFileList.h"
#include "ScrShotsDeviceWatcher.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsStrLoader.h"
#include "ScrShotsDriveGrid.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsTextLabel.h"
#include "ScrShotsStringPool.h"
#include "EventLogger.h"

#ifdef ENABLE_THREE_SCREEN_UI
#include <aknlists.h> 
#include <barsread.h> 
#include <eikclb.h> 
#include <StringLoader.h>
#include <ScrShotsAutomatic.rsg>
#include <mw/eikclbd.h> 
#include <eiklabel.h>
#include <aknutils.h>
#include <gulcolor.h>
#include <gdi.h>
#include <gulicon.h>
#include <mw/AknIconArray.h> 
//#include <CAknFileNamePromptDialog.h>
#include <mw/AknQueryDialog.h> 

//#define KLandscapeCaptionRectHeightFactor 0.12
#define KLandscapeListRectHeightFactor 0.70
#define KLandscapeSelectedNameTextRectHeightFactor 0.075
#define KLandscapeCurrentNameTextRectHeightFactor 0.075
#define KLandscapeOptionsRectHeightFactor 0.15

//#define KPotraitCaptionRectHeightFactor 0.08
#define KPotraitListRectHeightFactor 0.75
#define KPotraitSelectedNameTextRectHeightFactor 0.06
#define KPotraitCurrentNameTextRectHeightFactor 0.06
#define KPotraitOptionsRectHeightFactor 0.13

#define KOptionsSelectLabelWidth 0.6
#define KOptionsBackButtonWidth 0.25
#define KOptionsExitButtonWidth 0.25

#define KAppListScreenComponentCount 3

#define KDriveListRectHeightFactor 0.65
#define KDriveThresholdHeightFactor 0.35

#define KDiskThresholdLabelHeightFactor 0.25
#define KDiskThresholdSliderHeightFactor 0.50
#define KDiskThresholdValueLabelHeightFactor 0.25

#define KDriveListNumberofTextCells 2
#define KAppsListNumberofTextCells 3

//_LIT(KAppsListCaption, "Select app to take screenshots.");
//_LIT(KFolderListCaption, "Select target folder.");
//_LIT(KDriveListCaption, "Select target drive.");


#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#define PRINT_RECT(rect,rectStr) RDebug::Printf("________%s[x:%d y:%d w:%d h:%d]________",rectStr,rect.iTl.iX,rect.iTl.iY,rect.Width(),rect.Width());
#else
#define TRACE_ON(x)
#define PRINT_RECT(rect,rectStr)
#endif

CScrShotsAppFileList* CScrShotsAppFileList::NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader)
{
    CScrShotsAppFileList* self = new (ELeave) CScrShotsAppFileList(aAppView,aButtonVibra,aImgLoader);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CScrShotsAppFileList::CScrShotsAppFileList(CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader):
        iAppView(aAppView)
,iButtonVibra(aButtonVibra)
,iImgLoader(aImgLoader)
,iStrLoader(iAppView->GetAppUi().GetStrLoader())
{

}

CScrShotsAppFileList::~CScrShotsAppFileList()
{
    iCurrentFolder.Close();
    iCurrentFileList.Close();
    
    delete iMultiLineText;
    
    if(iSelectionName)
        delete iSelectionName;
    if(iCurrentName)
        delete iCurrentName;
    
#ifdef ENABLE_CAPTION
    if(iScreenCaption)
        delete iScreenCaption;
#endif
#ifdef ENABLE_DRIVE_LIST
    if(iDiskThresholdLabel)
        delete iDiskThresholdLabel;
    
    if(iDiskThresholdValueLabel)
        delete iDiskThresholdValueLabel;
#endif
    if(iListBox)
        delete iListBox;

    if(iSkinDrawer){
        delete iSkinDrawer;
        iSkinDrawer = NULL;
    }
    
    if(iDriveGrid){
        delete iDriveGrid;
        iDriveGrid = NULL;
    }
}

void CScrShotsAppFileList::Draw(const TRect& aRect) const
{
    TRACE_ON(RDebug::Printf("In CScrShotsAppFileList::Draw() <<");)
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    

    // Gets the control's extent
    iSkinDrawer->DrawSkin(gc,aRect);
    gc.SetPenColor(ScrShotsThemeUtils::MainScreenLineColor());

//    gc.DrawRect(iOptionsRect);
//    gc.DrawRect(iBackButtonRect);
//    gc.DrawRect(iSelectLabelRect);
//    gc.DrawRect(iExitButtonRect);
    TInt coolFactor = (iOptionsRect.Height() * 10)/100;
    
    gc.DrawLine(TPoint(iOptionsRect.iTl.iX + coolFactor, iOptionsRect.iTl.iY),TPoint(iOptionsRect.iBr.iX - coolFactor, iOptionsRect.iTl.iY));
    TInt centerPtX = 0;
    switch(iCurrentListMode){
        case EAppsListMode:{
            //calculate the center point, where the bitmap is to be drawn
            centerPtX = (iRefreshButtonRect.Width() / 2) -  (iImgLoader.GetBitmap(ERefresh)->SizeInPixels().iWidth / 2);
            gc.BitBlt(TPoint(iRefreshButtonRect.iTl.iX + centerPtX, iRefreshButtonRect.iTl.iY),iImgLoader.GetBitmap(ERefresh));
            gc.DrawLine(TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iTl.iY + coolFactor),TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iBr.iY - coolFactor));
        }
        break;
        case EFileListMode:{
            centerPtX = (iGoOneUpOptionRect.Width() / 2) -  (iImgLoader.GetBitmap(EFolderUpIcon)->SizeInPixels().iWidth / 2);
            gc.BitBlt(TPoint(iGoOneUpOptionRect.iTl.iX + centerPtX, iGoOneUpOptionRect.iTl.iY),iImgLoader.GetBitmap(EFolderUpIcon));
            gc.DrawLine(TPoint(iGoOneUpOptionRect.iTl.iX, iGoOneUpOptionRect.iTl.iY + coolFactor),TPoint(iGoOneUpOptionRect.iTl.iX, iGoOneUpOptionRect.iBr.iY - coolFactor));
            centerPtX = (iCreateNewFolderRect.Width() / 2) -  (iImgLoader.GetBitmap(ECreateFolderIcon)->SizeInPixels().iWidth / 2);
            gc.BitBlt(TPoint(iCreateNewFolderRect.iTl.iX + centerPtX, iCreateNewFolderRect.iTl.iY),iImgLoader.GetBitmap(ECreateFolderIcon));
            gc.DrawLine(TPoint(iCreateNewFolderRect.iTl.iX, iCreateNewFolderRect.iTl.iY + coolFactor),TPoint(iCreateNewFolderRect.iTl.iX , iCreateNewFolderRect.iBr.iY - coolFactor));
        }
            break;
#ifdef ENABLE_DRIVE_LIST
        case EDriveListMode:{
//            gc.DrawRect(iDiskThresholdRect); 
//            gc.DrawRoundRect(iDiskThresholdLabelRect, TSize(4,4));
//            gc.DrawRoundRect(iDiskThresholdSliderRect, TSize(4,4));
//            gc.DrawRoundRect(iDiskThresholdValueLabelRect, TSize(4,4));
//            gc.DrawRect(iRefreshButtonRect);
            gc.BitBlt(TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iTl.iY),iImgLoader.GetBitmap(ERefresh));
            gc.DrawLine(TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iTl.iY + 8),TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iBr.iY - 8));
        }
            break;            
#else
            
        case EDriveListMode:{
            centerPtX = (iRefreshButtonRect.Width() / 2) -  (iImgLoader.GetBitmap(ERefresh)->SizeInPixels().iWidth / 2);
            gc.BitBlt(TPoint(iRefreshButtonRect.iTl.iX + centerPtX, iRefreshButtonRect.iTl.iY),iImgLoader.GetBitmap(ERefresh));
            gc.DrawLine(TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iTl.iY + coolFactor),TPoint(iRefreshButtonRect.iTl.iX, iRefreshButtonRect.iBr.iY - coolFactor));
        }
            break;
#endif            
    }
    centerPtX = (iBackButtonRect.Width() / 2) -  (iImgLoader.GetBitmap(EBackApplist)->SizeInPixels().iWidth / 2);
    gc.BitBlt(TPoint(iBackButtonRect.iTl.iX + centerPtX, iBackButtonRect.iTl.iY),iImgLoader.GetBitmap(EBackApplist));
    
    centerPtX = (iExitButtonRect.Width() / 2) -  (iImgLoader.GetBitmap(EExitIcon)->SizeInPixels().iWidth / 2);
    gc.BitBlt(TPoint(iExitButtonRect.iTl.iX + centerPtX, iExitButtonRect.iTl.iY),iImgLoader.GetBitmap(EExitIcon));
    
    gc.DrawLine(TPoint(iExitButtonRect.iTl.iX, iExitButtonRect.iTl.iY + coolFactor),TPoint(iExitButtonRect.iTl.iX, iExitButtonRect.iBr.iY - coolFactor));
    
    CEikBorderedControl::Draw(aRect);
}

void CScrShotsAppFileList::SizeChanged()
{
    ComputeRects(Rect(), ETrue);
    
    if(iSkinDrawer)
        iSkinDrawer->ResizeGradientBitmap(Rect());

    DrawNow();
}

void CScrShotsAppFileList::UpdateDriveLabelsL()
{
    TStringPoolObj msg(256), selectedPath(256);
    iAppView->GetAppSettings()->GetOutputFolderCompletePathL(selectedPath.Des());
    iCurrentFolder.Copy(iListBox->CurrentPath());
    msg.Des().Format(iStrLoader->GetStringHBuf(EFolderSelectNameFormatString)->Des(), &(selectedPath.Des()));         
    iSelectionName->SetTextL(msg.Des());
    msg.Des().Format(iStrLoader->GetStringHBuf(EFolderCurrentNameFormatString)->Des(), &iCurrentFolder);
    iCurrentName->SetTextL(msg.Des());
}

void CScrShotsAppFileList::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{
    TPoint touchPoint = aPointerEvent.iPosition;

    if(iAppsFileListRect.Contains(touchPoint)){
        
        if(iCurrentListMode == EDriveListMode){
            iDriveGrid->HandlePointerEventL(aPointerEvent);
            TStringPoolObj str(4);
            iAppView->GetAppSettings()->OutputFolderDrive(str.Des());
            SetCurrentSelectionNameL(str.Des());
            if(aPointerEvent.iType == TPointerEvent::EButton1Up)
                DrawNow();
        }
        else{
            if(!iListBox->IsFocused())
                iListBox->SetFocus(ETrue,EDrawNow);

            iListBox->HandlePointerEventL(aPointerEvent);
            
            if(aPointerEvent.iType == TPointerEvent::EButton1Up)
                ComputeListboxRects();
            
            iListBox->RecalcSrollBarVisibiltyL();
            
            if(iCurrentListMode == EFileListMode)
                UpdateDriveLabelsL();
            
            if(aPointerEvent.iType == TPointerEvent::EButton1Up)
                DrawNow();
            
        }
    }else if(iOptionsRect.Contains(touchPoint) && aPointerEvent.iType == TPointerEvent::EButton1Up){
        iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
        
        if(iExitButtonRect.Contains(touchPoint)){
            HBufC* msg = iStrLoader->GetStringHBuf(EExitPromptString);//dont delete
            if(iAppView->GetAppUi().IsForeground() && CEikonEnv::Static()->QueryWinL(msg->Des(),KNullDesC))
                iAppView->GetAppUi().Exit();
        }
        else if(iBackButtonRect.Contains(touchPoint)){
            if(iListBox->IsFocused())
                iListBox->SetFocus(EFalse,EDrawNow);

            //move back to the main control screen
            iAppView->StartSwitchToMainControl();
        }else{
#ifdef ENABLE_DRIVE_LIST             
            if(iCurrentListMode == EDriveListMode || iCurrentListMode == EAppsListMode){
#else
            if(iCurrentListMode == EAppsListMode){
#endif
                if(iRefreshButtonRect.Contains(touchPoint)){
                    //refresh the list
                    iListBox->RefreshListboxL();
                    DrawNow();
                }
            } else if(iCurrentListMode == EDriveListMode){
                if(iRefreshButtonRect.Contains(touchPoint)){
                    //refresh the grid
                    iDriveGrid->RefreshDriveDataL();
                    DrawNow();
                }                
            }
            else{
                if(iGoOneUpOptionRect.Contains(touchPoint)){
                    iListBox->GoOneUpFolderL();
                    ComputeListboxRects();
                    //update the label to show the current and selected folder
                    UpdateDriveLabelsL();
                    iListBox->SetFocus(ETrue,EDrawNow);
                    DrawNow();
                }else if(iCreateNewFolderRect.Contains(touchPoint)){
                    //updated the current rootpath
                    iCurrentFolder.Copy(iListBox->CurrentPath());
                    //now ask the user the folder name
                    PromptAndCreateFolderNameL();
                    //refresh list now, with updating the scrollbars as we will do it again
                    iListBox->RefreshListboxL(EFalse);
                    
                    ComputeListboxRects();             
                    DrawNow();
                }                
            }
        }
    }
}

void CScrShotsAppFileList::PromptAndCreateFolderNameL()
{
   //check that the folder name is not greater than the prescribed limit else we will have a panic
    if(iCurrentFolder.Length() > (256 - 12)){
        //todo : show a dialog with the error msg
        CEikonEnv::Static()->InfoWinL(_L("Cannot create folder.Path length exceeds platform limits."),KNullDesC);
        return;
    }
    //get the new folder name using a query dialog
    //TStringPoolObj newFolderName(12);
    TBuf<12> newFolderName;
    //TInt returnValue = CAknFileNamePromptDialog::RunDlgLD( newFolderName,iCurrentFolder, R_NEW_FOLDER_DIALOG);
    
    CAknTextQueryDialog* Dialog = CAknTextQueryDialog::NewL(newFolderName,CAknQueryDialog::ENoTone);
    Dialog->PrepareLC(R_NEW_FOLDER_DIALOG);
    Dialog->SetPromptL(_L("Enter Folder Name"));
    TInt returnValue = Dialog->RunLD();
    
    if(returnValue){
        //check the value entered if its the correct folder name
        //BaflUtils::ValidateFolderNameTypedByUserL()
    
        //create the new folder
        TStringPoolObj newPath(1024);
        newPath.Des().Format(_L("%S%S\\"),&iCurrentFolder, &(newFolderName));
        TInt errCode = CEikonEnv::Static()->FsSession().MkDir(newPath.Des());
        
        CEikonEnv::Static()->InfoWinL(( errCode == KErrNone) ?iStrLoader->GetStringHBuf(EFolderCreateOkString)->Des() : iStrLoader->GetStringHBuf(EFolderCreateFailString)->Des() , newPath.Des());
    }
}

void CScrShotsAppFileList::ConstructL(const TRect& aRect)
{
    iCurrentFolder.Create(1024);
    CreateWindowL();
    SetRect(aRect);
    iSkinDrawer = CScrShotsGradientBitmapSkin::NewL(aRect,ScrShotsThemeUtils::BackgroundSkinGradientStartColor(), ScrShotsThemeUtils::BackgroundSkinGradientEndColor());
    SetBorder(TGulBorder::ERaised | TGulBorder::EThreeStep 
            //| TGulBorder::EWithInline 
            | TGulBorder::EWithOutline 
            | TGulBorder::EAddTwoPixels 
            | TGulBorder::EAddFourRoundingPixels);
    
    ComputeRects(Rect());
    
#ifdef ENABLE_CUSTOM_LISTBOX
#ifdef ENABLE_DRIVE_LIST
    CreateCustomListboxL(EDriveListMode);
#else
    CreateCustomListboxL(EAppsListMode);
#endif
#else
    CreateListboxL();
#endif
    
    CreateOptionsL(); 
    
    switch(iCurrentListMode){
        case EAppsListMode:{
            TStringPoolObj appName(1024);
            iAppView->GetAppSettings()->AppNameForCaptureL(appName.Des());
            if(appName.Des().Length())
                LoadListAndChangeCaptionL( EAppsListMode, appName.Des());
            else
                LoadListAndChangeCaptionL( EAppsListMode, iStrLoader->GetStringHBuf(ENoneString)->Des()); 
        }
        break;
        default:
            break;
    }
    
    iDriveGrid = CScrShotsDriveGrid::NewL(iAppsFileListRect, this);
    iDriveGrid->ActivateL();
}

void CScrShotsAppFileList::CreateOptionsL()
{

    if(iMultiLineText)
        delete iMultiLineText;
    
    iSelectionName = CreateAndSetCustomLabelL(iStrLoader->GetStringHBuf(ENoneString)->Des(), ScrShotsThemeUtils::MainScreenTextColor(),iSelectNameLabelRect);
    iCurrentName = CreateAndSetCustomLabelL(iStrLoader->GetStringHBuf(ENoneString)->Des(), ScrShotsThemeUtils::MainScreenTextColor(),iCurrentNameLabelRect);
    
#ifdef ENABLE_CAPTION    
    iScreenCaption = new (ELeave) CEikLabel;
    iScreenCaption->SetContainerWindowL( *this );
    //Set your custom font here.
    iScreenCaption->SetFont(font);
    iScreenCaption->SetAlignment(EHCenterVCenter);
    iScreenCaption->SetTextL(iMultiLineText->Des());
    iScreenCaption->OverrideColorL( EColorLabelText, KRgbWhite );
    SetScreenCaptionL(KDriveListCaption);
#endif

#ifdef ENABLE_DRIVE_LIST    
    iDiskThresholdLabel = new (ELeave) CEikLabel;
    iDiskThresholdLabel->SetContainerWindowL( *this );
    //Set your custom font here.
    iDiskThresholdLabel->SetFont(font);
    iDiskThresholdLabel->SetAlignment(EHCenterVCenter);
    iDiskThresholdLabel->SetTextL(iStrLoader->GetStringHBuf(EDriveUsageLimitString)->Des());
    iDiskThresholdLabel->OverrideColorL( EColorLabelText, KRgbWhite );

    iDiskThresholdValueLabel = new (ELeave) CEikLabel;
    iDiskThresholdValueLabel->SetContainerWindowL( *this );
    //Set your custom font here.
    iDiskThresholdValueLabel->SetFont(font);
    iDiskThresholdValueLabel->SetAlignment(EHCenterVCenter);
    iDiskThresholdValueLabel->SetTextL(iStrLoader->GetStringHBuf(EDriveSizeFormatString)->Des());
    iDiskThresholdValueLabel->OverrideColorL( EColorLabelText, KRgbWhite );
#endif
}



#ifdef ENABLE_CUSTOM_LISTBOX

void CScrShotsAppFileList::CreateCustomListboxL(TListMode aViewMode)
{
    iCurrentListMode = aViewMode;
    TListType listType = EDrivesList;
    TRect listRect;
    switch(aViewMode){
        case EAppsListMode: listType = EAppsList;   listRect = iAppsFileListRect;        break;
        case EFileListMode:  listType = EFileFolderList;  listRect = iAppsFileListRect;        break;
//        case EDriveListMode:  listType = EDrivesList;  listRect = iDriveListRect;        break;
    }
    iListBox = new (ELeave) CScrShotsCustomListbox(this, listType);
    iListBox->SetContainerWindowL(*this);
    TInt margin = 2;
    switch(iListBox->ListType()){
        case EDrivesList:{
            CFont* font = ScrShotsGenUtils::InitFontL(20);      
            iCalculatedItemHeight = (font->HeightInPixels() + (margin * 2)) * KDriveListNumberofTextCells;
            ScrShotsGenUtils::FreeFontL(font);
        } break;
        case EAppsList:{
            CFont* font = ScrShotsGenUtils::InitFontL(20);      
            iCalculatedItemHeight = (font->HeightInPixels() + (margin * 2)) * KAppsListNumberofTextCells;
            ScrShotsGenUtils::FreeFontL(font);        
        } break;
        case EFileFolderList:{
            CFont* font = ScrShotsGenUtils::InitFontL(24);      
            iCalculatedItemHeight = (font->HeightInPixels() + (margin * 2));
            ScrShotsGenUtils::FreeFontL(font);
        } break;
    }
    iListBox->ConstructL(this, EAknListBoxMarkableList);
    //set the colors
    iListBox->ItemDrawer()->SetBackColor(ScrShotsThemeUtils::MainScreenGradientEndColor());
    iListBox->ItemDrawer()->SetHighlightedBackColor(ScrShotsThemeUtils::ListBoxHighlightColor());
    iListBox->ItemDrawer()->SetTextColor(ScrShotsThemeUtils::ListBoxNormalItemTextColor());
    iListBox->ItemDrawer()->SetHighlightedTextColor(ScrShotsThemeUtils::ListBoxHighlightedItemTextColor());
            
    PRINT_RECT(listRect,"listRect")
    iListBox->SetExtent(listRect.iTl,listRect.Size());
    iListBox->SetItemHeightL(iCalculatedItemHeight);
    iListBox->View()->SetMatcherCursor(EFalse);
    iListBox->LoadItemTextArrayL();
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    if(iListBox->ScrollBarFrame()->VerticalScrollBar()){
        TSize sz = iListBox->ScrollBarFrame()->VerticalScrollBar()->Size();
    }
    iListBox->SetFocus(ETrue,EDrawNow);
    
#if 0
    iListBox->SetListBoxObserver(this);
#endif 
    
    iListBox->ScrollBarFrame()->SetScrollBarFrameObserver(this);
    iCurrentFolder.Copy(iAppView->GetAppUi().GetDeviceWatcher()->GetCurrentRootPath());
    
    switch(iListBox->ListType()){
        case EDrivesList:{
#if 0        
        
            //adjust the rect size
            TInt totalRectHeight = iListBox->Model()->NumberOfItems() * iListBox->ItemHeight();
            if(totalRectHeight < iListBox->Size().iHeight){
                TRect newListRect(listRect.iTl,TSize(listRect.Size().iWidth,totalRectHeight));
                //iListBox->SetExtent(listRect.iTl,TSize(listRect.Size().iWidth,totalRectHeight));
                iListBox->SetExtent(listRect.iTl,TSize(100,200));
                
                iListBox->SetFocus(ETrue,EDrawNow);
            }
            //fill in the icons now
            TSize cell0Size = iListBox->ItemDrawer()->ColumnData()->SubCellSize(0);
            iImgLoader.SetIconSize(ECDrvSelected,cell0Size);
            iImgLoader.SetIconSize(ECDrvUnselected,cell0Size);
            iImgLoader.SetIconSize(EEDrvSelected,cell0Size);
            iImgLoader.SetIconSize(EEDrvUnselected,cell0Size);
            iImgLoader.SetIconSize(EFDrvSelected,cell0Size);
            iImgLoader.SetIconSize(EFDrvUnselected,cell0Size);
            
            CArrayPtr<CGulIcon>* iconList = new (ELeave) CAknIconArray(1);
            CleanupStack::PushL(iconList);
            CGulIcon* newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(ECDrvSelected), iImgLoader.GetBitmapMask(ECDrvSelected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(ECDrvUnselected), iImgLoader.GetBitmapMask(ECDrvUnselected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EEDrvSelected), iImgLoader.GetBitmapMask(EEDrvSelected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EEDrvUnselected), iImgLoader.GetBitmapMask(EEDrvUnselected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EFDrvSelected), iImgLoader.GetBitmapMask(EFDrvSelected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EFDrvUnselected), iImgLoader.GetBitmapMask(EFDrvUnselected) );
            iconList->AppendL( newIcon );            
            iListBox->SetCustomIconArray( iconList );
            CleanupStack::Pop(iconList);                
#endif    
        } break;
        case EAppsList:{
            CArrayPtr<CGulIcon>* iconList = new (ELeave) CAknIconArray(1);
            CleanupStack::PushL(iconList);
            //first add the app selected/unselected icons
            //set the icon size
            TSize cell0Size = iListBox->ItemDrawer()->ColumnData()->SubCellSize(0);
            iImgLoader.SetIconSize(EAppSelected,cell0Size);
            iImgLoader.SetIconSize(EAppUnselectedIcon,cell0Size);
            iImgLoader.SetIconSize(EAppDefaultIcon,iListBox->ItemDrawer()->ColumnData()->SubCellSize(1));  
            
            CGulIcon* newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EAppSelected), iImgLoader.GetBitmapMask(EAppSelected) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EAppUnselectedIcon), iImgLoader.GetBitmapMask(EAppUnselectedIcon) );
            iconList->AppendL( newIcon );            
            CGulIcon* defaultAppIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EAppDefaultIcon), iImgLoader.GetBitmapMask(EAppDefaultIcon) );

            //now add the app icons
            const RPointerArray<TAppIconInfo>& appIconArray = iAppView->GetAppUi().GetDeviceWatcher()->GetAppIconArray();
            CGulIcon* prevGulIcon = NULL;
            for(TInt i = 0;i < appIconArray.Count(); i++){
                TAppIconInfo* appIcon = appIconArray[i];
                //set the icons sizes
                CGulIcon* newIcon = NULL;
                if(!appIcon->iDummyIcon){
                    AknIconUtils::SetSize(appIcon->iBmp, iListBox->ItemDrawer()->ColumnData()->SubCellSize(1));
                    newIcon = CGulIcon::NewL( appIcon->iBmp, appIcon->iBmp );
                    prevGulIcon = newIcon;
                    iconList->AppendL( newIcon );
                }
                else if(appIcon->iDummyIcon){
                    iconList->AppendL( defaultAppIcon );
                }
            }
            
            iListBox->SetCustomIconArray( iconList );
            CleanupStack::Pop(iconList);
            TBuf<256> appName;
            TUint32 appUidInt = iAppView->GetAppSettings()->AppUidForCapture();
            iAppView->GetAppSettings()->AppNameForCaptureL(appName);
            TUid appuid;
            appuid.iUid = appUidInt;
            iListBox->SetSelectedApp(appName, appuid);
            iListBox->View()->SetListEmptyTextL(_L("No apps to list!!!"));
            DrawNow();
        } break;
        case EFileFolderList:{
            CArrayPtr<CGulIcon>* iconList = new (ELeave) CAknIconArray(1);
            CleanupStack::PushL(iconList);
            //set the icons sizes
            TSize cell2Size = iListBox->ItemDrawer()->ColumnData()->SubCellSize(2);
            if(cell2Size.iHeight > cell2Size.iWidth)
                iImgLoader.SetIconSize(EFolderGoInside,TSize(cell2Size.iWidth,cell2Size.iWidth));
            else
                iImgLoader.SetIconSize(EFolderGoInside,TSize(cell2Size.iHeight,cell2Size.iHeight));
            
            TSize cell1Size = iListBox->ItemDrawer()->ColumnData()->SubCellSize(1);
            if(cell1Size.iHeight > cell1Size.iWidth){
                iImgLoader.SetIconSize(EFolderSelect,TSize(cell1Size.iWidth,cell1Size.iWidth));
                iImgLoader.SetIconSize(EFolderUnselect,TSize(cell1Size.iWidth,cell1Size.iWidth));
            }
            else{
                iImgLoader.SetIconSize(EFolderSelect,TSize(cell1Size.iHeight,cell1Size.iHeight));
                iImgLoader.SetIconSize(EFolderUnselect,TSize(cell1Size.iHeight,cell1Size.iHeight));                
            }
            //adding to the list icon array
            CGulIcon* newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EFolderGoInside), iImgLoader.GetBitmapMask(EFolderGoInside) );
            iconList->AppendL( newIcon );            
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EFolderSelect), iImgLoader.GetBitmapMask(EFolderSelect) );
            iconList->AppendL( newIcon );
            newIcon = CGulIcon::NewL( iImgLoader.GetBitmap(EFolderUnselect), iImgLoader.GetBitmapMask(EFolderUnselect) );
            iconList->AppendL( newIcon );
            iListBox->SetCustomIconArray( iconList );
            CleanupStack::Pop(iconList);        
            iListBox->View()->SetListEmptyTextL(_L("No folder to list!!!"));
            DrawNow();
        } break;
    }
    
}

#else
void CScrShotsAppFileList::CreateListboxL()
{
    iListBox = new ( ELeave ) CAknSingleNumberStyleListBox();  
    
    iListBox->SetContainerWindowL( *this );
    
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, 
                    R_LISTBOX_SINGLE_NUMBER_STYLE );
    
    // Construct listbox from resource    
    iListBox->ConstructFromResourceL( reader );
    CleanupStack::PopAndDestroy(); 
    
    // Creates scrollbar.
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( 
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto ); 
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
    iListBox->ItemDrawer()->SetSkinEnabledL(EFalse);
    // Set the listbox empty text
    HBufC* emptyText = StringLoader::LoadLC( R_LISTBOX_EMPTYTEXT );   
    iListBox->View()->SetListEmptyTextL( emptyText->Des() );  
    CleanupStack::PopAndDestroy( emptyText );        
}
#endif

void CScrShotsAppFileList::ComputeRects(const TRect& aScreenRect,TBool aSizeChanged )
{
    TInt scrWidth = aScreenRect.Width();
    TInt scrHeight = aScreenRect.Height();
    if(scrWidth > scrHeight){ 
        //we are in landscape mode
        iSelectNameLabelRect = TRect(TPoint(4,4),TSize(scrWidth - 8, scrHeight * KLandscapeSelectedNameTextRectHeightFactor - 2));
        iCurrentNameLabelRect = TRect(TPoint(4,iSelectNameLabelRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KLandscapeCurrentNameTextRectHeightFactor - 2));
        iAppsFileListRect = TRect(TPoint(4,iCurrentNameLabelRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KLandscapeListRectHeightFactor - 2));
        iOptionsRect = TRect(TPoint(4,iAppsFileListRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KLandscapeOptionsRectHeightFactor - 2));
    }else { 
        //we are in potrait mode
        iSelectNameLabelRect = TRect(TPoint(4,4),TSize(scrWidth - 8, scrHeight * KPotraitSelectedNameTextRectHeightFactor - 2));
        iCurrentNameLabelRect = TRect(TPoint(4,iSelectNameLabelRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KPotraitCurrentNameTextRectHeightFactor - 2));
        iAppsFileListRect = TRect(TPoint(4,iCurrentNameLabelRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KPotraitListRectHeightFactor - 2));
        iOptionsRect = TRect(TPoint(4,iAppsFileListRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KPotraitOptionsRectHeightFactor - 2));
    }
#ifdef ENABLE_DRIVE_LIST    
    iDriveListRect = iDiskThresholdRect = iAppsFileListRect;
    iDriveListRect.SetHeight(iAppsFileListRect.Height() * KDriveListRectHeightFactor);
    
    UpdateDiskThresholdRect();  
#endif
    if(iDriveGrid && iCurrentListMode == EDriveListMode)
        iDriveGrid->SetRect(iAppsFileListRect);
    if(iListBox && iCurrentListMode != EDriveListMode)
        ComputeListboxRects(aSizeChanged);
#ifdef ENABLE_DRIVE_LIST    
    iOptionsRect.iTl.iY =  iDiskThresholdValueLabelRect.iBr.iY;
#endif
    //options button rects calculation
    iBackButtonRect = iExitButtonRect = iGoOneUpOptionRect = iCreateNewFolderRect = iRefreshButtonRect = iOptionsRect;
    iBackButtonRect.SetWidth(iOptionsRect.Width() *  KOptionsBackButtonWidth);
    iGoOneUpOptionRect.iTl.iX = iBackButtonRect.iBr.iX;
    
    iRefreshButtonRect.iTl.iX = iBackButtonRect.iBr.iX;
    iRefreshButtonRect.SetWidth(iOptionsRect.Width() *  KOptionsBackButtonWidth * 2);
    
    iGoOneUpOptionRect.SetWidth(iOptionsRect.Width() *  KOptionsBackButtonWidth);
    iCreateNewFolderRect.iTl.iX = iGoOneUpOptionRect.iBr.iX;
    iCreateNewFolderRect.SetWidth(iOptionsRect.Width() *  KOptionsBackButtonWidth);    
    iExitButtonRect.iTl.iX = iCreateNewFolderRect.iBr.iX;
    iExitButtonRect.SetWidth(iOptionsRect.Width() *  KOptionsExitButtonWidth);
    
    if(iSelectionName)
        iSelectionName->SetExtent(iSelectNameLabelRect.iTl,iSelectNameLabelRect.Size());
    if(iCurrentName)
        iCurrentName->SetExtent(iCurrentNameLabelRect.iTl,iCurrentNameLabelRect.Size());
    
#ifdef ENABLE_DRIVE_LIST    
    if(iDiskThresholdLabel)
        iDiskThresholdLabel->SetExtent(iDiskThresholdLabelRect.iTl,iDiskThresholdLabelRect.Size());
    if(iDiskThresholdValueLabel)
        iDiskThresholdValueLabel->SetExtent(iDiskThresholdValueLabelRect.iTl,iDiskThresholdValueLabelRect.Size());
#endif
    if((iAppView->GetCurrentScreen() == CScrShotsAutomaticAppView::EAppsListScreen || iAppView->GetCurrentScreen() == CScrShotsAutomaticAppView::EFileListScreen))
        UpdateIconSizes();
}

void CScrShotsAppFileList::UpdateIconSizes()
{
    iImgLoader.SetIconSize(EBack ,iBackButtonRect.Size());
    iImgLoader.SetIconSize(ERefresh ,iRefreshButtonRect.Size());
    iImgLoader.SetIconSize(EExitIcon ,iExitButtonRect.Size());
    iImgLoader.SetIconSize(EBackApplist, iBackButtonRect.Size());
    iImgLoader.SetIconSize(EFolderUpIcon,iGoOneUpOptionRect.Size());
    iImgLoader.SetIconSize(ECreateFolderIcon,iCreateNewFolderRect.Size());
    if(iDriveGrid && iCurrentListMode == EDriveListMode)
        iDriveGrid->UpdateIconSizeL();
}

#ifdef ENABLE_DRIVE_LIST
void CScrShotsAppFileList::UpdateDiskThresholdRect()
{
    iDiskThresholdRect.iTl.iY = iDriveListRect.iBr.iY;
    iDiskThresholdRect.SetHeight(iAppsFileListRect.Height() * KDriveThresholdHeightFactor);
    
    //PRINT_RECT(iDiskThresholdRect,"iDiskThresholdRect")
    
    iDiskThresholdLabelRect = iDiskThresholdSliderRect = iDiskThresholdValueLabelRect = iDiskThresholdRect;
    iDiskThresholdLabelRect.SetHeight(iDiskThresholdRect.Height() * KDiskThresholdLabelHeightFactor);
    iDiskThresholdSliderRect.iTl.iY = iDiskThresholdLabelRect.iBr.iY;
    iDiskThresholdSliderRect.SetHeight(iDiskThresholdRect.Height() * KDiskThresholdSliderHeightFactor);
    if(iSliderControl)
        iSliderControl->SetRect(iDiskThresholdSliderRect);
    iDiskThresholdValueLabelRect.iTl.iY = iDiskThresholdSliderRect.iBr.iY;
    iDiskThresholdValueLabelRect.SetHeight(iDiskThresholdRect.Height() * KDiskThresholdValueLabelHeightFactor);
    

}
#endif

void CScrShotsAppFileList::ComputeListboxRects(TBool aSizeChanged)
{
    TRACE_ON(RDebug::Printf("In CScrShotsAppFileList::ComputeListboxRects() <<");)
    //set the listbox rect
    TInt scrollToThisItem = 0;
    if(iListBox ){
        switch(iCurrentListMode){
            case EAppsListMode:
            case EFileListMode:{
                scrollToThisItem = iListBox->View()->TopItemIndex();
                RDebug::Printf("Current scroll index : %d",scrollToThisItem);
//                TInt itemHeight = iListBox->ItemHeight();
                TInt numberofItems = iListBox->Model()->NumberOfItems();
                if(numberofItems && iListBox->View()->NumberOfItemsThatFitInRect(iAppsFileListRect) != numberofItems){
                    TInt totalRectHeight = iListBox->Model()->NumberOfItems() * iListBox->ItemHeight();
                    //do a set extent only when the new rect does not match the old rect, as doing a setrect will trigger a draw
                    //introducing flicker in the listbox screen
                    TRACE_ON(RDebug::Printf("totalRectHeight : %d", totalRectHeight);)
                    TRACE_ON(RDebug::Printf("iListBox->Size().iHeight : %d", iListBox->Size().iHeight);)
                    if(totalRectHeight != iListBox->Size().iHeight){
                        TRect newListRect(iAppsFileListRect.iTl,TSize(iAppsFileListRect.Size().iWidth,totalRectHeight));
                        if(newListRect.Size().iHeight < iAppsFileListRect.Size().iHeight)
                            iListBox->SetExtent(newListRect.iTl,newListRect.Size());
                        else
                            iListBox->SetExtent(iAppsFileListRect.iTl,iAppsFileListRect.Size());
                    }
                    else{
                        if(iAppsFileListRect.iTl != iListBox->Position())
                            iListBox->SetExtent(iAppsFileListRect.iTl,iAppsFileListRect.Size());
                    }
                }else
                    iListBox->SetExtent(iAppsFileListRect.iTl,iAppsFileListRect.Size());
                
                //update the scrollbars again
                iListBox->RecalcSrollBarVisibiltyL();
                
            } break;
#ifdef ENABLE_DRIVE_LIST            
            case EDriveListMode:{
                TInt itemHeight = iListBox->ItemHeight();
                if(iListBox->View()->NumberOfItemsThatFitInRect(iDriveListRect) > iListBox->Model()->NumberOfItems()){
                    TInt totalRectHeight = iListBox->Model()->NumberOfItems() * iListBox->ItemHeight();
                    if(totalRectHeight < iListBox->Size().iHeight){
                        TRect newListRect(iDriveListRect.iTl,TSize(iDriveListRect.Size().iWidth,totalRectHeight));
                        iListBox->SetExtent(newListRect.iTl,newListRect.Size());
                    }
                }else
                    iListBox->SetExtent(iDriveListRect.iTl,iDriveListRect.Size());
            } break;
#endif            
        }
        //update the drawer to update the list item layouts
        iListBox->UpdateItemDrawerL();
        if(scrollToThisItem && aSizeChanged){
            iListBox->SetCurrentItemIndexAndDraw(scrollToThisItem);
//            iListBox->View()->ScrollToMakeItemVisible(scrollToThisItem);
//            iListBox->ScrollToMakeItemVisible(scrollToThisItem);
//            iListBox->DrawNow();
            
        }
    }
}

CCoeControl* CScrShotsAppFileList::ComponentControl(TInt aIndex) const 
{
    switch(iCurrentListMode){
        case EAppsListMode:{
            switch(aIndex){
                case 0:            return iCurrentName;
                case 1:            return iListBox;
#ifdef ENABLE_CAPTION                
                case 2:            return iScreenCaption;
#endif                
            }
        }

        case EFileListMode:{
            switch(aIndex){
                case 0:            return iSelectionName;
                case 1:            return iCurrentName;
                case 2:            return iListBox;
#ifdef ENABLE_CAPTION                
                case 3:            return iScreenCaption;
#endif                
            }
        }
        case EDriveListMode:{
            switch(aIndex){
#ifdef ENABLE_DRIVE_LIST                
                case 0:            return iListBox;
                case 1:            return iDiskThresholdLabel;
                case 2:            return iDiskThresholdValueLabel;
                case 3:				return iSliderControl;
#ifdef ENABLE_CAPTION                
                case 4:            return iScreenCaption;
#endif
#else
                case 0:            return iCurrentName;
                case 1:            return iDriveGrid;
#ifdef ENABLE_CAPTION                
                case 2:            return iScreenCaption;
#endif                
#endif
                
            }
        }
    }
    return NULL;
}

TInt CScrShotsAppFileList::CountComponentControls() const 
{
    switch(iCurrentListMode){
        case EAppsListMode:{
#ifdef ENABLE_CAPTION                  
            return 3;    
#else
            return 2;
#endif        
        }
        case EFileListMode:{
#ifdef ENABLE_CAPTION                  
            return 4;    
#else
            return 3;
#endif        
        }
        case EDriveListMode:{
#ifdef ENABLE_DRIVE_LIST
        #ifdef ENABLE_CAPTION
            return 5;
#endif
            return 4;
#else
#ifdef ENABLE_CAPTION                  
            return 3;    
#else
            return 2;
#endif            
#endif

        }
    }
    //wait till the browser control is fully loaded    
    return 0;
}

void CScrShotsAppFileList::SetListMode(TListMode aViewMode)
{
    if(aViewMode == iCurrentListMode)
        return;
    iPrevPos = 0;
    iCurrentListMode = aViewMode;
    ComputeRects(Rect());
    switch(iCurrentListMode){
#ifdef ENABLE_CAPTION        
        case EAppsListMode:{    LoadListAndChangeCaptionL(KAppsListCaption, EAppsListMode, iStrLoader->GetStringHBuf(ENoneString)->Des()); }   break;
        case EFileListMode:{    LoadListAndChangeCaptionL(KFolderListCaption, EFileListMode, iStrLoader->GetStringHBuf(ENoneString)->Des());   }   break;
        case EDriveListMode:{   LoadListAndChangeCaptionL(KDriveListCaption, EDriveListMode, iStrLoader->GetStringHBuf(ENoneString)->Des());   }   break;
#else
        case EAppsListMode:{    
            TStringPoolObj appName(1024);
            iAppView->GetAppSettings()->AppNameForCaptureL(appName.Des());
            if(appName.Des().Length())
                LoadListAndChangeCaptionL( EAppsListMode, appName.Des());
            else
                LoadListAndChangeCaptionL( EAppsListMode, iStrLoader->GetStringHBuf(ENoneString)->Des()); 
        }   break;
        case EFileListMode:{
            TBuf<3> drv;
            TStringPoolObj dir(1024), fullPath(1024);
            iAppView->GetAppSettings()->OutputFolderDrive(drv);
            iAppView->GetAppSettings()->OutputFolderNameL(dir.Des());
            
            fullPath.Des().Format(KPathFormatStringWithColon,&drv, &(dir.Des()));
            //get the current selected folder name
            LoadListAndChangeCaptionL( EFileListMode, fullPath.Des());
            iListBox->SetExtent(iAppsFileListRect.iTl,iAppsFileListRect.Size());
            iListBox->MakeVisible(ETrue);
            iListBox->SetFocus(ETrue,EDrawNow);
            if(iDriveGrid){
                iDriveGrid->SetFocus(EFalse,ENoDrawNow);
                iDriveGrid->SetExtent(TPoint(0,0), TSize(0,0));
            }
        }   break;
#ifdef ENABLE_DRIVE_LIST         
        case EDriveListMode:{   LoadListAndChangeCaptionL( EDriveListMode, iStrLoader->GetStringHBuf(ENoneString)->Des());   }   break;
#else
        case EDriveListMode:{  LoadDriveGridL();  } break;
#endif

#endif
    }
    ComputeRects(Rect());
    //we want to hide the listbox
    switch(iCurrentListMode){
        case EDriveListMode:{  iListBox->SetExtent(TPoint(0,0), TSize(0,0));  } break;
    }
    
    DrawNow();
}

void CScrShotsAppFileList::LoadDriveGridL()
{
    iListBox->MakeVisible(EFalse);
    iDriveGrid->SetRect(iAppsFileListRect);
    TStringPoolObj str(4);
    iAppView->GetAppSettings()->OutputFolderDrive(str.Des());
    SetCurrentSelectionNameL(str.Des());
}

void CScrShotsAppFileList::LoadListAndChangeCaptionL(
#ifdef ENABLE_CAPTION        
        const TDesC& aScreenCaption, 
#endif
        TListMode aListMode, const TDesC& aSelectionCaption)
{
#ifdef ENABLE_CAPTION    
    SetScreenCaptionL(aScreenCaption);
#endif
    if(iListBox /*&& aListMode != TListMode(iListBox->ListType())*/){
        delete iListBox;
        iListBox = NULL;
        
        CreateCustomListboxL(aListMode);
    }
    SLOGF(aSelectionCaption);
    SetCurrentSelectionNameL(aSelectionCaption);
}

#ifdef ENABLE_CAPTION
void CScrShotsAppFileList::SetScreenCaptionL(const TDesC& aTitle)
{
    if(iMultiLineText)
        delete iMultiLineText;
    
    iMultiLineText = aTitle.AllocL();
    iScreenCaption->SetTextL(iMultiLineText->Des());
    iScreenCaption->DrawNow();
}
#endif

#if 0
void CScrShotsAppFileList::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
{
    
//    switch(aEventType){
//        case EEventItemClicked:{
//            TInt index = aListBox->CurrentItemIndex();
//            //now get the array and check what is the index item and update the caption,
//            //based on the list type mode
//            switch(iCurrentListMode){
//                case EAppsListMode:{
//                    const RPointerArray<TApaAppInfo>& appArray = iAppView->GetAppUi().GetDeviceWatcher()->GetAppListWithInfoL(ETrue);
//                    TApaAppInfo* appInfo = appArray[index];
//                    SetCurrentSelectionNameL(appInfo->iShortCaption);
//                    //save the value in the db
//                }  break;
//                case EFileListMode:{   /* not required as done within the listbox itself*/   }  break;
//                case EDriveListMode:{     }  break;
//            }
//        } break;
//    }
    DrawNow();
}
#endif 

void CScrShotsAppFileList::SetCurrentSelectionNameL(const TDesC& aSelectionName)
{
    TStringPoolObj msg(1024);
    //prepare the selection caption based 
    //list mode
    switch(iCurrentListMode){
        case EAppsListMode:{            
            msg.Des().Format(iStrLoader->GetStringHBuf(EAppSelectFormatString)->Des(), &aSelectionName);  
            iCurrentName->SetTextL(msg.Des());
            iSelectionName->SetTextL(KEmptyStr());
            break;
        }
        case EFileListMode:{
            TStringPoolObj select(aSelectionName.Length());
            msg.Des().Format(iStrLoader->GetStringHBuf(EFolderSelectNameFormatString)->Des(), &aSelectionName);
            iSelectionName->SetTextL(msg.Des());
            msg.Des().Zero();
            msg.Des().Format(iStrLoader->GetStringHBuf(EFolderCurrentNameFormatString)->Des(), &iCurrentFolder);
            iCurrentName->SetTextL(msg.Des());
        }        
        break;         
        
        case EDriveListMode:{            
            msg.Des().Format(iStrLoader->GetStringHBuf(EDriveSelectFormatString)->Des(), &aSelectionName);   
            iCurrentName->SetTextL(msg.Des());     
            iSelectionName->SetTextL(KEmptyStr());
            break;
        }
    }
    
}

void CScrShotsAppFileList::HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType)
{
    TInt newPosition = aScrollBar->ThumbPosition();
    //get the number of items per view
    float itemsperview = (float)iListBox->Size().iHeight / (float)iListBox->ItemHeight();
    int itemtoffocus = 0;
    int offset = newPosition / iListBox->ItemHeight();
    switch( aEventType )
    {
        case EEikScrollUp: /* Moving up one step */ { /*get this event only in case of key events or end arrows based scroll*/ break; }
        case EEikScrollDown: /* Moving down one step */  { /*get this event only in case of key events or end arrows based scroll*/ break; }
        case EEikScrollThumbReleaseVert: /* Drag released */ { break;   }
        //cases those matter start here
        case EEikScrollThumbDragVert: 
        {// Drag started
            if(iPrevPos < newPosition) // moving down
                itemtoffocus = itemsperview + offset;
            else if(iPrevPos > newPosition) {//move up 
                itemtoffocus = offset;
                if(itemtoffocus < 0 )
                    itemtoffocus = 0;
            }
            iListBox->View()->ScrollToMakeItemVisible(itemtoffocus );
            iPrevPos = newPosition;
        }
        break;
        case EEikScrollPageUp:
        {
            itemtoffocus = offset;
            if(itemtoffocus < 0 )
                itemtoffocus = 0;
            iListBox->View()->ScrollToMakeItemVisible(itemtoffocus );
            iPrevPos = newPosition;
            break;
        }
        case EEikScrollPageDown:
        {
            if(iPrevPos < newPosition) // moving down
                itemtoffocus = itemsperview + offset;
            iListBox->View()->ScrollToMakeItemVisible(itemtoffocus );
            iPrevPos = newPosition;
            break;
        }
        //cases those matter end here
        default: {   break;    }
    }
    DrawNow();
}

void CScrShotsAppFileList::UpdateSelectionLabelL(const TDesC& aNewSelection)
{
    switch(iCurrentListMode){
        case EAppsListMode:{
            SetCurrentSelectionNameL(aNewSelection);
        }
            break;
        case EFileListMode:{
            SetCurrentSelectionNameL(aNewSelection);
        }
            break;
    }
}

void CScrShotsAppFileList::HandleAppSelectionDoneL(const TDesC& aSelectedAppName, const TUid& aSelectedAppUid)
{
    iAppView->GetAppSettings()->SetAppInfoForCaptureL(aSelectedAppName, aSelectedAppUid.iUid);
}

TBool CScrShotsAppFileList::IsCurrentScreen()
{
    return (iAppView->GetCurrentScreen() == CScrShotsAutomaticAppView::EAppsListScreen 
            || iAppView->GetCurrentScreen() == CScrShotsAutomaticAppView::EFileListScreen);
}

CScrShotsTextLabel* CScrShotsAppFileList::CreateAndSetCustomLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect)
{
    CScrShotsTextLabel* label = CScrShotsTextLabel::NewL();
    label->SetTextColor(aLabelColor);
    label->SetExtent(aLabelRect.iTl, aLabelRect.Size());
    label->SetLabelTextAlignment(CScrShotsTextLabel::ECenter);
    label->SetFontL(iAppView->GetAppUi().GetCustomFont());
    label->SetTextL(aLabelText);
    return label;
}

void CScrShotsAppFileList::UpdateAppFileListScreenL()
{
    switch(iCurrentListMode){
        case EDriveListMode:{
            TStringPoolObj str(4);
            iDriveGrid->RefreshDriveDataL();
            iAppView->GetAppSettings()->OutputFolderDrive(str.Des());
            SetCurrentSelectionNameL(str.Des());
            DrawNow();
        }
        break;
        case EFileListMode:{
            TBuf<3> drv;
            TStringPoolObj dir(1024), fullPath(1024);
            iAppView->GetAppSettings()->OutputFolderDrive(drv);
            iAppView->GetAppSettings()->OutputFolderNameL(dir.Des());
            
            fullPath.Des().Format(KPathFormatStringWithColon,&drv, &(dir.Des()));
            //get the current selected folder name
            LoadListAndChangeCaptionL( EFileListMode, fullPath.Des());
            iListBox->SetExtent(iAppsFileListRect.iTl,iAppsFileListRect.Size());
            iListBox->MakeVisible(ETrue);
            iListBox->SetFocus(ETrue,EDrawNow);
        }
        break; 
        case EAppsListMode:{
            if(iListBox){
                TBuf<256> appName;
                iAppView->GetAppSettings()->AppNameForCaptureL(appName);
                TUid appUid;
                appUid.iUid = iAppView->GetAppSettings()->AppUidForCapture();
                iListBox->SetSelectedApp(appName, appUid);
                iListBox->RefreshListboxL();
                if(appName.Length())
                    UpdateSelectionLabelL(appName);
                else
                    UpdateSelectionLabelL( iStrLoader->GetStringHBuf(ENoneString)->Des());                
            }
            
            DrawNow();
        }
        break;
    }
}

#endif //ENABLE_THREE_SCREEN_UI
