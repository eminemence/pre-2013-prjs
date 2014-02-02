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

#ifndef __SCRSHOTSTEXTLABEL_H__
#define __SCRSHOTSTEXTLABEL_H__

#include <coecntrl.h>


class CScrShotsTextLabel : public CCoeControl
{
    public:
        enum TLabelTextAlign{
            ELeft,
            ECenter,
            ERight
        };

    public:
        static CScrShotsTextLabel* NewL();
        ~CScrShotsTextLabel();
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();
        // text set with \n will be parsed and taken as a separate line
        void SetTextL(const TDesC& aText);
        void SetFontL(CFont* aLabelFont);
        void SetLineSpacing(const TInt& aLineSpacing);
        void SetLabelMargin(const TInt& aLabelMargin);
        void SetLabelTextAlignment(CScrShotsTextLabel::TLabelTextAlign aLabelAlignment);
        void SetChangeFontSizeToFitTextL(TBool aEnable);
        void SetTextColor(const TRgb& aTextColor);
        //works only on the default label font
        void SetFontSize(const TInt& aFontSize);
        void TrimFolderNameToShowLatestFolderL(TDesC& aFolderValue, TDes& aTrimmedFolderValue);
        
    private:
        CScrShotsTextLabel();
        void ConstructL();
        
    private:
        CDesCArray* iLabelText;
        CFont*  iLabelFont;
        TInt    iLineSpacing;
        TLabelTextAlign     iLabelAlignment;
        TInt    iLabelMargin;
        TBool   iChangeFontSizeToFitText;
        TRgb    iLabelTextColor;
        TInt    iFontSize;
        TBool   iCustomFont;
};


#endif //__SCRSHOTSTEXTLABEL_H__

