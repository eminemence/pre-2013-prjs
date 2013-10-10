/*
 * Copyright (C) 2011-2012  Mayur Kankanwadi(eminemence@gmail.com)
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

#include "ScrShotsConvertor.h"
#include "ScrShotsSettings.h"
#include "EventLogger.h"

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif


#ifdef ENABLE_ENGINE
//check the bitmap queue every 0.50 seconds
const TInt KWaitDurationInMicroSec = 500000;

CScrShotsConvertor* CScrShotsConvertor::NewL(CScrShotsSettings* aScrShotsSettings,CScrShotsProducer* aScrShotsProducer)
{
	CScrShotsConvertor* self = new (ELeave) CScrShotsConvertor(aScrShotsSettings,aScrShotsProducer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CScrShotsConvertor::~CScrShotsConvertor()
{
	Cancel();
	iWaitDuration.Close();
	delete iImageConvertor;
	iImageConvertor = NULL;
	iOutputFsSession.Close();
	if(iInputBitmap){
	    delete iInputBitmap;
	    iInputBitmap = NULL;
	}
}

CScrShotsConvertor::CScrShotsConvertor(CScrShotsSettings* aScrShotsSettings,CScrShotsProducer* aScrShotsProducer):
		CActive(EPriorityStandard)
		,iScrShotsSettings(aScrShotsSettings)
		,iScrShotsProducer(aScrShotsProducer)
{
	CActiveScheduler::Add(this);
}

void CScrShotsConvertor::ConstructL()
{
	iWaitDuration.CreateLocal();
	User::LeaveIfError(iOutputFsSession.Connect());
	FireTimerL();
}

void CScrShotsConvertor::RunL()
{
	if(iStatus.Int() == KErrNone){
		switch(iConvertorMode){
			case ETimerMode:{
				CheckQueueForItemsToProcessL();
			}
			break;
			case EImageConvertorMode:{
				//delete that item from the list
			    if(iScrShotsProducer->iImgList.Count()){
			        delete iInputBitmap;
			        iInputBitmap = NULL;
			        iScrShotsProducer->DeleteHeadAndRemoveRawFileL();
			        CheckQueueForItemsToProcessL();
			    }
			    else{
			        //wait for some time before processing the next item
			        FireTimerL();
			    }
			}
			break;
		}
	}
	else{
	    TRACE_ON(RDebug::Printf("------Image convert error : %d ---------",iStatus.Int()));
	}
}

void CScrShotsConvertor::DoCancel()
{
    switch(iConvertorMode){
        case ETimerMode:
            iWaitDuration.Cancel();
            break;
        case EImageConvertorMode:
            break;
    }
}

void CScrShotsConvertor::FireTimerL()
{
    if(IsActive())
        return;
    iConvertorMode = ETimerMode;
    iWaitDuration.After(iStatus, TTimeIntervalMicroSeconds32(KWaitDurationInMicroSec));
//    SLOGF(_L("CAlling SetActive in CTimerX::FireTimerL"));
    SetActive();
}

void CScrShotsConvertor::CheckQueueForItemsToProcessL()
{
	if(iScrShotsProducer->iImgList.Count()){
	    iInputFileName.Copy(iScrShotsProducer->iImgList[0]->Des());
		ProcessImgFileL();
	}
	else
	    FireTimerL();
}

void CScrShotsConvertor::ProcessImgFileL()
{
    //open the bitmap file and load in a CFbsBitmap
    //generate the output filename
    //open the image encoder 
    //get the target conversion format from the settings
    //fire the convert operation
    TBuf<8> fileExit;
    TBuf8<32> mimeType;
    if(iInputBitmap){
        delete iInputBitmap;
        iInputBitmap = NULL;
    }
    iInputBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iInputBitmap->Load(iInputFileName,0,EFalse));
    
    //creating the outputfilename
    iScrShotsSettings->GetSaveImageFormatExtStringL(fileExit);
    iScrShotsSettings->GetSaveImageFormatMimeTypeStringL(mimeType);
    iOutputFileName.Copy(iInputFileName.Left(iInputFileName.LocateReverse(TChar('.'))));
    iOutputFileName.Append(fileExit);
    
    //get the image encoder ready
    if(iImageConvertor){
         delete iImageConvertor;
         iImageConvertor = NULL;
    }
    iImageConvertor = CImageEncoder::FileNewL(iOutputFsSession, iOutputFileName, mimeType);
    iImageConvertor->Convert(&iStatus, *iInputBitmap);
    SLOGF(_L("CAlling SetActive in  CScrShotsConvertor::ProcessImgFileL"));
    SetActive();
    iConvertorMode = EImageConvertorMode;
}

void CScrShotsConvertor::StartImageConsumerL()
{
    FireTimerL();
}

void CScrShotsConvertor::StopImageConsumerL()
{
    Cancel();
}
#endif
