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

#include "EventLogger.h"
#include <s32file.h> 
#ifdef ENABLE_LOGGING
_LIT(KLogFilePath,"c:\\Data\\Logs\\LogErr\\666.txt");
_LIT(KCRLF,"\r\n");

//void CEventLogger::SLog(TPtrC aString)
//{
//    RFileWriteStream iLogStream;
//    RFs              iLogFs;
//    RFile           iLogFile;
//    TInt posi = 0;
//    if(iLogFs.Connect()!= KErrNone)
//        return;
//    CleanupClosePushL(iLogFs);
//    if(iLogFile.Open(iLogFs, KLogFilePath, EFileWrite) != KErrNone){
//        if(iLogFile.Create(iLogFs, KLogFilePath, EFileWrite) != KErrNone){
//            CleanupStack::PopAndDestroy(1);
//            return;
//        }
//    }
//    CleanupClosePushL(iLogFile);
//    if(iLogFile.Seek(ESeekEnd,posi)== KErrNone){
//        
//        iLogStream.Attach(iLogFile, posi);
//        CleanupClosePushL(iLogStream);
//        
//        iLogStream.WriteL(aString);
//        iLogStream.WriteL(KCRLF);
//        iLogStream.CommitL();
//        
//        CleanupStack::PopAndDestroy(3);
//    }
//    else
//        CleanupStack::PopAndDestroy(2);
//}

void CEventLogger::SLog(TPtrC aString,const char* /*aFunctionName*/)
{
    RFileWriteStream iLogStream;
    RFs              iLogFs;
    RFile           iLogFile;
    TInt posi = 0;
    if(iLogFs.Connect()!= KErrNone)
        return;
    //CleanupClosePushL(iLogFs);//1
    if(iLogFile.Open(iLogFs, KLogFilePath, EFileWrite) != KErrNone){
        if(iLogFile.Create(iLogFs, KLogFilePath, EFileWrite) != KErrNone){
            iLogFs.Close();
//            CleanupStack::PopAndDestroy(1);
            return;
        }
    }
    //CleanupClosePushL(iLogFile);//2
    if(iLogFile.Seek(ESeekEnd,posi)== KErrNone){
        
        iLogStream.Attach(iLogFile, posi);
      //  CleanupClosePushL(iLogStream);//3
        
        iLogStream.WriteL(aString);
        iLogStream.WriteL(KCRLF);
        iLogStream.CommitL();
        iLogStream.Close();
//        CleanupStack::PopAndDestroy(1);
    }
    iLogFile.Close();
    iLogFs.Close();
//    CleanupStack::PopAndDestroy(2);
}
#endif
