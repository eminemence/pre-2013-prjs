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
#include "ScrShotsAutomatic.hrh"
#include "ScrShotsAutomaticDocument.h"
#include "ScrShotsAutomaticApplication.h"

#include "EventLogger.h"

// ============================ MEMBER FUNCTIONS ===============================

void ScrShotsExceptionHandler(TExcType /*aExcType*/)
{
#if 0    
    RFs fs;
    RFile file;
    TBuf<256> fName;
    fName.Format(_L("C:\\Data\\Excpt%d.txt"),User::TickCount());
    if(fs.Connect() == KErrNone){
        if(file.Create(fs,fName,EFileWrite) == KErrNone){
            TBuf8<256> msg;
            msg.Format(_L8("Exception type : %d"),aExcType);
            file.Write(msg);
            file.Close();        
        }
        fs.Close();
    }
#endif
    // we dont want to handle any other exceptions we are done
    User::Exit(0);
}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CScrShotsAutomaticApplication::CreateDocumentL()
{
    SLOGF(_L(" CScrShotsAutomaticApplication::CreateDocumentL() done"))
    User::SetExceptionHandler(&ScrShotsExceptionHandler, 0xffffffff);
	// Create an ScrShotsAutomatic document, and return a pointer to it
	return CScrShotsAutomaticDocument::NewL(*this);
}

// -----------------------------------------------------------------------------
// CScrShotsAutomaticApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CScrShotsAutomaticApplication::AppDllUid() const
	{
	// Return the UID for the ScrShotsAutomatic application
	return KUidScrShotsAutomaticApp;
	}

// End of File
