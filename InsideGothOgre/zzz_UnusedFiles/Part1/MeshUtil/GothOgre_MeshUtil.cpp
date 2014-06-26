#include "GothOgre_StdHeaders.h"
#include "GothOgre_MeshUtil.h"


namespace GothOgre
{
	static void moveVertexData(VertexData* _vertexData, const Vector3& _delta)
	{
		if(!_vertexData)
			return;

		const Ogre::VertexElement* posElem = _vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		if(!posElem)
			return;

		Ogre::VertexElementType elemType = posElem->getType();
		Ogre::HardwareVertexBufferSharedPtr hvb = _vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
		if(hvb.isNull())
			return;

		void* p = hvb->lock(Ogre::HardwareBuffer::HBL_NORMAL);
		size_t start = _vertexData->vertexStart;
		size_t count = _vertexData->vertexCount;
		size_t vsize = hvb->getVertexSize();
		size_t voffset = posElem->getOffset();
		for(size_t i = start; i != count; ++i)
		{
			void* pi = (void*) ((size_t) p + i * vsize + voffset);
			switch(elemType)
			{
				case Ogre::VET_FLOAT3: 
				{
					float* pf = (float*) pi;
					pf[0] += _delta.x;
					pf[1] += _delta.y;
					pf[2] += _delta.z;
					break;
				}
				default:
					GOTHOGRE_EXCEPT("This never should be!");
			}
		}

		hvb->unlock();
	}
	//-----------------------------------------------------------------------------
	void MeshUtil::moveMesh(const MeshPtr& _mesh, const Vector3& _delta)
	{
		moveVertexData(_mesh->sharedVertexData, _delta);
		Mesh::SubMeshIterator it = _mesh->getSubMeshIterator();
		while(it.hasMoreElements())
		{
			SubMesh* subMesh = it.getNext();
			moveVertexData(subMesh->vertexData, _delta);
		}

		AxisAlignedBox box = _mesh->getBounds();
		box.setMinimum(box.getMinimum() + _delta);
		box.setMaximum(box.getMaximum() + _delta);
		_mesh->_setBounds(box, false);
	}

} // namespace GothOgre