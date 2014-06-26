#ifndef GUISYSTEM_WRAPS_ATTRIBUTE_2_H
#define GUISYSTEM_WRAPS_ATTRIBUTE_2_H

#include "GUISystem_WrapsAttribute.h"

namespace attribute
{
	// Class attribute: main widget's name as it is specified in *.layout file.
	// The default value is "_Main".
	DECLARE_ATTRIBUTE_CLASS(AttributeMainWidget, std::string);

#define ATTRIBUTE_CLASS_MAIN_WIDGET(_class, _value) \
	ATTRIBUTE_CLASS(AttributeMainWidget, _class, _value)


	// Class attribute: name of section in file ui.cfg
	// There is no default value; if the attribute is not specified 
	// then widget's coordinates will not be stored in configuration file.
	DECLARE_ATTRIBUTE_CLASS(AttributeConfigSection, std::string);

#define ATTRIBUTE_CLASS_CONFIG_SECTION(_class, _value) \
	ATTRIBUTE_CLASS(AttributeConfigSection, _class, _value)

	// Field attribute: specifies name of the widget in the layout.
	// This is mostly the same as ATTRIBUTE_FIELD_WIDGET_NAME.
	// There is only one difference: This attribute is specifies the name for "optional" widget,
	// i.e. BaseLayout will not throw an exception if it will not able to find the widget in layout.
	DECLARE_ATTRIBUTE_FIELD(AttributeFieldWidgetNameOptional, std::string, FieldSetterWidget);

#define ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(_class, _field, _value) \
	ATTRIBUTE_FIELD(AttributeFieldWidgetNameOptional, _class, _field, _value)

} // namespace GUISystem

#endif // GUISYSTEM_WRAPS_ATTRIBUTE_2_H
