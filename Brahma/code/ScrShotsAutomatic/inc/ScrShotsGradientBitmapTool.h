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

#ifndef __SCRSHOTSGRADIENTBITMAPSKIN_H__
#define __SCRSHOTSGRADIENTBITMAPSKIN_H__

#include <e32base.h>
#include <w32std.h>
 
// CLASS DECLARATION
class CScrShotsGradientBitmapSkin : public CBase
{
    public:
        static CScrShotsGradientBitmapSkin* NewL(const TRect& aRect, TRgb aStartColor, TRgb aEndColor);
        ~CScrShotsGradientBitmapSkin();
        void DrawSkin(CWindowGc& aGc,const TRect aDrawRect);
        static CFbsBitmap* CreateGradientBitmapL(TRgb aGradientStartColor, TRgb aGradientEndColor, TRect aGradientRect);
        void ResizeGradientBitmap(const TRect& aRect);
    private:
        void ConstructL(const TRect& aRect);
        CScrShotsGradientBitmapSkin( TRgb aGradientStartColor, TRgb aGradientEndColor);
        void InitGradientBitmapL(const TRect& aRect);
        CFbsBitmap* GetGradientBitmap(){
            return iGradientBitmap;
        }
        
    private:
        CFbsBitmap* iGradientBitmap;
        RWsSession  iWs;
        CWsScreenDevice* iScreenDevice;//dont own dont delete
        TRgb iGradientStartColor, iGradientEndColor;
};
#endif //__SCRSHOTSGRADIENTBITMAPSKIN_H__
