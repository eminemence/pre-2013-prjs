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

#ifndef __SCRSHOTSVECTORIMGLOADER_H__
#define __SCRSHOTSVECTORIMGLOADER_H__

#include <AknIconUtils.h>
#include <ScrShotsScreenIcons.mbg>

//Note the enums here should match the order of the enums in the .mbg file, 
//since the loading is done in the order mentioned in mbg file 
enum TScrShotsIconNames
	{
	ENumber0 = 0,
	ENumber1,
	ENumber2,
	ENumber3,
	ENumber4,
	ENumber5,
	ENumber6,
	ENumber7,
	ENumber8,
	ENumber9,
	ERefresh,
	EBack,
	ESoundOffIcon,
	ESoundOnIcon,
	EAppScrshotTillExit,
	ESingleScrshot,
	EDisabledPageForward,
	EBackApplist,
	EAppMultiScrshotForDuration,
	EInfoIcon,
	EFDrvSelected,
	EFDrvUnselected,
	EBmpSelect,
	EStopAction,
	EDisabledPageBack,
	EAppIcon,
	EPageBack,
	EMultiScrshot,
	EPageForward,
	ECDrvSelected,
	EEDrvSelected,
	ECDrvUnselected,
	EEDrvUnselected,
	EAppDefaultIcon,
	EJpegUnselectedIcon,
	EGifUnselectedIcon,
	EAppSelected,
	EPngSelectedIcon,
	EFolderGoInside,
	EPngUnselectedIcon,
	EFolderSelectAction,
	EFolderSelect,
	EFolderUnselect,
	EBmpSelectedIcon,
	EBmpUnselectedIcon,
	EJpegSelectedIcon,
	EGifSelectedIcon,
	EDrvSelectAction,
	EExitIcon,
	EFolderUpIcon,
	ECreateFolderIcon,
	EAppUnselectedIcon,
    EDDrvSelected,
    EDDrvUnselected,
	ELastIndex
	};

class CPrivateIconsProvider : public CBase, public MAknIconFileProvider
	{
public:
	explicit CPrivateIconsProvider(RFs& aSession);
	static CPrivateIconsProvider* NewL(RFs& aSession, const TDesC& aFilename);
	void ConstructL(const TDesC& aFilename);
	~CPrivateIconsProvider();
private:
	//from MAknIconFileProvider
	void RetrieveIconFileHandleL(RFile& aFile, const TIconFileType aType);
	void Finished();
private:
	RFs& iFs;
	RBuf iIconFilename;
	};

//this class will maintain a list of bitmaps that is loaded 
//from svg's
class CScrShotsVectorImgLoader : public CBase
	{
public:
	static CScrShotsVectorImgLoader* NewL();
	~CScrShotsVectorImgLoader();
	void LoadAllVectorsL();
	CFbsBitmap* GetBitmap(TScrShotsIconNames aIconNameIndex);
	CFbsBitmap* GetBitmapMask(TScrShotsIconNames aIconNameIndex);
	void SetIconSize(TScrShotsIconNames aIconName, TSize aIconSize);

private:
	CScrShotsVectorImgLoader();
	void ConstructL();
	void CleanAvkonSvgCache();

private:
	CPrivateIconsProvider* iIconProvider;
	//hold the bitmaps in an array so that they can be destroyed in one shot
	RPointerArray<CFbsBitmap> iBitmapArray;
	};

#endif //__SCRSHOTSVECTORIMGLOADER_H__
