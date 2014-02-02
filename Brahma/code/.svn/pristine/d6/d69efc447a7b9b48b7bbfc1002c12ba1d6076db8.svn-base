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

#include "ScrShotsInfoScreen.h"
#include "ScrShotsVectorImgLoader.h"
#include "ScrShotsCommonDefs.h"
#include "ScrShotsStrLoader.h"
#include "ScrShotsGenUtils.h"

#ifdef ENABLE_THREE_SCREEN_UI
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>

#define KLandscapeInfoTextRectHeightFactor 0.8
#define KLandscapeOptionsRectHeightFactor 0.2
#define KPotraitInfoTextRectHeightFactor 0.88
#define KPotraitOptionsRectHeightFactor 0.12

#define KOptionsBackRectWidth 0.25

#define KComponentCount 1

_LIT(KInfoFileName,"menu.html");

CScrShotsInfoScreen* CScrShotsInfoScreen::NewL(const TRect& aRect,CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader)
{
    CScrShotsInfoScreen* self = new (ELeave) CScrShotsInfoScreen(aAppView, aButtonVibra, aImgLoader);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CScrShotsInfoScreen::CScrShotsInfoScreen(CScrShotsAutomaticAppView* aAppView
        ,CScrShotsActionFeedback& aButtonVibra,CScrShotsVectorImgLoader& aImgLoader):
        iAppView(aAppView)
        ,iButtonVibra(aButtonVibra)
        ,iImgLoader(aImgLoader)
{
}

CScrShotsInfoScreen::~CScrShotsInfoScreen()
{
    if(iSkinDrawer){
        delete iSkinDrawer;
        iSkinDrawer = NULL;
    }
    
    if(iInfoCtl){
        iInfoCtl->RemoveLoadEventObserver(this);
        delete iInfoCtl;
        iInfoCtl = NULL;
    }
//    iInfoHtmlFile.Close();
}

void CScrShotsInfoScreen::Draw(const TRect& aRect) const
{
    
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    
    gc.SetPenColor(ScrShotsThemeUtils::MainScreenLineColor());
    
    // Gets the control's extent
    TRect drawRect(Rect());
//    
    iSkinDrawer->DrawSkin(gc,aRect);
    //gc.DrawRect(iOptionsRect);
//    gc.DrawRect(iOptionsExitRect);
//    gc.DrawRect(iOptionsWebpageBackRect);
//    gc.DrawRect(iOptionsWebpageForwardRect);    
//    gc.DrawRect(iOptionsBackRect);
    gc.DrawLine(TPoint(iOptionsExitRect.iBr.iX, iOptionsExitRect.iTl.iY + 8), TPoint(iOptionsExitRect.iBr.iX, iOptionsExitRect.iBr.iY - 8));
    gc.DrawLine(TPoint(iOptionsWebpageBackRect.iBr.iX, iOptionsWebpageBackRect.iTl.iY + 8), TPoint(iOptionsWebpageBackRect.iBr.iX, iOptionsWebpageBackRect.iBr.iY - 8));
    gc.DrawLine(TPoint(iOptionsWebpageForwardRect.iBr.iX, iOptionsWebpageForwardRect.iTl.iY + 8), TPoint(iOptionsWebpageForwardRect.iBr.iX, iOptionsWebpageForwardRect.iBr.iY - 8));
    
    gc.BitBlt(TPoint(iOptionsBackRect.iTl.iX, iOptionsBackRect.iTl.iY),iImgLoader.GetBitmap(EBack));
    gc.BitBlt(TPoint(iOptionsExitRect.iTl.iX, iOptionsExitRect.iTl.iY),iImgLoader.GetBitmap(EExitIcon));
    
    if(iInfoCtl->NavigationAvailable(TBrCtlDefs::ENavigationBack))
        gc.BitBlt(TPoint(iOptionsWebpageBackRect.iTl.iX, iOptionsWebpageBackRect.iTl.iY),iImgLoader.GetBitmap(EPageBack));
    else //draw the disabled back icon
        gc.BitBlt(TPoint(iOptionsWebpageBackRect.iTl.iX, iOptionsWebpageBackRect.iTl.iY),iImgLoader.GetBitmap(EDisabledPageBack));
    
    if(iInfoCtl->NavigationAvailable(TBrCtlDefs::ENavigationForward))
        gc.BitBlt(TPoint(iOptionsWebpageForwardRect.iTl.iX, iOptionsWebpageForwardRect.iTl.iY),iImgLoader.GetBitmap(EPageForward));
    else //draw the disabled forward icon
        gc.BitBlt(TPoint(iOptionsWebpageForwardRect.iTl.iX, iOptionsWebpageForwardRect.iTl.iY),iImgLoader.GetBitmap(EDisabledPageForward));
    
    CEikBorderedControl::Draw(aRect);
}

void CScrShotsInfoScreen::SizeChanged()
{
    
    ComputeRects(Rect());
    if(iSkinDrawer){
        delete iSkinDrawer;
        iSkinDrawer = NULL;
    }
    iSkinDrawer = CScrShotsGradientBitmapSkin::NewL(Rect()
            ,ScrShotsThemeUtils::BackgroundSkinGradientStartColor() 
        , ScrShotsThemeUtils::BackgroundSkinGradientEndColor());

    //DrawNow();
}

void CScrShotsInfoScreen::HandlePointerEventL(const TPointerEvent& aPointerEvent)
{
     if(iInfoTextRect.Contains(aPointerEvent.iPosition)){
         iInfoCtl->HandlePointerEventL(aPointerEvent);
    }else if(iOptionsRect.Contains(aPointerEvent.iPosition) && aPointerEvent.iType == TPointerEvent::EButton1Up){        
        if(iOptionsBackRect.Contains(aPointerEvent.iPosition)){
            iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            iAppView->StartSwitchToMainControl();
        }else if(iOptionsExitRect.Contains(aPointerEvent.iPosition)){
            iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
            HBufC* msg = iAppView->GetAppUi().GetStrLoader()->GetStringHBuf(EExitPromptString);//dont delete
            if(iAppView->GetAppUi().IsForeground() && CEikonEnv::Static()->QueryWinL(msg->Des(),KNullDesC))
                iAppView->GetAppUi().Exit();
        }else if(iOptionsWebpageBackRect.Contains(aPointerEvent.iPosition)){
            if(iInfoCtl->NavigationAvailable(TBrCtlDefs::ENavigationBack)){
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
                iInfoCtl->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandBack);
            }
        }else if(iOptionsWebpageForwardRect.Contains(aPointerEvent.iPosition)){
            if(iInfoCtl->NavigationAvailable(TBrCtlDefs::ENavigationForward)){
                iButtonVibra.VibrateforButtonPressL(this, aPointerEvent);
                iInfoCtl->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandForward);
            }
        }
    }
     DrawNow();
}

