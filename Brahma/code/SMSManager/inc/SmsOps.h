#include <e32std.h>
#include <coemain.h>
//for mtm api
#include <msvapi.h>
//for the entryid
#include <msvids.h>
#include <msvuids.h>
//for CClientMtmRegistry
#include <mtclreg.h>
//for the KuidSms thingy
#include <smut.h>
//for CBaseMtm
#include <mtclbase.h>
//for CRichText
#include <txtrich.h>
//for message query dialog
#include <aknmessagequerydialog.h> 

#include <eikenv.h>
#include <SMSManager.rsg>

class CSmsOps : public CBase,
				public MMsvSessionObserver
{
	public :
		void DeleteSms(TInt aIndex);

		void ViewMemSmsDetails(TInt aIndex);		

		void ViewMemSms(TInt aIndex);		
		
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);		

		void InitSMSList();		
		
		TInt GetSmsCount();
		
		static CSmsOps* NewL();

		static CSmsOps* NewLC();
		
		void GetDetails(TInt aIndex,TDes &aDetailsBuf);
	
		void GetDescription(TInt aIndex,TDes &aDescriptionBuf);
		
		void ExportToTxt(TInt aIndex,TDesC& aPath);
		
		
		~CSmsOps();		
				
	private :
	
		void  ShowInfoDialog(TInt aTitleResId,TDesC& aInfoTxt);
		
		CSmsOps();
	
//    	RArray<TSmsEntry> iEntryType;
	    RArray<TMsvId> iEntryId;
		CDesCArray* iEntryDetails;
		CDesCArray* iEntryDescription;

    	TInt iSmsCount;
};	