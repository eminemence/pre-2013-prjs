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

// INCLUDE FILES
#include <coemain.h>
#include "ExtendedTimerAppView.h"
#include <eikenv.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppView* CExtendedTimerAppView::NewL(const TRect& aRect)
    {
    CExtendedTimerAppView* self = CExtendedTimerAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppView* CExtendedTimerAppView::NewLC(const TRect& aRect)
    {
    CExtendedTimerAppView* self = new (ELeave) CExtendedTimerAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppView::ConstructL(const TRect& aRect)
    {
    
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::CExtendedTimerAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppView::CExtendedTimerAppView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::~CExtendedTimerAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppView::~CExtendedTimerAppView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppView::Draw(const TRect& /*aRect*/) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect(Rect());

    // Clears the screen
    gc.Clear(drawRect);
    
    gc.UseFont(CEikonEnv::Static()->TitleFont());
    TBuf<200> msg;
    msg.Format(_L("No of OneMins : %d"),iOneMinCount);
    TBuf<200> msg1;
    msg1.Format(_L("No of TwoMins : %d"),iTwoMinCount);    
    TBuf<200> msg2;
    msg2.Format(_L("No of OneHour : %d"),iOneHourCount);    
    TBuf<200> msg3;
    msg3.Format(_L("No of OneSec : %d"),iOneSecCount);    
    int ty = 50;
    gc.DrawText(msg,TPoint(50,ty));
    gc.DrawText(msg1,TPoint(50,ty + CEikonEnv::Static()->TitleFont()->HeightInPixels() * 1));
    gc.DrawText(msg2,TPoint(50,ty + CEikonEnv::Static()->TitleFont()->HeightInPixels() * 2));
    gc.DrawText(msg3,TPoint(50,ty + CEikonEnv::Static()->TitleFont()->HeightInPixels() * 3));
    gc.DiscardFont();

    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppView::SizeChanged()
    {
    DrawNow();
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppView::HandlePointerEventL(
        const TPointerEvent& aPointerEvent)
    {

    // Call base class HandlePointerEventL()
    CCoeControl::HandlePointerEventL(aPointerEvent);
    }

// End of File
