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

#include "ScrShotsCustomList.h"
#include "ScrShotsAppFileList.h"
#include "ScrShotsDeviceWatcher.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsStrLoader.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsMainControl.h"
#include "ScrShotsStringPool.h"
#include "EventLogger.h"

#include <aknutils.h>
#include <eikfrlbd.h> 
#include <eiklbv.h>
#include <gulicon.h>

//drive name char graphic | free size text | threshold level text | selected graphic
_LIT(KDriveListItemFormat,"%d\t%S\t%S\t%d");
_LIT(KAppsListItemFormat,"%d\t%d\t%S\t%x\t%S");
_LIT(KFolderListFormat,"%d\t%S\t%d");

const TInt KMaxDriveNames = 3;


//cell width and height defines
#define KDriveIconsCellWidth 0.20
#define KDriveTextCellWidth 0.80

#define KSelectedIconCellWidth 0.20
#define KAppIconCellWidth 0.15
#define KAppIconCellHeight 0.66
#define KFullPathCellTextWidth 0.80
#define KAppCaptionTextCellWidth 0.65
#define KUid3TextCellWidth 0.65
#define KTextCellHeight 0.33
    //cell indices
#define KSelectedIconCellIndex 0
#define KAppIconCellIndex 1
#define KAppCaptionCellIndex 2
#define KUid3CellIndex 3
#define KFullPathCellIndex 4

#define KDirListSelectedIconCellWidth 0.20
#define KDirListMoreEntriesCellWidth 0.20
#define KDirListFolderNameCellWidth 0.60

const TInt KCDrvSelectedIconIndex = 0;
const TInt KCDrvUnselectedIconIndex = 1;
const TInt KEDrvSelectedIconIndex = 2;
const TInt KEDrvUnselectedIconIndex = 3;
const TInt KFDrvSelectedIconIndex = 4;
const TInt KFDrvUnselectedIconIndex = 5;

#define KFileFolderLineCutFactor 4

const int KGoInsideIconIndex = 0;
const int KFolderSelectIndex = 1;
const int KFolderUnSelectIndex = 2;


_LIT(KPathFormatString,"%S%S\\");

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif

//------------------item drawer class defn starts---------------
CScrShotsCustomListboxDrawer::CScrShotsCustomListboxDrawer(MTextListBoxModel *aTextListBoxModel
        , const CFont *aFont, CFormattedCellListBoxData *aFormattedCellData,CScrShotsCustomListbox* aListBox) :
    CFormattedCellListBoxItemDrawer(aTextListBoxModel, aFont, aFormattedCellData)
    , iItemData(aFormattedCellData), iTextListBoxModel(aTextListBoxModel), iListBox(aListBox)
{
    
}

#if 0
void CScrShotsCustomListboxDrawer::DrawEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                    TBool aViewIsDimmed) const
{
    TRACE_ON(RDebug::Printf("Calling %s",__PRETTY_FUNCTION__));
    
//    switch(iListBox->ListType()){
//        case EFileFolderList: DrawFileFolderListEmptyItem(aItemIndex, aItemRectPos, aViewIsDimmed); break;
//        default:
//        case EAppsList: DrawAppsListEmptyItem(aItemIndex, aItemRectPos, aViewIsDimmed); break;
//        //case EDrivesList: DrawDrivesListItem(aItemIndex, aItemRectPos, aViewIsDimmed); break;
//    }
    CFormattedCellListBoxItemDrawer::DrawEmptyItem(aItemIndex, aItemRectPos, aViewIsDimmed);
}

void CScrShotsCustomListboxDrawer::DrawFileFolderListEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                    TBool aViewIsDimmed) const
{
    CWindowGc* gcPtr = Gc();
    gcPtr->SetPenColor(this->HighlightedTextColor());
    gcPtr->SetBrushColor(this->HighlightedBackColor());
    gcPtr->SetPenStyle(CWindowGc::ESolidPen);
    gcPtr->SetBrushStyle(CWindowGc::ESolidBrush);
    gcPtr->UseFont(iItemData->SubCellFont(1));
    TPoint txtPt = aItemRectPos;
    txtPt.iY = iItemData->SubCellFont(1)->HeightInPixels() + 2;
    gcPtr->DrawText( _L("No more folders!!!"), txtPt);
    gcPtr->DiscardFont();
}

void CScrShotsCustomListboxDrawer::DrawAppsListEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                    TBool aViewIsDimmed) const
{
    TRACE_ON(RDebug::Printf("Calling %s",__PRETTY_FUNCTION__));
}
#endif


void CScrShotsCustomListboxDrawer::DrawItemText (TInt aItemIndex, const TRect &aItemTextRect, 
        TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aItemIsSelected) const 
{
    TRACE_ON(RDebug::Printf("Calling %s",__PRETTY_FUNCTION__));
    TRACE_ON(RDebug::Printf("aItemIndex : %d",aItemIndex));
    TRACE_ON(RDebug::Printf("this->iListBox->Model()->NumberOfItems() : %d",this->iListBox->Model()->NumberOfItems()));
    switch(iListBox->ListType()){
        case EFileFolderList: DrawFileFolderListItemText(aItemIndex, aItemTextRect, aItemIsCurrent, aViewIsEmphasized, aItemIsSelected); break;
        default:
        case EAppsList: DrawAppsListItemText(aItemIndex, aItemTextRect, aItemIsCurrent, aViewIsEmphasized, aItemIsSelected); break;
        //case EDrivesList: DrawDrivesListItemText(aItemIndex, aItemTextRect, aItemIsCurrent, aViewIsEmphasized, aItemIsSelected); break;
    }
    
    //also check if the listbox item can be removed and some other item added at that index
    //do formatting on the basis if the item is selected or not
//    CFormattedCellListBoxItemDrawer::DrawItemText(aItemIndex, aItemTextRect,
//    aItemIsCurrent, aViewIsEmphasized, aItemIsSelected);
}

void CScrShotsCustomListboxDrawer::DrawFileFolderListItemText(TInt aItemIndex, const TRect &aItemTextRect,
        TBool aItemIsCurrent, TBool /*aViewIsEmphasized*/, TBool /*aItemIsSelected*/) const
{
    if(aItemIsCurrent){
        Gc()->SetPenColor(this->HighlightedTextColor());
        Gc()->SetBrushColor(this->HighlightedBackColor());
    }
    else{
        Gc()->SetPenColor(this->TextColor());
        Gc()->SetBrushColor(this->BackColor());
    }
    Gc()->SetPenStyle(CWindowGc::ESolidPen);
    Gc()->SetBrushStyle(CWindowGc::ESolidBrush);
    
    TBuf<255> colText;
    TPtrC cellText(colText);
    TPtrC text = iTextListBoxModel->ItemText(aItemIndex);
    TPoint pt1 = aItemTextRect.iTl;
    
    //cells
#ifdef DRAW_RECTS    
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(0),iItemData->SubCellSize(0)),TSize(4,4));
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(1),iItemData->SubCellSize(1)),TSize(4,4));
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(2),iItemData->SubCellSize(2)),TSize(4,4));
#else
    TRect cellRect1(pt1 + iItemData->SubCellPosition(0),iItemData->SubCellSize(0));
    TRect cellRect2(pt1 + iItemData->SubCellPosition(1),iItemData->SubCellSize(1));
    TRect cellRect3(pt1 + iItemData->SubCellPosition(2),iItemData->SubCellSize(2));
    Gc()->SetPenStyle(CWindowGc::ENullPen);
    Gc()->DrawRect(aItemTextRect);
    Gc()->SetPenStyle(CWindowGc::ESolidPen);
    Gc()->DrawLine(TPoint(cellRect1.iBr.iX,cellRect1.iTl.iY + KFileFolderLineCutFactor),TPoint(cellRect1.iBr.iX,cellRect1.iBr.iY - KFileFolderLineCutFactor));
    Gc()->DrawLine(TPoint(cellRect2.iBr.iX,cellRect2.iTl.iY + KFileFolderLineCutFactor),TPoint(cellRect2.iBr.iX,cellRect2.iBr.iY - KFileFolderLineCutFactor));
    Gc()->DrawLine(TPoint(aItemTextRect.iTl.iX,aItemTextRect.iBr.iY),aItemTextRect.iBr);
    if(aItemIsCurrent)
        Gc()->SetPenSize(TSize(2,2));
    Gc()->DrawLine(TPoint(aItemTextRect.iTl.iX + KFileFolderLineCutFactor, aItemTextRect.iBr.iY-2),TPoint(aItemTextRect.iBr.iX - KFileFolderLineCutFactor, aItemTextRect.iBr.iY-2));
    Gc()->SetPenSize(TSize(1,1));    
