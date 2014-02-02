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

#include "ScrShotsStringPool.h"

ScrShotsStringPool* ScrShotsStringPool::iStringPool = NULL;
const TInt KMinBufferSize = 64;
const TInt KBufferCount = 20;

const TInt KUsedFlag = 1;
const TInt KUnusedFlag = 0;

ScrShotsStringPool* ScrShotsStringPool::GetPool()
{
    if(!iStringPool){
        iStringPool = new (ELeave)ScrShotsStringPool;
        iStringPool->ConstructL();
    }
    return iStringPool;
}

ScrShotsStringPool::~ScrShotsStringPool()
{
    for(TInt i = iPool.Count() - 1; i >= 0 ; i--){
        StringBuffer* obj = iPool[i];
        obj->iString->Close();
        delete obj->iString;
        iPool.Remove(i);
    }
    iPool.Close();
}

RBuf* ScrShotsStringPool::GetStringL(TUint aSize)
{
    TInt index = aSize/KMinBufferSize;
    if(aSize%16 == 0){
        //absolute sizes mean absolute index - 1
        if(iPool[(index == 0) ? index : index - 1]->iUsedFlag){
            int i = 0;
            //keep searching till the next unused buffer is not fetched
            while(iPool[index + i]->iUsedFlag)
                i++;
            iPool[index + i]->iUsedFlag = KUsedFlag;
            return iPool[index + i]->iString;
        }
        else{
            iPool[(index == 0) ? index : index - 1]->iUsedFlag = KUsedFlag;
            return iPool[(index == 0) ? index : index - 1]->iString;
        }
    }
    else{
        if(iPool[index]->iUsedFlag){
            int i = 0;
            //keep searching till the next unused buffer is not fetched
            while(iPool[index + i]->iUsedFlag)
                i++;
            iPool[index]->iUsedFlag = KUsedFlag;
            return iPool[index + i]->iString;
        }
        else{
            iPool[index]->iUsedFlag = KUsedFlag;
            return iPool[index]->iString;
        }        
    }
}

void ScrShotsStringPool::MarkBuffer(RBuf* aBuffer)
{
    iPool[(aBuffer->MaxLength() / KMinBufferSize) - 1]->iUsedFlag = KUsedFlag;
}

void ScrShotsStringPool::UnmarkBuffer(RBuf* aBuffer)
{
    aBuffer->Zero();
    iPool[(aBuffer->MaxLength() / KMinBufferSize) - 1]->iUsedFlag = KUnusedFlag;
}

void ScrShotsStringPool::ConstructL()
{
    for(TInt i = 0; i < KBufferCount ;i++){
        StringBuffer* obj = new StringBuffer;
        obj->iString = new (ELeave) RBuf;
        obj->iString->Create((i+1) * KMinBufferSize);
        obj->iUsedFlag = KUnusedFlag;
        iPool.Append(obj);
    }
}

