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

#ifndef __SCRSHOTSNUMBEREDITCONTROL_H__
#define __SCRSHOTSNUMBEREDITCONTROL_H__

#include "ScrShotsVectorImgLoader.h"
#include "TimerX.h"
#include <coecntrl.h>


class CScrShotsVectorImgLoader;
class CScrShotsNumberEditCtrl;

class MDataSaveClient{
public:
    virtual void DoDataSaveL(CScrShotsNumberEditCtrl* aCtrl, TInt aChangedValue) = 0;
    virtual void HandleThresholdReached(TBool aIsUpperRange) = 0;
};

class MActiveScreenNotifier{
public:
    virtual TBool IsCurrentScreen() = 0;
};

class CScrShotsNumberEditCtrl : public CCoeControl,MTimerXCallback 
{
public:
    enum TEditTypes{
        ETwoPane,
        EThreePane
    };
    enum TNumberPlace{
        EOnes,
        ETens,
        EHundreds
    };
public:
    static CScrShotsNumberEditCtrl* NewL(TEditTypes aEditType,CScrShotsVectorImgLoader& aImgLoader, MDataSaveClient* aDataSaveClient, MActiveScreenNotifier* aParentControl);
    ~CScrShotsNumberEditCtrl();
    void Draw(const TRect& aRect) const;
    virtual void SizeChanged();
    virtual void PrepareForFocusLossL();

private:
    CScrShotsNumberEditCtrl(TEditTypes aEditType,CScrShotsVectorImgLoader& aImgLoader, MDataSaveClient* aDataSaveClient, MActiveScreenNotifier* aParentControl);
    void ConstructL();
    void CalculateIconValues();
    
public:
    void SetValue(TInt aValue){
        iValue = aValue;
        CalculateIconValues();
    }
    TInt Value(){
        return iValue;
    }
    void SetRanges(TUint aLowerRange, TUint aUpperRange );
    TUint LowerRange();
    TUint UpperRange();
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    void IncrementRange(TNumberPlace aNumPlace);
    void DecrementRange(TNumberPlace aNumPlace);
    TBool IsValueSaved();
    void SetValueIsSaved(){
        iIsValueSaved = ETrue;
    }
    void UpdateIconSizes();
    void HandleTimerExpireL(TInt aTimerId);
    
private:
    TEditTypes iEditTypes;
    TInt iLowerRange, iUpperRange;
    TRect iUpperRect, iLowerRect;
    TRect hunsRect, tensRect, onesRect;
    TInt iValue;
    CScrShotsVectorImgLoader& iImgLoader;
    TScrShotsIconNames iHunIcon, iTenIcon, iOneIcon;
    TBool iIsValueSaved;
    MDataSaveClient* iDataSaveClient;//refs only dont dlete
    MActiveScreenNotifier* iParentControl;//refs only dont dlete
    CTimerX* iInactivityTimer;
};

#endif //__SCRSHOTSNUMBEREDITCONTROL_H__