#endif
    
    //draw folder name
    Gc()->UseFont(iItemData->SubCellFont(1));
    TextUtils::ColumnText(cellText,1, &text, TChar('\t'));
    TPoint txtPt1 = pt1 + iItemData->SubCellPosition(1);
    TInt txtWidth = iItemData->SubCellFont(1)->TextWidthInPixels(cellText);
    TInt itemHeight = iListBox->ItemHeight();
    TInt txtPosY = (itemHeight/ 2 +  iItemData->SubCellFont(1)->HeightInPixels()/2);
    if(txtWidth > (iItemData->SubCellSize(1).iWidth - 4)){
        TStringPoolObj msg(cellText.Length());
        msg.Des().Copy(cellText);
        const CFont* font = iItemData->SubCellFont(1);
        TextUtils::ClipToFit(msg.Des(),*font,iItemData->SubCellSize(1).iWidth - 4 );
        Gc()->DrawText(msg.Des(), TPoint(txtPt1.iX + 4,txtPt1.iY + txtPosY));
    }
    else
        Gc()->DrawText(cellText, TPoint(txtPt1.iX + 4,txtPt1.iY + txtPosY));
    
    
    TextUtils::ColumnText(cellText,0, &text, TChar('\t'));
    TLex lx;
    lx.Assign(cellText);
    TInt iconIndex = 0;
    lx.Val(iconIndex);
    CArrayPtr<CGulIcon>* iconArr = iListBox->CustomIconArray();
    
    CGulIcon* gulIcon = iconArr->At(iconIndex);
    //calculate the center of the cell to draw the folder select /unselect icon
    TPoint centrPt;
    int cell0HalfWidth =  iItemData->SubCellSize(0).iWidth / 2 ;
    int iconhalfwidth = gulIcon->Bitmap()->SizeInPixels().iWidth/2;
    centrPt.iX = pt1.iX + iItemData->SubCellPosition(0).iX + ((cell0HalfWidth) - (iconhalfwidth));
    centrPt.iY = pt1.iY + iItemData->SubCellPosition(0).iY + ((iItemData->SubCellSize(0).iHeight / 2 ) - (gulIcon->Bitmap()->SizeInPixels().iHeight/2));
    Gc()->BitBlt(centrPt, gulIcon->Bitmap());    
    
    gulIcon = iconArr->At(KGoInsideIconIndex);
    //calculate the center of the cell to draw the go inside folder icon
    centrPt.iX = pt1.iX + iItemData->SubCellPosition(2).iX + ((iItemData->SubCellSize(2).iWidth / 2 ) - (gulIcon->Bitmap()->SizeInPixels().iWidth/2));
    centrPt.iY = pt1.iY + iItemData->SubCellPosition(2).iY + ((iItemData->SubCellSize(2).iHeight / 2 ) - (gulIcon->Bitmap()->SizeInPixels().iHeight/2));
    Gc()->BitBlt(centrPt, gulIcon->Bitmap());
    
    Gc()->DiscardFont();
}

void CScrShotsCustomListboxDrawer::DrawAppsListItemText(TInt aItemIndex, const TRect &aItemTextRect,
        TBool aItemIsCurrent, TBool /*aViewIsEmphasized*/
        , TBool /*aItemIsSelected*/
        ) const
{
    CWindowGc* gcPtr = Gc();
    
    if(aItemIsCurrent){
        gcPtr->SetPenColor(this->HighlightedTextColor());
        gcPtr->SetBrushColor(this->HighlightedBackColor());
        gcPtr->SetBrushStyle(CWindowGc::ESolidBrush);
    }
    else{
        Gc()->SetPenColor(this->TextColor());
        gcPtr->SetPenStyle(CWindowGc::ESolidPen);
        gcPtr->SetBrushColor(this->BackColor());
        gcPtr->SetBrushStyle(CWindowGc::ESolidBrush);
    }
    
    TBuf<255> colText;
    TPtrC cellText(colText);
    TPtrC text = iTextListBoxModel->ItemText(aItemIndex);

    TPoint pt = iItemData->SubCellPosition(0);
    TSize sz = iItemData->SubCellSize(0);
    pt += aItemTextRect.iTl;
    
    TPoint pt1 = aItemTextRect.iTl;
    //icons
    TSize iconSize = iItemData->SubCellSize(0);
#ifdef DRAW_RECTS     
    gcPtr->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(0),iconSize),TSize(4,4));
    gcPtr->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(1),iItemData->SubCellSize(1)),TSize(4,4));
    //text
    gcPtr->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(3),iItemData->SubCellSize(3)),TSize(4,4));
    gcPtr->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(2),iItemData->SubCellSize(2)),TSize(4,4));
    gcPtr->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(4),iItemData->SubCellSize(4)),TSize(4,4));
#else
    TRect cellRect0(pt1 + iItemData->SubCellPosition(0),iconSize);
    TRect cellRect1(pt1 + iItemData->SubCellPosition(1),iItemData->SubCellSize(1));
    TRect cellRect2(pt1 + iItemData->SubCellPosition(3),iItemData->SubCellSize(3));
    TRect cellRect3(pt1 + iItemData->SubCellPosition(2),iItemData->SubCellSize(2));
    TRect cellRect4(pt1 + iItemData->SubCellPosition(4),iItemData->SubCellSize(4));
    Gc()->SetPenStyle(CWindowGc::ENullPen);
    Gc()->DrawRect(aItemTextRect);
    Gc()->SetPenStyle(CWindowGc::ESolidPen);
    gcPtr->DrawLine(TPoint(cellRect0.iBr.iX,cellRect0.iTl.iY + KFileFolderLineCutFactor), TPoint(cellRect0.iBr.iX,cellRect0.iBr.iY - KFileFolderLineCutFactor));
    gcPtr->DrawLine(TPoint(cellRect1.iBr.iX,cellRect1.iTl.iY + KFileFolderLineCutFactor), TPoint(cellRect1.iBr.iX,cellRect1.iBr.iY - KFileFolderLineCutFactor));
    if(aItemIsCurrent)
        gcPtr->SetPenSize(TSize(2,2));
    gcPtr->DrawLine(TPoint(aItemTextRect.iTl.iX + KFileFolderLineCutFactor, aItemTextRect.iBr.iY-2),TPoint(aItemTextRect.iBr.iX - KFileFolderLineCutFactor, aItemTextRect.iBr.iY-2));
    gcPtr->SetPenSize(TSize(1,1));
#endif
    gcPtr->UseFont(iItemData->SubCellFont(2));
