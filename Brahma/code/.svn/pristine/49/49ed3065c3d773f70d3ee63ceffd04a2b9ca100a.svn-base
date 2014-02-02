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

#include "ScrShotsNumberEditControl.h"
#include "ScrShotsGenUtils.h"


CScrShotsNumberEditCtrl* CScrShotsNumberEditCtrl::NewL(TEditTypes aEditType,CScrShotsVectorImgLoader& aImgLoader, MDataSaveClient* aDataSaveClient, MActiveScreenNotifier* aParentControl)
{
    CScrShotsNumberEditCtrl* self = new (ELeave) CScrShotsNumberEditCtrl(aEditType, aImgLoader, aDataSaveClient, aParentControl);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;    
}

CScrShotsNumberEditCtrl::~CScrShotsNumberEditCtrl()
{
    delete iInactivityTimer;
    iInactivityTimer = NULL;
}

void CScrShotsNumberEditCtrl::Draw(const TRect& /*aRect*/) const
{
    CWindowGc& gc = SystemGc();
    gc.SetPenColor(ScrShotsThemeUtils::MainScreenLineColor());
    //gc.DrawRoundRect(aRect, TSize(2,2));
//    gc.DrawRoundRect(onesRect, TSize(2,2));
//    gc.DrawRoundRect(tensRect, TSize(2,2));
//    if(iEditTypes == EThreePane){
//        gc.DrawRoundRect(hunsRect, TSize(2,2));
//    }
    //draw the numbers now
    gc.BitBlt(onesRect.iTl, iImgLoader.GetBitmap(iOneIcon));
    gc.BitBlt(tensRect.iTl, iImgLoader.GetBitmap(iTenIcon));
    if(iEditTypes == EThreePane){
        gc.BitBlt(hunsRect.iTl, iImgLoader.GetBitmap(iHunIcon));
    }
}

void CScrShotsNumberEditCtrl::SizeChanged()
{
#define KThreePaneWidth 0.33
#define KTwoPaneWidth 0.50
    
    iUpperRect =  iLowerRect = Rect();
    iUpperRect.SetHeight(Rect().Height() * 0.50);
    iLowerRect.iTl.iY = iUpperRect.iBr.iY; 
    iLowerRect.SetHeight(Rect().Height() * 0.50);
    iUpperRect.SetWidth(Rect().Width());
    iLowerRect.SetWidth(Rect().Width());
    if(iEditTypes == EThreePane){
        hunsRect = tensRect = onesRect = Rect();
        hunsRect.SetWidth(Rect().Width() * KThreePaneWidth);
        tensRect.iTl.iX = hunsRect.iBr.iX;
        tensRect.SetWidth(hunsRect.Width());
        onesRect.iTl.iX = tensRect.iBr.iX;
        onesRect.SetWidth(hunsRect.Width());
    }else if(iEditTypes == ETwoPane){
        tensRect = onesRect = Rect();
        tensRect.SetWidth(Rect().Width() * KTwoPaneWidth);
        onesRect.iTl.iX = tensRect.iBr.iX;
        onesRect.SetWidth(tensRect.Width());
    }
    UpdateIconSizes(); 
    //DrawNow();
}

void CScrShotsNumberEditCtrl::UpdateIconSizes()
{
    if(iParentControl->IsCurrentScreen() && onesRect.Size().iHeight && onesRect.Size().iWidth){
        iImgLoader.SetIconSize(ENumber0 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber1 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber2 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber3 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber4 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber5 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber6 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber7 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber8 ,onesRect.Size());
        iImgLoader.SetIconSize(ENumber9 ,onesRect.Size());
    }
//    DrawNow();
}

CScrShotsNumberEditCtrl::CScrShotsNumberEditCtrl(TEditTypes aEditType,CScrShotsVectorImgLoader& aImgLoader, MDataSaveClient* aDataSaveClient,MActiveScreenNotifier* aParentControl):
        iEditTypes(aEditType)
,iImgLoader(aImgLoader)
,iDataSaveClient(aDataSaveClient)
,iParentControl(aParentControl)
{

}

