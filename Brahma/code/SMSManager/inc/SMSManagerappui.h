
#ifndef __SMSManager_APPUI_H__
#define __SMSManager_APPUI_H__

#include <aknappui.h>

class CSMSManagerAppView;

class CSMSManagerAppUi : public CAknAppUi
    {
public:
    void ConstructL();

    CSMSManagerAppUi();


    ~CSMSManagerAppUi();
    
void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);


public: // from CEikAppUi
    void HandleCommandL(TInt aCommand);

private:
    CSMSManagerAppView* iAppView;
    };


#endif // __SMSManager_APPUI_H__

