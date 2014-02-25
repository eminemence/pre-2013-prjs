#ifndef __EVENTLOGGER_H__
#define __EVENTLOGGER_H__


#include <e32std.h>
#include <e32base.h>
#include <flogger.h>

class CEventLogger : public CBase
{
		
	public :	
		//needed construction destruction functions
		~CEventLogger();
		static CEventLogger* NewL();
		static CEventLogger* NewLC();	
		void Log(TDesC& aString);
		static void SLog(TPtrC aString);
		void Write(const TDesC& aModuleName, const TDesC& aMessage);
		static void SWrite(const TDesC& aModuleName, const TDesC& aMessage);

	private:
		void ConstructL();	
		
		
	
	private:
		RFileLogger iLogger;
//		TFileName iLogName;
};

#endif	//__EVENTLOGGER_H__
