#ifndef GOTHOGRE_FRAME_STATS_H
#define GOTHOGRE_FRAME_STATS_H


namespace GothOgre
{
	class StandardFrameStatsValues;

	//------------------------------------------------------------------------
	class GOTHOGRE_EXPORT FrameStatsValue
	{
	public:
		FrameStatsValue(const String& _key, bool _addToDefaultTextTemplate = false);
		virtual ~FrameStatsValue();
		const String& getKey() const {return mKey;}

		virtual String getValue() = 0;

	private:
		String   mKey;
	};


	//------------------------------------------------------------------------
	class GOTHOGRE_EXPORT FrameStats : public Singleton<FrameStats>
	{
	public:
		typedef map<String, FrameStatsValue*>::type KeyValueMap;
		typedef KeyValueMap::value_type             KeyValuePair;
		
		class KeyIterator : public IteratorWrapper<KeyValueMap, KeyValueMap::const_iterator, String>
		{
		public:
			typedef const String& ValueType;
			typedef const String* PointerType;
			KeyIterator(const KeyValueMap& _c) : IteratorWrapper<KeyValueMap, KeyValueMap::const_iterator, String>(_c.begin(), _c.end())	{}
			ValueType getNext() {return (this->mCurrent++)->first;}	
			ValueType peekNext() const {return (this->mCurrent)->first;}
			PointerType peekNextPtr() const {return &((this->mCurrent)->first);}
		};

	public:
		FrameStats();
		~FrameStats();

		KeyIterator getKeyIterator() const {return KeyIterator(mKeyValueMap);}

		String getValue(const String& _key) const;
		
		void setDefaultTextTemplate(const String& _textTemplate) {mDefaultTextTemplate = _textTemplate;}
		const String& getDefaultTextTemplate() const {return mDefaultTextTemplate;}

		String getText(const String& _textTemplate) const;
		String getText(const String& _textTemplate, const String& _fmtValueBegin, const String& _fmtValueEnd) const;

	private:
		friend class FrameStatsValue;
		void addFrameStatsValue(FrameStatsValue* _value, bool _addToDefaultTextTemplate = false);
		void removeFrameStatsValue(FrameStatsValue* _value);

	private:
		KeyValueMap	               mKeyValueMap;
		String                     mDefaultTextTemplate;
		StandardFrameStatsValues*  mStandardValues;
	};

}

#endif // GOTHOGRE_FRAME_STATS_H