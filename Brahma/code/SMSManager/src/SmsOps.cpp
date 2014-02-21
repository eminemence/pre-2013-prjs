#include <aknnotedialog.h> // for the query dialogs
#include <eikenv.h>
#include <smsclnt.h>
#include <smuthdr.h>
#include <eikfutil.h>
//#include <smutset.h>
#include <smscmds.h>
// for TMmcCardInfo
#include <d_mmcif.h>
#include "SmsOps.h"

#define KBufferLength					256

//macro to display a note dlg. 
#define DISPLAY_NOTE_DLG(msg)   {_LIT(KMsg,msg);\
								TBuf16<KBufferLength> note;\
								note.Copy(KMsg);\
								CAknNoteDialog *NoteDlg = new (ELeave)CAknNoteDialog;\
								NoteDlg->PrepareLC(R_SMSMGR_BLANK_NOTE);\
								NoteDlg->SetTextL(note);\
								NoteDlg->RunLD();}\


#define CREATE_MSG_SESSION  CMsvSession 		*VarMsgSession 	= CMsvSession::OpenSyncL(*this);\
							CClientMtmRegistry* VarReg 			= CClientMtmRegistry::NewL(*VarMsgSession,0);\
							CMsvEntry 			*VarRootEntry 	= VarMsgSession->GetEntryL(VarEntryId);	

#define CREATE_MSG_SESSION_WITH_MTM CMsvSession *VarMsgSession	= CMsvSession::OpenSyncL(*this);\
									CClientMtmRegistry* VarReg	= CClientMtmRegistry::NewL(*VarMsgSession);\
									CBaseMtm* VarSmsMtm 		= VarReg->NewMtmL(KUidMsgTypeSMS);


void CSmsOps::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
{
}

void CSmsOps::InitSMSList()
{
	CMsvEntrySelection *VarChildren;
	CMsvEntry *VarEntry;
	
	TInt VarEntryId = KMsvGlobalInBoxIndexEntryId;
	iEntryId.Reset();
	iSmsCount = 0;
	
	CREATE_MSG_SESSION
	CBaseMtm* 			VarSmsMtm 		= VarReg->NewMtmL(KUidMsgTypeSMS);


	TInt VarCount = VarRootEntry->Count();
	iSmsCount = VarCount;
	
   	if(iEntryDetails){
   		iEntryDetails->Reset();
   		delete iEntryDetails;
   		iEntryDetails = NULL;
   	}
   	
   	if(iEntryDescription){
   		iEntryDescription->Reset();
   		delete iEntryDescription;
   		iEntryDescription = NULL;
   	}

	iEntryDetails = new (ELeave) CDesCArrayFlat(iSmsCount);
	iEntryDescription = new (ELeave) CDesCArrayFlat(iSmsCount);
	
	if(VarCount > 0 ){
		VarChildren = VarRootEntry->ChildrenL();
		for(TInt i=0;i<VarCount;i++){
			VarEntry = VarMsgSession->GetEntryL((*VarChildren)[i]);
			const TMsvEntry& EntryRef = VarEntry->Entry();
			VarSmsMtm->SwitchCurrentEntryL(EntryRef.Id());
			iEntryId.Append(EntryRef.Id());

			TBuf<200> aBuf;
			_LIT(KItem,"%S");
			
			aBuf.Format(KItem,&EntryRef.iDetails);			
			iEntryDetails->AppendL(aBuf);
			aBuf.Format(KItem,&EntryRef.iDescription);			
			iEntryDescription->AppendL(aBuf);
		}
	}
		
	VarReg = NULL;
	VarSmsMtm = NULL;
	VarMsgSession = NULL;
	VarChildren = NULL;
	VarRootEntry = NULL;
	VarEntry = NULL;
}

void CSmsOps::DeleteSms(TInt aIndex)
{
	TInt VarEntryId 			= KMsvGlobalInBoxIndexEntryId;
	
	CREATE_MSG_SESSION
	
	VarRootEntry->DeleteL(iEntryId[aIndex]);
	
	delete VarReg;
	VarReg = NULL;
	VarMsgSession = NULL;

	InitSMSList();
}

