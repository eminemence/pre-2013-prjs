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

#ifndef __SCRSHOTSSTRINGPOOL_H__
#define __SCRSHOTSSTRINGPOOL_H__

#include <e32base.h>

const TUint KDefaultSize32 = 32;

struct StringBuffer{
    RBuf* iString;
    unsigned int iUsedFlag:1;
};
// we will have pools of muliples of 16 sizes with max being 512

class ScrShotsStringPool : public CBase
{
    public:
        static ScrShotsStringPool* GetPool();
        static void DestroyPool(){
            delete iStringPool;
        }
        RBuf* GetStringL(TUint aSize);
        void MarkBuffer(RBuf* aBuffer);
        void UnmarkBuffer(RBuf* aBuffer);
        
    private:
        ScrShotsStringPool(){
        
        }
        ~ScrShotsStringPool();
        void ConstructL();
    
    private:
        static ScrShotsStringPool* iStringPool;
        RPointerArray<StringBuffer> iPool;
};

class TStringPoolObj{
public:
    TStringPoolObj(TInt aBufferSize){
        iString = ScrShotsStringPool::GetPool()->GetStringL(aBufferSize);
        ScrShotsStringPool::GetPool()->MarkBuffer(iString);
    }
    ~TStringPoolObj(){
        ScrShotsStringPool::GetPool()->UnmarkBuffer(iString);
        iString = NULL;
    }
    inline TDesC& DesC(){
        return *iString;
    }
    inline TDes& Des(){
        return *iString;
    }
private:
    TStringPoolObj(){
        //we dont want this to be called any time
    }
private:
    RBuf* iString;
};

#endif //__SCRSHOTSSTRINGPOOL_H__
