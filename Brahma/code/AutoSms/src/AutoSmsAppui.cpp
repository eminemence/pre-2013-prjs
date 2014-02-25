
#include <avkon.hrh>
#include <aknnotewrappers.h> 
#include <AutoSms.rsg> 

#include "AutoSms.pan"
#include "AutoSmsAppUi.h"
#include "AutoSmsAppView.h"
#include "AutoSms.hrh"
#include "MsgTypes.h"
#include <aknmessagequerydialog.h> 

// ConstructL is called by the application framework
void CAutoSmsAppUi::ConstructL()
    {
    BaseConstructL();

    iAppView = CAutoSmsAppView::NewL(ClientRect());    

    AddToStackL(iAppView);
    }

CAutoSmsAppUi::CAutoSmsAppUi()                              
    {
	// no implementation required
    }

CAutoSmsAppUi::~CAutoSmsAppUi()
    {
    if (iAppView)
        {
        RemoveFromStack(iAppView);
        delete iAppView;
        iAppView = NULL;
        }
    }

// handle any menu commands
void CAutoSmsAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;

        case EAutoSmsLeavingHomeCmd:
        {
        	iAppView->SendMessageL(TMsgLeavinHome);
        }
		break;
        case EAutoSmsReachedOfficeCmd:
        {
        	iAppView->SendMessageL(TMsgReachedOffice);
        }
		break;
        case EAutoSmsInRickCmd:
        {
        	iAppView->SendMessageL(TMsgInRick);
        }
		break;
        case EAutoSmsAtStopCmd:
        {
        	iAppView->SendMessageL(TMsgAtStop);
        }
		break;
        case EAutoSmsInBusCmd:
        {
        	iAppView->SendMessageL(TMsgInBus);
        }
		break;
        case EAutoSmsReachedHomeCmd:
        {
        	iAppView->SendMessageL(TMsgReachedHome);
        }
		break;

        case EAutoSmsShowAboutCmd:
        {
        	ShowAbout();
        }
		break;

        default:
            Panic(EAutoSmsBasicUi);
            break;
        }
    }

void CAutoSmsAppUi::ShowAbout()
{
	HBufC *aText = iEikonEnv->AllocReadResourceLC( R_ABOUT);
	ShowInfoDialog(R_ABOUT_TITLE,*aText);
	CleanupStack::PopAndDestroy( aText ); 	
}

void  CAutoSmsAppUi::ShowInfoDialog(TInt aTitleResId,TDesC& aInfoTxt)
{
	HBufC *aTitle = iEikonEnv->AllocReadResourceLC( aTitleResId); 
	CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog(); 
	
	dlg->PrepareLC( R_MESSAGE_QUERY ); 
	
	dlg->SetMessageTextL(aInfoTxt);
	dlg->QueryHeading()->SetTextL( aTitle->Des() ); 
	dlg->RunLD(); 

	CleanupStack::PopAndDestroy( aTitle ); 
}


