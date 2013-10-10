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

#ifndef __SCRSHOTSCUSTOMLIST_H__
#define __SCRSHOTSCUSTOMLIST_H__

#include <eikfrlbd.h> 
#include <eikenv.h>
#include <aknlists.h>
#include <EIKSBFRM.H>

// This is total experimental effort with no gaurantee that it will succeed
// Also we will start with just two cells with two text items, if it works
// then another cell with graphic will be added

class CScrShotsAppFileList;

enum TListType{
    EAppsList = 0,
    EFileFolderList,
    EDrivesList
};

enum TFileLoadType{
    ELoadFromSettings,
    ELoadCurrentPath
};


class CScrShotsCustomListbox : public CEikFormattedCellListBox
{   
public:
    explicit CScrShotsCustomListbox(CScrShotsAppFileList* aParentControl, TListType aListType = EFileFolderList);
    ~CScrShotsCustomListbox();
    TListType ListType() const{
        return iListType;
    }
    void LoadItemTextArrayL(TBool aLoadCachedValue = ETrue, TFileLoadType aLoadType = ELoadFromSettings);
    void UpdateItemDrawerL();
#if 0    
    CFbsBitmap* NormalListItemBackground(){
        return iNormalListItemBackground;
    }
    CFbsBitmap* SelectedItemBackground(){
        return iSelectedItemBackground;
    }
#endif
    void GoOneUpFolderL();
    TBool GoToNextFolderL(const TDesC& aPathName);
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    CScrShotsAppFileList* GetParentControl(){
        return iParentControl;
    }
    void RefreshListboxL(TBool aUpdateScrollBars = ETrue);
    void SetCustomIconArray(CArrayPtr<CGulIcon>* aIconArray);
    CArrayPtr<CGulIcon>* CustomIconArray(){
        return iIconArray;
    }
    void SetSelectedPathL(const TDesC& aFolderPath);
    void GetSelectedAppNameL(TDes& aAppName);
    TUint32 GetSelectedAppUidL(){
        return iSelectedAppUid.iUid;
    }
    void SetSelectedApp(const TDesC& aAppName,const TUid& aAppUid);
    RBuf& CurrentPath(){
        return iCurrentPath;
    }
    void RecalcSrollBarVisibiltyL();
private:
    void CreateItemDrawerL(void);
    CFormattedCellListBoxData* CreateFileFolderListItemDataLC();
    CFormattedCellListBoxData* CreateAppsListItemDataLC();
    void LoadAppsListL(TBool aLoadCachedValue = ETrue);
    void LoadFileFoldersListL(TBool aLoadCachedValue = ETrue, TFileLoadType aLoadType = ELoadFromSettings);
    void UpdateAppsListItemDataL();
    void UpdateFileFolderListItemDataL();
    
#ifdef ENABLE_DRIVE_LIST       
    CFormattedCellListBoxData* CreateDrivesListItemDataLC();
    void LoadDriveListL(TBool aLoadCachedValue = ETrue);
    void UpdateDrivesListItemDataL();
#endif
    
private:
    TListType iListType;
    CScrShotsAppFileList* iParentControl;//don't own. don't delete.
    CFont* iCellFont15, *iCellFont18;
    CFont* iFullPathCellFont;
#if 0    
    CFbsBitmap* iNormalListItemBackground;
    CFbsBitmap* iSelectedItemBackground;
#endif
    RBuf iCurrentPath;
    RBuf iSelectedPath;
    CArrayPtr<CGulIcon>* iIconArray;
    RBuf    iSelectedAppName;
    TUid    iSelectedAppUid;
};

class CScrShotsCustomListboxDrawer : public CFormattedCellListBoxItemDrawer 
{
public:
    CScrShotsCustomListboxDrawer(MTextListBoxModel *aTextListBoxModel, 
                    const CFont *aFont, 
                    CFormattedCellListBoxData* aFormattedCellData,
                    CScrShotsCustomListbox* aListBox);
#if 0    
    void DrawEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                        TBool aViewIsDimmed) const;
#endif
    
private:
    void DrawItemText (TInt aItemIndex, const TRect &aItemTextRect,
                TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aItemIsSelected) const;
    
    void DrawFileFolderListItemText(TInt aItemIndex, const TRect &aItemTextRect,
            TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aItemIsSelected) const;
    void DrawAppsListItemText(TInt aItemIndex, const TRect &aItemTextRect,
            TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aItemIsSelected) const; 
    void DrawDrivesListItemText(TInt aItemIndex, const TRect &aItemTextRect,
            TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aItemIsSelected) const; 

    void DrawItemMark(TBool aItemIsSelected, TBool aViewIsDimmed, const TPoint& aMarkPos) const;
#if 0
    void DrawFileFolderListEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                        TBool aViewIsDimmed) const;
    void DrawAppsListEmptyItem(TInt aItemIndex, TPoint aItemRectPos, 
                                        TBool aViewIsDimmed) const;
#endif
    
private:
    CFormattedCellListBoxData*  iItemData;
    MTextListBoxModel*         iTextListBoxModel;
    //check if this is needed
    CScrShotsCustomListbox*     iListBox;//dont own
};

#endif //__SCRSHOTSCUSTOMLIST_H__
