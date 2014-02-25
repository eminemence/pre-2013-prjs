
#include "AutoSmsDocument.h"
#include "AutoSmsApplication.h"

// UID for the application, this should correspond to the uid defined in the mmp file
static const TUid KUidAutoSmsApp = {0x10005B78};

CApaDocument* CAutoSmsApplication::CreateDocumentL()
    {  
    // Create an AutoSms document, and return a pointer to it
    CApaDocument* document = CAutoSmsDocument::NewL(*this);
    return document;
    }

TUid CAutoSmsApplication::AppDllUid() const
    {
    // Return the UID for the AutoSms application
    return KUidAutoSmsApp;
    }

