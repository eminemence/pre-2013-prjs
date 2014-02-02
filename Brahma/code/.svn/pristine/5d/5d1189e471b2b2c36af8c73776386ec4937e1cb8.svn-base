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

#include "ScrShotsSliderControl.h"

#define KMaxSteps 50

CScrShotsSliderControl* CScrShotsSliderControl::NewL()
{
    CScrShotsSliderControl* self = new (ELeave) CScrShotsSliderControl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CScrShotsSliderControl::~CScrShotsSliderControl()
{

}

void CScrShotsSliderControl::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetBrushColor(KRgbRed);
    gc.DrawRoundRect(aRect, TSize(2,2));
    
}

void CScrShotsSliderControl::SizeChanged()
{

}

CScrShotsSliderControl::CScrShotsSliderControl()
{

}

void CScrShotsSliderControl::ConstructL()
{

}

void CScrShotsSliderControl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{

    switch(aPointerEvent.iType){
//        case EButton1Up:
//            break;
//        case EButton1Down:
//            break;
//        case EDrag:
//            break;
    }
}

void CScrShotsSliderControl::SetRangeL(TInt aLowerRange, TInt aUpperRange)
{
    iLowerRange = aLowerRange;
    iUpperRange = aUpperRange;
    if(((iUpperRange - iLowerRange)/KMaxSteps) > 0){
        iIncrement = (iUpperRange - iLowerRange)/KMaxSteps; 
    }
    else{
        iIncrement = 0;
        TInt steps = KMaxSteps;
        TInt rangeValue = iUpperRange - iLowerRange;
        while(iIncrement <= 0 && steps > 0){
            steps -= 2;
            iIncrement = rangeValue / steps;
        }
    }
}

void CScrShotsSliderControl::SetIncrementL(TInt aIncrements)
{

}

