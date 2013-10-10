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

#include "ScrShotsTextLabel.h"
#include "ScrShotsGenUtils.h"
#include "ScrShotsStringPool.h"

#include <aknutils.h>

const TInt KDefaultLineSpacing = 4;
const TInt KDefaultLabelMargin = 4;
const TInt KDefaultFontSize = 12;

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif


CScrShotsTextLabel* CScrShotsTextLabel::NewL()
{
    CScrShotsTextLabel* self = new (ELeave) CScrShotsTextLabel();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CScrShotsTextLabel::~CScrShotsTextLabel()
{
    delete iLabelText;
    if(!iCustomFont)
        ScrShotsGenUtils::FreeFontL(iLabelFont);
    iLabelFont = NULL;
}

void CScrShotsTextLabel::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetPenColor(iLabelTextColor);
    TInt linespecs = iLineSpacing + iLabelFont->HeightInPixels();
    TInt maxlinesInRect = Rect().Height() / linespecs;
    TInt totalLineHeight = (iLineSpacing + iLabelFont->HeightInPixels()) * iLabelText->Count();
    //readjust the totallineHeight to adjust to the rect height 
    totalLineHeight = (totalLineHeight > Rect().Height()) ? maxlinesInRect * linespecs : totalLineHeight;
    TInt x = aRect.iTl.iX + iLabelMargin ;
    TInt y = Rect().iTl.iY + ((Rect().Height() / 2) - (totalLineHeight / 2));
    gc.UseFont(iLabelFont);
    CGraphicsContext::TTextAlign aAlignment;
    switch(iLabelAlignment){
        case CScrShotsTextLabel::ELeft:
            aAlignment = CGraphicsContext::ELeft;
            break;
        case CScrShotsTextLabel::ECenter:
            aAlignment = CGraphicsContext::ECenter;
            break;
        case CScrShotsTextLabel::ERight:
            aAlignment = CGraphicsContext::ERight;
            break;
    }
    
    for(int i = 0;i < iLabelText->Count() && i < maxlinesInRect ; i++){
        TPtrC txt(iLabelText->operator [](i));
        TRect box(TPoint(x,y), TPoint(x+Rect().Width() - iLabelMargin,y+iLabelFont->HeightInPixels()));
        //void DrawText(const TDesC &aText, const TRect &aBox, TInt aBaselineOffset, TTextAlign aAlignment=ELeft, TInt aLeftMargin=0)=0;
        if(i == maxlinesInRect - 1 && iLabelText->Count() > maxlinesInRect){
            //add trailing '...' to indicate a value longer than can be displayed
            TBuf<256> msg;
            msg.Copy(txt.Left( txt.Length() - 3));
            msg.Append(_L("..."));
            gc.DrawText(msg, box, iLabelFont->HeightInPixels(),aAlignment);
        }
        else
            gc.DrawText(txt, box, iLabelFont->HeightInPixels(),aAlignment);
        y += iLineSpacing + iLabelFont->HeightInPixels();
    }
    gc.DiscardFont();
}

void CScrShotsTextLabel::SizeChanged()
{

}

CScrShotsTextLabel::CScrShotsTextLabel():
iLineSpacing(KDefaultLineSpacing),
iLabelAlignment(ELeft),
iLabelMargin(KDefaultLabelMargin),
iChangeFontSizeToFitText(EFalse),
iLabelTextColor(KRgbRed),
iFontSize(KDefaultFontSize)
{

}

void CScrShotsTextLabel::ConstructL()
{
    iLabelText = new (ELeave) CDesCArrayFlat(4);
    iLabelFont = ScrShotsGenUtils::InitFontL(iFontSize);
}

void CScrShotsTextLabel::SetTextL(const TDesC& aText)
{
    TInt linewidth = Rect().Size().iWidth - (2 * iLabelMargin);
    //remove the prev text
    iLabelText->Reset();

    // this is how we will parse the text
    // check if there are any new lines
    // if yes, extract that line 
    // then make sure it fits in a line, else break further into the next line
    // do the above till text exhausts
    TPtrC txtPtr(aText);
    TPtrC srcPtr;
    CArrayFix<TPtrC>* tmpArray = new (ELeave) CArrayFixFlat<TPtrC>(2);
    TBool breakNow = EFalse;
    while(!breakNow){
        TInt pos = txtPtr.Find(_L("\n"));
        if(pos != KErrNotFound){
            srcPtr.Set(txtPtr.Left(pos));
            txtPtr.Set(txtPtr.Right(txtPtr.Length() - (pos+1)));
        }
        else{
            srcPtr.Set(txtPtr);
            breakNow = ETrue;
        }
        TRACE_ON(RDebug::Print(_L("Txt : %S"),&srcPtr));
        //now break the current string into multiple lines if required.
        TRAPD(errCode,AknTextUtils::WrapToArrayL(srcPtr, linewidth, *iLabelFont, *tmpArray));
        for(int i = 0; i < tmpArray->Count() ; i++){
            HBufC* txt = tmpArray->At(i).Alloc();
            iLabelText->AppendL(txt->Des());
            delete txt;
        }
        tmpArray->Reset();
    }
    delete tmpArray;
    
    
}

void CScrShotsTextLabel::SetFontL(CFont* aLabelFont)
{
    if(!iCustomFont)
        ScrShotsGenUtils::FreeFontL(iLabelFont);
    iCustomFont = ETrue;
    iLabelFont = aLabelFont;
}

void CScrShotsTextLabel::SetLineSpacing(const TInt& aLineSpacing)
{
    iLineSpacing = aLineSpacing;
}

void CScrShotsTextLabel::SetLabelMargin(const TInt& aLabelMargin)
{
    iLabelMargin = aLabelMargin;
}

void CScrShotsTextLabel::SetLabelTextAlignment(CScrShotsTextLabel::TLabelTextAlign aLabelAlignment)
{
    iLabelAlignment = aLabelAlignment;
}

void CScrShotsTextLabel::SetChangeFontSizeToFitTextL(TBool aEnable)
{
    iChangeFontSizeToFitText = aEnable;
}

void CScrShotsTextLabel::SetTextColor(const TRgb& aTextColor)
{
    iLabelTextColor = aTextColor;
}

void CScrShotsTextLabel::SetFontSize(const TInt& aFontSize)
{
    if(!iCustomFont){
        if(iLabelFont)
            ScrShotsGenUtils::FreeFontL(iLabelFont);
        iLabelFont = ScrShotsGenUtils::InitFontL(aFontSize);
    }
}

void CScrShotsTextLabel::TrimFolderNameToShowLatestFolderL(TDesC& aFolderValue, TDes& aTrimmedFolderValue)
{
//    if(aFolderValue.Length() < aMaxFolderLength){
//        aTrimmedFolderValue.Copy(aFolderValue);
//    }
//    else{
//        TStringPoolObj finalVal(aFolderValue.Length());
//        TInt pos = aFolderValue.LocateReverse('\\');
//        TPtrC dirVal = aFolderValue.Right(pos);
//        pos = dirVal.LocateReverseF('\\');
//        dirVal.Set(dirVal.Right('\\'));
//        finalVal.Des().Copy(dirVal);
//    }
}