void CScrShotsNumberEditCtrl::ConstructL()
{
    iInactivityTimer = CTimerX::NewL(this);
}

void CScrShotsNumberEditCtrl::SetRanges(TUint aLowerRange, TUint aUpperRange)
{
    //iValue = aLowerRange;
    iLowerRange = aLowerRange;
    iUpperRange = aUpperRange;
    CalculateIconValues();
    iIsValueSaved = EFalse;//flag that the value has changed and has to be saved
}

TUint CScrShotsNumberEditCtrl::LowerRange()
{
    return iLowerRange;
}

TUint CScrShotsNumberEditCtrl::UpperRange()
{
    return iUpperRange;
}

void CScrShotsNumberEditCtrl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{ 
    if(aPointerEvent.iType == TPointerEvent::EButton1Up){
        TPoint point = aPointerEvent.iPosition;
        if(hunsRect.Contains(point) && iEditTypes == EThreePane){
            if(iUpperRect.Contains(point)){
                IncrementRange(EHundreds);
            }else if(iLowerRect.Contains(point)){
                DecrementRange(EHundreds);
            }
        }else if(tensRect.Contains(point)){
            if(iUpperRect.Contains(point)){
                IncrementRange(ETens);
            }else if(iLowerRect.Contains(point)){
                DecrementRange(ETens);   
            }
        }else if(onesRect.Contains(point)){
            if(iUpperRect.Contains(point)){
                IncrementRange(EOnes);
            }else if(iLowerRect.Contains(point)){
                DecrementRange(EOnes);
            }        
        }
    }
    if(!iInactivityTimer->IsActive())
        iInactivityTimer->StartSecondTimerL(1,EFalse);
}

void CScrShotsNumberEditCtrl::IncrementRange(TNumberPlace aNumPlace)
{
    TInt num = 0;
    switch(aNumPlace){
        case EOnes:            num = 1;            break;
        case ETens:            num = 10;            break;
        case EHundreds:        num = 100;            break;
    }
    if((iValue + num) <= iUpperRange){
        iValue += num;
        iIsValueSaved = EFalse;//flag that the value has changed and has to be saved
        CalculateIconValues();
    }
    else{
        iDataSaveClient->HandleThresholdReached(ETrue);
    }
        
}

void CScrShotsNumberEditCtrl::DecrementRange(TNumberPlace aNumPlace)
{
    TInt num = 0;
    switch(aNumPlace){
        case EOnes:            num = 1;            break;
        case ETens:            num = 10;            break;
        case EHundreds:        num = 100;            break;
    }
    if((iValue - num) >= iLowerRange){
        iValue -= num;
        iIsValueSaved = EFalse;//flag that the value has changed and has to be saved
        CalculateIconValues();
    }
    else{
        iDataSaveClient->HandleThresholdReached(EFalse);
    }
    
}

void CScrShotsNumberEditCtrl::CalculateIconValues()
{
    iHunIcon = iTenIcon = iOneIcon = ENumber0;
    TInt res = 0;
    TInt tmpValue = iValue;
    TScrShotsIconNames* currentIcon = NULL;
    int i = 0;
    while(i < 3){
        if(i == 0){
            res = tmpValue / 100;
            currentIcon = &iHunIcon;
        }else if(i == 1){
            res = tmpValue / 10;
            currentIcon = &iTenIcon;
        }else if( i == 2){
            res = tmpValue;
            currentIcon = &iOneIcon;
        }

        *currentIcon =   (TScrShotsIconNames)res;

        if(i == 0 && res)
            tmpValue -= (res * 100);
        else if(i == 1 && res)
            tmpValue -= (res * 10);
        else if(i == 2 && res)
            tmpValue -= (res);
        
        i++;
    }
}

TBool CScrShotsNumberEditCtrl::IsValueSaved()
{
    return iIsValueSaved;
}

void CScrShotsNumberEditCtrl::PrepareForFocusLossL()
{
    iDataSaveClient->DoDataSaveL(this, iValue);
}

void CScrShotsNumberEditCtrl::HandleTimerExpireL(TInt aTimerId)
{
    iDataSaveClient->DoDataSaveL(this, iValue);
}
