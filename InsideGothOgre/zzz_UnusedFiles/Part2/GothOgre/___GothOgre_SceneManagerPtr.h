#ifndef GOTHOGRE_SCENE_MANAGER_PTR_H
#define GOTHOGRE_SCENE_MANAGER_PTR_H


namespace GothOgre
{
	class SceneManager;


	/** This is a wrapper of SceneManager*.
	This is not a smart pointer's implementation, 
	it contains a simple C pointer. */
	class SceneManagerPtr
	{
	public:
		SceneManagerPtr() 
		{
			mPtr = nullptr;
		}
		
		SceneManagerPtr(Ogre::SceneManager* _ptr) 
		{
			mPtr = (SceneManager*) _ptr;
		}
		
		SceneManagerPtr(const SceneManagerPtr& _src) 
		{
			mPtr = _src.mPtr;
		}

		const SceneManagerPtr& operator =(const SceneManagerPtr& _src) 
		{
			mPtr = _src.mPtr; 
			return *this;
		}

		SceneManager& operator*() const
		{
			assert(mPtr);
			return *mPtr;
		}

		SceneManager* operator->() const
		{
			assert(mPtr);
			return mPtr;
		}

		SceneManager* get() const
		{
			return mPtr;
		}

		SceneManager* getPointer() const
		{
			return mPtr;
		}

		operator SceneManager*() const
		{
			return mPtr;
		}

		bool isNull() const 
		{ 
			return mPtr == nullptr; 
		}

        void setNull() 
		{ 
			mPtr = nullptr;
		}

	private:
		SceneManager* mPtr;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SCENE_MANAGER_PTR_H