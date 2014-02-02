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

#ifndef __EXTENDEDTIMERAPPUI_h__
#define __EXTENDEDTIMERAPPUI_h__

// INCLUDES
#include <aknappui.h>
#include "TimerX.h"
#include "ExtendedTimerAppView.h"

// CLASS DECLARATION
/**
 * CExtendedTimerAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CExtendedTimerAppUi : public CAknAppUi, public MTimerXCallback
    {
public:
    // Constructors and destructor

    /**
     * ConstructL.
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * CExtendedTimerAppUi.
     * C++ default constructor. This needs to be public due to
     * the way the framework constructs the AppUi
     */
    CExtendedTimerAppUi();

    /**
     * ~CExtendedTimerAppUi.
     * Virtual Destructor.
     */
    virtual ~CExtendedTimerAppUi();
    
public:
    virtual void HandleTimerExpireL(TInt aTimerId);
    

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
//    CArrayFix<TCoeHelpContext>* HelpContextL() const;

private:
    enum TTimerType{
    	EOneMinTimer,
    	ETwoMinTimer,
    	EOneHourTimer,
    	EOneSecTimer
    };
    
    struct TTimerMap{
    	TTimerType type;
    	TUint32 iTimerId;
    };
    CExtendedTimerAppView* 	iAppView;
    RPointerArray<CTimerX>			iTimerArray;
    RArray<TTimerMap>				iTimerMap;

    };

#endif // __EXTENDEDTIMERAPPUI_h__
// End of File
