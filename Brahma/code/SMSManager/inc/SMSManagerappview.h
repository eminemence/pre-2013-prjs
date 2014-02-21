
#ifndef __SMSManagerAPPVIEW_H__
#define __SMSManagerAPPVIEW_H__


#include <coecntrl.h>
#include <msvapi.h >
#include <aknlists.h> 
#include <akniconarray.h>
#include <eiklbo.h>
#include "SmsEntry.h"
#include "SmsOps.h"


class CSMSManagerAppView : public CCoeControl,
							public MEikListBoxObserver
							
    {
public:

    static CSMSManagerAppView* NewL(const TRect& aRect);

    static CSMSManagerAppView* NewLC(const TRect& aRect);


     ~CSMSManagerAppView();

    void UserDraw() const;


public:  // from CCoeControl
    void Draw(const TRect& aRect) const;
  

    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

	TCoeInputCapabilities InputCapabilities() const;
	
	void EnumSmsEntries();
	
	//from MEikListBoxObserver
	
	void HandleListBoxEventL(CEikListBox* aListBox,TListBoxEvent aEventType);
	

	
	TInt CountComponentControls() const;
	
	CCoeControl* ComponentControl(TInt aIndex) const;
	
	void AppViewMove2Mmc();

	void AppViewDeleteSms();

	void AppViewViewSmsDetails();

	void AppViewViewSms();

	void AppViewShowMmcSmsList();
	
	void AppViewInitSMSList();
	
	void TestCompress();
	
	void ItemClickHandlerFunction();
	
	void AppViewExport2Mem();
	
	void AppViewExport2Mmc();	
	
//	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

private:

	void CreateListBoxL();
	
	TBool  ShowConfirmationQueryDialog(TInt aResourceId);

    void ConstructL(const TRect& aRect);

    CSMSManagerAppView();
    
    
   	CEikFormattedCellListBox*	iListBox;
   	CSmsOps *iSmsOps;
   	CArrayPtr<CGulIcon>* iIconList;
	CDesCArray *iCheckStatus;
	CDesCArray *iItemList;
	TInt iSmsCount;
	TInt iSelectedCount;
   	
//    RArray<TInt> iEntrySize;    
};


#endif // __SMSManagerAPPVIEW_H__
