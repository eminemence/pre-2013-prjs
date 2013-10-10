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

#include "ScrShotsDriveGrid.h"
#include "ScrShotsDeviceWatcher.h"
#include "ScrShotsAppFileList.h"
#include "ScrShotsStrLoader.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsTextLabel.h"
#include "EventLogger.h"

#include <eiklabel.h>

#define KGridCellWidth 0.50
#define KGridCellHeight 0.50

#define KIconCellWidth 0.40
#define KIconCellHeight 0.50
#define KTotalSizeTextCellHeight 0.16
#define KTotalSizeTextPotraitCellWidth 0.60
#define KTotalSizeTextLandscapeCellWidth KIconCellWidth
#define KNumberEditCtrlCellWidth 0.60
#define KNumberEditCtrlCellPotraitHeight 0.50
#define KNumberEditCtrlCellLandscapeHeight 0.70
#define KThresholdTextPotraitWidth 0.40
#define KThresholdTextLandscapeWidth KNumberEditCtrlCellWidth
#define KThresholdInfoTextPotraitHeight KNumberEditCtrlCellPotraitHeight
#define KThresholdInfoTextLandscapeHeight 0.30


CScrShotsDriveGrid* CScrShotsDriveGrid::NewL(const TRect& aRect,CScrShotsAppFileList* aParentControl)
{
    CScrShotsDriveGrid* self = new (ELeave) CScrShotsDriveGrid(aParentControl);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    return self;
}

CScrShotsDriveGrid::~CScrShotsDriveGrid()
{
    iGridModel.ResetAndDestroy();
    iGridModel.Close();
    
    iThresholdEditCtrlArray.ResetAndDestroy();
    iThresholdEditCtrlArray.Close();
    
    iLabelArray.ResetAndDestroy();
    iLabelArray.Close();
}

