#include "SMSManagerAppUi.h"
#include "SMSManagerDocument.h"


// Standard Symbian OS construction sequence
CSMSManagerDocument* CSMSManagerDocument::NewL(CEikApplication& aApp)
    {
    CSMSManagerDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

CSMSManagerDocument* CSMSManagerDocument::NewLC(CEikApplication& aApp)
    {
    CSMSManagerDocument* self = new (ELeave) CSMSManagerDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CSMSManagerDocument::ConstructL()
    {
    // Add any construction that can leave here
    }    

CSMSManagerDocument::CSMSManagerDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
    // Add any construction that can not leave here
    }   

CSMSManagerDocument::~CSMSManagerDocument()
    {
    // Any destruction code here
    }

CEikAppUi* CSMSManagerDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it
    CEikAppUi* appUi = new (ELeave) CSMSManagerAppUi;
    return appUi;
    }

