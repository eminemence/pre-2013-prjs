#include "SmsEngine.h"


void CSmsSendingEngine::SendMessageL(TMsgType aMsgType)
{
	HBufC* iBuf;
	iBuf = LoadMsgFromFileLC(aMsgType);
	TPtr aPTr = iBuf->Des();
	TRAPD(err,SendMsgUsingMsvL(aPTr));
	CleanupStack::PopAndDestroy(iBuf);//iBuf
}

HBufC* CSmsSendingEngine::LoadMsgFromFileLC(TMsgType aMsgType)
{
	HBufC* msgBuf = HBufC::NewLC(256);
	TPtr bufptr = msgBuf->Des();
	switch(aMsgType)
	{
		case TMsgLeavinHome:
			bufptr.Copy(KMsgLeavinHome);
		break;
		case TMsgReachedOffice:
			bufptr.Copy(KMsgReachedOffice);
		break;
		case TMsgInRick:
			bufptr.Copy(KMsgInRick);
		break;
		case TMsgAtStop:
			bufptr.Copy(KMsgAtStop);		
		break;
		case TMsgInBus:
			bufptr.Copy(KMsgInBus);		
		break;
		case TMsgReachedHome:
			bufptr.Copy(KMsgReachedHome);		
		break;

	}
	return msgBuf;
	
}

void CSmsSendingEngine::SendMsgUsingMsvL(TDesC& aMsgText)
{
	_LIT(KDummyMobileNumber,"+919876543210");
	CreateSMSMessageL(KDummyMobileNumber,aMsgText);
	SendSMSL();
//	DeleteMessageL(iSmsId);
	
}

// Tells when the session has been opened
void CSmsSendingEngine::HandleSessionEventL(TMsvSessionEvent aEvent,
                                                TAny* /*aArg1*/,
                                                TAny* /*aArg2*/,
                                                TAny* /*aArg3*/)
    {
    switch (aEvent)
        {
        // This event tells us that the session has been opened
        case EMsvServerReady:
            TRAPD(err,CreateMtmClientL());
            iObserver->HandleEngineEvent();
            break;

        default:
            // do nothing
            break;
        }
    }
        
        
// Creates CSmsClientMtm after session has been opened.
void CSmsSendingEngine::CreateMtmClientL()
{
    // Client-side MTM registry.
    iClientMtmReg = CClientMtmRegistry::NewL(*iSession);

    //3rd: edition notice: If capabilities are missing, then iSmsMtm stays null
    // Get the SMS Mtm client from the registry
    iSmsMtm = static_cast<CSmsClientMtm*>(iClientMtmReg->NewMtmL(KUidMsgTypeSMS));
}

TMsvId CSmsSendingEngine::CreateSMSMessageL(const TDesC& aAddress,const TDesC& aMessage)
{
    // Set SMS parameters
    TMsvEntry indexEntry;
	indexEntry.iDate.HomeTime();
    indexEntry.SetInPreparation(ETrue);
	// This is an SMS message
    indexEntry.iMtm = KUidMsgTypeSMS;
    indexEntry.iType = KUidMsvMessageEntry;

	//Gets the ID of the current SMS service.
	//in 3rd edition crashes here if capabilities are wrong
    indexEntry.iServiceId = iSmsMtm->ServiceId();

    // Create entry to drafts
    iSmsMtm->SwitchCurrentEntryL(KMsvDraftEntryId);
    //iSmsMtm->SwitchCurrentEntryL(KMsvGlobalOutBoxIndexEntryId); //test!

	// Creates a new child entry owned by the context synchronously.
    iSmsMtm->Entry().CreateL(indexEntry);

    // Set the MTM's active context to the new message
    iSmsId = indexEntry.Id();
    iSmsMtm->SwitchCurrentEntryL(iSmsId);

    // Add message body. Body is set twice because index entry keeps a copy
	// of some summary information. Index entry and full stored entry
	// must be in sync.
    CRichText& body = iSmsMtm->Body();
    body.Reset();
    body.InsertL(0, aMessage);
    indexEntry.iDescription.Set(aMessage);

    // Add destination address (recipient).Copy address also to the index entry
    iSmsMtm->AddAddresseeL(aAddress);
    indexEntry.iDetails.Set(aAddress);

    // Commit changes because index entry is only a local variable
    iSmsMtm->Entry().ChangeL(indexEntry);

    // Save full message data to the store
    iSmsMtm->SaveMessageL();

    return iSmsId;

}

