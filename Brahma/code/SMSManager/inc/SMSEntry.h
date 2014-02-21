#include <e32base.h>

//class to be used in an RArray to maintain the SMS entries.
class TSmsEntry
{
	private :
		//type of the entry of the MTM entry
		TBuf<30> iEntryType;
		
	public:
		TSmsEntry();		
		
		void SetEntryName(TDesC & aEntryType);
	
};