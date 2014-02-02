/*
 * Copyright (C) 2011  Mayur Kankanwadi(eminemence@gmail.com)
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

#ifndef __EXTENDEDTIMERAPPVIEW_h__
#define __EXTENDEDTIMERAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>


// CLASS DECLARATION
class CExtendedTimerAppView : public CCoeControl 
    {
public:
    // New methods

    /**
     * NewL.
     * Two-phased constructor.
     * Create a CExtendedTimerAppView object, which will draw itself to aRect.
     * @param aRect The rectangle this view will be drawn to.
     * @return a pointer to the created instance of CExtendedTimerAppView.
     */
    static CExtendedTimerAppView* NewL(const TRect& aRect);

    /**
     * NewLC.
     * Two-phased constructor.
     * Create a CExtendedTimerAppView object, which will draw itself
     * to aRect.
     * @param aRect Rectangle this view will be drawn to.
     * @return A pointer to the created instance of CExtendedTimerAppView.
     */
    static CExtendedTimerAppView* NewLC(const TRect& aRect);

    /**
     * ~CExtendedTimerAppView
     * Virtual Destructor.
     */
    virtual ~CExtendedTimerAppView();
    
    void IncOneMinCount(){ 	iOneMinCount++;   }
    TInt GetOneMinCount(){ return iOneMinCount; }
    void IncTwoMinCount(){ 	iTwoMinCount++;   }
    TInt GetTwoMinCount(){ return iTwoMinCount; }
    void IncOneHourCount(){ iOneHourCount++;   }
    TInt GetOneHourCount(){ return iOneHourCount; }
    void IncOneSecCount(){ 	iOneSecCount++;   }
    TInt GetOneSecCount(){ return iOneSecCount; }

public:
    // Functions from base classes

    /**
     * From CCoeControl, Draw
     * Draw this CExtendedTimerAppView to the screen.
     * @param aRect the rectangle of this view that needs updating
     */
    void Draw(const TRect& aRect) const;

    /**
     * From CoeControl, SizeChanged.
     * Called by framework when the view size is changed.
     */
    virtual void SizeChanged();

    /**
     * From CoeControl, HandlePointerEventL.
     * Called by framework when a pointer touch event occurs.
     * Note: although this method is compatible with earlier SDKs, 
     * it will not be called in SDKs without Touch support.
     * @param aPointerEvent the information about this event
     */
    virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);    

private:
    // Constructors

    /**
     * ConstructL
     * 2nd phase constructor.
     * Perform the second phase construction of a
     * CExtendedTimerAppView object.
     * @param aRect The rectangle this view will be drawn to.
     */
    void ConstructL(const TRect& aRect);

    /**
     * CExtendedTimerAppView.
     * C++ default constructor.
     */
    CExtendedTimerAppView();
    
    TUint iOneMinCount;
    TUint iTwoMinCount;
    TUint iOneHourCount;
    TUint iOneSecCount;

    };

#endif // __EXTENDEDTIMERAPPVIEW_h__
// End of File
