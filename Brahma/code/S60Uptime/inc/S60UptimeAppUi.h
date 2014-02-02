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


#ifndef __S60UPTIMEAPPUI_h__
#define __S60UPTIMEAPPUI_h__

// INCLUDES
#include <aknappui.h>

// FORWARD DECLARATIONS
class CS60UptimeAppView;

// CLASS DECLARATION
/**
 * CS60UptimeAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CS60UptimeAppUi : public CAknAppUi
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CS60UptimeAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CS60UptimeAppUi();

	/**
	 * ~CS60UptimeAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CS60UptimeAppUi();

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	//CArrayFix<TCoeHelpContext>* HelpContextL() const;
	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

private:
	// Data

	/**
	 * The application view
	 * Owned by CS60UptimeAppUi
	 */
	CS60UptimeAppView* iAppView;

	};

#endif // __S60UPTIMEAPPUI_h__
// End of File
