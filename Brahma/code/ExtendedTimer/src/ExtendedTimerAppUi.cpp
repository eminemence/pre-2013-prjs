/*
 * Copyright (C) 2011  Mayur Kankanwadi(eminemence@gmail.com)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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
#include <e32base.h>
//#include <aknmessagequerydialog.h>
//#include <aknnotewrappers.h>
//#include <stringloader.h>
//#include <f32file.h>
//#include <s32file.h>
//#include <hlplch.h>

#include <extendedtimer_0xA0011833.rsg>

//#ifdef _HELP_AVAILABLE_
//#include "ExtendedTimer_0xA0011833.hlp.hrh"
//#endif
#include "ExtendedTimer.hrh"
#include "ExtendedTimer.pan"
#include "ExtendedTimerApplication.h"
#include "ExtendedTimerAppUi.h"
_LIT( KText, "Hello World!");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CExtendedTimerAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppUi::ConstructL()
    {
    // Initialise app UI with standard value.
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    
    // Create view object
    iAppView = CExtendedTimerAppView::NewL(ClientRect());

    }
// -----------------------------------------------------------------------------
// CExtendedTimerAppUi::CExtendedTimerAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppUi::CExtendedTimerAppUi()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CExtendedTimerAppUi::~CExtendedTimerAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CExtendedTimerAppUi::~CExtendedTimerAppUi()
{
	for(int i = 0;i<iTimerArray.Count();i++){
		delete iTimerArray[i];
	}
	iTimerArray.Close();
	iTimerMap.Close();
    if (iAppView)
        {
        delete iAppView;
        iAppView = NULL;
        }

}

// -----------------------------------------------------------------------------
// CExtendedTimerAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppUi::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;
        case ESetMinuteTimerCommand:{
			CTimerX* timer = CTimerX::NewL(this);
			iTimerArray.Append(timer);
			timer->StartMinTimerL(1,ETrue);
			TTimerMap map;
			map.iTimerId = timer->GetTimerId();
			map.type = EOneMinTimer;
			iTimerMap.Append(map);
        }
        	break;
        case ESetTwoMinuteTimerCommand:{
			CTimerX* timer = CTimerX::NewL(this);
			iTimerArray.Append(timer);
			timer->StartMinTimerL(2,EFalse);
			TTimerMap map;
			map.iTimerId = timer->GetTimerId();
			map.type = ETwoMinTimer;
			iTimerMap.Append(map);
			
        }
        	break;
        case ESet60MinuteTimerCommand:{
			CTimerX* timer = CTimerX::NewL(this);
			iTimerArray.Append(timer);
			timer->StartMinTimerL(60,ETrue);
			TTimerMap map;
			map.iTimerId = timer->GetTimerId();
			map.type = EOneHourTimer;
			iTimerMap.Append(map);
			
        }
        	break;
        case ESetSecondTimerCommand:{
			CTimerX* timer = CTimerX::NewL(this);
			iTimerArray.Append(timer);        
			timer->StartSecondTimerL(1,ETrue);
			TTimerMap map;
			map.iTimerId = timer->GetTimerId();
			map.type = EOneSecTimer;
			iTimerMap.Append(map);
			
        }
        	break;
        case EStopTimerCommand:{
			for(int i = 0;i< iTimerArray.Count();i++){
				CTimerX* timer = iTimerArray[i];
				timer->StopTimerL();	
			}
			
        }
        	break;
        default:
            Panic( EExtendedTimerUi);
            break;
        }
    }
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CExtendedTimerAppUi::HandleStatusPaneSizeChange()
    {
    iAppView->SetRect(ClientRect());
    }

//CArrayFix<TCoeHelpContext>* CExtendedTimerAppUi::HelpContextL() const
//    {
//#warning "Please see comment about help and UID3..."
//    // Note: Help will not work if the application uid3 is not in the
//    // protected range.  The default uid3 range for projects created
//    // from this template (0xE0000000 - 0xEFFFFFFF) are not in the protected range so that they
//    // can be self signed and installed on the device during testing.
//    // Once you get your official uid3 from Symbian Ltd. and find/replace
//    // all occurrences of uid3 in your project, the context help will
//    // work. Alternatively, a patch now exists for the versions of 
//    // HTML help compiler in SDKs and can be found here along with an FAQ:
//    // http://www3.symbian.com/faq.nsf/AllByDate/E9DF3257FD565A658025733900805EA2?OpenDocument
//#ifdef _HELP_AVAILABLE_
//    CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
//    CleanupStack::PushL(array);
//    array->AppendL(TCoeHelpContext(KUidExtendedTimerApp, KGeneral_Information));
//    CleanupStack::Pop(array);
//    return array;
//#else
//    return NULL;
//#endif
//    }

void CExtendedTimerAppUi::HandleTimerExpireL(TInt aTimerId)
{
	for(int i = 0;i<iTimerArray.Count();i++){
		CTimerX* timer = (CTimerX*)iTimerArray[i];
		if(aTimerId == timer->GetTimerId()){
			for(int j = 0 ; j< iTimerMap.Count();j++){
				TTimerMap mp = iTimerMap[j];
				if(aTimerId == mp.iTimerId){
					switch(mp.type){
						case EOneMinTimer:
							iAppView->IncOneMinCount();
							break;
						case ETwoMinTimer:
							iAppView->IncTwoMinCount();
							break;
						case EOneHourTimer:
							iAppView->IncOneHourCount();
							break;
						case EOneSecTimer:
							iAppView->IncOneSecCount();
							break;
					}
				}
			}
		}
	}
	iAppView->DrawNow();
}


// End of File
