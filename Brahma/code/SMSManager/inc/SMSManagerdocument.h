
#ifndef __SMSManager_DOCUMENT_H__
#define __SMSManager_DOCUMENT_H__


#include <akndoc.h>

// Forward references
class CSMSManagerAppUi;
class CEikApplication;


class CSMSManagerDocument : public CAknDocument
    {
public:

    static CSMSManagerDocument* NewL(CEikApplication& aApp);

    static CSMSManagerDocument* NewLC(CEikApplication& aApp);

    ~CSMSManagerDocument();

    CEikAppUi* CreateAppUiL();
private:

    void ConstructL();

    CSMSManagerDocument(CEikApplication& aApp);

    };


#endif // __SMSManager_DOCUMENT_H__