#define KLeftTextMargin 4
    //todo : clip the text so that the margin is also maintained on the right side
    TextUtils::ColumnText(cellText,2, &text, TChar('\t'));
    TPoint txtPt1 = pt1 + iItemData->SubCellPosition(2);
    TInt itemHeight = iItemData->SubCellSize(2).iHeight;
    TInt txtPosY = (itemHeight/ 2 +  iItemData->SubCellFont(2)->HeightInPixels()/2);
    gcPtr->DrawText(cellText, TPoint(txtPt1.iX + KLeftTextMargin,txtPt1.iY + txtPosY));

    TextUtils::ColumnText(cellText,3, &text, TChar('\t'));
    //copy  the uid into buffer
    TBuf<16> uidBuf;
    TUint32 uidInt;
    uidBuf.Copy(cellText);
    TLex lx;
    lx.Assign(uidBuf);
    lx.Val(uidInt, EHex);
    //copy the uid done
    TPoint txtPt2 = pt1 + iItemData->SubCellPosition(3);
    itemHeight = iItemData->SubCellSize(3).iHeight;
    txtPosY = (itemHeight/ 2 +  iItemData->SubCellFont(3)->HeightInPixels()/2);
    gcPtr->DrawText(cellText, TPoint(txtPt2.iX + KLeftTextMargin,txtPt2.iY + txtPosY));
    gcPtr->DiscardFont();
    
    gcPtr->UseFont(iItemData->SubCellFont(4));
    TextUtils::ColumnText(cellText,4, &text, TChar('\t'));
    TPoint txtPt3 = pt1 + iItemData->SubCellPosition(4);
    itemHeight = iItemData->SubCellSize(4).iHeight;
    txtPosY = (itemHeight/ 2 +  iItemData->SubCellFont(4)->HeightInPixels()/2);
    gcPtr->DrawText(cellText, TPoint(txtPt3.iX + KLeftTextMargin,txtPt3.iY + txtPosY));
    gcPtr->DiscardFont();
    
    CArrayPtr<CGulIcon>* iconArr = iListBox->CustomIconArray();
    const int KAppSelectedIconIndex = 0;
    const int KDefaultAppIconIndex = 1;
    CGulIcon* gulIcon = NULL;
    if(uidInt == iListBox->GetSelectedAppUidL())
        gulIcon = iconArr->At(KAppSelectedIconIndex);
    else
        gulIcon = iconArr->At(KDefaultAppIconIndex);
    TPoint centrPt;
    int cell0HalfWidth =  iItemData->SubCellSize(0).iWidth / 2 ;
    int iconhalfwidth = gulIcon->Bitmap()->SizeInPixels().iWidth/2;
    centrPt.iX = pt1.iX + iItemData->SubCellPosition(0).iX + ((cell0HalfWidth) - (iconhalfwidth));
    centrPt.iY = pt1.iY + iItemData->SubCellPosition(0).iY + ((iItemData->SubCellSize(0).iHeight / 2 ) - (gulIcon->Bitmap()->SizeInPixels().iHeight/2));    
    TRect iconRect(TPoint(pt1 + iItemData->SubCellPosition(0) + TPoint(2,2)), iItemData->SubCellSize(0) - TSize(2,2));
    gcPtr->DrawBitmap(iconRect, gulIcon->Bitmap());
    
    if(iconArr && (aItemIndex+2 < iconArr->Count())){
        CGulIcon* gulIcon = iconArr->At(aItemIndex+2);
        if(gulIcon)
            gcPtr->DrawBitmap(TRect(TPoint(pt1 + iItemData->SubCellPosition(1)),TSize(iItemData->SubCellSize(1))), gulIcon->Bitmap());
//        else{
//            iListBox->GetParentControl()->GetVectorImgLoader().SetIconSize(EDefaultAppIcon, iItemData->SubCellSize(1));
//            Gc()->BitBlt(pt1 + iItemData->SubCellPosition(1), iListBox->GetParentControl()->GetVectorImgLoader().GetBitmap(EDefaultAppIcon));
//        }
    }
}

void CScrShotsCustomListboxDrawer::DrawDrivesListItemText(TInt aItemIndex, const TRect &aItemTextRect,
        TBool aItemIsCurrent, TBool /*aViewIsEmphasized*/
        , TBool /*aItemIsSelected*/
        ) const
{
#if 0    
    Gc()->SetBrushStyle(CWindowGc::EPatternedBrush);
    if(aItemIsCurrent)
        Gc()->UseBrushPattern(iListBox->NormalListItemBackground());
    else
        Gc()->UseBrushPattern(iListBox->SelectedItemBackground());
#endif
    if(aItemIsCurrent){
        Gc()->SetBrushColor(this->HighlightedBackColor());
        Gc()->SetPenColor(this->HighlightedTextColor());
    }else{
        Gc()->SetBrushColor(this->BackColor());
        Gc()->SetPenColor(this->TextColor());
    }
    Gc()->SetPenStyle(CWindowGc::ESolidPen);
    Gc()->SetBrushStyle(CWindowGc::ESolidBrush);

    TBuf<255> colText;
    TPtrC cellText(colText);
    TPtrC text = iTextListBoxModel->ItemText(aItemIndex);
    TPoint pt1 = aItemTextRect.iTl;
    
    //draw the rounded rects
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(0),iItemData->SubCellSize(0)),TSize(4,4));
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(1),iItemData->SubCellSize(1)),TSize(4,4));
    Gc()->DrawRoundRect(TRect(pt1 + iItemData->SubCellPosition(2),iItemData->SubCellSize(2)),TSize(4,4));
#if 0    
    Gc()->DiscardBrushPattern();
#endif
    
    //draw the icon, get the icon index from the format string
    CArrayPtr<CGulIcon>* iconArr = iListBox->CustomIconArray();
    TextUtils::ColumnText(cellText,0, &text, TChar('\t'));
    TLex lx;
    lx.Assign(cellText);
    TInt iconIndex = 0;
    lx.Val(iconIndex);
    CGulIcon* gulIcon = iconArr->At(iconIndex);
    Gc()->DrawBitmap(TRect(TPoint(pt1 + iItemData->SubCellPosition(0)),TSize(iItemData->SubCellSize(0))), gulIcon->Bitmap());    
    Gc()->UseFont(iItemData->SubCellFont(1));
    
    TextUtils::ColumnText(cellText,1, &text, TChar('\t'));
    TPoint txtPt1 = pt1 + iItemData->SubCellPosition(1);
    TInt textWidth = iItemData->SubCellFont(1)->TextWidthInPixels(cellText);
    TInt txtX = (iItemData->SubCellSize(1).iWidth / 2) - (textWidth / 2);
    Gc()->DrawText(cellText, TPoint(txtPt1.iX + txtX,txtPt1.iY + iItemData->SubCellBaselinePos(1)));
    
    TextUtils::ColumnText(cellText,2, &text, TChar('\t'));
    TPoint txtPt2 = pt1 + iItemData->SubCellPosition(2);
    textWidth = iItemData->SubCellFont(2)->TextWidthInPixels(cellText);
    txtX = (iItemData->SubCellSize(2).iWidth / 2) - (textWidth / 2);    
    Gc()->DrawText(cellText, TPoint(txtPt2.iX + txtX,txtPt2.iY + iItemData->SubCellBaselinePos(2)));

    Gc()->DiscardFont();   
}


void CScrShotsCustomListboxDrawer::DrawItemMark(
        TBool /*aItemIsSelected*/, 
        TBool /*aViewIsDimmed*/, 
                               const TPoint& /*aMarkPos*/) const
{

}

    

//----------------------listbox class defn starts here-----------------------

CScrShotsCustomListbox::CScrShotsCustomListbox(CScrShotsAppFileList* aParentControl,TListType aListType)
:CEikFormattedCellListBox()
,iListType (aListType)
,iParentControl(aParentControl)
{
    if(iListType == EFileFolderList){
        iCurrentPath.Create(1024);
        iSelectedPath.Create(1024);
    }
    else if(iListType == EAppsList){
        iSelectedAppName.Create(256);
    }
}

void CScrShotsCustomListbox::SetSelectedPathL(const TDesC& aFolderPath)
{
    if(aFolderPath.Length() > iSelectedPath.MaxLength()){
        iSelectedPath.ReAlloc(aFolderPath.Length());
    }
    iSelectedPath.Copy(aFolderPath);
}


