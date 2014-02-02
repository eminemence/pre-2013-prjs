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

#ifndef __EXTENDEDTIMERDOCUMENT_h__
#define __EXTENDEDTIMERDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CExtendedTimerAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CExtendedTimerDocument application class.
 * An instance of class CExtendedTimerDocument is the Document part of the
 * AVKON application framework for the ExtendedTimer example application.
 */
class CExtendedTimerDocument : public CAknDocument
    {
public:
    // Constructors and destructor

    /**
     * NewL.
     * Two-phased constructor.
     * Construct a CExtendedTimerDocument for the AVKON application aApp
     * using two phase construction, and return a pointer
     * to the created object.
     * @param aApp Application creating this document.
     * @return A pointer to the created instance of CExtendedTimerDocument.
     */
    static CExtendedTimerDocument* NewL(CEikApplication& aApp);

    /**
     * NewLC.
     * Two-phased constructor.
     * Construct a CExtendedTimerDocument for the AVKON application aApp
     * using two phase construction, and return a pointer
     * to the created object.
     * @param aApp Application creating this document.
     * @return A pointer to the created instance of CExtendedTimerDocument.
     */
    static CExtendedTimerDocument* NewLC(CEikApplication& aApp);

    /**
     * ~CExtendedTimerDocument
     * Virtual Destructor.
     */
    virtual ~CExtendedTimerDocument();

public:
    // Functions from base classes

    /**
     * CreateAppUiL
     * From CEikDocument, CreateAppUiL.
     * Create a CExtendedTimerAppUi object and return a pointer to it.
     * The object returned is owned by the Uikon framework.
     * @return Pointer to created instance of AppUi.
     */
    CEikAppUi* CreateAppUiL();

private:
    // Constructors

    /**
     * ConstructL
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * CExtendedTimerDocument.
     * C++ default constructor.
     * @param aApp Application creating this document.
     */
    CExtendedTimerDocument(CEikApplication& aApp);

    };

#endif // __EXTENDEDTIMERDOCUMENT_h__
// End of File