void CScrShotsDriveGrid::Draw(const TRect& /*aRect*/) const
{
    CWindowGc& gc = SystemGc();
    
    for(int i = 0 ;i < iGridModel.Count(); i++){
        CScrShotsDriveGridCellModel* cell = iGridModel[i];
        if(cell->iCurrentlySelected){
            gc.SetPenColor(ScrShotsThemeUtils::SelectedGridCellColor());
            gc.SetBrushColor(ScrShotsThemeUtils::SelectedGridCellColor());
        }
        else{
            gc.SetPenColor(ScrShotsThemeUtils::UnselectedGridCellColor());
            gc.SetBrushColor(ScrShotsThemeUtils::UnselectedGridCellColor());
        }
        
        TRect gRect;
        switch(cell->iDrvChar[0]){
            case 'C': case 'c':gRect = iDrive1Rect;  break;
            case 'D': case 'd':gRect = iDrive2Rect;  break;
            case 'E': case 'e':gRect = iDrive3Rect;  break;
            case 'F': case 'f':gRect = iDrive4Rect;  break;
        }
        gc.DrawRoundRect(gRect, TSize(2,2));
        if(cell->iCurrentlySelected)
            gc.BitBlt(cell->iIconRect.iTl, cell->iSelectedDriveIcon);
        else
            gc.BitBlt(cell->iIconRect.iTl, cell->iUnselectedDriveIcon);
#ifdef DRAW_RECTS        
        gc.DrawRoundRect(cell->iDriveTypeRect, TSize(2,2));
        gc.DrawRoundRect(cell->iFreeSizeRect, TSize(2,2));
        gc.DrawRoundRect(cell->iTotalSizeRect, TSize(2,2));
//        gc.DrawRoundRect(cell->iIconRect, TSize(2,2));
//        gc.DrawRoundRect(cell->iThresholdEditRect, TSize(2,2));
        gc.DrawRoundRect(cell->iThresholdInfoTextRect, TSize(2,2));
#else
        if(Rect().Width() > Rect().Height()){
            //vertical lines
            gc.DrawLine(TPoint(cell->iIconRect.iBr.iX, cell->iIconRect.iTl.iY + 4),TPoint(cell->iIconRect.iBr.iX, cell->iIconRect.iBr.iY - 4));
            gc.DrawLine(TPoint(cell->iIconRect.iBr.iX , cell->iIconRect.iTl.iY + 4), TPoint(cell->iIconRect.iBr.iX, gRect.iBr.iY - 4));
            //horizontal lines
            gc.DrawLine(TPoint(cell->iFreeSizeRect.iTl.iX + 4, cell->iFreeSizeRect.iBr.iY), TPoint(cell->iFreeSizeRect.iBr.iX - 4, cell->iFreeSizeRect.iBr.iY));
            gc.DrawLine(TPoint(cell->iTotalSizeRect.iTl.iX + 4, cell->iTotalSizeRect.iBr.iY), TPoint(cell->iTotalSizeRect.iBr.iX - 4, cell->iTotalSizeRect.iBr.iY));
            gc.DrawLine(TPoint(cell->iIconRect.iTl.iX + 4, cell->iIconRect.iBr.iY), TPoint(cell->iIconRect.iBr.iX - 4, cell->iIconRect.iBr.iY));                        
            gc.DrawLine(TPoint(cell->iThresholdEditRect.iTl.iX + 4, cell->iThresholdEditRect.iBr.iY), TPoint(cell->iThresholdEditRect.iBr.iX - 4, cell->iThresholdEditRect.iBr.iY));
        }
        else{
            //vertical lines
            gc.DrawLine(TPoint(cell->iIconRect.iBr.iX, cell->iIconRect.iTl.iY + 4),TPoint(cell->iIconRect.iBr.iX, cell->iIconRect.iBr.iY - 4));
            gc.DrawLine(TPoint(cell->iThresholdEditRect.iBr.iX, cell->iThresholdEditRect.iTl.iY + 4),TPoint(cell->iThresholdEditRect.iBr.iX, cell->iThresholdEditRect.iBr.iY - 4));
            //horizontal lines
            gc.DrawLine(TPoint(cell->iFreeSizeRect.iTl.iX + 4, cell->iFreeSizeRect.iBr.iY), TPoint(cell->iFreeSizeRect.iBr.iX - 4, cell->iFreeSizeRect.iBr.iY));
            gc.DrawLine(TPoint(cell->iTotalSizeRect.iTl.iX + 4, cell->iTotalSizeRect.iBr.iY), TPoint(cell->iTotalSizeRect.iBr.iX - 4, cell->iTotalSizeRect.iBr.iY));
            gc.DrawLine(TPoint(cell->iIconRect.iTl.iX + 4, cell->iIconRect.iBr.iY), TPoint(gRect.iBr.iX - 4, cell->iIconRect.iBr.iY));            
        }
#endif
        
    }
}

void CScrShotsDriveGrid::SizeChanged()
{
    ComputeGridRects();
    LoadDriveGridsL();
}

CScrShotsDriveGrid::CScrShotsDriveGrid(CScrShotsAppFileList* aParentControl)
:iParentControl(aParentControl)
{

}

void CScrShotsDriveGrid::ConstructL(const TRect& aRect)
{
    SetRect(aRect);
    ComputeGridRects();
    LoadDriveGridsL();
}