void CSmsOps::ViewMemSmsDetails(TInt aIndex)
{
	/*"Sender:%S\n
	Date:%S\n
	Size:%S\n
	Attachment:%S\n
	Unread:%S\n
	New:%S\n
	Recipients:%S\n
	Priority:%S\n
	Read-only:%S\n"*/

	CREATE_MSG_SESSION_WITH_MTM
	
	VarSmsMtm->SwitchCurrentEntryL(iEntryId[aIndex]);
	const TMsvEntry& aEntry = VarSmsMtm->Entry().Entry();
	
	HBufC *aText = CEikonEnv::Static()->AllocReadResourceLC( R_SMS_DETAILS_TEXT); 
	HBufC *aText1 = HBufC::NewLC(650);
	
	/*0 - Attachment,1-Unread,2-New,3-Read-Only,4-MultipleRecipients*/
	TBuf<10> FlagBuf[5];
	_LIT(KYes,"YES");
	_LIT(KNo,"NO");	
	_LIT(KMultiple,"MULTIPLE");
	_LIT(KSingle,"SINGLE");	
	
	(aEntry.Attachment())	? FlagBuf[0].Copy(KYes):FlagBuf[0].Copy(KNo);	
	(aEntry.Unread())		? FlagBuf[1].Copy(KYes):FlagBuf[1].Copy(KNo);	
	(aEntry.New())			? FlagBuf[2].Copy(KYes):FlagBuf[2].Copy(KNo);	
	(aEntry.ReadOnly())		? FlagBuf[3].Copy(KYes):FlagBuf[3].Copy(KNo);	
	
	(aEntry.MultipleRecipients())?FlagBuf[4].Copy(KMultiple):FlagBuf[4].Copy(KSingle);	
	
	TTime aTime = aEntry.iDate;
	TDateTime aDateTime = aTime.DateTime();
	TBuf<20> DateTimeBuf;
	_LIT(KDateTime,"%d:%d:%d %d/%d/%d");
	DateTimeBuf.Format(KDateTime,aDateTime.Hour(),aDateTime.Minute(),aDateTime.Second(),aDateTime.Day()+1,aDateTime.Month(),aDateTime.Year());
	
	TMsvPriority aPrio = aEntry.Priority();
	TBuf<10> MsgPriority;
	if(aPrio == EMsvHighPriority){
		MsgPriority.Copy(_L("High"));
	}
	else if(aPrio == EMsvMediumPriority){
		MsgPriority.Copy(_L("Medium"));	
	}
	else{
		MsgPriority.Copy(_L("Low"));	
	} 
 
	aText1->Des().Format(aText->Des(),&aEntry.iDetails,
									&DateTimeBuf,
									aEntry.iSize,
									&FlagBuf[0],
									&FlagBuf[1],									
									&FlagBuf[2],									
									&FlagBuf[4],	
									&MsgPriority,
									&FlagBuf[3]	);
	ShowInfoDialog(R_ABOUT_SMS,*aText1);
	CleanupStack::PopAndDestroy( aText1 ); 	
	CleanupStack::PopAndDestroy( aText ); 
}


void CSmsOps::ViewMemSms(TInt aIndex)
{
	CREATE_MSG_SESSION_WITH_MTM
	
	VarSmsMtm->SwitchCurrentEntryL(iEntryId[aIndex]);
	CSmsClientMtm* sms_mtm = static_cast<CSmsClientMtm*>(VarSmsMtm);
	
	TRAPD(aError,sms_mtm->LoadMessageL());
			
	if(aError !=KErrNone){
		TBuf<100> aText;	
		if(aError == KErrNotFound){
			aText.Copy(_L("No text in msg or binary file."));		
		}
		else{
			_LIT(KText,"Error : %d");
			aText.Format(KText,aError);
		}
		CEikonEnv::Static()->InfoWinL(_L("-[SMSManager]-"),aText);
		return;
	}
			
	HBufC *aText = CEikonEnv::Static()->AllocReadResourceLC( R_SMS_TEXT); 
	HBufC *aText1 = HBufC::NewLC(600);
	TBuf<300> aBuf;

	aBuf.Copy(VarSmsMtm->Body().Read(0));
	aText1->Des().Format(aText->Des(),&aBuf);
	ShowInfoDialog(R_ABOUT_TITLE,*aText1);
	
	CleanupStack::PopAndDestroy( aText1 ); 	
	CleanupStack::PopAndDestroy( aText ); 
	
	delete VarReg;
	VarReg = NULL;
	VarSmsMtm = NULL;	
	VarMsgSession = NULL;
}


void  CSmsOps::ShowInfoDialog(TInt aTitleResId,TDesC& aInfoTxt)
{
	HBufC *aTitle = CEikonEnv::Static()->AllocReadResourceLC( aTitleResId); 
	CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog(); 
	
	dlg->PrepareLC( R_MESSAGE_QUERY_TASK ); 
	dlg->SetMessageTextL(aInfoTxt);
	dlg->QueryHeading()->SetTextL( aTitle->Des() ); 
	dlg->RunLD(); 

	CleanupStack::PopAndDestroy( aTitle ); 
}

