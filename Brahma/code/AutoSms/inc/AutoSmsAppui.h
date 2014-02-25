
#ifndef __AUTOSMS_APPUI_H__
#define __AUTOSMS_APPUI_H__

#include <aknappui.h>

// Forward reference
class CAutoSmsAppView;

/*! 
  @class CAutoSmsAppUi
  
  @discussion An instance of class CAutoSmsAppUi is the UserInterface part of the AVKON
  application framework for the AutoSms example application
  */
class CAutoSmsAppUi : public CAknAppUi
    {
public:
/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CAutoSmsAppUi object
  this needs to be public due to the way the framework constructs the AppUi 
  */
    void ConstructL();

/*!
  @function CAutoSmsAppUi
  
  @discussion Perform the first phase of two phase construction.
  This needs to be public due to the way the framework constructs the AppUi 
  */
    CAutoSmsAppUi();


/*!
  @function ~CAutoSmsAppUi
  
  @discussion Destroy the object and release all memory objects
  */
    ~CAutoSmsAppUi();


public: // from CAknAppUi
/*!
  @function HandleCommandL
  
  @discussion Handle user menu selections
  @param aCommand the enumerated code for the option selected
  */
    void HandleCommandL(TInt aCommand);

private:
/*! @var iAppView The application view */
    CAutoSmsAppView* iAppView;
    
    void ShowAbout();
	void ShowInfoDialog(TInt aTitleResId,TDesC& aInfoTxt);

    };


#endif // __AUTOSMS_APPUI_H__

