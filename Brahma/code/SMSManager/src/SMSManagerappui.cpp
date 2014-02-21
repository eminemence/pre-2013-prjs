
#include <hal.h>
#include <aknnotewrappers.h>
#include <avkon.hrh>
#include <eikmenup.h>

#include <stringloader.h>
#include <SMSManager.rsg>

#include "SMSManager.pan"
#include "SMSManagerAppUi.h"
#include "SMSManagerAppView.h"
#include "SMSManager.hrh"

/*static void Log(TDesC& aMsg)
{

//#ifdef TESTING
	// User::InfoPrint(aMsg);
	RFs fs;
	fs.Connect();
	RFile file;
	TBuf<32> aFileName = _L("C:\\");

	// Add new data to buffer
	aFileName.Append(aMsg);
	file.Create(fs,aFileName,EFileWrite);

	//Closes() the file or directory. Any open files are closed when the file server session is closed.
	file.Close(); 

	fs.Close();
//#endif

}*/

// ConstructL is called by the application framework
void CSMSManagerAppUi::ConstructL()
{
    BaseConstructL();
    
    iAppView = CSMSManagerAppView::NewL(ClientRect());    

    AddToStackL(iAppView);
}

CSMSManagerAppUi::CSMSManagerAppUi()                              
{
    // add any construction that cannot leave here
}

CSMSManagerAppUi::~CSMSManagerAppUi()
{
    if (iAppView){
        RemoveFromStack(iAppView);
        delete iAppView;
        iAppView = NULL;
	}
}

// handle any menu commands
void CSMSManagerAppUi::HandleCommandL(TInt aCommand)
{
    switch(aCommand)
        {
		 case ESMSManagerMemSave2MemCmd:
			 iAppView->AppViewExport2Mem(); 				 
		 break;
 		case ESMSManagerMemMove2MMCCmd:
			 iAppView->AppViewExport2Mmc(); 
		 break; 		
 		case ESMSManagerMemViewDetailsCmd:
		 	iAppView->AppViewViewSmsDetails(); 		
		 break; 		
 		case ESMSManagerMemViewSMSCmd:
		 	iAppView->AppViewViewSms(); 		
		 break; 		
 		case ESMSManagerMemDeleteEntryCmd:
		 	iAppView->AppViewDeleteSms(); 		
		 break; 		
         case EAknSoftkeyExit:
            Exit();
         break;
        default:
            User::Panic (_L("SMSManager"), ESMSManagerBasicUi);
            break;
   }
}

void CSMSManagerAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
	//get the present machine uid 
	//and compare with 7650's UID 
	//if 7650 dim the menu item
	TInt m7650_Uid = 0x101f4fc3;
	TInt mUid;
	HAL::Get(HAL::EMachineUid,mUid);
	if(aResourceId == R_SMSMGR_MEM_OPTIONS_SUB_MENU){
		if(mUid == m7650_Uid){
			aMenuPane->SetItemDimmed(ESMSManagerMemMove2MMCCmd,ETrue);
		}
	}
}





