#ifndef GOTHOGRE_MESH_EX_H
#define GOTHOGRE_MESH_EX_H

#include "GothOgre_SubclassSharedPtr.h"
#include "GothOgre_Mtl.h"
#include "GothOgre_MatGenParams.h"


namespace GothOgre
{
	class SubMeshEx;


	//--------------------------------------------------------------------------------------
	/** Version of the "Ogre::Mesh" class extended for GothOgre.
	This version contains some more information than the standard Mesh:
	1) it stores an information to create materials instead of names of materials themselves. 
	@Note This MeshEx class is safe replacement of the Mesh class,
	I mean, it's allowed to pass a pointer to MeshEx to any function
	which expects a pointer to Mesh. Also it's allowed to pass a pointer 
	to SubMeshEx to any function which expects a pointer to SubMesh. */
	class GOTHOGRE_EXPORT MeshEx : public Mesh
	{
	public:
		MeshEx(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader);
		~MeshEx();

		/** Creates a new SubMesh. */
		SubMeshEx* createSubMesh() {return (SubMeshEx*) Mesh::createSubMesh();}

		/** Creates a new SubMesh and gives it a name. */
		SubMeshEx* createSubMesh( const String& _name ) {return (SubMeshEx*) Mesh::createSubMesh( _name );}

		/** Destroy a SubMesh with the given index. */
		SubMeshEx* getSubMesh( unsigned short _index ) const {return (SubMeshEx*) Mesh::getSubMesh( _index );}
		
		/** Destroy a SubMesh with the given name. */
		SubMeshEx* getSubMesh( const String& _name ) const {return (SubMeshEx*) Mesh::getSubMesh( _name );}

		/** Iterator to iterate over submeshes. */
		class SubMeshExIterator : public Mesh::SubMeshIterator
		{
		public:
			SubMeshExIterator( const Mesh::SubMeshIterator& _srcIterator )
				: Mesh::SubMeshIterator( _srcIterator ) {}

			SubMeshEx* getNext() {return (SubMeshEx*) Mesh::SubMeshIterator::getNext();}
			SubMeshEx* peekNext() const {return (SubMeshEx*) Mesh::SubMeshIterator::peekNext();}
		};
		typedef SubMeshExIterator SubMeshIterator;

		/** Gets an iterator over the available submeshes. */
		SubMeshExIterator getSubMeshIterator()
		{
			return (SubMeshExIterator) Mesh::getSubMeshIterator();
		}
	};


	//---------------------------------------------------------------------------
	/** SubMeshEx is a part of a complete MeshEx,
	like SubMesh is a part of a complete mesh. */
	class GOTHOGRE_EXPORT SubMeshEx : public SubMesh
	{
	public:
		/** Sets an instance of the Mtl (material source data) class
		which must be used to create a material for this submesh. */
		void setMtl( const MtlPtr& _mtl ) 
		{
			getAddon().mtl = _mtl;
		}

		/** Returns an instance of the Mtl which was set for this submesh. */
		const MtlPtr& getMtl() const
		{
			return getAddon().mtl;
		}

		/** Sets an instance of the MatGenParams (additional material generation information) 
		which must be used to create a material for this submesh. */
		void setMatGenParams( const MatGenParams& _matGenParams )
		{
			getAddon().matGenParams = _matGenParams;
		}

		/** Returns an instance of the MatGenParams class 
		which was set for this submesh. */
		const MatGenParams& getMatGenParams() const
		{
			return getAddon().matGenParams;
		}

		/** Returns a pointer to parent Mesh (not a smart pointer 
		because child does not keep parent alive). */ 
		MeshEx* getParent() const
		{
			return (MeshEx*) parent;
		}

	private:
		struct Addon
		{
			MtlPtr        mtl;
			MatGenParams  matGenParams;
		};

		// Returns a reference to a structure containing 
		// an addon for this SubMesh.
		// (I could just add member variables to this class,
		// however I prefer to keep the size of this class unchanged
		// to allow easy type-casting SubMesh* <-> SubMeshEx*)
		Addon& getAddon();
		const Addon& getAddon() const;
	};


	//---------------------------------------------------------------------------
	/** Shared pointer to MeshEx. */
	typedef SubclassSharedPtr<MeshEx, MeshPtr> MeshExPtr;


} // namespace GothOgre

#endif // GOTHOGRE_MESH_EX_H