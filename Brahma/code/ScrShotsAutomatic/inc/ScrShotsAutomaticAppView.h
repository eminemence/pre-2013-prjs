/*
 * Copyright (C) 2011-2012  Mayur Kankanwadi(eminemence@gmail.com)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */


#ifndef __SCRSHOTSAUTOMATICAPPVIEW_h__
#define __SCRSHOTSAUTOMATICAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>
#include "ScrShotsAutomaticAppUI.h"
#include "TimerX.h"

#ifdef ENABLE_THREE_SCREEN_UI
#include "ScrShotsButtonFeedback.h"
class   CScrShotsMainControl;
class   CScrShotsInfoScreen;
class   CScrShotsAppFileList;

#endif //ENABLE_THREE_SCREEN_UI

// CLASS DECLARATION
class CScrShotsAutomaticAppView : public CCoeControl
#ifdef ENABLE_THREE_SCREEN_UI
,MTimerXCallback
#endif //ENABLE_THREE_SCREEN_UI
{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CScrShotsAutomaticAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CScrShotsAutomaticAppView.
	 */
	static CScrShotsAutomaticAppView* NewL(const TRect& aRect,CScrShotsAutomaticAppUi& aAppUi, CScrShotsVectorImgLoader& aImgLoader);


	/**
	 * ~CScrShotsAutomaticAppView
	 * Virtual Destructor.
	 */
	virtual ~CScrShotsAutomaticAppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this CScrShotsAutomaticAppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();

	/**
	 * From CoeControl, HandlePointerEventL.
	 * Called by framework when a pointer touch event occurs.
	 * Note: although this method is compatible with earlier SDKs, 
	 * it will not be called in SDKs without Touch support.
	 * @param aPointerEvent the information about this event
	 */
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	
#ifdef ENABLE_THREE_SCREEN_UI
    enum TCurrentScreen{
        EMainScreen,
        EAppsListScreen,
        EFileListScreen,
        EInfoScreen
    };
	
	virtual CCoeControl * ComponentControl(TInt aIndex) const;
	virtual TInt CountComponentControls() const;	
	void HandleTimerExpireL(TInt aTimerId);
    void StartSwitchToMainControl();
    void StartSwitchToInfoScreenL();
    void StartSwitchToListScreenL(); 
    CScrShotsSettings*  GetAppSettings(){
        return iAppUi.GetAppSettings();
    }
    CScrShotsAppFileList* GetAppsFileListControl(){
        return iAppFileList;
    }
    CScrShotsInfoScreen*  GetInfoScreenControl(){
        return iInfoScreen;
    }
    CScrShotsAutomaticAppUi& GetAppUi(){
        return iAppUi;
    }
    void SettingsUpdatedL(const TChangedSettingData& aChangedSetting);
    TCurrentScreen GetCurrentScreen() { return iCurrentScreen; }
    CScrShotsMainControl* GetMainControl() { return iMainControl; }
    void StartAppMonitoring();
    void StopAppMonitoring();
    
    
#endif //ENABLE_THREE_SCREEN_UI
	
private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CScrShotsAutomaticAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * CScrShotsAutomaticAppView.
	 * C++ default constructor.
	 */
	CScrShotsAutomaticAppView(CScrShotsAutomaticAppUi& aAppUi, CScrShotsVectorImgLoader& aImgLoader);
#ifdef ENABLE_THREE_SCREEN_UI
	TInt GetFibonacciNo(TInt aSeqNum);
	void AddControlToStackL(CCoeControl* aOldControl, CCoeControl* aNewControl);
#endif 
	
private:
#ifdef ENABLE_THREE_SCREEN_UI
	enum TCurrentTransition{
	    ENone,
	    ESwitchToInfoScreen,
	    ESwitchToListScreen,
	    ESwitchToMainScreen
	};
#endif //ENABLE_THREE_SCREEN_UI
	
private:
	CScrShotsAutomaticAppUi& iAppUi;
#ifdef ENABLE_THREE_SCREEN_UI
	CScrShotsMainControl*   iMainControl;
	CScrShotsInfoScreen*    iInfoScreen;
	CScrShotsAppFileList*   iAppFileList;
	TCurrentScreen          iCurrentScreen;
	CTimerX*                iSwitchTimer;
	TInt                    iSeqNum;
	TInt                    iUsageNum;
	TInt                    iNextIncrement;
	TCurrentTransition      iCurrentTransition;
	CScrShotsActionFeedback*    iButtonVibra;
	CScrShotsVectorImgLoader& iImgLoader;
	
#endif 

};

#endif // __SCRSHOTSAUTOMATICAPPVIEW_h__
// End of File
