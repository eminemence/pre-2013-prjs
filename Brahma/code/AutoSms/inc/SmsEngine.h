#ifndef __SMSENGINE_H__
#define __SMSENGINE_H__
#include <e32base.h>
#include <mtclreg.h>
#include <smut.h>
#include <SMUTHDR.h>
#include <SMSCLNT.h>
#include <msvapi.h>  // MMsvSessionObserver
#include <badesca.h> // CDesCArrayFlat
#include <txtrich.h>
#include <smscmds.h>
#include "MsgStrings.h"
#include "MsgTypes.h"

// FORWARD DECLARATIONS
class CClientMtmRegistry;
class CSmsClientMtm;

class MSmsSendingEngineObserver 
{
	public:
	virtual void HandleEngineEvent() = 0;
};

class CSmsSendingEngine : public CActive, public MMsvSessionObserver
{
	public:
		static CSmsSendingEngine* NewL(MSmsSendingEngineObserver* iObserver);
		
		static CSmsSendingEngine* NewLC(MSmsSendingEngineObserver* iObserver);
		
		~CSmsSendingEngine();
		
	private:
		void ConstructL(MSmsSendingEngineObserver* iObserver);
		
		CSmsSendingEngine();

	//from 	CActive
	private:
		void RunL();
		
		void DoCancel();
		
    public:
        void SendMessageL(TMsgType aMsgType);
        
        void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1,TAny* aArg2, TAny* aArg3);
        
        void CreateMtmClientL();
        
		TMsvId CreateSMSMessageL(const TDesC& aAddress,const TDesC& aMessage);
		
		void SendSMSL();
    
    private:
		HBufC* LoadMsgFromFileLC(TMsgType aMsgType);
		
    	void SendMsgUsingMsvL(TDesC& aMsgText);
    	
    	void DeleteMessageL( TMsvId aMessageId );
    	
    private:    	
    	MSmsSendingEngineObserver* iObserver;
    	
		CSmsClientMtm*      iSmsMtm;

        CMsvOperation*      iMsvOper;

        CMsvEntrySelection* iEntrySelection;

        CMsvSession*        iSession;   

        CClientMtmRegistry* iClientMtmReg;
       
		RArray<TMsvId>*     iIdArray;
       
        TMsvId              iSmsId;	
};

#endif //__SMSENGINE_H__