void CScrShotsDriveGrid::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{
    TPoint te = aPointerEvent.iPosition;
    if(aPointerEvent.iType == TPointerEvent::EButton1Up){
        for(int i = 0; i < iGridModel.Count() ; i++){
            CScrShotsDriveGridCellModel* cell = iGridModel[i];
            if(!cell->iGridRect.Contains(te))
                continue;
            if(cell->iIconRect.Contains(te)){
                //provide touch feedback for icon selection
                iParentControl->GetTouchFeedback().VibrateforButtonPressL(this, aPointerEvent);
                if(!cell->iCurrentlySelected){
                    //get the old drive and unset it
                    for(int j = 0; j < iGridModel.Count() ;j++){
                        CScrShotsDriveGridCellModel* cell1 = iGridModel[j];
                        if(cell1->iCurrentlySelected){
                            cell1->iCurrentlySelected = EFalse;
                            //set the label color to unselected one
                            iLabelArray[j*4 + 0]->SetTextColor( ScrShotsThemeUtils::UnselectedGridCellColor() );
                            iLabelArray[j*4 + 1]->SetTextColor( ScrShotsThemeUtils::UnselectedGridCellColor() );
                            iLabelArray[j*4 + 2]->SetTextColor( ScrShotsThemeUtils::UnselectedGridCellColor() );
                            iLabelArray[j*4 + 3]->SetTextColor( ScrShotsThemeUtils::UnselectedGridCellColor() );
                            break;
                        }
                    }
                    //make it the currently selected drive
                    cell->iCurrentlySelected = ETrue;
                    //set the label color to selected one
                    iLabelArray[i*4 + 0]->SetTextColor( ScrShotsThemeUtils::SelectedGridCellColor() );
                    iLabelArray[i*4 + 1]->SetTextColor( ScrShotsThemeUtils::SelectedGridCellColor() );
                    iLabelArray[i*4 + 2]->SetTextColor( ScrShotsThemeUtils::SelectedGridCellColor() );
                    iLabelArray[i*4 + 3]->SetTextColor( ScrShotsThemeUtils::SelectedGridCellColor() );
                    //save the settings
                    iParentControl->GetAppView()->GetAppUi().GetAppSettings()->SetOutputFolderDriveL(cell->iDrvChar[0]);
                    iParentControl->GetAppView()->GetAppUi().GetAppSettings()->SetDiskFillThresholdL(iThresholdEditCtrlArray[i]->Value());
                    //restart the disk watcher
                    iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->StopWatchingDriveThresholdL();
                    iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->StartWatchingDriveThresholdL(cell->iDrvChar[0], iThresholdEditCtrlArray[i]->Value());
                }
            }else if(cell && cell->iCurrentlySelected && cell->iThresholdEditRect.Contains(te)){
                //provide touch feedback for number edit control selection
                iParentControl->GetTouchFeedback().VibrateforButtonPressL(this, aPointerEvent);
                iThresholdEditCtrlArray[i]->HandlePointerEventL(aPointerEvent);
            }
        }
    }
}

void CScrShotsDriveGrid::DoDataSaveL(CScrShotsNumberEditCtrl* aCtrl, TInt aChangedValue)
{
    for(int i = 0 ;i < iThresholdEditCtrlArray.Count() ; i++){
        if(aCtrl == iThresholdEditCtrlArray[i]){
            CScrShotsDriveGridCellModel* cell = iGridModel[i];
            if(!cell->iCurrentlySelected)
                continue;
            iParentControl->GetAppView()->GetAppUi().GetAppSettings()->SetDiskFillThresholdL(aChangedValue);
            //restart the disk watcher with updated values
            iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->StopWatchingDriveThresholdL();
            iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->StartWatchingDriveThresholdL(cell->iDrvChar[0], aChangedValue);
            break;
        }
    }
}

