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

#include "ScrShotsGradientBitmapTool.h"
#include <mw/gulutil.h> 
#include <eikenv.h>

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif

CScrShotsGradientBitmapSkin* CScrShotsGradientBitmapSkin::NewL(const TRect& aRect, TRgb aStartColor, TRgb aEndColor)
{
    CScrShotsGradientBitmapSkin* self = new (ELeave) CScrShotsGradientBitmapSkin(aStartColor, aEndColor);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CScrShotsGradientBitmapSkin::~CScrShotsGradientBitmapSkin()
{
    if(iGradientBitmap){
        delete iGradientBitmap;
        iGradientBitmap = NULL;
    }
    iWs.Close();
}

void CScrShotsGradientBitmapSkin::ConstructL(const TRect& aRect)
{
    User::LeaveIfError(iWs.Connect());
    InitGradientBitmapL(aRect);
}

CScrShotsGradientBitmapSkin::CScrShotsGradientBitmapSkin(TRgb aGradientStartColor, TRgb aGradientEndColor):
        iGradientStartColor(aGradientStartColor)
        ,iGradientEndColor(aGradientEndColor)
,iScreenDevice(CEikonEnv::Static()->ScreenDevice())
{

}

void CScrShotsGradientBitmapSkin::InitGradientBitmapL(const TRect& aRect)
{
    iGradientBitmap = new (ELeave) CFbsBitmap;
    iGradientBitmap->Create(aRect.Size(),iScreenDevice->DisplayMode());
    TRAPD(errCode,ColorUtils::CreateGradientBitmapL(*iGradientBitmap,iWs,aRect.Height() + 100
            , ColorUtils::EBitmapOrientationVertical, iGradientStartColor,iGradientEndColor));
}

void CScrShotsGradientBitmapSkin::ResizeGradientBitmap(const TRect& aRect)
{
    delete iGradientBitmap;
    iGradientBitmap = new (ELeave) CFbsBitmap;
    iGradientBitmap->Create(aRect.Size(),iScreenDevice->DisplayMode());
    TRAPD(errCode,ColorUtils::CreateGradientBitmapL(*iGradientBitmap,iWs,aRect.Height() + 100
            , ColorUtils::EBitmapOrientationVertical, iGradientStartColor,iGradientEndColor));
    
}

void CScrShotsGradientBitmapSkin::DrawSkin(CWindowGc& aGc,const TRect aDrawRect)
{
    TRACE_ON(RDebug::Printf("CScrShotsGradientBitmapSkin::DrawSkin::aDrawRect : %d %d %d %d", aDrawRect.iTl.iX, aDrawRect.iTl.iY, aDrawRect.Width(), aDrawRect.Height());)
    aGc.SetBrushStyle(CWindowGc::EPatternedBrush);
    aGc.UseBrushPattern(iGradientBitmap);
    aGc.DrawRect(aDrawRect);
    aGc.DiscardBrushPattern();
//    aGc.SetBrushColor(TRgb(115,115,115));
//    aGc.SetBrushStyle(CWindowGc::ESolidBrush);
//    aGc.DrawRect(aDrawRect);
}

CFbsBitmap* CScrShotsGradientBitmapSkin::CreateGradientBitmapL(TRgb aGradientStartColor
        , TRgb aGradientEndColor, TRect aGradientRect)
{
    CFbsBitmap* gradientBitmap = new (ELeave) CFbsBitmap;
    gradientBitmap->Create(aGradientRect.Size(),CEikonEnv::Static()->ScreenDevice()->DisplayMode());
    TRAPD(errCode,ColorUtils::CreateGradientBitmapL(*gradientBitmap,CEikonEnv::Static()->WsSession(),aGradientRect.Height() + 100
            , ColorUtils::EBitmapOrientationVertical, aGradientStartColor, aGradientEndColor));
    return gradientBitmap;
}