void CScrShotsCustomListbox::CreateItemDrawerL(void)
{
    //create the gradient bitmaps here
#if 0
    TInt itemHeight = this->ItemHeight();
    TRect itemRect(TPoint(0,0),TSize(Rect().Width(),itemHeight));
    TRgb normalItemStartColor(0,56,140);
    TRgb normalItemEndColor(178,230,255);
    TRgb selectedItemStartColor(28,0,140);
    TRgb selectedItemEndColor(191,178,255);
    
    if(!iNormalListItemBackground)
        iNormalListItemBackground = CScrShotsGradientBitmapSkin::CreateGradientBitmapL(normalItemStartColor,normalItemEndColor,itemRect);
    if(!iSelectedItemBackground)
        iSelectedItemBackground = CScrShotsGradientBitmapSkin::CreateGradientBitmapL(selectedItemStartColor,selectedItemEndColor,itemRect);
#endif
    
    CFormattedCellListBoxData* item = NULL;
    switch(iListType){
        case EFileFolderList:  item = CreateFileFolderListItemDataLC();      break;
        default:
        case EAppsList:        item = CreateAppsListItemDataLC();      break;
#ifdef ENABLE_DRIVE_LIST        
        case EDrivesList:      item = CreateDrivesListItemDataLC();      break;
#endif        
    }
    iItemDrawer = new (ELeave) CScrShotsCustomListboxDrawer(Model(), iEikonEnv->NormalFont(), item, this);
    CleanupStack::Pop(item);
}

void CScrShotsCustomListbox::UpdateItemDrawerL()
{
    switch(iListType){
        case EFileFolderList:  UpdateFileFolderListItemDataL();      break;
        default:
        case EAppsList:        UpdateAppsListItemDataL();      break;
#ifdef ENABLE_DRIVE_LIST        
        case EDrivesList:      UpdateDrivesListItemDataL();      break;
#endif        
    }
}

CFormattedCellListBoxData* CScrShotsCustomListbox::CreateFileFolderListItemDataLC()
{
    CFormattedCellListBoxData* item = CFormattedCellListBoxData::NewL();
    CleanupStack::PushL(item);
    
    TRect appListRect = iParentControl->GetAppsFileListRect();
    
    iCellFont15 = iParentControl->GetAppView()->GetAppUi().GetCustomFont();
    TInt itemHeight = this->ItemHeight();

    
    //calculate the cell rects now
    TRect selectIconCellRect, folderNameCellRect, moreEntriesIconCellRect;
    selectIconCellRect.iTl.iX = folderNameCellRect.iTl.iX = moreEntriesIconCellRect.iTl.iX = 0;
    
    selectIconCellRect.iTl.iY = 0;
    selectIconCellRect.SetWidth(appListRect.Width() * KDirListSelectedIconCellWidth );
    selectIconCellRect.SetHeight(itemHeight);
    
    folderNameCellRect.iTl.iX = selectIconCellRect.iBr.iX;
    folderNameCellRect.SetWidth(appListRect.Width() * KDirListFolderNameCellWidth );
    folderNameCellRect.SetHeight(itemHeight);

    moreEntriesIconCellRect.iTl.iX = folderNameCellRect.iBr.iX;
    moreEntriesIconCellRect.SetWidth(appListRect.Width() * KDirListMoreEntriesCellWidth );
    moreEntriesIconCellRect.SetHeight(itemHeight);
     
    item->SetSubCellPositionL(0, selectIconCellRect.iTl );
    item->SetSubCellSizeL(0, selectIconCellRect.Size() );
    item->SetGraphicsSubCellL(0, ETrue);
    item->SetSubCellAlignmentL(0, CGraphicsContext::ELeft);
     
    item->SetSubCellPositionL(1, folderNameCellRect.iTl );
    item->SetSubCellSizeL(1, folderNameCellRect.Size() );
    item->SetSubCellBaselinePosL(1, iCellFont15->HeightInPixels());
    item->SetSubCellFontL(1, iCellFont15 );
    item->SetSubCellAlignmentL(1, CGraphicsContext::ELeft);

    item->SetSubCellPositionL(2, moreEntriesIconCellRect.iTl );
    item->SetSubCellSizeL(2, moreEntriesIconCellRect.Size() );
    item->SetGraphicsSubCellL(2, ETrue);
    item->SetSubCellAlignmentL(2, CGraphicsContext::ELeft);

    return item;
}

void CScrShotsCustomListbox::UpdateFileFolderListItemDataL()
{
    CFormattedCellListBoxData* itemData = this->ItemDrawer()->ColumnData();
    TRect appListRect = iParentControl->GetAppsFileListRect();
    TInt itemHeight = this->ItemHeight();
    
    TRect selectIconCellRect, folderNameCellRect, moreEntriesIconCellRect;
    selectIconCellRect.iTl.iX = folderNameCellRect.iTl.iX = moreEntriesIconCellRect.iTl.iX = 0;
    
    selectIconCellRect.iTl.iY = 0;
    selectIconCellRect.SetWidth(appListRect.Width() * KDirListSelectedIconCellWidth );
    selectIconCellRect.SetHeight(itemHeight);
    
    folderNameCellRect.iTl.iX = selectIconCellRect.iBr.iX;
    folderNameCellRect.SetWidth(appListRect.Width() * KDirListFolderNameCellWidth );
    folderNameCellRect.SetHeight(itemHeight);

    moreEntriesIconCellRect.iTl.iX = folderNameCellRect.iBr.iX;
    moreEntriesIconCellRect.SetWidth(appListRect.Width() * KDirListMoreEntriesCellWidth );
    moreEntriesIconCellRect.SetHeight(itemHeight);

    itemData->SetSubCellPositionL(0, selectIconCellRect.iTl );
    itemData->SetSubCellSizeL(0, selectIconCellRect.Size() );
     
    itemData->SetSubCellPositionL(1, folderNameCellRect.iTl );
    itemData->SetSubCellSizeL(1, folderNameCellRect.Size() );

    itemData->SetSubCellPositionL(2, moreEntriesIconCellRect.iTl );
    itemData->SetSubCellSizeL(2, moreEntriesIconCellRect.Size() );
}


CFormattedCellListBoxData* CScrShotsCustomListbox::CreateAppsListItemDataLC()
{
    CFormattedCellListBoxData* item = CFormattedCellListBoxData::NewL();
    CleanupStack::PushL(item);
    // add item layout
    //_LIT(KAppsListItemFormat,"%d\t%d\t%S\t%S\t%S");

    
    //create the font for the items
    iCellFont18 = iParentControl->GetAppView()->GetAppUi().GetCustomFont();
    TInt itemHeight = iParentControl->CalculatedItemHeight();

    //calculate the cell rects first
    TRect appListRect = iParentControl->GetAppsFileListRect();
    TRect appListSelectedRect, appListAppIconRect, appListFullPathCellRect, appListAppCaptionCellRect, appListUid3CellRect;
    appListSelectedRect.iTl.iX = appListAppIconRect.iTl.iX = appListFullPathCellRect.iTl.iX = appListAppCaptionCellRect.iTl.iX = appListUid3CellRect.iTl.iX = 0;
    
    appListSelectedRect.iTl.iY = 0;
    appListSelectedRect.SetWidth(appListRect.Width() * KSelectedIconCellWidth );
    appListSelectedRect.SetHeight(itemHeight);
    
    appListAppIconRect.iTl.iX = appListSelectedRect.iBr.iX;
    appListAppIconRect.SetWidth(appListRect.Width() * KAppIconCellWidth );
    appListAppIconRect.SetHeight(itemHeight * KAppIconCellHeight );
    
    appListAppCaptionCellRect.iTl.iX = appListAppIconRect.iBr.iX;
    appListAppCaptionCellRect.SetWidth(appListRect.Width() * KAppCaptionTextCellWidth);
    appListAppCaptionCellRect.SetHeight(itemHeight * KTextCellHeight );
    
    appListUid3CellRect.iTl.iX = appListAppIconRect.iBr.iX;
    appListUid3CellRect.iTl.iY = appListAppCaptionCellRect.iBr.iY;
    appListUid3CellRect.SetWidth(appListRect.Width() * KUid3TextCellWidth);
    appListUid3CellRect.SetHeight(itemHeight * KTextCellHeight );

    appListFullPathCellRect.iTl.iX = appListSelectedRect.iBr.iX;
    appListFullPathCellRect.iTl.iY = appListAppIconRect.iBr.iY;
    appListFullPathCellRect.SetWidth(appListRect.Width() * KFullPathCellTextWidth);
    appListFullPathCellRect.SetHeight(itemHeight * KTextCellHeight );
    
    
    //first cell to be drawn     
    item->SetSubCellPositionL(KSelectedIconCellIndex, appListSelectedRect.iTl );
    item->SetSubCellSizeL(KSelectedIconCellIndex, appListSelectedRect.Size() );
    item->SetSubCellAlignmentL(KSelectedIconCellIndex, CGraphicsContext::ELeft);
    item->SetGraphicsSubCellL(KSelectedIconCellIndex,ETrue);
     
    item->SetSubCellPositionL(KAppIconCellIndex, appListAppIconRect.iTl );
    item->SetSubCellSizeL(KAppIconCellIndex, appListAppIconRect.Size() );
    item->SetSubCellAlignmentL(KAppIconCellIndex, CGraphicsContext::ELeft);
    item->SetGraphicsSubCellL(KAppIconCellIndex,ETrue);

    item->SetSubCellPositionL(KAppCaptionCellIndex, appListAppCaptionCellRect.iTl  );
    item->SetSubCellSizeL(KAppCaptionCellIndex, appListAppCaptionCellRect.Size() );
    item->SetSubCellBaselinePosL(KAppCaptionCellIndex, iCellFont18->HeightInPixels());
    item->SetSubCellFontL(KAppCaptionCellIndex, iCellFont18 );
    item->SetSubCellAlignmentL(KAppCaptionCellIndex, CGraphicsContext::ELeft);

    item->SetSubCellPositionL(KUid3CellIndex, appListUid3CellRect.iTl  );
    item->SetSubCellSizeL(KUid3CellIndex, appListUid3CellRect.Size() );
    item->SetSubCellBaselinePosL(KUid3CellIndex, iCellFont18->HeightInPixels());
    item->SetSubCellFontL(KUid3CellIndex, iCellFont18 );
    item->SetSubCellAlignmentL(KUid3CellIndex, CGraphicsContext::ELeft);
    
    iCellFont15 = iParentControl->GetAppView()->GetAppUi().GetCustomSmallFont();
    iFullPathCellFont = iParentControl->GetAppView()->GetAppUi().GetCustomSmallFont();

    item->SetSubCellPositionL(KFullPathCellIndex,appListFullPathCellRect.iTl  );
    item->SetSubCellSizeL(KFullPathCellIndex, appListFullPathCellRect.Size() );
    item->SetSubCellBaselinePosL(KFullPathCellIndex, iFullPathCellFont->HeightInPixels());
    item->SetSubCellFontL(KFullPathCellIndex, iFullPathCellFont );
    item->SetSubCellAlignmentL(KFullPathCellIndex, CGraphicsContext::ELeft);
        
    return item;
}