CSmsOps* CSmsOps::NewL()
{
	CSmsOps* self = CSmsOps::NewLC();
	CleanupStack::Pop(self);
	return self;
}

CSmsOps* CSmsOps::NewLC()
{
	CSmsOps* self = new (ELeave) CSmsOps();
	CleanupStack::PushL(self);
	return self;
}


CSmsOps::CSmsOps()
{
}


CSmsOps::~CSmsOps()
{
    // Add any destruction code here
   	iEntryId.Close();
  		   	
   	if(iEntryDetails){
   		iEntryDetails->Reset();
   		delete iEntryDetails;
   		iEntryDetails = NULL;
   	}
   	
   	if(iEntryDescription){
   		iEntryDescription->Reset();
   		delete iEntryDescription;
   		iEntryDescription = NULL;
   	}
}

TInt CSmsOps::GetSmsCount()
{
	return iSmsCount;
}


void CSmsOps::GetDetails(TInt aIndex,TDes &aDetailsBuf)
{
	_LIT(KItem,"%S");
	TBuf<50> varDetails;
	varDetails.Format(KItem,&iEntryDetails->MdcaPoint(aIndex));
	aDetailsBuf.Copy(varDetails);
}

void CSmsOps::GetDescription(TInt aIndex,TDes &aDescriptionBuf)
{
	_LIT(KItem,"%S");
	TBuf<200> varDescription;
	varDescription.Format(KItem,&iEntryDescription->MdcaPoint(aIndex));
	aDescriptionBuf.Copy(varDescription);
}


