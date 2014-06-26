#ifndef GOTHOGRE_APP_BLOCK_UNICODE_SYSTEM_H
#define GOTHOGRE_APP_BLOCK_UNICODE_SYSTEM_H

#include "GothOgre_AppBlock.h
#include "GothOgre_UnicodeManagerEnumerator.h"
#include "GothOgre_DefaultUnicodeManager.h"
#include "GothOgre_CodepageConverterManager.h"


namespace GothOgre
{

	class UnicodeSystemAppBlock : public AppBlock
	{
	public:
		UnicodeSystemAppBlock()
			: mUnicodeSystem(nullptr), mUnicodeManagerEnumerator(nullptr),
			  mDefaultUnicodeManagerFactory(nullptr), mUnicodeManager(nullptr)
		{
			mCodepageConverterManager = new CodepageConverterManager;
			mUnicodeManagerEnumerator = new UnicodeManagerEnumerator;
			mDefaultUnicodeManagerFactory = new DefaultUnicodeManagerFactory;			
		}

		~UnicodeSystemAppBlock()
		{
			if(mDefaultUnicodeManagerFactory)
			{
				delete mDefaultUnicodeManagerFactory;
				mDefaultUnicodeManagerFactory = nullptr;
			}
			if(mUnicodeManagerEnumerator)
			{
				delete mUnicodeManagerEnumerator;
				mUnicodeManagerEnumerator = nullptr;
			}
			if(mCodepageConverterManager)
			{
				delete mCodepageConverterManager;
				mCodepageConverterManager = nullptr;
			}
		}

		void initialise()
		{
			mUnicodeManager = mUnicodeManagerEnumerator->createUnicodeManager();
		}

		void shutdown()
		{
			mUnicodeManagerEnumerator->destroyUnicodeManager(mUnicodeManager);
			mUnicodeManager = nullptr;
		}

	private:
		UnicodeManagerEnumerator*       mUnicodeManagerEnumerator;
		DefaultUnicodeManagerFactory*   mDefaultUnicodeManagerFactory;
		CodepageConverterManager*       mCodepageConverterManager;
		UnicodeManager*                 mUnicodeManager;
	};

}

#endif // GOTHOGRE_APP_BLOCK_UNICODE_SYSTEM_H