void CScrShotsCustomListbox::UpdateAppsListItemDataL()
{
    CFormattedCellListBoxData* itemData = this->ItemDrawer()->ColumnData();
    TRect appListRect = iParentControl->GetAppsFileListRect();
    TInt itemHeight = this->ItemHeight();
    
    TRect appListSelectedRect, appListAppIconRect, appListFullPathCellRect, appListAppCaptionCellRect, appListUid3CellRect;
    appListSelectedRect.iTl.iX = appListAppIconRect.iTl.iX = appListFullPathCellRect.iTl.iX = appListAppCaptionCellRect.iTl.iX = appListUid3CellRect.iTl.iX = 0;
    
    appListSelectedRect.iTl.iY = 0;
    appListSelectedRect.SetWidth(appListRect.Width() * KSelectedIconCellWidth );
    appListSelectedRect.SetHeight(itemHeight);
    
    appListAppIconRect.iTl.iX = appListSelectedRect.iBr.iX;
    appListAppIconRect.SetWidth(appListRect.Width() * KAppIconCellWidth );
    appListAppIconRect.SetHeight(itemHeight * KAppIconCellHeight );
    
    appListAppCaptionCellRect.iTl.iX = appListAppIconRect.iBr.iX;
    appListAppCaptionCellRect.SetWidth(appListRect.Width() * KAppCaptionTextCellWidth);
    appListAppCaptionCellRect.SetHeight(itemHeight * KTextCellHeight );
    
    appListUid3CellRect.iTl.iX = appListAppIconRect.iBr.iX;
    appListUid3CellRect.iTl.iY = appListAppCaptionCellRect.iBr.iY;
    appListUid3CellRect.SetWidth(appListRect.Width() * KUid3TextCellWidth);
    appListUid3CellRect.SetHeight(itemHeight * KTextCellHeight );

    appListFullPathCellRect.iTl.iX = appListSelectedRect.iBr.iX;
    appListFullPathCellRect.iTl.iY = appListAppIconRect.iBr.iY;
    appListFullPathCellRect.SetWidth(appListRect.Width() * KFullPathCellTextWidth);
    appListFullPathCellRect.SetHeight(itemHeight * KTextCellHeight );
    
    
    //first cell to be drawn     
    itemData->SetSubCellPositionL(KSelectedIconCellIndex, appListSelectedRect.iTl );
    itemData->SetSubCellSizeL(KSelectedIconCellIndex, appListSelectedRect.Size() );
     
    itemData->SetSubCellPositionL(KAppIconCellIndex, appListAppIconRect.iTl );
    itemData->SetSubCellSizeL(KAppIconCellIndex, appListAppIconRect.Size() );

    itemData->SetSubCellPositionL(KAppCaptionCellIndex, appListAppCaptionCellRect.iTl  );
    itemData->SetSubCellSizeL(KAppCaptionCellIndex, appListAppCaptionCellRect.Size() );

    itemData->SetSubCellPositionL(KUid3CellIndex, appListUid3CellRect.iTl  );
    itemData->SetSubCellSizeL(KUid3CellIndex, appListUid3CellRect.Size() );
    
    itemData->SetSubCellPositionL(KFullPathCellIndex,appListFullPathCellRect.iTl  );
    itemData->SetSubCellSizeL(KFullPathCellIndex, appListFullPathCellRect.Size() );
    
}
#ifdef ENABLE_DRIVE_LIST
CFormattedCellListBoxData* CScrShotsCustomListbox::CreateDrivesListItemDataLC()
{
    CFormattedCellListBoxData* item = CFormattedCellListBoxData::NewL();
    CleanupStack::PushL(item);
    // add item layout
    
    //create the font for the items
//    ScrShotsGenUtils::FreeFontL(iCellFont);
    iCellFont = ScrShotsGenUtils::InitFontL(16);

    //do cell calculation here
    TRect drvListRect = iParentControl->GetDrivesListRect();
    TInt itemHeight = this->ItemHeight();
    //drvListRect.SetWidth(drvListRect.Width() - this->ScrollBarFrame()->VerticalScrollBar()->Size().iWidth);
    //drvListRect.SetHeight(drvListRect.Height() / 3 );
    TRect drvIconRect, drvFreeSizeTextRect, drvThresholdTextRect;//, iSelectedDrvIcon;
    //drvIconRect = drvFreeSizeTextRect = drvThresholdTextRect = iSelectedDrvIcon = drvListRect;
    drvIconRect.iTl.iX = drvIconRect.iTl.iY = 0;
    drvIconRect.SetWidth(drvListRect.Width() * KDriveIconsCellWidth);
    drvIconRect.SetHeight(itemHeight);
    
    drvFreeSizeTextRect.iTl.iX = drvThresholdTextRect.iTl.iX = drvIconRect.iBr.iX;
    drvFreeSizeTextRect.SetWidth(drvListRect.Width() * KDriveTextCellWidth);
    drvFreeSizeTextRect.SetHeight(itemHeight / 2);
    
    drvThresholdTextRect.iTl.iY = drvFreeSizeTextRect.iBr.iY;
    drvThresholdTextRect.SetWidth(drvListRect.Width() * KDriveTextCellWidth);
    drvThresholdTextRect.SetHeight(itemHeight / 2);
    
//    iSelectedDrvIcon.iTl.iX = drvFreeSizeTextRect.iBr.iX;
//    iSelectedDrvIcon.SetWidth(drvListRect.Width() * KDriveIconsCellWidth);
//    iSelectedDrvIcon.SetHeight(itemHeight);
    
    // drive Icon (type of number)
    item->SetSubCellSizeL(0, drvIconRect.Size() );
    item->SetSubCellPositionL(0, drvIconRect.iTl );
    item->SetGraphicsSubCellL(0,ETrue);
    item->SetSubCellAlignmentL(0, CGraphicsContext::ECenter);

    // free size text     
    item->SetSubCellPositionL(1, drvFreeSizeTextRect.iTl );
    item->SetSubCellSizeL(1, drvFreeSizeTextRect.Size() );
    item->SetSubCellBaselinePosL(1, iCellFont->HeightInPixels());
    item->SetSubCellFontL(1, iCellFont );
    item->SetSubCellAlignmentL(1, CGraphicsContext::ELeft);

    // threshold size text
    item->SetSubCellPositionL(2, drvThresholdTextRect.iTl );
    item->SetSubCellSizeL(2, drvThresholdTextRect.Size() );
    item->SetSubCellBaselinePosL(2, iCellFont->HeightInPixels());
    item->SetSubCellFontL(2, iCellFont );
    item->SetSubCellAlignmentL(2, CGraphicsContext::ELeft);

    // selected drive icon,CELL REMOVED!!
//    item->SetSubCellSizeL(3, iSelectedDrvIcon.Size() );
//    item->SetSubCellPositionL(3, iSelectedDrvIcon.iTl );
//    item->SetSubCellBaselinePosL(3, 16);
//    item->SetSubCellFontL(3, iEikonEnv->DenseFont() );
//    item->SetGraphicsSubCellL(3,ETrue);
//    item->SetSubCellAlignmentL(3, CGraphicsContext::ECenter);
    
    return item;
}

