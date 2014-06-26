#ifndef GOTHOGRE_MATERIAL_H
#define GOTHOGRE_MATERIAL_H

#include "GothOgre_MaterialPtr.h"


namespace GothOgre
{
	struct MatGroup
	{
		enum Enum
		{
			UNDEF,
			METALL,
			STONE,
			WOOD,
			EARTH,
			WATER
		};
	};

	class MaterialPtr;

	/** This class extends the standard "Ogre::Material" class for this project.
	This class does not contain member variables, and a pointer to Material
	can be directly transformed from a pointer to Ogre::Material:
	Ogre::Material* mat = ...;
	Material* ex = reinterpret_cast<Material*>(mat) 
	*/
	class GOTHOGRE_EXPORT Material : public Ogre::Material
	{
	private:
		/** Private constructor and destructor:
		Objects of the class are not created and not destroyed;
		these constructor and destructor are never called. */
		Material();
		virtual ~Material();

	public:
		/** Returns true if the specified material is portal,
		i.e. its name starts with "P:". */
		bool isPortal() const;

		/** Returns a pair of two strings: roomFrom and roomTo,
		which are parts of the specified portal name. */
		std::pair<String, String> getPortalRooms() const;

		/** Returns true if the material' name is "ghostoccluder". */
		bool isGhostOccluder() const;

		void setMatGroup(MatGroup::Enum _matGroup) const;
		MatGroup::Enum getMatGroup() const;

		void setSmoothAngle(Degree _smoothAngle);
		Degree getSmoothAngle() const;

		void setColor(const ColourValue& _color);
		const ColourValue& getColor() const;

		void setTexture(const String& _texture);
		const String& getTexture() const;

		void setTexAniFPS(Real _texAniFPS);
		Real getTexAniFPS() const;

		/** Call it after you finish with changing the material. 
		The function corrects alpha,
		makes portals and ghostoccluder invisible,
		updates the material's selected version (if it exists).	*/
		void update();

		bool isSelectedVersion() const;
		MaterialPtr getSelectedVersion() const;
		MaterialPtr getUnselectedVersion() const;

	private:
		struct Extra
		{
			Extra();

			MatGroup::Enum	matGroup;
			Degree			smoothAngle;
		};
		
		typedef SharedPtr<Extra> ExtraPtr;

	private:
		friend std::ostream& operator <<(std::ostream& _o, const ExtraPtr& _ptr) {return _o;}
		Extra* getExtra() const;
		vector<String>::type findAnimatedTextureNames(const String& _texture);
		String getSelectedVersionName() const;
		String getUnselectedVersionName() const;
		void initialiseSelectedVersion(MaterialPtr& _selVer) const;
		void updateSelectedMaterial();
		void correctAlpha();
	};

} // namespace GothOgre

#endif // GOTHOGRE_MATERIAL_H