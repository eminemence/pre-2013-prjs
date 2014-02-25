#include "EventLogger.h"

_LIT(KLogDir,"LogErr");
_LIT(KLogFile,"test.log");


CEventLogger::~CEventLogger()
{
	// Close the log file and the connection to the server.
	iLogger.CloseLog();
	iLogger.Close();
}

CEventLogger* CEventLogger::NewL()
{
	CEventLogger* self = CEventLogger::NewLC();
	CleanupStack::Pop(self);
	return self;
}

CEventLogger* CEventLogger::NewLC()
{
	CEventLogger* self = new (ELeave)CEventLogger();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CEventLogger::ConstructL()
{	
	iLogger.Connect();
	iLogger.CreateLog(KLogDir,KLogFile,EFileLoggingModeAppend);
}

void CEventLogger::Log(TDesC& aString)
{
	iLogger.Write(aString);
}

void CEventLogger::SLog(TPtrC aString)
{
	RFileLogger iLogger;
	iLogger.Connect();
	iLogger.CreateLog(KLogDir,KLogFile,EFileLoggingModeAppend);
	iLogger.Write(aString);
	iLogger.CloseLog();
	iLogger.Close();
}

void CEventLogger :: Write(const TDesC& aModuleName, const TDesC& aMessage)
{
	_LIT(CrLfValue,"\r\n");
	TBuf<5> CrLf;
	CrLf.Format(_L("%S"),&CrLfValue);

	_LIT(hyp,"-");
	TBuf<2> Hyphen;
	Hyphen.Format(_L("%S"),&hyp);				

	TInt MessageLen,ModuleNameLen,MessageToLogLen;
	MessageLen = ModuleNameLen = MessageToLogLen = 0;

	MessageLen = aMessage.Length();
	ModuleNameLen = aModuleName.Length();

	MessageToLogLen = ModuleNameLen;
	MessageToLogLen += Hyphen.Length();
	MessageToLogLen += MessageLen;
	MessageToLogLen += CrLf.Length();

	HBufC* MessageToLog = HBufC :: NewLC(MessageToLogLen);

	MessageToLog->Des().Copy(aModuleName);
	MessageToLog->Des().Append(Hyphen);
	MessageToLog->Des().Append(aMessage);
	MessageToLog->Des().Append(CrLf);

	iLogger.Write(*MessageToLog );

	CleanupStack::PopAndDestroy(MessageToLog);
}

void CEventLogger :: SWrite(const TDesC& aModuleName, const TDesC& aMessage)
{
	RFileLogger iLogger;
	iLogger.Connect();
	iLogger.CreateLog(KLogDir,KLogFile,EFileLoggingModeAppend);

	_LIT(CrLfValue,"\r\n");
	TBuf<5> CrLf;
	CrLf.Format(_L("%S"),&CrLfValue);

	_LIT(hyp,"-");
	TBuf<2> Hyphen;
	Hyphen.Format(_L("%S"),&hyp);				

	TInt MessageLen,ModuleNameLen,MessageToLogLen;
	MessageLen = ModuleNameLen = MessageToLogLen = 0;

	MessageLen = aMessage.Length();
	ModuleNameLen = aModuleName.Length();

	MessageToLogLen = ModuleNameLen;
	MessageToLogLen += Hyphen.Length();
	MessageToLogLen += MessageLen;
	MessageToLogLen += CrLf.Length();

	HBufC* MessageToLog = HBufC :: NewLC(MessageToLogLen);

	MessageToLog->Des().Copy(aModuleName);
	MessageToLog->Des().Append(Hyphen);
	MessageToLog->Des().Append(aMessage);
	MessageToLog->Des().Append(CrLf);

	iLogger.Write(*MessageToLog);

	CleanupStack::PopAndDestroy(MessageToLog);

	iLogger.CloseLog();
	iLogger.Close();
}

	