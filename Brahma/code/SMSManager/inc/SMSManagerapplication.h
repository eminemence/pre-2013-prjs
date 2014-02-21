
#ifndef __SMSManager_APPLICATION_H__
#define __SMSManager_APPLICATION_H__

#include <aknapp.h>


class CSMSManagerApplication : public CAknApplication
    {
public:  // from CApaApplication

    TUid AppDllUid() const;

protected: // from CEikApplication
    CApaDocument* CreateDocumentL();
    };

#endif // __SMSManager_APPLICATION_H__