void CScrShotsInfoScreen::ConstructL(const TRect& aRect)
{
    CreateWindowL();
//    SetFocus(EFalse);
    SetRect(aRect);
    
    ComputeRects(Rect());
    
    iSkinDrawer = CScrShotsGradientBitmapSkin::NewL(aRect, ScrShotsThemeUtils::BackgroundSkinGradientStartColor(), ScrShotsThemeUtils::BackgroundSkinGradientEndColor());
    SetBorder(TGulBorder::ERaised | TGulBorder::EThreeStep 
            //| TGulBorder::EWithInline 
            | TGulBorder::EWithOutline 
            | TGulBorder::EAddTwoPixels 
            | TGulBorder::EAddFourRoundingPixels);
    
    //ActivateL();
    
    iBrCtlCapabilities = TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw |
                         TBrCtlDefs::ECapabilityGraphicalHistory | TBrCtlDefs::ECapabilityGraphicalPage | TBrCtlDefs::ECapabilityCursorNavigation;
    // TODO: need to detect 3rd Ed and use
    // TRect brCtlRect(Position(), Size());   // in 3rd Ed devices
    // need a device to test on first    
    iInfoCtl = ::CreateBrowserControlL(
            this,  iInfoTextRect,
            TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityClientNotifyURL | TBrCtlDefs::ECapabilityClientResolveEmbeddedURL,
            iCommandBase, NULL, this, NULL);
    iInfoCtl->AddLoadEventObserverL(this);
    
//    iInfoCtl->SetRect(iInfoTextRect);
    OpenInfoFileL();
//    iInfoCtl->LoadFileL(iInfoHtmlFile);
    iInfoCtl->LoadFileL(iInfoFileName);
    iInfoCtl->ActivateL();
    //iInfoCtl->DrawDeferred();
    
}

void CScrShotsInfoScreen::OpenInfoFileL()
{
    
    User::LeaveIfError(CEikonEnv::Static()->FsSession().PrivatePath(iInfoFileName));
    iInfoFileName.Append( KInfoFileName );
    iInfoFileName.Insert(0, KCDriveWithColon);
//    User::LeaveIfError(iInfoHtmlFile.Open(CEikonEnv::Static()->FsSession(), iInfoFileName, EFileRead));
}

