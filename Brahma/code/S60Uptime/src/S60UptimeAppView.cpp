/*
 * Copyright (C) 2011  Mayur Kankanwadi(eminemence@gmail.com)
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


// INCLUDE FILES
#include <coemain.h>
#include <eikenv.h>
#include "S60UptimeAppView.h"
#include <AknUtils.h> 


// drawing colors, active option is green color
// inactive color is grey
const TRgb KActiveColor(38,127,0);
const TRgb KDisableColor(192,192,192);

// message string literals
_LIT(KFormatMsgFigures,"%02d day(s) %02d hrs %02d mins %02d secs");
_LIT(KHeaderMsg1,"flaminghorns.com - S60Uptime");
_LIT(KHeaderMsg2,"Measure of Symbian stability.");
_LIT(KNoOfSecs,"or %d secs since last boot.");
_LIT(KCurrentMode,"Current update mode : %s");
_LIT(KPerHour,"Per Hour");
_LIT(KPerMin,"Per Minute");
_LIT(KPerSec,"Per Second");

#ifdef  __SERIES60_3X__
_LIT(KKeyBasedOption1,"Press 1 for uptime refresh/second.");
_LIT(KKeyBasedOption2,"Press 2 for uptime refresh/minute.");
_LIT(KKeyBasedOption3,"Press 3 for uptime refresh/hour.");
#else
_LIT(KKeyBasedOption1,"Uptime refresh/second active.");
_LIT(KKeyBasedOption2,"Uptime refresh/minute active.");
_LIT(KKeyBasedOption3,"Uptime refresh/hour active.");
#endif

const TUint KClientRectOffset = 25;
const TUint KFirstDrawDurationTimer = 200 * 1000;// 1/5 second
const TUint KDrawSecDurationTimer = 1;// 1 second
const TUint KDrawMinDurationTimer = 1;// 1 min
const TUint KDrawHourDurationTimer = 60;// 60 mins

const TUint KDistanceBetweenLines = 5;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CS60UptimeAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CS60UptimeAppView* CS60UptimeAppView::NewL(const TRect& aRect)
	{
	CS60UptimeAppView* self = CS60UptimeAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CS60UptimeAppView* CS60UptimeAppView::NewLC(const TRect& aRect)
	{
	CS60UptimeAppView* self = new (ELeave) CS60UptimeAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

void CS60UptimeAppView::CalculateCoords(const TRect& aRect)
{
	TInt halfScrWidth = CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth / 2;
	iHeaderPt1.iY = KClientRectOffset;
	iHeaderPt1.iX = (halfScrWidth) 
					- (iOptionsFont->TextWidthInPixels(KHeaderMsg1)/2);

	iHeaderPt2.iY = iHeaderPt1.iY + iOptionsFont->HeightInPixels() + KDistanceBetweenLines;
	iHeaderPt2.iX = (halfScrWidth) 
					- (iOptionsFont->TextWidthInPixels(KHeaderMsg2)/2);

	
	iMsgPt.iY = iHeaderPt2.iY + iUptimeFont->HeightInPixels() + KDistanceBetweenLines;
	iNoOfSecsPt.iY = iMsgPt.iY + iUptimeFont->HeightInPixels() + KDistanceBetweenLines;
	
	iOptionPt1.iY = iNoOfSecsPt.iY + iOptionsFont->HeightInPixels() + (KDistanceBetweenLines * 2);
	iOptionPt1.iX = halfScrWidth - (iOptionsFont->TextWidthInPixels(KKeyBasedOption1)/2);
	
	iOptionPt2.iY = iOptionPt1.iY + iOptionsFont->HeightInPixels() + KDistanceBetweenLines;
	iOptionPt2.iX = halfScrWidth - (iOptionsFont->TextWidthInPixels(KKeyBasedOption2)/2);

	iOptionPt3.iY = iOptionPt2.iY + iOptionsFont->HeightInPixels() + KDistanceBetweenLines;
	iOptionPt3.iX = halfScrWidth - (iOptionsFont->TextWidthInPixels(KKeyBasedOption3)/2);

	
	iMsgPt.iX = (CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth / 2) 
			- (iUptimeFont->TextWidthInPixels(iMsg)/2);

	iNoOfSecsPt.iX = (CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth / 2) 
					- (iUptimeFont->TextWidthInPixels(iMsg2)/2);

	
}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppView::ConstructL(const TRect& aRect)
{
	CalculateCoords(aRect);
	
	//init the timer
	iDrawTimer = CTimerX::NewL(this);
	
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	
	//start the timer
	iDrawTimer->StartSecondTimerL(KDrawSecDurationTimer, ETrue);
}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::CS60UptimeAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CS60UptimeAppView::CS60UptimeAppView():
		iUptimeFont(CEikonEnv::Static()->DenseFont())
		,iOptionsFont(CEikonEnv::Static()->DenseFont())
		,iIsTouchDevice(
#ifdef __SERIES60_3X__				
				EFalse			
#else
				//as PenEnabled has been added from 3.1 sdk only
				AknLayoutUtils::PenEnabled()
#endif
)
,iCurrentMode(ESecondMode)
{
	UserHal::TickPeriod(iTickPeriod);
	// No implementation required
}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::~CS60UptimeAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CS60UptimeAppView::~CS60UptimeAppView()
{
	iDrawTimer->StopTimerL();
	delete iDrawTimer;
	iDrawTimer = NULL;
}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppView::Draw(const TRect& /*aRect*/) const
	{
	
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();
	gc.SetBrushColor(KRgbBlack);
	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);
	gc.UseFont(iOptionsFont);
	gc.SetPenColor(TRgb(255,117,5));
	gc.DrawText(KHeaderMsg1(), iHeaderPt1);
	gc.DrawText(KHeaderMsg2(), iHeaderPt2);
	
	gc.SetPenColor(KRgbWhite);
	
	if(iCurrentMode == ESecondMode){
#ifdef __SERIES60_3X__		
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption1(), iOptionPt1);
		gc.SetPenColor(KDisableColor);
		gc.DrawText(KKeyBasedOption2(), iOptionPt2);
		gc.DrawText(KKeyBasedOption3(), iOptionPt3);
