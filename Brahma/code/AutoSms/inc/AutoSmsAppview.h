
#ifndef __AUTOSMS_APPVIEW_H__
#define __AUTOSMS_APPVIEW_H__


#include <coecntrl.h>
#include "SmsEngine.h"
#include <aknwaitdialog.h> 
/*! 
  @class CAutoSmsAppView
  
  @discussion An instance of the Application View object for the AutoSms 
  example application
  */
class CAutoSmsAppView : public CCoeControl,public MSmsSendingEngineObserver
    {
public:

/*!
  @function NewL
   
  @discussion Create a CAutoSmsAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CAutoSmsAppView
  */
    static CAutoSmsAppView* NewL(const TRect& aRect);

/*!
  @function NewLC
   
  @discussion Create a CAutoSmsAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CAutoSmsAppView
  */
    static CAutoSmsAppView* NewLC(const TRect& aRect);


/*!
  @function ~CAutoSmsAppView
  
  @discussion Destroy the object and release all memory objects
  */
     ~CAutoSmsAppView();


public:  // from CCoeControl
/*!
  @function Draw
  
  @discussion Draw this CAutoSmsAppView to the screen
  @param aRect the rectangle of this view that needs updating
  */
    void Draw(const TRect& aRect) const;
    
    void HandleEngineEvent();
    
    
  

private:

/*!
  @function ConstructL
  
  @discussion  Perform the second phase construction of a CAutoSmsAppView object
  @param aRect the rectangle this view will be drawn to
  */
    void ConstructL(const TRect& aRect);

/*!
  @function CAutoSmsAppView
  
  @discussion Perform the first phase of two phase construction 
  */
    CAutoSmsAppView();
    
	public:
		void SendMessageL(TMsgType aMsgType);
		
	private:
		CSmsSendingEngine* iEngine;
		CAknWaitDialog* iWaitDialog; 		
};


#endif // __AUTOSMS_APPVIEW_H__