void CScrShotsCustomListbox::UpdateDrivesListItemDataL()
{
    CFormattedCellListBoxData* itemData = this->ItemDrawer()->ColumnData();
    //do cell calculation again as the orientation has changed
    TRect drvListRect = iParentControl->GetDrivesListRect();
    TRect drvIconRect, drvFreeSizeTextRect, drvThresholdTextRect, iSelectedDrvIcon;
    TInt itemHeight = this->ItemHeight();
    
    drvIconRect.iTl.iX = drvIconRect.iTl.iY = 0;
    drvIconRect.SetWidth(drvListRect.Width() * KDriveIconsCellWidth);
    drvIconRect.SetHeight(itemHeight);
    
    drvFreeSizeTextRect.iTl.iX = drvThresholdTextRect.iTl.iX = drvIconRect.iBr.iX;
    drvFreeSizeTextRect.SetWidth(drvListRect.Width() * KDriveTextCellWidth);
    drvFreeSizeTextRect.SetHeight(itemHeight / 2);
    
    drvThresholdTextRect.iTl.iY = drvFreeSizeTextRect.iBr.iY;
    drvThresholdTextRect.SetWidth(drvListRect.Width() * KDriveTextCellWidth);
    drvThresholdTextRect.SetHeight(itemHeight / 2);

    //NOT ADDING THIS!!
//    iSelectedDrvIcon.iTl.iX = drvFreeSizeTextRect.iBr.iX;
//    iSelectedDrvIcon.SetWidth(drvListRect.Width() * KDriveIconsCellWidth);
//    iSelectedDrvIcon.SetHeight(itemHeight);
    
    // drive Icon (type of number)
    itemData->SetSubCellSizeL(0, drvIconRect.Size() );
    itemData->SetSubCellPositionL(0, drvIconRect.iTl );

    // free size text
    itemData->SetSubCellPositionL(1, drvFreeSizeTextRect.iTl );
    itemData->SetSubCellSizeL(1, drvFreeSizeTextRect.Size() );

    // threshold size text
    itemData->SetSubCellPositionL(2, drvThresholdTextRect.iTl );
    itemData->SetSubCellSizeL(2, drvThresholdTextRect.Size() );

    // selected drive icon
//    itemData->SetSubCellSizeL(3, iSelectedDrvIcon.Size() );
//    itemData->SetSubCellPositionL(3, iSelectedDrvIcon.iTl );
}
#endif


CScrShotsCustomListbox::~CScrShotsCustomListbox()
{
//    CFormattedCellListBoxData* itemData = this->ItemDrawer()->ColumnData();
    //release the font held in the item list data
    iCellFont15 = iCellFont18 = iFullPathCellFont = NULL;
    
    if(iListType == EFileFolderList){    
        iSelectedPath.Close();
        iCurrentPath.Close();
    }else if(iListType == EAppsList){
        iSelectedAppName.Close();
    }

#if 0    
    delete iNormalListItemBackground;
    delete iSelectedItemBackground;
#endif
    //todo: commented this call as its giving a KERN-EXEC 3
    // figure out the reason of the crash
    //ScrShotsGenUtils::FreeFontL(iCellFont);
    //ScrShotsGenUtils::FreeFontL(iFullPathCellFont);
}


void CScrShotsCustomListbox::LoadItemTextArrayL(TBool aLoadCachedValue, TFileLoadType aLoadType)
{
    SLOGF(_L("CScrShotsCustomListbox::LoadItemTextArrayL"))
    switch(ListType()){
        case EFileFolderList: LoadFileFoldersListL(aLoadCachedValue, aLoadType); break;
        default:
        case EAppsList: LoadAppsListL(aLoadCachedValue); break;
#ifdef ENABLE_DRIVE_LIST        
        case EDrivesList: LoadDriveListL(aLoadCachedValue); break;
#endif        
    }
}

void CScrShotsCustomListbox::LoadAppsListL(TBool aLoadCachedValue)
{
    SLOGF(_L("CScrShotsCustomListbox::LoadAppsListL"))
    TStringPoolObj msg(512);
    TBuf<16> uidStr;
    const RPointerArray<TApaAppInfo>& appArray = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetAppListWithInfoL(aLoadCachedValue);
    CDesCArray* appnames = new (ELeave) CDesCArrayFlat(10);
    TRACE_ON(RDebug::Printf("Total app count : %d", appArray.Count());)
    for(TInt i = 0; i < appArray.Count() ; i++){
        TApaAppInfo* appInfo = appArray[i];
        
        //_LIT(KAppsListItemFormat,"%d\t%d\t%S\t%S\t%S");
        msg.Des().Format(KAppsListItemFormat,i
                ,i
                ,&(appInfo->iShortCaption)
                ,appInfo->iUid
                ,&(appInfo->iFullName));
        appnames->AppendL(msg.Des());
        appInfo = NULL;
    }
    Model()->SetItemTextArray(appnames);
    Model()->SetOwnershipType(ELbmOwnsItemArray);
}

//this is usually called on listbox init so it will usually load the selected dir
void CScrShotsCustomListbox::LoadFileFoldersListL(TBool aLoadCachedValue, TFileLoadType aLoadType)
{
    //get the folder list for the root drive
    TBuf<3> drv;
    TStringPoolObj dir(1024), fullPath(1024);
    TBuf<256> msg;
    if(aLoadType == ELoadCurrentPath){
        fullPath.Des().Copy(iCurrentPath);
    }else if(aLoadType == ELoadFromSettings){
        iParentControl->GetAppView()->GetAppSettings()->OutputFolderDrive(drv);
        iParentControl->GetAppView()->GetAppSettings()->OutputFolderNameL(dir.Des());
        fullPath.Des().Format(KPathFormatStringWithColon,&drv, &(dir.Des()));
        iCurrentPath.Copy(fullPath.Des());
    }
    
    TInt errCode = KErrNone;
    RArray<TEntry>& dirList = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetFolderList(fullPath.Des(), errCode,!aLoadCachedValue);
    
    //_LIT(KFolderListFormat,"%d\t%S\t%d");    
    CDesCArray* dirNames = new (ELeave) CDesCArrayFlat(10);
    //dont check the first entry in dirList as its the same directory only.
    for(TInt i = 0; i < dirList.Count() ; i++){
        msg.Format(KFolderListFormat, KFolderUnSelectIndex,&((dirList[i]).iName),i);
        dirNames->AppendL(msg);
    }
    if(aLoadType == ELoadFromSettings){
        SetSelectedPathL(fullPath.Des());
    }
    Model()->SetItemTextArray(dirNames);
    Model()->SetOwnershipType(ELbmOwnsItemArray);
}

