
#include <e32std.h>
#include <coemain.h>
#include <aknnotewrappers.h>
#include <SMSManager.rsg>
#include <ezlib.h>
#include "SMSManagerAppView.h"
#include "SMSManagerMbmHeaders.h"
#ifdef __WINSCW__
	_LIT(KAvkonBitmapFile,"z:\\system\\data\\SmsManager.mbm");
#else
	_LIT(KAvkonBitmapFile,"c:\\system\\apps\\SmsManager\\SmsManager.mbm");
#endif



/*static void Log(TDesC& aMsg)
{

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
}*/


CSMSManagerAppView* CSMSManagerAppView::NewL(const TRect& aRect)
{
    CSMSManagerAppView* self = CSMSManagerAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
}

CSMSManagerAppView* CSMSManagerAppView::NewLC(const TRect& aRect)
{
    CSMSManagerAppView* self = new (ELeave) CSMSManagerAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
}

void CSMSManagerAppView::ConstructL(const TRect& aRect)
{
    // Create a window for this application view
    CreateWindowL();
    
    iSmsOps = CSmsOps::NewL();
    iSmsCount = 0;
    iSelectedCount = 0;

	CreateListBoxL();
    // Set the windows size
    SetRect(aRect);
	// Activate the window, which makes it ready to be drawn
    ActivateL();
}

CSMSManagerAppView::CSMSManagerAppView()
{
}


CSMSManagerAppView::~CSMSManagerAppView()
{
	delete iIconList;
	
   	iCheckStatus->Reset();
   	delete iCheckStatus;
	
	delete iSmsOps;
}


// Draw this application's view to the screen
void CSMSManagerAppView::Draw(const TRect& /*aRect*/) const
{
   // Get the standard graphics context 
    CWindowGc& gc = SystemGc();
    
    // Gets the control's extent
    TRect rect = Rect();
    
    // Clears the screen
    gc.Clear(rect);
}

TKeyResponse CSMSManagerAppView::OfferKeyEventL(
		const TKeyEvent& aKeyEvent,TEventCode aType)
{
	// we only want the key press, not the key up/down event
	if (aType == EEventKey){
	   switch ( aKeyEvent.iCode ){
			case EKeyUpArrow:
			case EKeyDownArrow:
				iListBox->OfferKeyEventL(aKeyEvent,aType);
			break;
			case EKeyOK:
				ItemClickHandlerFunction();				
			break;
			default:
			break;
		}
	}
	return EKeyWasNotConsumed;
}


void  CSMSManagerAppView::ItemClickHandlerFunction()
{
	TInt ItemCheckBitmapNumber = 0,ItemUnCheckBitmapNumber = 1,BmpNum;
	TBuf<200> Item;
	
	TInt SelIndex = iListBox->CurrentItemIndex();

	iItemList->Delete(SelIndex);

	_LIT(KItemFormatString,"\t%S\t%S\t%d");	

	TBuf<50> VarDetails;
	TBuf<200> VarDescription;

	iSmsOps->GetDetails(SelIndex,VarDetails);
	iSmsOps->GetDescription(SelIndex,VarDescription);
	TInt CheckResult = iCheckStatus->MdcaPoint(SelIndex).Find(_L("0"));
	
	BmpNum = (CheckResult!=KErrNotFound)?ItemCheckBitmapNumber:ItemUnCheckBitmapNumber;
			
	if(CheckResult != KErrNotFound){// if checked 
		_LIT(KCheck,"1");
		iCheckStatus->InsertL(SelIndex,KCheck);
		iSelectedCount++;
	}
	else{
		_LIT(KUnCheck,"0");
		iCheckStatus->InsertL(SelIndex,KUnCheck);
		iSelectedCount--;
	}
	
	Item.Format(KItemFormatString,&VarDescription,&VarDetails,BmpNum);
	iItemList->InsertL(SelIndex,Item);
	iListBox->View()->DrawItem(SelIndex);
}

TCoeInputCapabilities CSMSManagerAppView::InputCapabilities() const
{
	// this class does not implement any 'unusual' input capabilities
	return TCoeInputCapabilities::ENone;
}


void CSMSManagerAppView::CreateListBoxL()
{
	//Delete the list box if already exists.
	if ( iListBox ){
		delete iListBox;
		iListBox = NULL;
	}

	iListBox = new (ELeave ) CAknFormDoubleStyleListBox();
	iListBox->ConstructL(this, EAknListBoxSelectionList );
	iListBox->SetContainerWindowL(*this);

	//add scroll bar to the list
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn,
														CEikScrollBarFrame::EAuto);

	const TPoint aPoint(0,0);
	const TSize aSize(180,150);
	
	//most important line , else the lst box simply disappears
	iListBox->SetExtent(aPoint,aSize);
	
	//Fill list box with the data
	AppViewInitSMSList();

	iIconList = new (ELeave) CAknIconArray(10); 
	
	iIconList->AppendL(iEikonEnv->CreateIconL(KAvkonBitmapFile, EMbmSmsmanagermbmheadersCheck_on, EMbmSmsmanagermbmheadersCheck_on_mask)); 
	iIconList->AppendL(iEikonEnv->CreateIconL(KAvkonBitmapFile, EMbmSmsmanagermbmheadersCheck_off, EMbmSmsmanagermbmheadersCheck_off_mask)); 
	iListBox->ItemDrawer()->ColumnData()->SetIconArray(iIconList); 
	iListBox->HandleItemAdditionL();
}



