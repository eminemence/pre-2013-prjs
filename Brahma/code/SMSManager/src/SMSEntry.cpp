#include "SMSEntry.h"

TSmsEntry::TSmsEntry()
{
	
}
		
void TSmsEntry::SetEntryName(TDesC & aEntryType)
{
	iEntryType.Copy(aEntryType);
}
