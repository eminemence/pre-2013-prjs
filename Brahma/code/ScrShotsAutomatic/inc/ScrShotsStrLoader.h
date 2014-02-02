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

#ifndef __SCRSHOTSSTRLOADER_H__
#define __SCRSHOTSSTRLOADER_H__

#include <e32base.h>

enum TAppStrList{
    ENoneString = 0,
    EExitPromptString,
    EAppListCaption,
    EFolderListCaption,
    EDriveListCaption,
    EAppSelectFormatString,
    EDriveSelectFormatString,
    EFolderSelectNameFormatString,
    EFolderCurrentNameFormatString,
    EFolderNamePromptString,
    EDriveUsageLimitString,
    EDriveSizeFormatString,
    EDriveListFreeSizeCellFormatString,
    EDriveListThresholdSizeCellFormatString,
    EFolderCreateOkString,
    EFolderCreateFailString,
    EDriveTotalSizeFormatString,
    ENoSoundInSilentProfileString,
    EDriveSelectLabelString,
    EFolderSelectLabelString,
    EFreeSpaceString,
    EThresholdPercentString,
    ENameSeriesSelectLabelString,
    ETargetFormatSelectLabelString,
    ECurrentModeLabelString,
    EScrShotsCountLabelString,
    EAppNameLabelString,
    EIntervalTitleString,
    EDurationTitleString,
    EDriveGridThresholdInfoString,
    ESingleScrShotModeString,
    EMultipleScrShotModeString,
    EAppScrShotTillExitModeString,
    EAppScrShotForDurationModeString,
    EFolderPermissionDeniedMsgString,
    EAppNotSelectedMsgString,
    ESingleScrShotActionString,
    EMultipleScrShotActionString,
    EMultipleScrShotActionFoAppTillDurationString,
    EMultipleScrShotActionFoAppTillExitString,
    EAppLaunchErrorMsgString,
    ETotalStringCount
};

//class the one class to load all strings
class CScrShotsStrLoader : public CBase
{
    public:
        static CScrShotsStrLoader* NewL();
        ~CScrShotsStrLoader();
        HBufC* GetStringHBuf(TAppStrList aStrEnum);//ownership is not transferred, do not delete the pointer
        const TDesC& GetStringDes(TAppStrList aStrEnum);
        
    private:
        CScrShotsStrLoader();
        void ConstructL();
        
    private:
        RPointerArray<HBufC> iStrArray;
};

#endif //__SCRSHOTSSTRLOADER_H__
