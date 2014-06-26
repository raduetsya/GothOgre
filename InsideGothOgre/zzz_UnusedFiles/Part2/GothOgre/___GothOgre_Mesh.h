#ifndef GOTHOGRE_MESH_H
#define GOTHOGRE_MESH_H

#include "GothOgre_MeshPtr.h"


namespace GothOgre
{
	class MeshPtr;

	/** This class extends the standard "Ogre::Mesh" class for this project.
	This class does not contain member variables, and a pointer to Mesh
	can be directly transformed from a pointer to Ogre::Mesh:
	Ogre::Mesh* mesh = ...;
	Mesh* ex = reinterpret_cast<Mesh*>(mesh) 
	*/
	class GOTHOGRE_EXPORT Mesh : public Ogre::Mesh
	{
	private:
		/** Protected constructor and destructor:
		Objects of the class are not created and not destroyed;
		these constructor and destructor are never called. */
		Mesh();
		~Mesh();

	public:
		
	};

} // namespace GothOgre

#endif // GOTHOGRE_MESH_H