#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenStringInterface2.h"

namespace GothOgre
{
	// types
	const String ZenParamInt::TYPE     = "Int";
	const String ZenParamEnum::TYPE    = "Enum";
	const String ZenParamReal::TYPE    = "Real";
	const String ZenParamBool::TYPE    = "Bool";
	const String ZenParamString::TYPE  = "String";
	const String ZenParamColour::TYPE  = "Colour";
	const String ZenParamVector3::TYPE = "Vector3";
	const String ZenParamQuaternion::TYPE = "Quaternion";
	const String ZenParamAxisAlignedBox::TYPE = "AxisAlignedBox";
	const String ZenParamTime::TYPE       = "Time";

	// subtypes
	const String ZenParamString::SubType::VISUAL_NAME = "VISUAL_NAME";
	const String ZenParamString::SubType::SOUND_NAME  = "SOUND_NAME";

} // namespace GothOgre