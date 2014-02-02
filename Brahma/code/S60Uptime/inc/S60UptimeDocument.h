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


#ifndef __S60UPTIMEDOCUMENT_h__
#define __S60UPTIMEDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CS60UptimeAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CS60UptimeDocument application class.
 * An instance of class CS60UptimeDocument is the Document part of the
 * AVKON application framework for the S60Uptime example application.
 */
class CS60UptimeDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CS60UptimeDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CS60UptimeDocument.
	 */
	static CS60UptimeDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CS60UptimeDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CS60UptimeDocument.
	 */
	static CS60UptimeDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CS60UptimeDocument
	 * Virtual Destructor.
	 */
	virtual ~CS60UptimeDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CS60UptimeAppUi object and return a pointer to it.
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
	 * CS60UptimeDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CS60UptimeDocument(CEikApplication& aApp);

	};

#endif // __S60UPTIMEDOCUMENT_h__
// End of File