#else
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption1(), iOptionPt1);
#endif
	}
	else if(iCurrentMode == EMinMode){
#ifdef __SERIES60_3X__		
		gc.SetPenColor(KDisableColor);
		gc.DrawText(KKeyBasedOption1(), iOptionPt1);
		gc.DrawText(KKeyBasedOption3(), iOptionPt3);
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption2(), iOptionPt2);
#else
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption2(), iOptionPt1);		
#endif
		
	}
	else if (iCurrentMode == EHourMode){
#ifdef __SERIES60_3X__		
		gc.SetPenColor(KDisableColor);
		gc.DrawText(KKeyBasedOption1(), iOptionPt1);
		gc.DrawText(KKeyBasedOption2(), iOptionPt2);
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption3(), iOptionPt3);
#else
		gc.SetPenColor(KActiveColor);
		gc.DrawText(KKeyBasedOption3(), iOptionPt1);		
#endif
	}
	
	gc.DiscardFont();
	
	gc.UseFont(iUptimeFont);
	gc.SetPenColor(KRgbWhite);
	gc.DrawText(iMsg, iMsgPt);
	gc.DrawText(iMsg2, iNoOfSecsPt);
	gc.DiscardFont();
	

	}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppView::SizeChanged()
{
	CalculateCoords(Rect());
	DrawNow();
}

// -----------------------------------------------------------------------------
// CS60UptimeAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CS60UptimeAppView::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{

	// Call base class HandlePointerEventL()
	CCoeControl::HandlePointerEventL(aPointerEvent);
	}

void CS60UptimeAppView::CalculateUpTime()
{
	TUint noOfSecsSinceStartUp = 0,noOfMinsSinceStartUp = 0,noOfHoursSinceStartUp = 0,noOfDaysSinceStartUp = 0;
	//--startup time code starts here --
	TUint tickCount = User::TickCount();
	TUint noOfTicksPerSecond = (1000 * 1000) / iTickPeriod.Int();
	//noOfMicroSecsSinceStartup = tickCount * microSec.Int();
	noOfSecsSinceStartUp = tickCount / noOfTicksPerSecond;//noOfMicroSecsSinceStartup / (1000 * 1000);
	noOfMinsSinceStartUp = noOfSecsSinceStartUp/ 60;
	noOfHoursSinceStartUp = noOfMinsSinceStartUp / 60;
	noOfDaysSinceStartUp = noOfHoursSinceStartUp / 24;
	//--startup time code ends here --

	iMsg.Format(KFormatMsgFigures,noOfDaysSinceStartUp,noOfHoursSinceStartUp % 24
			,noOfMinsSinceStartUp % 60,noOfSecsSinceStartUp % 60);
	
	iMsg2.Format(KNoOfSecs,noOfSecsSinceStartUp);
	

}

void CS60UptimeAppView::UpdateUpTime()
{
	CalculateUpTime();
	
	CalculateCoords(Rect());

	DrawNow();

}

void CS60UptimeAppView::HandleTimerExpireL(TInt aTimerId)
{
	UpdateUpTime();
}

void CS60UptimeAppView::SetSecondUpdate()
{
	iDrawTimer->StopTimerL();
	iDrawTimer->StartSecondTimerL(KDrawSecDurationTimer, ETrue);
	iCurrentMode = ESecondMode;
	UpdateUpTime();
}

void CS60UptimeAppView::SetMinuteUpdate()
{
	iDrawTimer->StopTimerL();
	iDrawTimer->StartMinTimerL(KDrawMinDurationTimer, ETrue);
	iCurrentMode = EMinMode;
	UpdateUpTime();
}

void CS60UptimeAppView::SetHourUpdate()
{
	iDrawTimer->StopTimerL();
	iDrawTimer->StartMinTimerL(KDrawHourDurationTimer, ETrue);
	iCurrentMode = EHourMode;
	UpdateUpTime();
}

TKeyResponse CS60UptimeAppView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
	if( aType == EEventKey)
	{
		switch(aKeyEvent.iScanCode){
			case 49:// Key 1
				SetSecondUpdate();
				return EKeyWasConsumed;	
			case 50:// Key 2
				SetMinuteUpdate();
				return EKeyWasConsumed;
			case 51:// Key 3
				SetHourUpdate();
				return EKeyWasConsumed;
				
		}
	}
	return EKeyWasNotConsumed;
}



// End of File
