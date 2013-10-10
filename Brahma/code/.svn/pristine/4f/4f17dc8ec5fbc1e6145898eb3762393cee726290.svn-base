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

#ifndef __SCRSHOTSGENUTILS_H__
#define __SCRSHOTSGENUTILS_H__

#include <e32base.h>
#include <gdi.h>

#include "ScrShotsStrLoader.h"
#include "nessie.h"

typedef RArray<TChar > RDriveList;
class CScrShotsSettings;

//providing a set of generic utitily functions for the screen shot app

class ScrShotsGenUtils
{
    public:
        static RDriveList GetDriveListL();
        static void GenerateUniqueFileNameL(const TDesC& aNameSeries, TDes& aOutFileName);
        static TBool ChkExistenceAndCreateFolderL(const TDesC& aFolderName,const TDesC& aTargetDrive);
        static void BlinkKeypadLight();
        static TBool DriveExists(TChar aDriveChar);
        static CFont* InitFontL(TInt aFontSize);
        static void FreeFontL(CFont* aFont);
        static void SetFreeSizeText(CScrShotsStrLoader* strLoader, TDes& aFreeSizeText, TInt64 aDriveFreeSize);
        static void DumpJsFileL(CScrShotsSettings* scrShotsSettings);
        static TBool CheckIfAppExists(const TDesC& aAppName, const TUid& aAppUid);
};

enum TScrShotsTheme{
	TDefaultTheme,
    TGrayTheme,
    TOrangeTheme,
    TBlueTheme
};

//class to maintain the current color scheme / theme
class ScrShotsThemeUtils
{
    public:
        static  TScrShotsTheme GetCurrentTheme();
        static void SetCurrentTheme(TScrShotsTheme aTheme);
        static const TRgb DriveListSelectIconColor();
        static const TRgb DriveListTextColor();
        static const TRgb MainScreenTextColor(){    return TRgb(108,192,99);}
        static const TRgb MainScreenGradientStartColor(){	return TRgb(230,230,230);}
        static const TRgb MainScreenGradientEndColor(){ return TRgb(115,115,115);}
        static const TRgb MainScreenLineColor(){    return TRgb(255,255,153); }
        static const TRgb ListBoxHighlightColor(){    return TRgb(236,118,9); }
        static const TRgb SelectedGridCellColor(){    return TRgb(25,255,25); }
        static const TRgb UnselectedGridCellColor(){  return TRgb(140,140,140);   }
        static const TRgb ListBoxNormalItemTextColor() { return TRgb(106,176,72); }
        static const TRgb ListBoxHighlightedItemTextColor() { return TRgb(0,64,128); } 
        static const TRgb BackgroundSkinGradientStartColor() { return TRgb(20,37,64); }
        static const TRgb BackgroundSkinGradientEndColor() { return TRgb(121,153,168); }
        
    private:
        static TScrShotsTheme iCurrentTheme;
};

#endif //__SCRSHOTSGENUTILS_H__
