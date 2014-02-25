
#ifndef __AUTOSMS_APPLICATION_H__
#define __AUTOSMS_APPLICATION_H__

#include <aknapp.h>


/*! 
  @class CAutoSmsApplication
  
  @discussion An instance of CAutoSmsApplication is the application part of the AVKON
  application framework for the AutoSms example application
  */
class CAutoSmsApplication : public CAknApplication
    {
public:  // from CAknApplication

/*! 
  @function AppDllUid
  
  @discussion Returns the application DLL UID value
  @result the UID of this Application/Dll
  */
    TUid AppDllUid() const;

protected: // from CAknApplication
/*! 
  @function CreateDocumentL
  
  @discussion Create a CApaDocument object and return a pointer to it
  @result a pointer to the created document
  */
    CApaDocument* CreateDocumentL();
    };

#endif // __AUTOSMS_APPLICATION_H__
