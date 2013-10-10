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

#ifndef __SCRSHOTSBUTTONFEEDBACK_H__
#define __SCRSHOTSBUTTONFEEDBACK_H__

// will contain calls to both vibrate pulse on button press 
// and light pulse on taking a screenshot

#ifdef ENABLE_THREE_SCREEN_UI

#include <e32base.h>
//#include <HWRMVibra.h>
#include <mw/touchfeedback.h> 


class CScrShotsActionFeedback : public CBase
{
    public:
        static CScrShotsActionFeedback* NewL();
        ~CScrShotsActionFeedback();
        
    private:
        CScrShotsActionFeedback();
        void ConstructL();
        
    public:
        void SetVibrateFeedbackL(TBool aVibrateEnable){  iVibrateEnable = aVibrateEnable;  }
        TBool IsVibrateEnabled(){  return iVibrateEnable;  }
        void VibrateforButtonPressL(CCoeControl* aCtrl,const TPointerEvent& aPointerEvent);
        void BlinkLightL();
        void SetLightFeedbackL(TBool aLightEnable) {iLightEnable = aLightEnable; } 
        TBool IsLightEnabled() { return iLightEnable; } 
        
    private:
        //CHWRMVibra*             iButtonVibra;
        TBool   iVibrateEnable, iLightEnable;
        MTouchFeedback* iTouchFeedback;


};

#endif //ENABLE_THREE_SCREEN_UI

#endif //__SCRSHOTSBUTTONFEEDBACK_H__
