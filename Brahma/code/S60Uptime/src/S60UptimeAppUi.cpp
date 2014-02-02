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
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>


#include <s60uptime_0xA0011832.rsg>

#include "S60Uptime.hrh"
#include "S60Uptime.pan"
#include "S60UptimeApplication.h"
#include "S60UptimeAppUi.h"
#include "S60UptimeAppView.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CS60UptimeAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	// Create view object
	iAppView = CS60UptimeAppView::NewL(ClientRect());
	
	AddToStackL(iAppView);

	}
// -----------------------------------------------------------------------------
// CS60UptimeAppUi::CS60UptimeAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CS60UptimeAppUi::CS60UptimeAppUi()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CS60UptimeAppUi::~CS60UptimeAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CS60UptimeAppUi::~CS60UptimeAppUi()
	{
	RemoveFromStack(iAppView);
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}

	}


// -----------------------------------------------------------------------------
// CS60UptimeAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;
		case ECommandUpdatePerSecond:
			iAppView->SetSecondUpdate();
			break;
		case ECommandUpdatePerMinute:
			iAppView->SetMinuteUpdate();
			break;
		case ECommandUpdatePerHour:
			iAppView->SetHourUpdate();
			break;
		case EAbout:
			{
			CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(); //msg
			dlg->RunLD();			
			}
			break;
		default:
			Panic( ES60UptimeUi);
			break;
		}
	}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CS60UptimeAppUi::HandleStatusPaneSizeChange()
{
	iAppView->SetRect(ClientRect());
}


void CS60UptimeAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
{
	TBool secondDimmed = EFalse,minuteDimmed = EFalse,hourDimmed = EFalse;
	if(aResourceId == R_MENU){
		if(iAppView->GetCurrentMode() == CS60UptimeAppView::ESecondMode){
			secondDimmed = ETrue;
		}
		else if(iAppView->GetCurrentMode() == CS60UptimeAppView::EMinMode){
			minuteDimmed = ETrue;
			
		}
		else if(iAppView->GetCurrentMode() == CS60UptimeAppView::EHourMode){
			hourDimmed = ETrue;
		}
		aMenuPane->SetItemDimmed(ECommandUpdatePerSecond, secondDimmed);
		aMenuPane->SetItemDimmed(ECommandUpdatePerMinute, minuteDimmed);
		aMenuPane->SetItemDimmed(ECommandUpdatePerHour, hourDimmed);
		
	}
}

// End of File