void CScrShotsDriveGrid::LoadDriveGridsL()
{
    iGridModel.ResetAndDestroy();
    iThresholdEditCtrlArray.ResetAndDestroy();
    iLabelArray.ResetAndDestroy();    
    
    const RArray<TDriveExtraInfo>& drvArray = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetDriveListWithInfo(EFalse);
    
    for(int i = 0 ; i < drvArray.Count(); i++){
        TDriveExtraInfo dvInfo = drvArray[i];
        CScrShotsDriveGridCellModel* cell = NULL;
        TRect parentRect;
        if(dvInfo.iDriveChar == 'c' || dvInfo.iDriveChar == 'C' ){
            cell = CScrShotsDriveGridCellModel::NewL();
            //set the icons
            cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(ECDrvSelected);
            cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(ECDrvUnselected);
            //compute the rects
            parentRect = iDrive1Rect;
        }
        if(dvInfo.iDriveChar == 'd' || dvInfo.iDriveChar == 'D' ){
            cell = CScrShotsDriveGridCellModel::NewL();
            //set the icons
            cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EDDrvSelected);
            cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EDDrvUnselected);
            //compute the rects
            parentRect = iDrive2Rect;
        }        
        else if(dvInfo.iDriveChar == 'e' || dvInfo.iDriveChar == 'E'){
            cell = CScrShotsDriveGridCellModel::NewL();
            //set the icons
            cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EEDrvSelected);
            cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EEDrvUnselected);
            parentRect = iDrive3Rect;
        }
        else if(dvInfo.iDriveChar == 'f' || dvInfo.iDriveChar == 'F'){
            cell = CScrShotsDriveGridCellModel::NewL();
            //set the icons
            cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EFDrvSelected);
            cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EFDrvUnselected);
            parentRect = iDrive4Rect;
        }
        
        if(cell){//load the other params
            cell->iGridRect = parentRect;
            cell->iCurrentlySelected = dvInfo.iCurrentTargetDrive;
#ifdef BRAHMA_DEBUG            
            TBuf<256> msg;
            msg.Format(_L("free size : %ld"), dvInfo.iDrvFreeSize);
            SLOGF(msg);
#endif
            ScrShotsGenUtils::SetFreeSizeText( iParentControl->GetAppView()->GetAppUi().GetStrLoader(), cell->iFreeSizeText,dvInfo.iDrvFreeSize);
#ifdef BRAHMA_DEBUG            
            SLOGF(cell->iFreeSizeText);
#endif
            SetDriveTypeText(cell->iDriveTypeText,dvInfo.iDriveType, dvInfo.iDriveIsRemoveable);
            SetTotalSizeText(cell->iTotalSizeText,dvInfo.iDrvTotalSize);
#ifdef BRAHMA_DEBUG            
            msg.Format(_L("total size : %ld"), dvInfo.iDrvTotalSize);
            SLOGF(msg);
            SLOGF(cell->iTotalSizeText);
#endif
            SetThresholdInfoText(cell->iThresholdInfoText);
            
            //rest of the cell rects are calculated here.
            cell->iIconRect = cell->iDriveTypeRect = cell->iFreeSizeRect = cell->iThresholdEditRect = cell->iThresholdInfoTextRect = cell->iTotalSizeRect = parentRect;
            
            cell->iIconRect.SetWidth(parentRect.Width() * KIconCellWidth);
            cell->iIconRect.SetHeight(parentRect.Height() * KIconCellHeight);
            
            TBool isPotrait = (Rect().Width() < Rect().Height());

            if(isPotrait){
                cell->iDriveTypeRect.iTl.iX = cell->iFreeSizeRect.iTl.iX = cell->iTotalSizeRect.iTl.iX = cell->iIconRect.iBr.iX;
            }
            else{
                cell->iDriveTypeRect.iTl.iY = cell->iFreeSizeRect.iTl.iY = cell->iTotalSizeRect.iTl.iY = cell->iIconRect.iBr.iY;
            }
            
            cell->iTotalSizeRect.SetWidth((isPotrait) ? parentRect.Width() * KTotalSizeTextPotraitCellWidth : parentRect.Width() * KTotalSizeTextLandscapeCellWidth);
            cell->iTotalSizeRect.SetHeight(parentRect.Height() * KTotalSizeTextCellHeight);
            
            cell->iFreeSizeRect.iTl.iY = cell->iTotalSizeRect.iBr.iY;
            cell->iFreeSizeRect.SetWidth((isPotrait) ? parentRect.Width() * KTotalSizeTextPotraitCellWidth : parentRect.Width() * KTotalSizeTextLandscapeCellWidth);
            cell->iFreeSizeRect.SetHeight(parentRect.Height() * KTotalSizeTextCellHeight);
            
            cell->iDriveTypeRect.iTl.iY = cell->iFreeSizeRect.iBr.iY;
            cell->iDriveTypeRect.SetWidth((isPotrait) ? parentRect.Width() * KTotalSizeTextPotraitCellWidth : parentRect.Width() * KTotalSizeTextLandscapeCellWidth);
            cell->iDriveTypeRect.SetHeight(parentRect.Height() * KTotalSizeTextCellHeight);

            iLabelArray.Append(CreateAndSetLabelL(cell->iFreeSizeText,(cell->iCurrentlySelected)?ScrShotsThemeUtils::SelectedGridCellColor():ScrShotsThemeUtils::UnselectedGridCellColor() ,cell->iFreeSizeRect));
            iLabelArray.Append(CreateAndSetLabelL(cell->iTotalSizeText,(cell->iCurrentlySelected)?ScrShotsThemeUtils::SelectedGridCellColor():ScrShotsThemeUtils::UnselectedGridCellColor() ,cell->iTotalSizeRect));
            iLabelArray.Append(CreateAndSetLabelL(cell->iDriveTypeText,(cell->iCurrentlySelected)?ScrShotsThemeUtils::SelectedGridCellColor():ScrShotsThemeUtils::UnselectedGridCellColor() ,cell->iDriveTypeRect));
            if(isPotrait){
                cell->iThresholdEditRect.iTl.iY = cell->iIconRect.iBr.iY;
            }else{
                cell->iThresholdEditRect.iTl.iX = cell->iIconRect.iBr.iX;
            }
            
            cell->iThresholdEditRect.SetWidth(parentRect.Width() * KNumberEditCtrlCellWidth);
            cell->iThresholdEditRect.SetHeight((isPotrait) ? parentRect.Height() * KNumberEditCtrlCellPotraitHeight : parentRect.Height() * KNumberEditCtrlCellLandscapeHeight);
            
            CScrShotsNumberEditCtrl* numEditCtrl = CScrShotsNumberEditCtrl::NewL(CScrShotsNumberEditCtrl::ETwoPane, iParentControl->GetVectorImgLoader(), this, iParentControl);
            numEditCtrl->SetRect(cell->iThresholdEditRect);
            
            //Check the current free space and set it as as the threshold lower range
            TInt64 driveUsedSize = dvInfo.iDrvTotalSize - dvInfo.iDrvFreeSize;
            TInt fillPercent = ((double)driveUsedSize / (double)dvInfo.iDrvTotalSize ) * 100;
            TInt lowerRange = 0;
            if(fillPercent <= KDefaultThresholdPercent)
                lowerRange = fillPercent;
            else
                lowerRange = KDefaultThresholdPercent;
            
            numEditCtrl->SetRanges(lowerRange,KDefaultThresholdPercent);
            if(dvInfo.iCurrentTargetDrive)
                numEditCtrl->SetValue(dvInfo.iThresholdPercent);
            else
                numEditCtrl->SetValue(KDefaultThresholdPercent);
            
            iThresholdEditCtrlArray.Append(numEditCtrl);
            if(isPotrait){
                cell->iThresholdInfoTextRect.iTl.iX = cell->iThresholdEditRect.iBr.iX;
                cell->iThresholdInfoTextRect.iTl.iY = cell->iIconRect.iBr.iY;
            }else{
                cell->iThresholdInfoTextRect.iTl.iX = cell->iThresholdEditRect.iTl.iX;
                cell->iThresholdInfoTextRect.iTl.iY = cell->iThresholdEditRect.iBr.iY;                
            }
            
            if(isPotrait){
                cell->iThresholdInfoTextRect.SetWidth(parentRect.Width() * KThresholdTextPotraitWidth);
                cell->iThresholdInfoTextRect.SetHeight(parentRect.Height() * KThresholdInfoTextPotraitHeight);
            }else{
                cell->iThresholdInfoTextRect.SetWidth(parentRect.Width() * KThresholdTextLandscapeWidth);
                cell->iThresholdInfoTextRect.SetHeight(parentRect.Height() * KThresholdInfoTextLandscapeHeight);
            }
            iLabelArray.Append(CreateAndSetLabelL(cell->iThresholdInfoText,(cell->iCurrentlySelected)?ScrShotsThemeUtils::SelectedGridCellColor():ScrShotsThemeUtils::UnselectedGridCellColor() ,cell->iThresholdInfoTextRect));
            //rect calculation done, now check the icons
            cell->iDrvChar.Append(dvInfo.iDriveChar);
            if(dvInfo.iDriveChar == 'c' || dvInfo.iDriveChar == 'C' ){
                if(iParentControl->IsCurrentScreen()){
                    iParentControl->GetVectorImgLoader().SetIconSize(ECDrvSelected, cell->iIconRect.Size());
                    iParentControl->GetVectorImgLoader().SetIconSize(ECDrvUnselected, cell->iIconRect.Size());
                }
                cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(ECDrvSelected);
                cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(ECDrvUnselected);
            }
            if(dvInfo.iDriveChar == 'd' || dvInfo.iDriveChar == 'D' ){
                if(iParentControl->IsCurrentScreen()){
                    iParentControl->GetVectorImgLoader().SetIconSize(EDDrvSelected, cell->iIconRect.Size());
                    iParentControl->GetVectorImgLoader().SetIconSize(EDDrvUnselected, cell->iIconRect.Size());
                }
                cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EDDrvSelected);
                cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EDDrvUnselected);
            }            
            else if(dvInfo.iDriveChar == 'e' || dvInfo.iDriveChar == 'E'){
                if(iParentControl->IsCurrentScreen()){
                    iParentControl->GetVectorImgLoader().SetIconSize(EEDrvSelected, cell->iIconRect.Size());
                    iParentControl->GetVectorImgLoader().SetIconSize(EEDrvUnselected, cell->iIconRect.Size());
                }
                cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EEDrvSelected);
                cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EEDrvUnselected);
            }
            else if(dvInfo.iDriveChar == 'f' || dvInfo.iDriveChar == 'F'){
                if(iParentControl->IsCurrentScreen()){
                    iParentControl->GetVectorImgLoader().SetIconSize(EFDrvSelected, cell->iIconRect.Size());
                    iParentControl->GetVectorImgLoader().SetIconSize(EFDrvUnselected, cell->iIconRect.Size());
                }
                cell->iSelectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EFDrvSelected);
                cell->iUnselectedDriveIcon = iParentControl->GetVectorImgLoader().GetBitmap(EFDrvUnselected);
            }
            iGridModel.Append(cell);
        }
    }
}

