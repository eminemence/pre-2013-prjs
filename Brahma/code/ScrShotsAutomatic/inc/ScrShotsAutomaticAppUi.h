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


#ifndef __SCRSHOTSAUTOMATICAPPUI_h__
#define __SCRSHOTSAUTOMATICAPPUI_h__

// INCLUDES
#include <aknappui.h>
#ifdef ENABLE_SETTINGS
#include "ScrShotsSettings.h"
#endif
#include "ScrShotsProducer.h"
#include "ScrShotsDeviceWatcher.h"

// FORWARD DECLARATIONS
class CScrShotsAutomaticAppView;
class CScrShotsVectorImgLoader;
class CScrShotsStrLoader;

#ifdef ENABLE_ENGINE
//class CScrShotsProducer;
class CScrShotsConvertor;
class CScrShotsDeviceWatcher;
#endif


// CLASS DECLARATION
/**
 * CScrShotsAutomaticAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CScrShotsAutomaticAppUi : public CAknAppUi
#ifdef ENABLE_SETTINGS
,MSettingsObserver
#endif
,MScrShotProgressObserver
,MOrientationNotifier
,MDeviceWatcher
	{
public:
	// Constructors and destructor

	void ConstructL();

	CScrShotsAutomaticAppUi();

	virtual ~CScrShotsAutomaticAppUi();
	
	CScrShotsSettings* GetAppSettings(){	    return iScrShotsSettings;	}
	CScrShotsDeviceWatcher* GetDeviceWatcher(){	    return iDeviceWatcher;	}
	CScrShotsProducer* GetScrShotsProducer(){	    return iScrShotsProducer;	}
	CFont* GetCustomFont() {   return iCustomFont; }
	CFont* GetCustomSmallFont() {   return iCustomSmallFont; }
	CScrShotsStrLoader* GetStrLoader(){	    return iStrLoader;	}
    void HandleScrShotEventL(TScrShotEvent aEvent);
    TAppUiOrientation GetCurrentOrientation();
    void HandleWsEventL(const TWsEvent & aEvent, CCoeControl * aCtrl);
    void HandleDiskThresholdReachedEventL();
    void HandleMemCardEventL(TBool aMemCardAdded);
    void HandleAppInstallerEventL(TBool aAppInstalled);
    TBool DiskHasSpace();
    
private:
	// Functions from base classes
	void HandleCommandL(TInt aCommand);
	void HandleStatusPaneSizeChange();
	
#ifdef ENABLE_SETTINGS
    void HandleSettingChangeL(const TChangedSettingData& aChangedSetting);
#endif
    
    void LoadCustomFontL();
    void FreeCustomFontL();
    virtual void HandleScreenDeviceChangedL();
    void UpdateScreenOrientation();

private:
	// Data
	CScrShotsAutomaticAppView* iAppView;

#ifdef ENABLE_SETTINGS	
	CScrShotsSettings*  iScrShotsSettings;//create before the producer and convertor
#endif	
#ifdef ENABLE_ENGINE	
	CScrShotsProducer*  iScrShotsProducer;
	CScrShotsConvertor*  iScrShotsConvertor;
	CScrShotsDeviceWatcher* iDeviceWatcher;
#endif	
	CScrShotsVectorImgLoader* iVectorLoader;
	CFont*  iCustomFont;
	CFont*  iCustomSmallFont;
	TInt    iCustomFontFileID;
	CScrShotsStrLoader* iStrLoader;
	TAppUiOrientation   iChangedOrientation;

	};

#endif // __SCRSHOTSAUTOMATICAPPUI_h__
// End of File
