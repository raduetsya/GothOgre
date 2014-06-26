#include "GothOgre_Precompiled.h"
#include "GothOgre_ProgressListener.h"
#include "GothOgre_Progress.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	ProgressListener::ProgressListener()
	{
		Progress::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	ProgressListener::~ProgressListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void ProgressListener::registerListener()
	{
		Progress::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void ProgressListener::unregisterListener()
	{
		if(Progress::getSingletonPtr())
			Progress::getSingleton()._removeListener(this);
	}
}