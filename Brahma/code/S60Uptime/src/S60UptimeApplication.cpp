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
#include "S60Uptime.hrh"
#include "S60UptimeDocument.h"
#include "S60UptimeApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CS60UptimeApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CS60UptimeApplication::CreateDocumentL()
	{
	// Create an S60Uptime document, and return a pointer to it
	return CS60UptimeDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CS60UptimeApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CS60UptimeApplication::AppDllUid() const
	{
	// Return the UID for the S60Uptime application
	return KUidS60UptimeApp;
	}

// End of File
