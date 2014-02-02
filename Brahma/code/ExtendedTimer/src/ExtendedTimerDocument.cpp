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
#include "ExtendedTimerAppUi.h"
#include "ExtendedTimerDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CExtendedTimerDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerDocument* CExtendedTimerDocument::NewL(CEikApplication& aApp)
    {
    CExtendedTimerDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CExtendedTimerDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerDocument* CExtendedTimerDocument::NewLC(CEikApplication& aApp)
    {
    CExtendedTimerDocument* self = new (ELeave) CExtendedTimerDocument(aApp);

    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CExtendedTimerDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CExtendedTimerDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CExtendedTimerDocument::CExtendedTimerDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CExtendedTimerDocument::CExtendedTimerDocument(CEikApplication& aApp) :
    CAknDocument(aApp)
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CExtendedTimerDocument::~CExtendedTimerDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CExtendedTimerDocument::~CExtendedTimerDocument()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CExtendedTimerDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CExtendedTimerDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it;
    // the framework takes ownership of this object
    return new (ELeave) CExtendedTimerAppUi;
    }

// End of File
