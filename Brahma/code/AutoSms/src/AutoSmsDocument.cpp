
#include "AutoSmsAppUi.h"
#include "AutoSmsDocument.h"

// Standard Symbian OS construction sequence
CAutoSmsDocument* CAutoSmsDocument::NewL(CEikApplication& aApp)
    {
    CAutoSmsDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

CAutoSmsDocument* CAutoSmsDocument::NewLC(CEikApplication& aApp)
    {
    CAutoSmsDocument* self = new (ELeave) CAutoSmsDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CAutoSmsDocument::ConstructL()
    {
	// no implementation required
    }    

CAutoSmsDocument::CAutoSmsDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
	// no implementation required
    }

CAutoSmsDocument::~CAutoSmsDocument()
    {
	// no implementation required
    }

CEikAppUi* CAutoSmsDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CAutoSmsAppUi;
    return appUi;
    }