void CSMSManagerAppView::AppViewInitSMSList(){
	
	iListBox->Reset();
	
	//refill the array to get the latest set of entries
	iSmsOps->InitSMSList();

	_LIT(KItemFormatString,"\t%S\t%S\t%d");
	TInt VarCount = iSmsOps->GetSmsCount();
	
    if(iItemList){
    	iItemList->Reset();
    	delete iItemList;
    	iItemList = NULL;
    }
	
	
	iItemList  = new (ELeave) CDesCArrayFlat(VarCount);
	
	if(iCheckStatus){
		iCheckStatus->Reset();
		delete iCheckStatus;
		iCheckStatus = NULL;
	}

	iCheckStatus = new (ELeave) CDesCArrayFlat(VarCount);

	for(TInt i=0;i<VarCount;i++){
		_LIT(KUnCheck,"0");
		iCheckStatus->InsertL(i,KUnCheck);
	}
	
	iSmsCount = VarCount;

	TBuf<300> Item;
	TInt ItemCheckBitmapNumber = 0,ItemUnCheckBitmapNumber = 1,BmpNum= 0;
	if(VarCount > 0 ){
		for(TInt i=0;i<VarCount;i++){
			TBuf<50> VarDetails;
			TBuf<200> VarDescription;
			iSmsOps->GetDetails(i,VarDetails);
			iSmsOps->GetDescription(i,VarDescription);
			
			if(iCheckStatus->MdcaPoint(i).Find(_L("0"))!=KErrNotFound){
				BmpNum = ItemUnCheckBitmapNumber;
			}
			else{
				BmpNum = ItemCheckBitmapNumber;
			}
			
			Item.Format(KItemFormatString,&VarDescription,&VarDetails,BmpNum);
			iItemList->AppendL(Item);
		}
		iListBox->Model()->SetItemTextArray(iItemList);		
	}
	
	//set items and ownership
	iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);	
}

TInt CSMSManagerAppView::CountComponentControls() const
{
	return 1;
}
	
CCoeControl* CSMSManagerAppView::ComponentControl(TInt aIndex) const
{
	switch(aIndex){
		case 0:
			return iListBox;
		default:
			return NULL;
	}
}

void CSMSManagerAppView::AppViewDeleteSms()
{
	/*get the list of selected SMS*/
	TBool ans = ShowConfirmationQueryDialog(R_SMSMGR_DELETE_SMS_QUERY);
	if(ans !=0){//go ahead with the deletion
		TInt TmpCount = iSmsCount;
		CDesCArray* array=static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
			
		for(TInt i=iSmsCount-1;i>=0;i--){
			if(iCheckStatus->MdcaPoint(i).Find(_L("1")) != KErrNotFound){//means selected
				iSmsOps->DeleteSms(i);		
				iCheckStatus->Delete(i);
				array->Delete(i); 
				TmpCount--;
			}
		}
		iSmsCount = TmpCount;
		
		iListBox->HandleItemRemovalL();		
		
		for(TInt i = 0;i<iSmsCount;i++){
			iListBox->DrawItem(i);
		}
		
		iListBox->Reset();
		iListBox->DrawNow();
	}
}

TBool  CSMSManagerAppView::ShowConfirmationQueryDialog(TInt aResourceId)
{
    CAknQueryDialog* dlg;
    dlg = CAknQueryDialog::NewL( CAknQueryDialog::ENoTone );
	TBool answer = dlg->ExecuteLD( aResourceId );
	return answer;
}


void CSMSManagerAppView::AppViewViewSmsDetails()
{
	iSmsOps->ViewMemSmsDetails(iListBox->CurrentItemIndex());
}

void CSMSManagerAppView::AppViewViewSms()
{
	iSmsOps->ViewMemSms(iListBox->CurrentItemIndex());
}

void CSMSManagerAppView::HandleListBoxEventL(CEikListBox* aListBox,TListBoxEvent aEventType)
{
}

void  CSMSManagerAppView::AppViewExport2Mem()
{
	_LIT(Kdata,"C:\\Sms");
	TBuf<100> path;
	path.Copy(Kdata);
	iSmsOps->ExportToTxt(iListBox->CurrentItemIndex(),path);
}

void  CSMSManagerAppView::AppViewExport2Mmc()
{
	_LIT(Kdata,"E:\\Sms");
	TBuf<100> path;
	path.Copy(Kdata);
	iSmsOps->ExportToTxt(iListBox->CurrentItemIndex(),path);
}
