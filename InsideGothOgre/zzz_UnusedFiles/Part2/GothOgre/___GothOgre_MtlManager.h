



	class ScriptInstance
	{
	public:
		ScriptInstance( ScriptInstanceManager* _manager ) 
			: mManager( _manager ), mStateCount( 0 ) {}
		
		virtual ~ScriptInstance() {}

		const String& getName() const {return mName;}
		
		void setName(const String& _name)
		{
			if(mName != _name)
			{
				mManager->nameChanging(mName, _name);
				mName = _name;
				_dirtyState();
			}
		}

		ScriptInstanceManager* getManager() const {return mScriptInstanceManager;}

		int getStateCount() const {return mStateCount;}
		void _dirtyState() {++mStateCount;}

	private:
		String                  mName;
		int                     mStateCount;
		ScriptInstanceManager*  mManager;
	};

	typedef SharedPtr<ScriptInstance> ScriptInstancePtr;



	class ScriptLoader
	{
	public:
		/** Returns true if this loader can load a specified file.
		This function can change a filename if it wants.
		@remark
		The default implementation returns true if the specified file
		exists and the extension of the specified file matchs to an extension
		returned by the getFileExt function. */
		virtual bool canLoad(String& _filename, const String& _resourceGroup) const;

		/** Load information about a resource from a specified data stream. */
		virtual void load(const DataStreamExPtr& _dataStream) = 0;
	};

	class ScriptSaver
	{
	public:
		/** Returns true if this loader can load a specified file.
		This function can change a filename if it wants.
		@remark
		The default implementation returns true if the specified file
		exists and the extension of the specified file matchs to an extension
		returned by the getFileExt function. */
		virtual bool canSave(String& _filename, const String& _resourceGroup) const;

		/** Load information about a resource from a specified data stream. */
		virtual void save(const DataStreamExPtr& _dataStream) = 0;
	};


	class ScriptInstanceManager
	{
	public:
		ScriptInstanceManager();
		~ScriptInstanceManager();

		ScriptInstancePtr findByName(const String& _name);
		ScriptInstancePtr createManual(const String& _name);

		void remove(ScriptInstancePtr& _mtl);
		void removeAll();

		void load(const String& _filename, const String& _resourceGroup);
		void save(const String& _filename, const String& _resourceGroup);

	protected:
		virtual ScriptInstance* createImpl(const String& _name)

	private:
		friend class ScriptInstance;
		void nameChanging(const String& _oldName, const String& _newName);

	private:
		typedef map<String, ScriptInstancePtr>::type ScriptInstanceByName;
		ScriptInstanceByName  mScriptInstanceByName;
	};
