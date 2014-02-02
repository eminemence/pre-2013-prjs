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


#ifndef __S60UPTIMEAPPVIEW_h__
#define __S60UPTIMEAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>
#include "TimerX.h"



// CLASS DECLARATION
class CS60UptimeAppView : public CCoeControl,public MTimerXCallback
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CS60UptimeAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CS60UptimeAppView.
	 */
	static CS60UptimeAppView* NewL(const TRect& aRect);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Create a CS60UptimeAppView object, which will draw itself
	 * to aRect.
	 * @param aRect Rectangle this view will be drawn to.
	 * @return A pointer to the created instance of CS60UptimeAppView.
	 */
	static CS60UptimeAppView* NewLC(const TRect& aRect);

	/**
	 * ~CS60UptimeAppView
	 * Virtual Destructor.
	 */
	virtual ~CS60UptimeAppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this CS60UptimeAppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();

	/**
	 * From CoeControl, HandlePointerEventL.
	 * Called by framework when a pointer touch event occurs.
	 * Note: although this method is compatible with earlier SDKs, 
	 * it will not be called in SDKs without Touch support.
	 * @param aPointerEvent the information about this event
	 */
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CS60UptimeAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * CS60UptimeAppView.
	 * C++ default constructor.
	 */
	CS60UptimeAppView();
	
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
	
	void CalculateCoords(const TRect& aRect);
	
	void CalculateUpTime();
	
	void UpdateUpTime();


public:
	enum TCurrentMode{
		ESecondMode,
		EMinMode,
		EHourMode
	};


public:
	void HandleTimerExpireL(TInt aTimerId);
	void SetSecondUpdate();
	void SetMinuteUpdate();
	void SetHourUpdate();
	TCurrentMode GetCurrentMode(){
		return iCurrentMode;
	}
	
private:
	CTimerX* iDrawTimer;
	TBuf<256> iMsg;
	TBuf<256> iMsg2;
	const CFont* iUptimeFont;
	const CFont* iOptionsFont;
	TPoint iMsgPt;
	TPoint iHeaderPt1,iHeaderPt2;
	TPoint iNoOfSecsPt;
	TPoint iOptionPt1,iOptionPt2,iOptionPt3; 	
	TBool iIsTouchDevice;
	TBool iHourTimer;
	TCurrentMode iCurrentMode;
	TTimeIntervalMicroSeconds32 iTickPeriod;
	};

#endif // __S60UPTIMEAPPVIEW_h__
// End of File
