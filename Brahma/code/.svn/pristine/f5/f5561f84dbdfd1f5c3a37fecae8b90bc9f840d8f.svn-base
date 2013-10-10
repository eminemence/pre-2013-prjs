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

#ifndef __SCRSHOTSINFOSCREEN_H__
#define __SCRSHOTSINFOSCREEN_H__

#ifdef ENABLE_THREE_SCREEN_UI
#include <coecntrl.h>
#include <mw/eikbctrl.h> 
#include "ScrShotsGradientBitmapTool.h"
#include <brctlinterface.h>
#include "ScrShotsAutomaticAppView.h" 
#include <mw/brctllinkresolver.h> 


class CScrShotsVectorImgLoader;

// CLASS DECLARATION
class CScrShotsInfoScreen : public CEikBorderedControl, MBrCtlLinkResolver ,public MBrCtlLoadEventObserver
{
    public:
        static CScrShotsInfoScreen* NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView
                ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        virtual ~CScrShotsInfoScreen();
    
    public:
        // Functions from base classes
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();
        virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
        virtual CCoeControl* ComponentControl(TInt aIndex) const;
        virtual TInt CountComponentControls() const;
        void UpdateIconSizes();
        void ReloadHtmlContent(){
            if(iInfoCtl)
                iInfoCtl->LoadFileL(iInfoFileName);
        }
        
        
    private:
        void ConstructL(const TRect& aRect);
        CScrShotsInfoScreen(CScrShotsAutomaticAppView* aAppView,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader);
        void ComputeRects(const TRect& aScreenRect);
        void OpenInfoFileL();
        
        virtual TBool ResolveEmbeddedLinkL(const TDesC& aEmbeddedUrl, const TDesC& aCurrentUrl, TBrCtlLoadContentType aLoadContentType, 
                                         MBrCtlLinkContent& aEmbeddedLinkContent); 
        virtual TBool ResolveLinkL(const TDesC& aUrl, const TDesC& aCurrentUrl, MBrCtlLinkContent& aBrCtlLinkContent);
        virtual void CancelAll();
        //from MBrCtlLoadEventObserver
        virtual void HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent, TUint aSize, TUint16 aTransactionId) ;

        
    private:
        CScrShotsGradientBitmapSkin* iSkinDrawer;
        TRect iInfoTextRect, iOptionsRect;
        TRect iOptionsBackRect, iOptionsWebpageBackRect, iOptionsWebpageForwardRect, iOptionsExitRect;
        TInt iCommandBase;
        // as its a private file so we will load it and then pass the handle to the browser control
        //RFile iInfoHtmlFile;
        CBrCtlInterface*    iInfoCtl;
        CScrShotsAutomaticAppView* iAppView;//dont own
        CScrShotsActionFeedback& iButtonVibra;
        CScrShotsVectorImgLoader& iImgLoader;
        TUint iBrCtlCapabilities;
        TFileName iInfoFileName;
        
        
};
#endif //ENABLE_THREE_SCREEN_UI

#endif //__SCRSHOTSINFOSCREEN_H__