CScrShotsTextLabel* CScrShotsDriveGrid::CreateAndSetLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect)
{
    CScrShotsTextLabel* label = CScrShotsTextLabel::NewL();
    label->SetContainerWindowL( *this );
    label->SetExtent(aLabelRect.iTl, aLabelRect.Size());
    label->SetFontL(iParentControl->GetAppView()->GetAppUi().GetCustomSmallFont());
    label->SetTextL(aLabelText);
    label->SetLabelTextAlignment(CScrShotsTextLabel::ECenter);
    label->SetTextColor( aLabelColor );
    return label;
}


CCoeControl* CScrShotsDriveGrid::ComponentControl(TInt aIndex) const
{
    if(aIndex < iThresholdEditCtrlArray.Count())
        return iThresholdEditCtrlArray[aIndex];
    else
        return iLabelArray[aIndex - iThresholdEditCtrlArray.Count()];
}

TInt CScrShotsDriveGrid::CountComponentControls() const
{
    return iThresholdEditCtrlArray.Count() + iLabelArray.Count();
}


void CScrShotsDriveGrid::SetTotalSizeText(TDes& aTotalSizeText, TInt64 aDriveTotalSize)
{
    TPtr formatStr = iParentControl->GetAppView()->GetAppUi().GetStrLoader()->GetStringHBuf(EDriveTotalSizeFormatString)->Des();

    if((aDriveTotalSize / (KOneGB)) > 0){
        aTotalSizeText.Format(formatStr,&KGBStr,aDriveTotalSize / (KOneGB));
    }
    else if((aDriveTotalSize / (KOneMB)) > 0){
        aTotalSizeText.Format(formatStr,&KMBStr,aDriveTotalSize / (KOneMB));
    }
    else if((aDriveTotalSize / (KOneKB)) > 0){
        aTotalSizeText.Format(formatStr,&KKBStr, aDriveTotalSize / (KOneKB));
    }
    else{
        aTotalSizeText.Format(formatStr,&KBytesStr,aDriveTotalSize );
    }

}

