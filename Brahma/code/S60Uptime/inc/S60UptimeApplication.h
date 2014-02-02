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


#ifndef __S60UPTIMEAPPLICATION_H__
#define __S60UPTIMEAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "S60Uptime.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidS60UptimeApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CS60UptimeApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CS60UptimeApplication is the application part of the
 * AVKON application framework for the S60Uptime example application.
 */
class CS60UptimeApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidS60UptimeApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CS60UptimeDocument document object. The returned
	 * pointer in not owned by the CS60UptimeApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __S60UPTIMEAPPLICATION_H__
// End of File
