#ifndef GOTHOGRE_ZEN_MANAGER_APP_BLOCK_H
#define GOTHOGRE_ZEN_MANAGER_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_BaseWorldObject.h"
#include "GothOgre_RootWorldObject.h"

namespace GothOgre
{

	class ZenManagerAppBlock : public AppBlock
	{
	public:
		ZenManagerAppBlock()
			: mZenManager(nullptr),
			  mBaseWorldObjectFactory(nullptr), mRootWorldObjectFactory(nullptr)
		{
			mZenManager = new ZenManager;
			mBaseWorldObjectFactory = new BaseWorldObjectFactory;
			mRootWorldObjectFactory = new RootWorldObjectFactory;
		}

		~ZenManagerAppBlock()
		{
			if(mRootWorldObjectFactory)
				delete mRootWorldObjectFactory;

			if(mBaseWorldObjectFactory)
				delete mBaseWorldObjectFactory;
		
			if(mZenManager)
				delete mZenManager;
		}

	private:
		ZenManager*                  mZenManager;
		BaseWorldObjectFactory*      mBaseWorldObjectFactory;
		RootWorldObjectFactory*      mRootWorldObjectFactory;
	};

}

#endif // GOTHOGRE_ZEN_MANAGER_APP_BLOCK_H