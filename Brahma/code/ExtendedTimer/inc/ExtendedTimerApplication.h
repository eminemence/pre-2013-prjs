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

#ifndef __EXTENDEDTIMERAPPLICATION_H__
#define __EXTENDEDTIMERAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "ExtendedTimer.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidExtendedTimerApp =
    {
    _UID3
    };

// CLASS DECLARATION

/**
 * CExtendedTimerApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CExtendedTimerApplication is the application part of the
 * AVKON application framework for the ExtendedTimer example application.
 */
class CExtendedTimerApplication : public CAknApplication
    {
public:
    // Functions from base classes

    /**
     * From CApaApplication, AppDllUid.
     * @return Application's UID (KUidExtendedTimerApp).
     */
    TUid AppDllUid() const;

protected:
    // Functions from base classes

    /**
     * From CApaApplication, CreateDocumentL.
     * Creates CExtendedTimerDocument document object. The returned
     * pointer in not owned by the CExtendedTimerApplication object.
     * @return A pointer to the created document object.
     */
    CApaDocument* CreateDocumentL();
    };

#endif // __EXTENDEDTIMERAPPLICATION_H__
// End of File
