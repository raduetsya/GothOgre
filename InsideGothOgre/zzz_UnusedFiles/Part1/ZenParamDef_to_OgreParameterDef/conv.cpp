	ZenParameterDef::operator Ogre::ParameterDef() const
	{
		const String& retName = mName;
		Ogre::ParameterType retType = *(Ogre::ParameterType*) &mType;
		
		StringStream ss;
		writeToStream(ss, mGroup);
		writeToStream(ss, mDescription);
		writeToStream(ss, mFlags);
		writeToStream(ss, mRangeMin);
		writeToStream(ss, mRangeMax);
		writeToStream(ss, mIRangeMin);
		writeToStream(ss, mIRangeMax);
		writeToStream(ss, (int) mEnumNameValues.size());
		for(size_t i = 0; i != mEnumNameValues.size(); ++i)
		{
			const NameIntPair& ni = mEnumNameValues[i];
			writeToStream(ss, ni.first);
			writeToStream(ss, ni.second);
		}
		const String& retDescription = ss.str();

		return Ogre::ParameterDef(retName, retDescription, retType);
	}
	//-------------------------------------------------------------------------
	ZenParameterDef::ZenParameterDef(const Ogre::ParameterDef& _parameterDef)
	{
		mName = _parameterDef.name;
		mType = *(ZenParameterType::Type*) &_parameterDef.paramType;

		StringStream ss(_parameterDef.description);
		readFromStream(ss, mGroup);
		readFromStream(ss, mDescription);
		readFromStream(ss, mFlags);
		readFromStream(ss, mRangeMin);
		readFromStream(ss, mRangeMax);
		readFromStream(ss, mIRangeMin);
		readFromStream(ss, mIRangeMax);

		int numEnumValues;
		readFromStream(ss, numEnumValues);
		mEnumNameValues.clear();
		mEnumNameValues.reserve(numEnumValues);
		for(int i = 0; i != numEnumValues; ++i)
		{
			mEnumNameValues.push_back(NameIntPair());
			NameIntPair& ni = mEnumNameValues.back();
			readFromStream(ss, ni.first);
			readFromStream(ss, ni.second);
		}
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::writeToStream(StringStream& _ss, const String& _str)
	{
		_ss << _str.length() << " " << _str;
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::readFromStream(StringStream& _ss, String& _str)
	{
		size_t len;
		_ss >> len;
		_ss.ignore(1);
		_str.resize(len);
		_ss.read(&_str[0], len);
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::writeToStream(StringStream& _ss, int _i)
	{
		_ss << _i << " ";
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::readFromStream(StringStream& _ss, int& _i)
	{
		_ss >> _i;
		_ss.ignore(1);
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::writeToStream(StringStream& _ss, Real _f)
	{
		_ss << _f << " ";
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::readFromStream(StringStream& _ss, Real& _f)
	{
		_ss >> _f;
		_ss.ignore(1);
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::writeToStream(StringStream& _ss, bool _b)
	{
		_ss << std::noboolalpha << _b << " ";
	}
	//-------------------------------------------------------------------------
	void ZenParameterDef::readFromStream(StringStream& _ss, bool& _b)
	{
		_ss >> std::noboolalpha >> _b;
		_ss.ignore(1);
	}