void CScrShotsDriveGrid::SetThresholdInfoText(TDes& aThresholdInfoText)
{
    TPtr formatStr = iParentControl->GetAppView()->GetAppUi().GetStrLoader()->GetStringHBuf(EDriveGridThresholdInfoString)->Des();
    aThresholdInfoText.Copy(formatStr);
}

//void CScrShotsDriveGrid::SetFreeSizeText(TDes& aFreeSizeText, TInt64 aDriveFreeSize)
//{
//    CScrShotsStrLoader* strLoader = iParentControl->GetAppView()->GetAppUi().GetStrLoader();
//    TPtr formatStrFreeSizeCell = strLoader->GetStringHBuf(EDriveListFreeSizeCellFormatString)->Des();
//    if((aDriveFreeSize / (KOneGB)) > 0)
//        aFreeSizeText.Format(formatStrFreeSizeCell, &KGBStr,aDriveFreeSize / (KOneGB));
//    else if((aDriveFreeSize / (KOneMB)) > 0)
//        aFreeSizeText.Format(formatStrFreeSizeCell, &KMBStr,aDriveFreeSize / (KOneMB));
//    else if((aDriveFreeSize / (KOneKB)) > 0)
//        aFreeSizeText.Format(formatStrFreeSizeCell,&KKBStr, aDriveFreeSize / (KOneKB));
//    else
//        aFreeSizeText.Format(formatStrFreeSizeCell, &KBytesStr,aDriveFreeSize );
//
//}

