
#include <coemain.h>
#include <AutoSms.rsg>
#include <smutset.h>


#include "AutoSmsAppView.h"

// Standard construction sequence
CAutoSmsAppView* CAutoSmsAppView::NewL(const TRect& aRect)
    {
    CAutoSmsAppView* self = CAutoSmsAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

CAutoSmsAppView* CAutoSmsAppView::NewLC(const TRect& aRect)
    {
    CAutoSmsAppView* self = new (ELeave) CAutoSmsAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

CAutoSmsAppView::CAutoSmsAppView()
    {
	// no implementation required
    }

CAutoSmsAppView::~CAutoSmsAppView()
    {
	delete iEngine;
    }

void CAutoSmsAppView::ConstructL(const TRect& aRect)
    {
    // Create a window for this application view
    CreateWindowL();
    iWaitDialog = new(ELeave)CAknWaitDialog(NULL, ETrue );
    
	iWaitDialog->ExecuteLD(R_AKNEXNOTE_WAIT_NOTE_SOFTKEY_EMPTY );
	
    iEngine = CSmsSendingEngine::NewL(this);
//	iEngine->SetObserver(this);
    
	iWaitDialog->ProcessFinishedL();
	
    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// Draw this application's view to the screen
void CAutoSmsAppView::Draw(const TRect& /*aRect*/) const
{
    // Get the standard graphics context 
    CWindowGc& gc = SystemGc();
    
    // Gets the control's extent
    TRect rect = Rect();
    
    // Clears the screen
    gc.Clear(rect);
}

void CAutoSmsAppView::SendMessageL(TMsgType aMsgType)
{
	iEngine->SendMessageL(aMsgType);
}

void CAutoSmsAppView::HandleEngineEvent()
{
	iWaitDialog->ProcessFinishedL();
}