// Before sending sms message should be created CreateSMSMessageL() and
// Validated ValidateCreatedSMS().
void CSmsSendingEngine::SendSMSL()
{
    // Changes the entry on which later actions are performed to the entry
	// with the specified TMsvId.
    iSmsMtm->SwitchCurrentEntryL(iSmsId);

    // Load the created message
    iSmsMtm->LoadMessageL();

    // Gets the current SMS service settings
    CSmsSettings& serviceSettings = iSmsMtm->ServiceSettings();


	// Gets the number of service centre addresses stored in this object.
	const TInt numSCAddresses = serviceSettings.NumSCAddresses();

	// There should always be a service center number
	if (numSCAddresses > 0)
		{
        TInt scIndex=0;
        scIndex = serviceSettings.DefaultSC();
        // get the service center number:
        if ((scIndex < 0) || (scIndex >= numSCAddresses))
	        {
	        //scIndex = 0;
	        User::Panic(_L("AUTOSMS"),2);
	        }
        CSmsNumber*	serviceCentreNumber = &(serviceSettings.SCAddress(
                                            serviceSettings.DefaultSC()));

        iSmsMtm->SmsHeader().SetServiceCenterAddressL(serviceCentreNumber->Address());

        }
	else
		{
		#ifndef __WINS__
		// Panic if there is no service center number
		//Panic(ESmsNoServiceSenterNumberSet);
		#endif
		}

    iSmsMtm->SaveMessageL();

    // Index entry must be Updated
    TMsvEntry indexEntry = iSmsMtm->Entry().Entry();

	// Set in-preparation flag
    indexEntry.SetInPreparation(EFalse);

	// Sets the sending state
    indexEntry.SetSendingState(KMsvSendStateWaiting);
    iSmsMtm->Entry().ChangeL(indexEntry);

    // Time to send the message
    Cancel(); // prepare iMsvOper for use
	iEntrySelection->Reset();
	iEntrySelection->AppendL(iSmsId);


    //The old way of sending (1st and 2nd edition)
    TBuf8<1> dummyParam;
	// There is also InvokeSyncFunctionL which is synchronous.
	iMsvOper = iSmsMtm->InvokeAsyncFunctionL(ESmsMtmCommandScheduleCopy,
	                                  *iEntrySelection, dummyParam, iStatus);

    //a change for 3rd edition:
    //CMsvEntry* entry = iSession->GetEntryL(KMsvDraftEntryId);
    //TRAPD(err,
    //entry->CopyL(iSmsMtm->Entry().EntryId(), iSmsMtm->ServiceId(), iStatus) );

    SetActive();
}



CSmsSendingEngine* CSmsSendingEngine::NewL(MSmsSendingEngineObserver* iObserver)
{
	CSmsSendingEngine* self = CSmsSendingEngine::NewLC(iObserver);
	CleanupStack::Pop(self);
	return self;
}
		
CSmsSendingEngine* CSmsSendingEngine::NewLC(MSmsSendingEngineObserver* iObserver)
{
	CSmsSendingEngine* self = new (ELeave)CSmsSendingEngine();
	CleanupStack::PushL(self);
	self->ConstructL(iObserver);
	return self;
}
		
CSmsSendingEngine::~CSmsSendingEngine()
{
	
}
		
void CSmsSendingEngine::ConstructL(MSmsSendingEngineObserver* aObserver)
{
	iObserver = aObserver;
    CActiveScheduler::Add(this);

    // iEntrySelection encapsulates an array of entry IDs
    iEntrySelection = new (ELeave) CMsvEntrySelection;

    // Represents a channel of communication between a client thread
    // (Client-side MTM, User Interface MTM, or message client application)
    // and the Message Server thread.
	// Session is opened asynchorously. CreateMtmClientL() is called afterwards.
	// Another possibility is use OpenSyncL which is synchronous.
    iSession = CMsvSession::OpenAsyncL(*this);

}
		
CSmsSendingEngine::CSmsSendingEngine():
CActive(EPriorityStandard)
{
	
}
void CSmsSendingEngine::DoCancel()
{
	if (iMsvOper)
	{
		iMsvOper->Cancel();
		delete iMsvOper;
		iMsvOper = NULL;
	}
}

void CSmsSendingEngine::RunL()
{
	
}

// Delete message from a folder. Notice that the iSmsMtm points to the parent,
// not to the message itself.
void CSmsSendingEngine::DeleteMessageL( TMsvId aMessageId )
{
	iSmsMtm->SwitchCurrentEntryL( aMessageId );
	TMsvId parent = iSmsMtm->Entry().Entry().Parent();

	iSmsMtm->SwitchCurrentEntryL( parent );
	iSmsMtm->Entry().DeleteL( aMessageId );
}


		