void CScrShotsDriveGrid::SetDriveTypeText(TDes& aFreeSizeText, TMediaType aDriveType, TBool aIsDriveRemoveable)
{
    if(aIsDriveRemoveable){
        aFreeSizeText.Copy(_L("Memory Card."));
    }
    else{
        switch(aDriveType){
            case EMediaHardDisk:aFreeSizeText.Copy(_L("Hard disk.")); break;
            case EMediaFlash:aFreeSizeText.Copy(_L("Flash media.")); break;
            case EMediaNANDFlash:aFreeSizeText.Copy(_L("NAND Flash.")); break;
            case EMediaRotatingMedia:  aFreeSizeText.Copy(_L("Rotating Media.")); break;
            default: aFreeSizeText.Copy(_L("Unknown.")); break;
        }
    }
}

void CScrShotsDriveGrid::ComputeGridRects()
{
    
    iDrive1Rect = iDrive2Rect = iDrive3Rect = iDrive4Rect = Rect();
    iDrive1Rect.SetWidth(Rect().Width() * KGridCellWidth);
    iDrive1Rect.SetHeight(Rect().Height() * KGridCellHeight);
    
    iDrive2Rect.iTl.iX = iDrive1Rect.iBr.iX;
    iDrive2Rect.SetWidth(Rect().Width() * KGridCellWidth);
    iDrive2Rect.SetHeight(Rect().Height() * KGridCellHeight);
    
    iDrive3Rect.iTl.iY = iDrive1Rect.iBr.iY;
    iDrive3Rect.SetWidth(Rect().Width() * KGridCellWidth);
    iDrive3Rect.SetHeight(Rect().Height() * KGridCellHeight);
    
    iDrive4Rect.iTl.iX = iDrive3Rect.iBr.iX;
    iDrive4Rect.iTl.iY = iDrive2Rect.iBr.iY;
    iDrive4Rect.SetWidth(Rect().Width() * KGridCellWidth);
    iDrive4Rect.SetHeight(Rect().Height() * KGridCellHeight);
    
}

void CScrShotsDriveGrid::UpdateIconSizeL()
{
    LoadDriveGridsL();
    if(iParentControl->IsCurrentScreen())
        iParentControl->DrawNow();
}

void CScrShotsDriveGrid::RefreshDriveDataL()
{
    LoadDriveGridsL();
    if(iParentControl->IsCurrentScreen()){
        SLOGF(_L("We are current screen so drawing now..."));
        iParentControl->DrawNow();    
    }
}


void CScrShotsDriveGrid::HandleThresholdReached(TBool aIsUpperRange)
{
    if(aIsUpperRange){
        CEikonEnv::Static()->InfoWinL(_L("Upper limit has been reached.Cannot increment further."), KNullDesC);
    }
    else{
        CEikonEnv::Static()->InfoWinL(_L("Lower limit has been reached.Cannot decrement further."), KNullDesC);
    }
}