#ifdef ENABLE_DRIVE_LIST
void CScrShotsCustomListbox::LoadDriveListL(TBool aLoadCachedValue)
{
    TBuf<128> msg, freeSize, thresholdText;
    const RArray<TDriveExtraInfo>& drvArray = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetDriveListWithInfo(aLoadCachedValue);
    CDesCArray* drvnames = new (ELeave) CDesCArrayFlat(3);
    CScrShotsStrLoader* strLoader = iParentControl->GetAppView()->GetAppUi().GetStrLoader(); 
    TPtr formatStrFreeSizeCell = strLoader->GetStringHBuf(EDriveListFreeSizeCellFormatString)->Des();
    TPtr formatStrThresholdSizeCell = strLoader->GetStringHBuf(EDriveListThresholdSizeCellFormatString)->Des();
    TBuf<2> drv;
    iParentControl->GetAppView()->GetAppSettings()->OutputFolderDrive(drv);
   
    for(TInt i = 0; i < drvArray.Count() ; i++){
        TDriveExtraInfo drvInfo = drvArray[i];
        if((drvInfo.iDrvFreeSize / (KOneGB)) > 0)
            freeSize.Format(formatStrFreeSizeCell, &KGBStr,drvInfo.iDrvFreeSize / (KOneGB));
        else if((drvInfo.iDrvFreeSize / (KOneMB)) > 0)
            freeSize.Format(formatStrFreeSizeCell, &KMBStr,drvInfo.iDrvFreeSize / (KOneMB));
        else if((drvInfo.iDrvFreeSize / (KOneKB)) > 0)
            freeSize.Format(formatStrFreeSizeCell,&KKBStr, drvInfo.iDrvFreeSize / (KOneKB));
        else
            freeSize.Format(formatStrFreeSizeCell, &KBytesStr,drvInfo.iDrvFreeSize );
        
        thresholdText.Format(formatStrThresholdSizeCell,drvInfo.iThresholdPercent);

        //loading the icon based on the drive selected
        if(drvInfo.iDriveChar == 'C' || drvInfo.iDriveChar == 'c'){
            if(drv[0] == drvInfo.iDriveChar)
                msg.Format(KDriveListItemFormat,KCDrvSelectedIconIndex, &freeSize, &thresholdText);
            else
                msg.Format(KDriveListItemFormat,KCDrvUnselectedIconIndex, &freeSize, &thresholdText);
        } else if(drvInfo.iDriveChar == 'E' || drvInfo.iDriveChar == 'e'){
            if(drv[0] == drvInfo.iDriveChar)
                msg.Format(KDriveListItemFormat,KEDrvSelectedIconIndex, &freeSize, &thresholdText);
            else
                msg.Format(KDriveListItemFormat,KEDrvUnselectedIconIndex, &freeSize, &thresholdText);            
        } else if(drvInfo.iDriveChar == 'F' || drvInfo.iDriveChar == 'f'){
            if(drv[0] == drvInfo.iDriveChar)
                msg.Format(KDriveListItemFormat,KFDrvSelectedIconIndex, &freeSize, &thresholdText);
            else
                msg.Format(KDriveListItemFormat,KFDrvUnselectedIconIndex, &freeSize, &thresholdText);            
        }
        drvnames->AppendL(msg);
    }
    
    Model()->SetItemTextArray(drvnames);
    Model()->SetOwnershipType(ELbmOwnsItemArray);

}
#endif

void CScrShotsCustomListbox::GoOneUpFolderL()
{
    RArray<TEntry>& dirList = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GoOneUpAndGetFolderListL(iCurrentPath);
    iCurrentPath.Copy(iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetCurrentRootPath());
    TBuf<256> msg;
    //_LIT(KFolderListFormat,"%d\t%S\t%d");    
    CDesCArray* dirNames = new (ELeave) CDesCArrayFlat(10);
    //dont check the first entry in dirList as its the same directory only.    
    TBool iDirFound = EFalse;
    {
        TInt selectedIconIndex = KFolderUnSelectIndex;
        TStringPoolObj fDir(512);
        for(TInt i = 0; i < dirList.Count() ; i++){
            if(!iDirFound){
                fDir.Des().Copy(iCurrentPath);
                fDir.Des().Append(((dirList[i]).iName));
                fDir.Des().Append(_L("\\"));
                if(!iSelectedPath.Compare(fDir.Des())){
                    selectedIconIndex = KFolderSelectIndex;
                    iDirFound = ETrue;
                }
            }
            msg.Format(KFolderListFormat, selectedIconIndex,&((dirList[i]).iName),i);
            dirNames->AppendL(msg);
            selectedIconIndex = KFolderUnSelectIndex;
        }
    }
    Model()->SetItemTextArray(dirNames);
    Model()->SetOwnershipType(ELbmOwnsItemArray);
    
    if(!this->ItemDrawer())
        this->CreateItemDrawerL();
    
    TInt prevCount = Model()->NumberOfItems();
    if(prevCount > Model()->NumberOfItems())
        HandleItemRemovalL();
    else
        HandleItemAdditionL();

    RecalcSrollBarVisibiltyL();
    DrawNow();
}

TBool CScrShotsCustomListbox::GoToNextFolderL(const TDesC& aPathName)
{
    TInt errCode = KErrNone;
    RArray<TEntry>& dirList = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetFolderList(aPathName, errCode);
    if(errCode != KErrNone){
        TPtr msgPtr = iParentControl->GetAppView()->GetAppUi().GetStrLoader()->GetStringHBuf(EFolderPermissionDeniedMsgString)->Des();
        CEikonEnv::Static()->AlertWin(msgPtr);
        return EFalse;
    }
    else{
        iCurrentPath.Copy(aPathName);
        TBuf<256> msg;
        //_LIT(KFolderListFormat,"%d\t%S\t%d");    
        CDesCArray* dirNames = new (ELeave) CDesCArrayFlat(10);
        //dont check the first entry in dirList as its the same directory only.
        TBool iDirFound = EFalse;
        {
            TInt selectedIconIndex = KFolderUnSelectIndex;
            TStringPoolObj fDir(512);
            for(TInt i = 0; i < dirList.Count() ; i++){
                if(!iDirFound){
                    fDir.Des().Copy(iCurrentPath);
                    fDir.Des().Append(((dirList[i]).iName));
                    fDir.Des().Append(_L("\\"));
                    if(!iSelectedPath.Compare(fDir.Des())){
                        selectedIconIndex = KFolderSelectIndex;
                        iDirFound = ETrue;
                    }
                }
                msg.Format(KFolderListFormat, selectedIconIndex,&((dirList[i]).iName),i);
                dirNames->AppendL(msg);
                selectedIconIndex = KFolderUnSelectIndex;
            }
        }
        TInt prevCount = Model()->NumberOfItems();
        Model()->SetItemTextArray(dirNames);
        Model()->SetOwnershipType(ELbmOwnsItemArray);
                
        if(prevCount > Model()->NumberOfItems())
            HandleItemRemovalL();
        else
            HandleItemAdditionL();
        
        RecalcSrollBarVisibiltyL();
        
        if(!(Model()->NumberOfItems())){
            CEikonEnv::Static()->AlertWin(*(this->View()->EmptyListText()));
        }
        else
            DrawNow();
        
        return ETrue;
    }
}


