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
#include "S60UptimeAppUi.h"
#include "S60UptimeDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CS60UptimeDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CS60UptimeDocument* CS60UptimeDocument::NewL(CEikApplication& aApp)
	{
	CS60UptimeDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CS60UptimeDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CS60UptimeDocument* CS60UptimeDocument::NewLC(CEikApplication& aApp)
	{
	CS60UptimeDocument* self = new (ELeave) CS60UptimeDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CS60UptimeDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CS60UptimeDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CS60UptimeDocument::CS60UptimeDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CS60UptimeDocument::CS60UptimeDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CS60UptimeDocument::~CS60UptimeDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CS60UptimeDocument::~CS60UptimeDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CS60UptimeDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CS60UptimeDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CS60UptimeAppUi;
	}

// End of File
