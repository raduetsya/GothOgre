#ifndef GOTHOGRE_UNICODE_SUPPORT_APP_BLOCK_H
#define GOTHOGRE_UNICODE_SUPPORT_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_UnicodeSupportRegistration.h"


namespace GothOgre
{

	class UnicodeSystemAppBlock : public AppBlock
	{
	public:
		UnicodeSystemAppBlock()
			: mUnicodeSupportRegistration(nullptr)
		{
			mUnicodeSupportRegistration = new UnicodeSupportRegistration;
		}

		~UnicodeSystemAppBlock()
		{
			if(mUnicodeSupportRegistration)
			{
				delete mUnicodeSupportRegistration;
				mUnicodeSupportRegistration = nullptr;
			}
		}

	private:
		UnicodeSupportRegistration*      mUnicodeSupportRegistration;
	};

}

#endif // GOTHOGRE_UNICODE_SUPPORT_APP_BLOCK_H