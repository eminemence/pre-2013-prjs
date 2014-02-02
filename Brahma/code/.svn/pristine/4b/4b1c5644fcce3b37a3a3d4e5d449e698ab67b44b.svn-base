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

#include "ScrShotsButtonFeedback.h"

#define KFeedbackIntensity 50

const TTimeIntervalMicroSeconds32 KFeedbackTimeout(100000);

CScrShotsActionFeedback* CScrShotsActionFeedback::NewL()
{
    CScrShotsActionFeedback* self = new (ELeave) CScrShotsActionFeedback();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CScrShotsActionFeedback::~CScrShotsActionFeedback()
{
    MTouchFeedback::DestroyInstance();
}

CScrShotsActionFeedback::CScrShotsActionFeedback():
        iVibrateEnable(ETrue), iLightEnable(ETrue)
{

}

void CScrShotsActionFeedback::ConstructL()
{
    iTouchFeedback = MTouchFeedback::CreateInstanceL();
    if(iVibrateEnable){
        iTouchFeedback->SetFeedbackEnabledForThisApp(ETrue,EFalse);
    }
}

void CScrShotsActionFeedback::VibrateforButtonPressL(CCoeControl* aCtrl,const TPointerEvent& aPointerEvent )
{
    if(iVibrateEnable){
        iTouchFeedback->StartFeedback(aCtrl, ETouchContinuousFlick, &aPointerEvent, KFeedbackIntensity, KFeedbackTimeout);
    }
}

void CScrShotsActionFeedback::BlinkLightL()
{

}