void CScrShotsInfoScreen::ComputeRects(const TRect& aScreenRect)
{
    TInt scrWidth = aScreenRect.Width();
    TInt scrHeight = aScreenRect.Height();
    if(scrWidth > scrHeight){ //we are in landscape mode
        iInfoTextRect = TRect(TPoint(2,2),TSize(scrWidth - 8, scrHeight * KLandscapeInfoTextRectHeightFactor - 2));
        iOptionsRect = TRect(TPoint(2,iInfoTextRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KLandscapeOptionsRectHeightFactor - 2));
    }else { //we are in potrait mode
        iInfoTextRect = TRect(TPoint(2,2),TSize(scrWidth - 8, scrHeight * KPotraitInfoTextRectHeightFactor - 2));
        iOptionsRect = TRect(TPoint(2,iInfoTextRect.iBr.iY),TSize(scrWidth - 8, scrHeight * KPotraitOptionsRectHeightFactor - 2));
    }
    iOptionsBackRect = iOptionsWebpageBackRect = iOptionsWebpageForwardRect = iOptionsExitRect = iOptionsRect;
    
    iOptionsExitRect.SetWidth(iOptionsRect.Width() * KOptionsBackRectWidth);
    
    iOptionsWebpageBackRect.iTl.iX = iOptionsExitRect.iBr.iX;
    iOptionsWebpageBackRect.SetWidth(iOptionsRect.Width() * KOptionsBackRectWidth);
    
    iOptionsWebpageForwardRect.iTl.iX = iOptionsWebpageBackRect.iBr.iX;
    iOptionsWebpageForwardRect.SetWidth(iOptionsRect.Width() * KOptionsBackRectWidth);
    
    iOptionsBackRect.iTl.iX = iOptionsWebpageForwardRect.iBr.iX;
    iOptionsBackRect.SetWidth(iOptionsRect.Width() * KOptionsBackRectWidth);
    
    if(iInfoCtl)
        iInfoCtl->SetRect(iInfoTextRect);
    
    if(iAppView->GetCurrentScreen() != CScrShotsAutomaticAppView::EInfoScreen)
        return;
    //set the icons only if we are current screen
    UpdateIconSizes();
}

void CScrShotsInfoScreen::UpdateIconSizes()
{
    iImgLoader.SetIconSize(EBack,iOptionsBackRect.Size());
    iImgLoader.SetIconSize(EExitIcon ,iOptionsExitRect.Size());
    iImgLoader.SetIconSize(EPageBack ,iOptionsWebpageBackRect.Size());
    iImgLoader.SetIconSize(EPageForward ,iOptionsWebpageForwardRect.Size());
    iImgLoader.SetIconSize(EDisabledPageBack ,iOptionsWebpageBackRect.Size());
    iImgLoader.SetIconSize(EDisabledPageForward ,iOptionsWebpageForwardRect.Size());
    DrawNow();
}

CCoeControl* CScrShotsInfoScreen::ComponentControl(TInt aIndex) const 
{
    if(aIndex == 0){
        return iInfoCtl;
    }
    return NULL;
}

TInt CScrShotsInfoScreen::CountComponentControls() const 
{
    if(iInfoCtl)
        return KComponentCount;
    return 0;//wait till the browser control is fully loaded
}


TBool CScrShotsInfoScreen::ResolveEmbeddedLinkL(const TDesC& /*aEmbeddedUrl*/, const TDesC& /*aCurrentUrl*/, TBrCtlLoadContentType /*aLoadContentType*/, 
                                 MBrCtlLinkContent& /*aEmbeddedLinkContent*/)
{
    return EFalse;
}

TBool CScrShotsInfoScreen::ResolveLinkL(const TDesC& aUrl, const TDesC& /*aCurrentUrl*/, MBrCtlLinkContent& /*aBrCtlLinkContent*/)
{
    if(aUrl.Find(_L("mailto:")) == 0){
        //launch the mail app to send mail
        return ETrue;
    }
        
    return EFalse;
}

void CScrShotsInfoScreen::CancelAll()
{

}

void CScrShotsInfoScreen::HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent, TUint aSize, TUint16 aTransactionId) 
{
    if (aLoadEvent == TBrCtlDefs::EEventLoadFinished)
        DrawNow();
}


#endif //ENABLE_THREE_SCREEN_UI