void CScrShotsCustomListbox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{
    
    TInt index = this->CurrentItemIndex();
    if(iListType == EFileFolderList && aPointerEvent.iType == TPointerEvent::EButton1Up){
        //just make sure that the item go into folder item cell is not selected
        TRect gotoItemRect(this->View()->ItemPos(index),this->View()->ItemSize(index));
        gotoItemRect.iTl.iX +=  (this->View()->ItemSize(index).iWidth * (KDirListSelectedIconCellWidth + KDirListFolderNameCellWidth - 0.10));
        
        TRect selectItemRect(this->View()->ItemPos(index),this->View()->ItemSize(index));
        selectItemRect.SetWidth(this->View()->ItemSize(index).iWidth * (KDirListSelectedIconCellWidth + 0.10));
        if(gotoItemRect.Contains(aPointerEvent.iPosition)){
            iParentControl->GetTouchFeedback().VibrateforButtonPressL(this, aPointerEvent);
            //we need to traverse to the selected folder
            TStringPoolObj colText(1024), msg(1024);
            TPtrC cellText(colText.Des());
            TPtrC text = Model()->ItemText(index);
            TextUtils::ColumnText(cellText,1, &text, TChar('\t'));
            msg.Des().Format(KPathFormatString, &iCurrentPath,&cellText);
            if(GoToNextFolderL(msg.Des())){
                //if go to next folder works only then copy the new path name
                iCurrentPath.Copy(msg.Des());
            }
        }
        else if(selectItemRect.Contains(aPointerEvent.iPosition)){
            iParentControl->GetTouchFeedback().VibrateforButtonPressL(this, aPointerEvent);
            //mark the item selected,unselect the last selected item
            TPtrC cellText;
            TPtrC text = Model()->ItemText(index);
            TextUtils::ColumnText(cellText,1, &text, TChar('\t'));
            //first check if the path can be selected at all!!!
            {
                TStringPoolObj dName(512);
                dName.Des().Copy(iCurrentPath);
                dName.Des().Append(cellText);
                dName.Des().Append(_L("\\"));
                TInt errCode = KErrNone;
//                RArray<TEntry>& dirList = iParentControl->GetAppView()->GetAppUi().GetDeviceWatcher()->GetFolderList(dName.Des(), errCode);
                if(errCode != KErrNone){
                    TPtr msgPtr = iParentControl->GetAppView()->GetAppUi().GetStrLoader()->GetStringHBuf(EFolderPermissionDeniedMsgString)->Des();
                    CEikonEnv::Static()->AlertWin(msgPtr);
                    return;
                }
            }
            if((iCurrentPath.Length() + cellText.Length()) > 256){
                //todo : show msg dialog and bail out
                CEikonEnv::Static()->InfoWinL(_L("Cannot create folder.Path length exceeds platform limits."),KNullDesC);
                return;
            }
            
            if((iCurrentPath.Length() + cellText.Length()) > iSelectedPath.MaxLength())
                iSelectedPath.ReAlloc(iCurrentPath.Length() + cellText.Length());
            iSelectedPath.Copy(iCurrentPath);
            iSelectedPath.Append(cellText);
            iSelectedPath.Append(_L("\\"));

            //update the parent about the new selection
            iParentControl->UpdateSelectionLabelL(iSelectedPath);
            
            //Remove the starting and trailing \\ chars as the settings does not contain them
            {
                TStringPoolObj dir(512);
                dir.Des().Copy(iSelectedPath);
                TParse p;
                p.Set(dir.Des(), NULL, NULL);
                dir.Des().Copy(p.Path());
                if(dir.Des().Find(_L("\\")) == 0)
                    dir.Des().Delete(0,1);
                
                if(dir.Des().LocateReverse(TChar('\\')) == (dir.Des().Length() - 1))
                    dir.Des().Delete((dir.Des().Length() - 1),1);
                //we will also save the folder name in settings now
                iParentControl->GetAppView()->GetAppSettings()->SetOutputFolderNameL(dir.Des());
            }
                        
            //update the listbox to show the updated selection icon
            //get the pointer to the item text array
            //remove the item and set the new icon index
            CDesCArray* dirNames = new (ELeave) CDesCArrayFlat(10);
            //dont check the first entry in dirList as its the same directory only.    
//            TBool iDirFound = EFalse;
            {
                TBuf<256> msg;
                TStringPoolObj fDir(512);
                for(TInt i = 0; i < Model()->NumberOfItems() ; i++){
                    TPtrC cellText = Model()->ItemText(i);
                    TPtrC dirName;
                    TextUtils::ColumnText(dirName,1, &cellText, TChar('\t'));
                    if(i == index)
                        msg.Format(KFolderListFormat, KFolderSelectIndex,&(dirName),i);
                    else
                        msg.Format(KFolderListFormat, KFolderUnSelectIndex,&(dirName),i);
                    dirNames->AppendL(msg);
                }
            }
            Model()->SetItemTextArray(dirNames);
            Model()->SetOwnershipType(ELbmOwnsItemArray);
            this->HandleItemAdditionL();
        }
    } else if(iListType == EAppsList && aPointerEvent.iType == TPointerEvent::EButton1Up){
        //just make sure that the item go into folder item cell is not selected
        TRect appSelectedItemRect(this->View()->ItemPos(index),this->View()->ItemSize(index));
        //reduce the item width to the icon cell
        appSelectedItemRect.SetWidth(appSelectedItemRect.Width() * KAppIconCellWidth);
        if(appSelectedItemRect.Contains(aPointerEvent.iPosition)){
            //provide feedback for the touch on the select app area.
            iParentControl->GetTouchFeedback().VibrateforButtonPressL(this, aPointerEvent);

            TStringPoolObj colText(256);
            TPtrC cellText(colText.Des());
            TPtrC text = Model()->ItemText(index);
            //get the app name
            TextUtils::ColumnText(cellText,KAppCaptionCellIndex, &text, TChar('\t'));
            if(iSelectedAppName.MaxLength() < cellText.Length()){
                iSelectedAppName.ReAlloc(cellText.Length());
            }
            iSelectedAppName.Copy(cellText);
            colText.Des().Zero();
            //get the app name
            TextUtils::ColumnText(cellText,KUid3CellIndex, &text, TChar('\t'));
            colText.Des().Copy(cellText);
            AknTextUtils::StripCharacters(colText.Des(), _L("["));
            AknTextUtils::StripCharacters(colText.Des(), _L("]"));
            TLex val;
            val.Assign(colText.Des());
            TUint32 uid = 0 ;
            val.Val(uid,EHex);
            iSelectedAppUid.iUid = uid;
            //update the parent about the new selection
            iParentControl->UpdateSelectionLabelL(iSelectedAppName);
            //notify the parent to save the app info into settings db
            iParentControl->HandleAppSelectionDoneL(iSelectedAppName, iSelectedAppUid);
            iParentControl->GetAppView()->GetMainControl()->HandleAppSelectionDoneL(iSelectedAppName, iSelectedAppUid);
        }

    }
#ifdef ENABLE_DRIVE_LIST    
    if(iListType == EDrivesList && aPointerEvent.iType == TPointerEvent::EButton1Up){
        
    }
#endif

    CEikFormattedCellListBox::HandlePointerEventL(aPointerEvent);
}

void CScrShotsCustomListbox::SetSelectedApp(const TDesC& aAppName,const TUid& aAppUid)
{
    if(iListType == EAppsList){
        if(iSelectedAppName.MaxLength() < aAppName.Length()){
            iSelectedAppName.ReAlloc(aAppName.Length());
        }
        iSelectedAppName.Copy(aAppName);
        iSelectedAppUid.iUid = aAppUid.iUid;
    }
}

void CScrShotsCustomListbox::RefreshListboxL(TBool aUpdateScrollBars)
{
    TInt prevCount = Model()->NumberOfItems();
    LoadItemTextArrayL(EFalse, ELoadCurrentPath);
    if(prevCount > Model()->NumberOfItems())
        HandleItemRemovalL();
    else
        HandleItemAdditionL();
    if(aUpdateScrollBars){
        RecalcSrollBarVisibiltyL();
    }
    SLOGF(_L("Calling drawnow on listbox..."))
    DrawNow();
}

void CScrShotsCustomListbox::RecalcSrollBarVisibiltyL()
{
    TInt numberOfItems = Model()->NumberOfItems(); 
    TInt itemsInView = View()->NumberOfItemsThatFitInRect(Rect());
    if(!numberOfItems || (itemsInView >= numberOfItems))
        this->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
    else if(numberOfItems)
        this->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOn);
    
}

void CScrShotsCustomListbox::SetCustomIconArray(CArrayPtr<CGulIcon>* aIconArray)
{
    if(iIconArray){
        iIconArray->Reset();
        delete iIconArray;
        iIconArray = NULL;
    }
    iIconArray = aIconArray;
}
