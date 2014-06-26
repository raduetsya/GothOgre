		
		
	class ZenParamDef
	{
	public:
		/** Pack this object to an object of class Ogre::ParameterDef. */
		operator Ogre::ParameterDef() const;

		/** Unpack data from an object of class Ogre::ParameterDef. */
		explicit ZenParamDef(const Ogre::ParameterDef& _parameterDef);
		
	private:
		static void writeToStream(StringStream& _ss, const String& _str);
		static void readFromStream(StringStream& _ss, String& _str);
		static void writeToStream(StringStream& _ss, Real _f);
		static void readFromStream(StringStream& _ss, Real& _f);
		static void writeToStream(StringStream& _ss, int _i);
		static void readFromStream(StringStream& _ss, int& _i);
		static void writeToStream(StringStream& _ss, bool _b);
		static void readFromStream(StringStream& _ss, bool& _b);	
	};