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


#ifndef __SCRSHOTSAUTOMATICDOCUMENT_H__
#define __SCRSHOTSAUTOMATICDOCUMENT_H__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CScrShotsAutomaticAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CScrShotsAutomaticDocument application class.
 * An instance of class CScrShotsAutomaticDocument is the Document part of the
 * AVKON application framework for the ScrShotsAutomatic example application.
 */
class CScrShotsAutomaticDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CScrShotsAutomaticDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CScrShotsAutomaticDocument.
	 */
	static CScrShotsAutomaticDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CScrShotsAutomaticDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CScrShotsAutomaticDocument.
	 */
	static CScrShotsAutomaticDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CScrShotsAutomaticDocument
	 * Virtual Destructor.
	 */
	virtual ~CScrShotsAutomaticDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CScrShotsAutomaticAppUi object and return a pointer to it.
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
	 * CScrShotsAutomaticDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CScrShotsAutomaticDocument(CEikApplication& aApp);

	};

#endif // __SCRSHOTSAUTOMATICDOCUMENT_H__
// End of File
