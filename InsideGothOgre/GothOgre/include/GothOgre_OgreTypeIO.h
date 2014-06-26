#ifndef GOTHOGRE_OGRE_TYPE_IO_H
#define GOTHOGRE_OGRE_TYPE_IO_H

#include "GothOgre_StrStream.h"


namespace GothOgre
{
	// Vector2
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const Vector2& _vec);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, Vector2& _vec);

	// Vector3
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const Vector3& _vec);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, Vector3& _vec);

	// Quaternion
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const Quaternion& _vec);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, Quaternion& _vec);

	// AxisAlignedBox
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const AxisAlignedBox& _aab);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, AxisAlignedBox& _aab);

	// ColourValue
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const ColourValue& _clr);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, ColourValue& _clr);

	// PolygonMode
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const PolygonMode& _pm);
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, PolygonMode& _pm);

} // namespace GothOgre

#endif // GOTHOGRE_OGRE_TYPE_IO_H