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

#ifndef __SCRSHOTSSLIDERCONTROL_H__
#define __SCRSHOTSSLIDERCONTROL_H__

#include <coecntrl.h>

class CScrShotsSliderControl : public CCoeControl
{
    public:
        static CScrShotsSliderControl* NewL();
        ~CScrShotsSliderControl();
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);
        void SetRangeL(TInt aLowerRange, TInt aUpperRange);
        void SetIncrementL(TInt aIncrements);
    
    private:
        CScrShotsSliderControl();
        void ConstructL();
    private:
        TInt iLowerRange, iUpperRange;
        TInt iIncrement;
};

#endif //__SCRSHOTSSLIDERCONTROL_H__
