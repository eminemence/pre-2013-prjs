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

#ifndef __SCRSHOTSCONVERTOR_H__
#define __SCRSHOTSCONVERTOR_H__

#include <e32base.h>
#include <ImageConversion.h>

#include "ScrShotsProducer.h"
#ifdef ENABLE_ENGINE

class CScrShotsSettings;

class CScrShotsConvertor : public CActive
{
public:
	static CScrShotsConvertor* NewL(CScrShotsSettings* aScrShotsSettings,CScrShotsProducer* aScrShotsProducer);
	~CScrShotsConvertor();
	void StartImageConsumerL();
	void StopImageConsumerL();
private:
	CScrShotsConvertor(CScrShotsSettings* aScrShotsSettings,CScrShotsProducer* aScrShotsProducer);
	void ConstructL();
	void RunL();
	void DoCancel();
	void FireTimerL();
	void CheckQueueForItemsToProcessL();
	void ProcessImgFileL();
private:
	enum TConvertorMode{
		EModeNone = 0,
		ETimerMode,
		EImageConvertorMode
	};
private:
	RTimer iWaitDuration;
	CScrShotsSettings* 	iScrShotsSettings;//ref don't delete
	CImageEncoder*	iImageConvertor;
	TConvertorMode	iConvertorMode;
	CScrShotsProducer* iScrShotsProducer;//ref only;do not delete
	TBuf<KBufferSize * 4>       iInputFileName;
	TBuf<KBufferSize * 4>       iOutputFileName;
	CFbsBitmap*     iInputBitmap;
	//RFile           iOutputFile;
	RFs             iOutputFsSession;
};
#endif

#endif //__SCRSHOTSCONVERTOR_H__
