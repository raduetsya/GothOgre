#ifndef ZEN_OBJECT_PROPERTIES_UTIL_H
#define ZEN_OBJECT_PROPERTIES_UTIL_H

namespace GUISystem
{
	namespace ZenObjectProperties
	{
		class ParamView;
		class Group;

		struct Util
		{
			static const String MULTIPLE_VALUES;

			static ParamView* createParamView(ZenParamDef* _paramDef, Group* _group);
			static bool equalsParamValues(ZenParamDef* _paramDef, ZenStringInterface* _obj1, ZenStringInterface* _obj2);
		};

	}
}

#endif // ZEN_OBJECT_PROPERTIES_UTIL_H