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

#include "ScrShotsVectorImgLoader.h"
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>

_LIT(KMBMFile,"ScrShotsScreenIcons.mbm");

#ifdef ENABLE_LOGGING
#define TRACE_ON(x) x
#else
#define TRACE_ON(x)
#endif

//todo:taken shamelessly from ScalableScreenExample, would need to do something about it!!!!!
void CPrivateIconsProvider::RetrieveIconFileHandleL(RFile& aFile,
		const TIconFileType aType)
	{
	_LIT(KMif, "mif");
	_LIT(KMbm, "mbm");
	TFileName fileName;
	fileName = iIconFilename.Left(iIconFilename.Length() - 3);
	fileName.Append((aType == EMifFile) ? KMif : KMbm);
	User::LeaveIfError(aFile.Open(iFs, fileName, EFileShareAny));
	}

void CPrivateIconsProvider::Finished()
	{
	}

// Since the AknIconUtils cannot access the private directory of application,
// mix-in class MAknIconFileProvider is implemented to provide handle of the
// icon files (ScalableScreenDrawingIcons.mbm in this case) to AknIconUtils.
CPrivateIconsProvider::CPrivateIconsProvider(RFs& aSession) :
	iFs(aSession)
	{
	}

// Destructor
CPrivateIconsProvider::~CPrivateIconsProvider()
	{
	iIconFilename.Close();
	}

CPrivateIconsProvider* CPrivateIconsProvider::NewL(RFs& aSession,
		const TDesC& aFilename)
	{
	CPrivateIconsProvider* self = new (ELeave) CPrivateIconsProvider(aSession);
	CleanupStack::PushL(self);
	self->ConstructL(aFilename);
	CleanupStack::Pop(self);
	return self;
	}

// Second-phase constructor
void CPrivateIconsProvider::ConstructL(const TDesC& aFilename)
	{
	iIconFilename.CreateL(aFilename.Length());
	iIconFilename.Copy(aFilename);

	}

// ------------------- CScrShotsVectorImgLoader starts here -----------------------

CScrShotsVectorImgLoader* CScrShotsVectorImgLoader::NewL()
	{
	CScrShotsVectorImgLoader* self = new (ELeave) CScrShotsVectorImgLoader();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CScrShotsVectorImgLoader::~CScrShotsVectorImgLoader()
	{
	CleanAvkonSvgCache();
	iBitmapArray.ResetAndDestroy();
	iBitmapArray.Close();

	delete iIconProvider;
	}

CScrShotsVectorImgLoader::CScrShotsVectorImgLoader()
	{

	}

void CScrShotsVectorImgLoader::ConstructL()
	{
	RFs& fs = CEikonEnv::Static()->FsSession();
	TPixelsTwipsAndRotation iPixTwipsRot;
	CEikonEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(
			iPixTwipsRot);
	TSize size = iPixTwipsRot.iPixelSize;

	TFileName completefilename;
	User::LeaveIfError(fs.PrivatePath(completefilename));

	completefilename.Append(KMBMFile);

	TParsePtrC parse(
			(CEikonEnv::Static()->EikAppUi()->Application())->AppFullName());
	completefilename.Insert(0, parse.Drive());

	iIconProvider = CPrivateIconsProvider::NewL(fs, completefilename);
	}

void CScrShotsVectorImgLoader::LoadAllVectorsL()
	{
	CFbsBitmap* iconBmp = NULL, *iconBmpMask = NULL;
	TInt bitmapId = EMbmScrshotsscreeniconsNumber_0;
	TInt bitmapMaskId = EMbmScrshotsscreeniconsNumber_0_mask;
	//load the icons
	for (TInt i = 0; i < ELastIndex; i++)
		{
	    TRACE_ON(RDebug::Printf("Bitmap Id :%d BitmapMaskId :%d",bitmapId,bitmapMaskId);)
		AknIconUtils::CreateIconL(iconBmp, iconBmpMask, *iIconProvider,
				bitmapId, bitmapMaskId);
		bitmapId += 2;
		bitmapMaskId += 2;
		AknIconUtils::PreserveIconData(iconBmp);
		iBitmapArray.Append(iconBmp);
		iBitmapArray.Append(iconBmpMask);
		iconBmp = iconBmpMask = NULL;
		}
	}

CFbsBitmap* CScrShotsVectorImgLoader::GetBitmap(
        TScrShotsIconNames aIconNameIndex)
    {

    if (aIconNameIndex < ELastIndex)
        {
        TInt iconIndex = aIconNameIndex * 2;
        return (CFbsBitmap*) iBitmapArray[iconIndex];
        }
    return NULL;
    }

CFbsBitmap* CScrShotsVectorImgLoader::GetBitmapMask(
		TScrShotsIconNames aIconNameIndex)
	{
	
	if (aIconNameIndex < ELastIndex)
	    {
	    TInt iconIndex = aIconNameIndex * 2 + 1;
        return (CFbsBitmap*) iBitmapArray[iconIndex];
	    }
	return NULL;
	}

void CScrShotsVectorImgLoader::SetIconSize(TScrShotsIconNames aIconName,
		TSize aIconSize)
{
	TScaleMode scalMode = EAspectRatioPreserved;
	CFbsBitmap* bitmap = GetBitmap(aIconName);
	TRACE_ON(RDebug::Printf("Before bitmap size : w :%d h :%d",bitmap->SizeInPixels().iWidth,bitmap->SizeInPixels().iHeight);)
	AknIconUtils::SetSize(bitmap, aIconSize, scalMode);
	TRACE_ON(RDebug::Printf("After bitmap size : w :%d h :%d",bitmap->SizeInPixels().iWidth,bitmap->SizeInPixels().iHeight);)
}

void CScrShotsVectorImgLoader::CleanAvkonSvgCache()
	{
	for (TInt i = ENumber0; i < ELastIndex; i++)
		{
		CFbsBitmap* bitmap = iBitmapArray[i];
		AknIconUtils::DestroyIconData(bitmap);
		}
	}
