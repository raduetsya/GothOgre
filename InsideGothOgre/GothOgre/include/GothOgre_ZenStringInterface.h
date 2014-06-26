#ifndef GOTHOGRE_ZEN_STRING_INTERFACE_H
#define GOTHOGRE_ZEN_STRING_INTERFACE_H

#include "GothOgre_FlagSet.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------------
	class ZenParamDef
	{
	public:
		virtual ~ZenParamDef() {}

		virtual const String& getName() = 0;
		virtual const String& getType() = 0;
		virtual size_t getTypeSize() = 0;

		virtual void doGet(const void* _target, void* _value) = 0;
		virtual void doSet(void* _target, const void* _value) {}

		virtual bool isReadOnly() {return false;}
		virtual bool isInternal() {return false;}
		virtual const String& getGroup() {return StringUtil::BLANK;}
		virtual const String& getSubType() {return StringUtil::BLANK;}
		virtual const String& getDescription() {return StringUtil::BLANK;}
	};


	//-----------------------------------------------------------------------------
	typedef list<String>::type				ZenParamNameList;
	typedef map<String, ZenParamDef*>::type	ZenParamDefMap;

	class GOTHOGRE_EXPORT ZenParamDictionary
	{
	public:
		void addParamDef(ZenParamDef* _param);
		
		ZenParamNameList getParamNameList() const;

		// returns nullptr if not found
		ZenParamDef* findParamDef(const String& _name) const;

	private:
		friend class ZenStringInterface;
		ZenParamDictionary();
		~ZenParamDictionary();

	private:
		ZenParamNameList	mParamNameList;
		ZenParamDefMap		mParamDefMap;
		size_t              mUseCount;
		map<String, ZenParamDictionary*>::type::iterator mIterator;
	};
	

	//-----------------------------------------------------------------------------
	typedef map<String, ZenParamDictionary*>::type ZenDictionaryMap;

	class GOTHOGRE_EXPORT ZenStringInterface
	{
	private:
		OGRE_STATIC_MUTEX( msDictionaryMutex );
		static ZenDictionaryMap		msDictionaryMap;
		ZenParamDictionary*			mParamDictionary;
		void destroyParamDictionary();

	protected:
		ZenStringInterface();
		virtual ~ZenStringInterface();

		bool createParamDictionary(const String& _className);
		ZenParamDictionary* getParamDictionary();
		const ZenParamDictionary* getParamDictionary() const;

	public:
		virtual void parameterChanged(const String& _name) {}
		virtual ZenParamNameList getParamNameList() const;
		virtual ZenParamDef* findParamDef(const String& _name, void** _targetPtr);

	public:
		ZenParamDef* findParamDef(const String& _name) const;
		ZenParamDef* findParamDef(const String& _name, const void** _targetPtr) const;
		ZenParamDef* getParamDef(const String& _name, void** _targetPtr);
		ZenParamDef* getParamDef(const String& _name, const void** _targetPtr) const;
		ZenParamDef* getParamDef(const String& _name) const;

		bool hasParameter(const String& _name) const
		{
			return findParamDef(_name) != nullptr;
		}

		template<typename T>
		T getParameter(const String& _name) const
		{
			const void* target;
			ZenParamDef* paramDef = getParamDef(_name, &target);
			if(paramDef->getTypeSize() != sizeof(T))
			{
				GOTHOGRE_EXCEPT("ZenParam '" << _name << "', type '" 
					<< paramDef->getType() << "' - Type mismatch; "
					<< paramDef->getTypeSize() << "!=" << sizeof(T));
			}
			T value;
			paramDef->doGet(target, &value);
			return value;
		}

		template<typename T>
		void setParameter(const String& _name, const T& _value)
		{
			void* target;
			ZenParamDef* paramDef = getParamDef(_name, &target);
			if(paramDef->getTypeSize() != sizeof(T))
			{
				GOTHOGRE_EXCEPT("ZenParam '" << _name << "', type '" 
					<< paramDef->getType() << "' - Type mismatch; "
					<< paramDef->getTypeSize() << "!=" << sizeof(T));
			}
			paramDef->doSet(target, &_value);
			parameterChanged(_name);
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_STRING_INTERFACE_H