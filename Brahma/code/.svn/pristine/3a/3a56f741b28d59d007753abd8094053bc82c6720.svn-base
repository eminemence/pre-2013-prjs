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

#ifndef __SCRSHOTSDRIVEGRID_H__
#define __SCRSHOTSDRIVEGRID_H__

#include <coecntrl.h>
#include "ScrShotsNumberEditControl.h"


class CScrShotsAppFileList;
class CScrShotsTextLabel;

// drive rects layout
//  1 | 2
// -------
//  3 | 4
//

//cell layout
//icon  w-40 h-50
//total w-60 h-16
//free w-60 h-16
//type w-60 h-16
//nedit w-60 h-50
//text w-40 h-50
// --------------------------
// |         |  total size  |
// | icon    |--------------|
// |         |  free size   |
// |         |--------------|
// |         |  drive type  |
// |-------- |--------------|
// |             |          |
// |  number     | text     |
// |  edit       | showing  |
// |  ctrl       | threshold|
// |             | level    |
// --------------------------



class CScrShotsDriveGridCellModel : public CBase
{
public:
        static CScrShotsDriveGridCellModel* NewL(){
        CScrShotsDriveGridCellModel* self = new (ELeave) CScrShotsDriveGridCellModel();
            CleanupStack::PushL(self);
            self->ConstructL();
            CleanupStack::Pop();
            return self;
        }
        ~CScrShotsDriveGridCellModel(){
            iTotalSizeText.Close();
            iFreeSizeText.Close();
            iDriveTypeText.Close();
            iThresholdInfoText.Close();
        }
private:
        CScrShotsDriveGridCellModel(){
        
        }
        void ConstructL(){
            iTotalSizeText.CreateL(64);
            iFreeSizeText.CreateL(64);
            iDriveTypeText.CreateL(64);
            iThresholdInfoText.CreateL(64);
        }
        
        
private:
        TRect iIconRect, iTotalSizeRect, iFreeSizeRect, iDriveTypeRect, iThresholdEditRect, iThresholdInfoTextRect;
        RBuf  iTotalSizeText, iFreeSizeText, iDriveTypeText, iThresholdInfoText;
        CFbsBitmap* iSelectedDriveIcon, *iUnselectedDriveIcon;//ptrs only dont delete
        TBuf<2> iDrvChar;
        TBool   iCurrentlySelected;
        TRect iGridRect;
        
        
        friend class CScrShotsDriveGrid; 
};

class CScrShotsDriveGrid : public CCoeControl,MDataSaveClient
{
public:
    static CScrShotsDriveGrid* NewL(const TRect& aRect, CScrShotsAppFileList* aParentControl);
    ~CScrShotsDriveGrid();
    void Draw(const TRect& aRect) const;
    virtual void SizeChanged();
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    CCoeControl* ComponentControl(TInt aIndex) const;
    TInt CountComponentControls() const;
    void DoDataSaveL(CScrShotsNumberEditCtrl* aCtrl, TInt aChangedValue);
    void HandleThresholdReached(TBool aIsUpperRange);
    void UpdateIconSizeL();
    void RefreshDriveDataL();
    

private:
    CScrShotsDriveGrid(CScrShotsAppFileList* aParentControl);
    void ConstructL(const TRect& aRect);
    void LoadDriveGridsL();
    void ComputeGridRects();
//    void SetFreeSizeText(TDes& aFreeSizeText, TInt64 aDriveFreeSize);
    void SetDriveTypeText(TDes& aFreeSizeText, TMediaType aDriveType, TBool aIsDriveRemoveable);
    void SetTotalSizeText(TDes& aTotalSizeText, TInt64 aDriveTotalSize);
    void SetThresholdInfoText(TDes& aThresholdInfoText);
    CScrShotsTextLabel* CreateAndSetLabelL(const TDesC& aLabelText, TRgb aLabelColor,TRect& aLabelRect);
    
private:
    TRect iDrive1Rect,iDrive2Rect,iDrive3Rect,iDrive4Rect;
    unsigned int iSelectedGrid : 2;
    RPointerArray<CScrShotsDriveGridCellModel> iGridModel;
    CScrShotsAppFileList* iParentControl;//do not delete
    RPointerArray<CScrShotsNumberEditCtrl> iThresholdEditCtrlArray;
    RPointerArray<CScrShotsTextLabel> iLabelArray;
};

#endif //__SCRSHOTSDRIVEGRID_H__
