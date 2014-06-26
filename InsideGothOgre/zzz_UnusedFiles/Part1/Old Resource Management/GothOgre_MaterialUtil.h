#ifndef GOTHOGRE_MATERIAL_UTIL_H
#define GOTHOGRE_MATERIAL_UTIL_H

namespace GothOgre
{
	struct MatGroup
	{
		enum Type
		{
			UNDEF,
			METALL,
			STONE,
			WOOD,
			EARTH,
			WATER
		};
	};

	class GOTHOGRE_EXPORT MaterialUtil
	{
	public:
		/** Returns true if the specified material is portal,
		i.e. its name starts with "P:". */
		static bool isPortal(const MaterialPtr& _material);

		/** Returns a pair of two strings: roomFrom and roomTo,
		which are parts of the specified portal name. */
		static std::pair<String, String> getPortalRooms(const MaterialPtr& _material);

		/** Returns true if the material' name is "ghostoccluder". */
		static bool isGhostOccluder(const MaterialPtr& _material);

		/** Corrects alpha for manual created materials.
		The function makes portals and ghostoccluder invisible. */
		static void correctAlpha(const MaterialPtr& _material);

		static MatGroup::Type getMatGroup(const MaterialPtr& _material);
		static void setMatGroup(const MaterialPtr& _material, MatGroup::Type _matGroup);

		static Degree getSmoothAngle(const MaterialPtr& _material);
		static void setSmoothAngle(const MaterialPtr& _material, Degree _smoothAngle);

		static void setColor(const MaterialPtr& _material, const ColourValue& _color);
		static const ColourValue& getColor(const MaterialPtr& _material);

		static void setTexture(const MaterialPtr& _material, const String& _texture);
		static const String& getTexture(const MaterialPtr& _material);

		static void setTexAniFPS(const MaterialPtr& _material, Real _fps);
		static Real getTexAniFPS(const MaterialPtr& _material);

		static String getSelectedVersionName(const String& _materialName);
		static String getUnselectedVersionName(const String& _selectedMaterialName);
		static MaterialPtr getSelectedVersion(const MaterialPtr& _material);
		static MaterialPtr getUnselectedVersion(const MaterialPtr& _selectedMaterial);
		static void updateSelectedVersion(const MaterialPtr& _material);
	};

} // namespace GothOgre

#endif // GOTHOGRE_MATERIAL_UTIL_H