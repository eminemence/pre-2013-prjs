
#ifndef __AUTOSMS_DOCUMENT_H__
#define __AUTOSMS_DOCUMENT_H__


#include <akndoc.h>

// Forward references
class CAutoSmsAppUi;
class CEikApplication;


/*! 
  @class CAutoSmsDocument
  
  @discussion An instance of class CAutoSmsDocument is the Document part of the AVKON
  application framework for the AutoSms example application
  */
class CAutoSmsDocument : public CAknDocument
    {
public:

/*!
  @function NewL
  
  @discussion Construct a CAutoSmsDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CAutoSmsDocument
  */
    static CAutoSmsDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CAutoSmsDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CAutoSmsDocument
  */
    static CAutoSmsDocument* NewLC(CEikApplication& aApp);

/*!
  @function ~CAutoSmsDocument
  
  @discussion Destroy the object and release all memory objects
  */
    ~CAutoSmsDocument();

public: // from CAknDocument
/*!
  @function CreateAppUiL 
  
  @discussion Create a CAutoSmsAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();

private:

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CAutoSmsDocument object
  */
    void ConstructL();

/*!
  @function CAutoSmsDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CAutoSmsDocument(CEikApplication& aApp);

    };


#endif // __AUTOSMS_DOCUMENT_H__