void  CSmsOps::ExportToTxt(TInt aIndex,TDesC& aPath)
{
	//open session to get the current entry
	CREATE_MSG_SESSION_WITH_MTM	
	
	VarSmsMtm->SwitchCurrentEntryL(iEntryId[aIndex]);
	const TMsvEntry& aEntry = VarSmsMtm->Entry().Entry();
	
	//from the entry attributes create the text to be written to the text file
	_LIT(KSmsDetails,"Sender:%S\nDescription:%S\nDate:%S\nSize:%d\nAttachment:%S\nUnread:%S\nNew:%S\nRecipients:%S\nPriority:%S\nRead-only:%S\n");
	_LIT(KYes,"YES");
	_LIT(KNo,"NO");	
	_LIT(KMultiple,"MULTIPLE");
	_LIT(KSingle,"SINGLE");	
	
	HBufC *aDetails1 = HBufC::NewLC(600);
	/*In Array  : 	0-Attachment,
					1-Unread,
					2-New,
					3-Read-Only,
					4-MultipleRecipients*/
	TBuf<10> FlagBuf[5];
	
	(aEntry.Attachment()) 	? FlagBuf[0].Copy(KYes) : FlagBuf[0].Copy(KNo);	
	(aEntry.Unread()) 		? FlagBuf[1].Copy(KYes) : FlagBuf[1].Copy(KNo);	
	(aEntry.New()) 			? FlagBuf[2].Copy(KYes) : FlagBuf[2].Copy(KNo);	
	(aEntry.ReadOnly()) 	? FlagBuf[3].Copy(KYes) : FlagBuf[3].Copy(KNo);	
	
	(aEntry.MultipleRecipients())? FlagBuf[4].Copy(KMultiple):FlagBuf[4].Copy(KSingle);	
	
	TTime aTime = aEntry.iDate;
	TDateTime aDateTime = aTime.DateTime();
	TBuf<20> DateTimeBuf;
	_LIT(KDateTime,"%2d:%2d:%2d %d/%d/%d");
	DateTimeBuf.Format(KDateTime,aDateTime.Hour(),aDateTime.Minute(),aDateTime.Second(),aDateTime.Day()+1,aDateTime.Month(),aDateTime.Year());
	
	TMsvPriority aPrio = aEntry.Priority();
	TBuf<10> MsgPriority;
	(aPrio == EMsvHighPriority)?MsgPriority.Copy(_L("High")):((aPrio == EMsvMediumPriority)?MsgPriority.Copy(_L("Medium")):MsgPriority.Copy(_L("Low")));	
 
	aDetails1->Des().Format(KSmsDetails,&aEntry.iDetails,
									&aEntry.iDescription,
									&DateTimeBuf,
									aEntry.iSize,
									&FlagBuf[0],
									&FlagBuf[1],									
									&FlagBuf[2],									
									&FlagBuf[4],	
									&MsgPriority,
									&FlagBuf[3]	);

	//again load the message to get the contents
	CBaseMtm* VarSmsMtm1 = VarReg->NewMtmL(KUidMsgTypeSMS);
	VarSmsMtm1->SwitchCurrentEntryL(iEntryId[aIndex]);
	CSmsClientMtm* sms_mtm = static_cast<CSmsClientMtm*>(VarSmsMtm1);
	
	TRAPD(aError,sms_mtm->LoadMessageL());
	HBufC *aText1 = NULL;
	if(aError == KErrNone){//its a text message then no error
		//copy the msg to the buffer
		aText1 = HBufC::NewLC(600);
		TBuf<300> aBuf;
		aBuf.Copy(VarSmsMtm1->Body().Read(0));
		_LIT(KBuf,"%S");
		aText1->Des().Format(KBuf,&aBuf);
	}
	else{//its a binary message copy file to the destination
		//get the msg's binary content file path
		TFileName aDir;
		VarSmsMtm->Entry().GetFilePath(aDir);
		
		TBuf<100> aNum;
		aNum.Copy(aDir.Right(13));
		aNum.Copy(aNum.Left(1));
		aNum.ZeroTerminate();
		
		TLex aNum1(aNum);	
		
		TBuf<20> aPart1;
		aPart1.Copy(aDir.Right(11));
		aPart1.Copy(aPart1.Left(7));
		
		//increase folder number to get the actual file
		TUint DirNum = 0;
		TRadix Rad = EHex;
		aNum1.Val(DirNum,Rad);
		
		if(DirNum<15){
			DirNum++;
		}
		else if(DirNum == 15){
			DirNum = 0;
		}
		
		TInt LeftPart = aDir.Length() - 13;
		//create the source and destination file path strings
		TBuf<200> aSourceFilePath,aDesFilePath;
		_LIT(KSourcePath,"%S%1x\\%S%1x_F\\%S");
		_LIT(KDesPath,"%S\\%S");		
		TBuf<100> LeftString;
		
		LeftString.Copy(aDir.Left(LeftPart));
		aSourceFilePath.Format(KSourcePath,&LeftString,DirNum,&aPart1,DirNum,&iEntryDescription->MdcaPoint(aIndex));

		aDesFilePath.Format(KDesPath,&aPath,&iEntryDescription->MdcaPoint(aIndex));
		
		//copy the file 
		BaflUtils::CopyFile(CEikonEnv::Static()->FsSession(),
							aSourceFilePath,
							aDesFilePath
							);
	}
	
	/*now write the sms to the file and then delete the entry*/
	//filename format (sender_name)_(second)_(date).txt
	_LIT(KFileName,"%S\\%S_%d_%d_%d_%d.txt");
	TBuf<100> BufFileName;
	BufFileName.Format(KFileName,&aPath,&aEntry.iDetails,aDateTime.Second(),aDateTime.Day()+1,aDateTime.Month(),aDateTime.Year());
	
	if(!BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),BufFileName)){
		RFile WFile;
		TRAPD(aFileError,WFile.Create(CEikonEnv::Static()->FsSession(),BufFileName,EFileWrite));
		HBufC* myBuf2 = NULL,*myBuf1 = NULL;	
		if(aFileError==KErrNone){//if it xists don't write again.
			TFileText aFile;
			aFile.Set(WFile);
			myBuf1 = HBufC::NewLC(aDetails1->Length()); 
		    myBuf1->Des().Copy(aDetails1->Des()); 
			aFile.Write(myBuf1->Des());
			
			if(aError == KErrNone){//its a text message then no error
				myBuf2 = HBufC::NewLC(aText1->Length()); 
		    	myBuf2->Des().Copy(aText1->Des()); 
				aFile.Write(myBuf2->Des());
				CleanupStack::PopAndDestroy( myBuf2 ); 						
			}
			WFile.Close();
			CleanupStack::PopAndDestroy( myBuf1 ); 	
		}
	}
	else{
		TBuf<100> aText;	
		aText.Copy(_L("Msg already saved."));			
		CEikonEnv::Static()->InfoWinL(_L("-[SMSManager]-"),aText);		
	}
	
	CleanupStack::PopAndDestroy( aDetails1 ); 	
			
	if(aError == KErrNone){//its a text message then no error	
		CleanupStack::PopAndDestroy( aText1 ); 		
	}

	VarSmsMtm = NULL;	
	VarMsgSession = NULL;
}
