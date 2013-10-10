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


// INCLUDE FILES
#include "ScrShotsAutomaticAppUi.h"
#include "ScrShotsAutomaticDocument.h"
#include "EventLogger.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CScrShotsAutomaticDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticDocument* CScrShotsAutomaticDocument::NewL(
		CEikApplication& aApp)
	{
	CScrShotsAutomaticDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticDocument* CScrShotsAutomaticDocument::NewLC(
		CEikApplication& aApp)
	{
	CScrShotsAutomaticDocument* self = new (ELeave) CScrShotsAutomaticDocument(
			aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CScrShotsAutomaticDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticDocument::CScrShotsAutomaticDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CScrShotsAutomaticDocument::CScrShotsAutomaticDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CScrShotsAutomaticDocument::~CScrShotsAutomaticDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CScrShotsAutomaticDocument::~CScrShotsAutomaticDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CScrShotsAutomaticDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CScrShotsAutomaticDocument::CreateAppUiL()
	{
    SLOGF(_L(" CScrShotsAutomaticDocument::CreateAppUiL() done"))
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CScrShotsAutomaticAppUi;
	}

// End of File
