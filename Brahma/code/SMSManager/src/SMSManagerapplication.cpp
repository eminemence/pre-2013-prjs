
#include "SMSManagerDocument.h"
#include "SMSManagerApplication.h"

// local constants
static const TUid KUidSMSManagerApp = {0x10005b65}; 

CApaDocument* CSMSManagerApplication::CreateDocumentL()
    {  
    // Create an SMSManager document, and return a pointer to it
    CApaDocument* document = CSMSManagerDocument::NewL(*this);
    return document;
    }

TUid CSMSManagerApplication::AppDllUid() const
    {
    // Return the UID for the SMSManager application
    return KUidSMSManagerApp;
    }

