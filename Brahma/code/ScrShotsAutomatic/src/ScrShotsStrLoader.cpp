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

#include "ScrShotsStrLoader.h"
#include <ScrShotsAutomatic.rsg>
#include <stringloader.h>
#include "ScrShotsPanics.pan"



CScrShotsStrLoader* CScrShotsStrLoader::NewL()
{
    CScrShotsStrLoader* self = new (ELeave) CScrShotsStrLoader();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

CScrShotsStrLoader::~CScrShotsStrLoader()
{
    iStrArray.ResetAndDestroy();
    iStrArray.Close();
}

HBufC* CScrShotsStrLoader::GetStringHBuf(TAppStrList aStrEnum)
{
    __ASSERT_ALWAYS(aStrEnum < ETotalStringCount, User::Panic(KScrShotsPanics,EStrLoadreBadIndex));
    return iStrArray[aStrEnum];
}

const TDesC& CScrShotsStrLoader::GetStringDes(TAppStrList aStrEnum)
{
    __ASSERT_ALWAYS(aStrEnum < ETotalStringCount, User::Panic(KScrShotsPanics,EStrLoadreBadIndex));
    return iStrArray[aStrEnum]->Des();
}

CScrShotsStrLoader::CScrShotsStrLoader()
{

}

void CScrShotsStrLoader::ConstructL()
{
    HBufC* tmpStr = NULL;
    for(TInt i = 0;i < ETotalStringCount; i++){
        switch(i){
            case ENoneString:       tmpStr = StringLoader::LoadL(R_STR_NONE);           break;
            case EExitPromptString: tmpStr = StringLoader::LoadL(R_STR_EXIT_PROMPT);    break;
            case EAppListCaption: tmpStr = StringLoader::LoadL(R_STR_APP_LIST_CAPTION);    break;
            case EFolderListCaption: tmpStr = StringLoader::LoadL(R_STR_FOLDER_LIST_CAPTION);    break;
            case EDriveListCaption: tmpStr = StringLoader::LoadL(R_STR_DRIVE_LIST_CAPTION);    break;
            case EAppSelectFormatString: tmpStr = StringLoader::LoadL(R_STR_APP_SELECT_FORMAT);    break;
            case EDriveSelectFormatString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_SELECT_FORMAT);    break;
            case EFolderSelectNameFormatString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_SELECT_NAME_FORMAT);    break;
            case EFolderCurrentNameFormatString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_CURRENT_NAME_FORMAT);    break;            
            case EFolderNamePromptString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_NAME_PROMPT);    break;
            case EDriveUsageLimitString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_USAGE_LIMIT);    break;
            case EDriveSizeFormatString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_SIZE_FORMAT);    break;
            case EDriveListFreeSizeCellFormatString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_LIST_FREE_SIZE_CELL_FORMAT);    break;
            case EDriveListThresholdSizeCellFormatString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_LIST_THRESHOLD_SIZE_CELL_FORMAT);    break;
            case EFolderCreateOkString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_CREATE_OK_MSG);    break;
            case EFolderCreateFailString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_CREATE_FAIL_MSG);    break;
            case EDriveTotalSizeFormatString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_TOTAL_FORMAT_STR); break;
            case ENoSoundInSilentProfileString: tmpStr = StringLoader::LoadL(R_STR_SOUND_ON_PROFILE_STR); break;
            case EDriveSelectLabelString:   tmpStr = StringLoader::LoadL(R_STR_DRV_SELECT_LABEL_STR); break;
            case EFolderSelectLabelString:  tmpStr = StringLoader::LoadL(R_STR_FOLDER_SELECT_LABEL_STR); break;
            case EFreeSpaceString: tmpStr = StringLoader::LoadL(R_STR_FREE_SPACE_STR); break;
            case EThresholdPercentString: tmpStr = StringLoader::LoadL(R_STR_THRESHOLD_PERCENT_STR); break;
            case ENameSeriesSelectLabelString: tmpStr = StringLoader::LoadL(R_STR_NAME_SERIES_SELECT_LABEL_STR); break;
            case ETargetFormatSelectLabelString: tmpStr = StringLoader::LoadL(R_STR_TARGET_FORMAT_SELECT_LABEL_STR); break;
            case ECurrentModeLabelString: tmpStr = StringLoader::LoadL(R_STR_CURRENT_MODE_LABEL_STR); break;
            case EScrShotsCountLabelString: tmpStr = StringLoader::LoadL(R_STR_SCR_SHOTS_COUNT_LABEL_STR); break;
            case EAppNameLabelString: tmpStr = StringLoader::LoadL(R_STR_APP_NAME_LABEL_STR); break;
            case EIntervalTitleString: tmpStr = StringLoader::LoadL(R_STR_INTERVAL_TITLE_LABEL_STR); break;
            case EDurationTitleString: tmpStr = StringLoader::LoadL(R_STR_DURATION_TITLE_LABEL_STR); break;
            case EDriveGridThresholdInfoString: tmpStr = StringLoader::LoadL(R_STR_DRIVE_GRID_THRESHOLD_INFO_STR); break;
            case ESingleScrShotModeString: tmpStr = StringLoader::LoadL(R_STR_SINGLE_SCRSHOT_MODE_STR); break;
            case EMultipleScrShotModeString: tmpStr = StringLoader::LoadL(R_STR_MULTIPLE_SCRSHOT_MODE_STR); break;
            case EAppScrShotTillExitModeString: tmpStr = StringLoader::LoadL(R_STR_APP_SCRSHOT_TILL_EXIT_MODE_STR); break;
            case EAppScrShotForDurationModeString: tmpStr = StringLoader::LoadL(R_STR_APP_SCRSHOT_FOR_DURATION_MODE_STR); break;
            case EFolderPermissionDeniedMsgString: tmpStr = StringLoader::LoadL(R_STR_FOLDER_PERMISSION_DENINED_STR); break;
            case EAppNotSelectedMsgString : tmpStr = StringLoader::LoadL(R_STR_APP_NOT_SELECTED_MSG_STR); break;
            case ESingleScrShotActionString: tmpStr = StringLoader::LoadL(R_STR_SINGLE_SCR_SHOT_ACTION_STR); break;
            case EMultipleScrShotActionString: tmpStr = StringLoader::LoadL(R_STR_MULTIPLE_SCR_SHOT_ACTION_STR); break;
            case EMultipleScrShotActionFoAppTillDurationString: tmpStr = StringLoader::LoadL(R_APP_MULTIPLE_SCR_SHOT_TILL_DURATION_STR); break;
            case EMultipleScrShotActionFoAppTillExitString: tmpStr = StringLoader::LoadL(R_STR_APP_MULTIPLE_SCR_SHOT_TILL_EXIT_STR); break;
            case EAppLaunchErrorMsgString: tmpStr = StringLoader::LoadL(R_STR_APP_LAUNCH_ERROR_MSG_STR); break;

        }
        iStrArray.Append(tmpStr);
    }
}
