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

#ifndef __SCRSHOTSAUTOMATICAPPLICATION_H__
#define __SCRSHOTSAUTOMATICAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "ScrShotsAutomatic.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidScrShotsAutomaticApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CScrShotsAutomaticApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CScrShotsAutomaticApplication is the application part of the
 * AVKON application framework for the ScrShotsAutomatic example application.
 */
class CScrShotsAutomaticApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidScrShotsAutomaticApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CScrShotsAutomaticDocument document object. The returned
	 * pointer in not owned by the CScrShotsAutomaticApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __SCRSHOTSAUTOMATICAPPLICATION_H__
// End of File
