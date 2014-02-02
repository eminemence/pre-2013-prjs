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
#include "ExtendedTimer.hrh"
#include "ExtendedTimerDocument.h"
#include "ExtendedTimerApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CExtendedTimerApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CExtendedTimerApplication::CreateDocumentL()
    {
    // Create an ExtendedTimer document, and return a pointer to it
    return CExtendedTimerDocument::NewL(*this);
    }

// -----------------------------------------------------------------------------
// CExtendedTimerApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CExtendedTimerApplication::AppDllUid() const
    {
    // Return the UID for the ExtendedTimer application
    return KUidExtendedTimerApp;
    }

// End of File
