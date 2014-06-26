#include "GothOgre_StdHeaders.h"
#include "GothOgre_MaterialUtil.h"
#include "GothOgre_MaterialManager.h"
#include "GothOgre_TextureManager.h"
#include "GothOgre_StringUtil.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------------
	struct MaterialInternal;
	typedef SharedPtr<MaterialInternal> MaterialInternalPtr;

	std::ostream& operator <<(std::ostream& o, const MaterialInternalPtr& _ptr);

	/** Internal material data which is stored in the UserObjectBindings
	of the first technique. */
	struct MaterialInternal
	{
		MatGroup::Type	mMatGroup;
		Degree			mSmoothAngle;
		String			mTexture;
		Real			mTexAniFPS;
	
		MaterialInternal() 
		{
			mMatGroup = MatGroup::UNDEF;
			mSmoothAngle = Degree(60);
			mTexture = "";
			mTexAniFPS = 0;
		}

		static MaterialInternal* get(const MaterialPtr& _material)
		{
			Technique* technique = _material->getTechnique(0);
			UserObjectBindings& uob = technique->getUserObjectBindings();
			const Any& any = uob.getUserAny();
			if(any.isEmpty())
			{
				MaterialInternal* ret = new MaterialInternal;
				uob.setUserAny(Any(MaterialInternalPtr(ret)));
				return ret;
			}
			MaterialInternalPtr ptr = any.operator ()<MaterialInternalPtr>();
			MaterialInternal* ret = ptr.get();
			return ret;
		}
	};

	std::ostream& operator <<(std::ostream& o, const MaterialInternalPtr& _ptr)
	{
		o << _ptr.get();
		return o;
	}
	//--------------------------------------------------------------------------------
	bool MaterialUtil::isPortal(const MaterialPtr& _material)
	{
		const String& name = _material->getName();
		if(name.length() >= 2)
		{
			if(tolower(name[0]) == 'p' && name[1] == ':')
				return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------
	std::pair<String, String> MaterialUtil::getPortalRooms(const MaterialPtr& _material)
	{
		const String& name = _material->getName();
		String roomFrom, roomTo;
		if(name.length() >= 4)
		{
			size_t pos = name.find('_');
			if(pos != String::npos)
			{
				roomFrom = name.substr(2, pos - 2);
				roomTo = name.substr(pos + 1);
			}
		}
		return std::make_pair(roomFrom, roomTo);
	}
	//--------------------------------------------------------------------------------
	bool MaterialUtil::isGhostOccluder(const MaterialPtr& _material)
	{
		return StringUtil::equalsNoCase(_material->getName(), "ghostoccluder");
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::correctAlpha(const MaterialPtr& _material)
	{
		if(isPortal(_material) || isGhostOccluder(_material))
		{
			// create fully transparent material
			Material::TechniqueIterator it = _material->getTechniqueIterator();
			while(it.hasMoreElements())
			{
				Technique* technique = it.getNext();
				Technique::PassIterator it2 = technique->getPassIterator();
				while(it2.hasMoreElements())
				{
					Pass* pass = it2.getNext();
					pass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_FAIL);
				}
			}
		}
		else
		{
			Material::TechniqueIterator it = _material->getTechniqueIterator();
			while(it.hasMoreElements())
			{
				Technique* technique = it.getNext();
				Technique::PassIterator it2 = technique->getPassIterator();
				while(it2.hasMoreElements())
				{
					Pass* pass = it2.getNext();
					pass->setAlphaRejectFunction(Ogre::CMPF_GREATER);
					pass->setAlphaRejectValue(220);
				}
			}
			_material->setReceiveShadows(true);
		}
	}
	//--------------------------------------------------------------------------------
	MatGroup::Type MaterialUtil::getMatGroup(const MaterialPtr& _material)
	{
		return MaterialInternal::get(_material)->mMatGroup;
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::setMatGroup(const MaterialPtr& _material, MatGroup::Type _matGroup)
	{
		MaterialInternal::get(_material)->mMatGroup = _matGroup;
	}
	//--------------------------------------------------------------------------------
	Degree MaterialUtil::getSmoothAngle(const MaterialPtr& _material)
	{
		return MaterialInternal::get(_material)->mSmoothAngle;
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::setSmoothAngle(const MaterialPtr& _material, Degree _smoothAngle)
	{
		MaterialInternal::get(_material)->mSmoothAngle = _smoothAngle;
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::setColor(const MaterialPtr& _material, const ColourValue& _color)
	{
		Technique* technique = _material->getTechnique(0);
		Pass* pass = technique->getPass(0);
		pass->setDiffuse( _color );
		pass->setAmbient( _color );
	}
	//--------------------------------------------------------------------------------
	const ColourValue& MaterialUtil::getColor(const MaterialPtr& _material)
	{
		Technique* technique = _material->getTechnique(0);
		Pass* pass = technique->getPass(0);
		return pass->getDiffuse();
	}
	//--------------------------------------------------------------------------------
	static void setTextureAndTexAniFPS(const MaterialPtr& _material, const String& _texture, Real _texAniFPS)
	{
		Technique* technique = _material->getTechnique(0);
		Pass* pass = technique->getPass(0);
		if(_texture.empty())
		{
			pass->removeAllTextureUnitStates();
		}
		else
		{
			TextureManager& tmgr = TextureManager::getSingleton();
			Ogre::TextureUnitState* tus = pass->getNumTextureUnitStates() ? pass->getTextureUnitState(0) : pass->createTextureUnitState();
			if(_texAniFPS == 0)
			{
				TexturePtr ogreTexture = tmgr.createOrRetrieve(_texture).first;
				const String& ogreTexName = ogreTexture->getName();
				tus->setTextureName(ogreTexName);
			}
			else
			{
				vector<String>::type ogreTexNames;
				TexturePtr ogreTexture = tmgr.createOrRetrieve(_texture).first;
				ogreTexNames.push_back(ogreTexture->getName());

				size_t dotpos = _texture.rfind('.');
				if(dotpos != String::npos && dotpos >= 1 
					&& '0' <= _texture[dotpos-1]  && _texture[dotpos-1] < '9')
				{
					String tmpname = _texture;
					char& dig0 = tmpname[dotpos - 1];
					++dig0;
					while(!tmgr.getByName(tmpname).isNull() || tmgr.canLoadResource(tmpname, TextureManager::GROUP_NAME))
					{
						TexturePtr ogreTexture = tmgr.createOrRetrieve(tmpname).first;
						ogreTexNames.push_back(ogreTexture->getName());
						++dig0;
						if(dig0 > '9')
						{
							if(dotpos >= 2 && '0' <= _texture[dotpos-2]  && _texture[dotpos-2] < '9')
							{
								char& dig1 = tmpname[dotpos-2];
								++dig1;
								dig0 = '0';
							}
							else
								break;
						}
					}
				}
				Real duration = ogreTexNames.size() / _texAniFPS;
				tus->setAnimatedTextureName(&ogreTexNames[0], ogreTexNames.size(), duration);
			}
		}
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::setTexture(const MaterialPtr& _material, const String& _texture)
	{
		MaterialInternal::get(_material)->mTexture = _texture;
		setTextureAndTexAniFPS(_material, _texture, MaterialInternal::get(_material)->mTexAniFPS);
	}
	//--------------------------------------------------------------------------------
	const String& MaterialUtil::getTexture(const MaterialPtr& _material)
	{
		return MaterialInternal::get(_material)->mTexture;
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::setTexAniFPS(const MaterialPtr& _material, Real _fps)
	{
		MaterialInternal::get(_material)->mTexAniFPS = _fps;
		setTextureAndTexAniFPS(_material, MaterialInternal::get(_material)->mTexture, _fps);
	}
	//--------------------------------------------------------------------------------
	Real MaterialUtil::getTexAniFPS(const MaterialPtr& _material)
	{
		return MaterialInternal::get(_material)->mTexAniFPS;
	}
	//--------------------------------------------------------------------------------
	String MaterialUtil::getSelectedVersionName(const String& _materialName)
	{
		String name(_materialName);
		if(name.empty() || name[0] != '\x01')
			name = '\x01' + name;
		return name;
	}
	//--------------------------------------------------------------------------------
	String MaterialUtil::getUnselectedVersionName(const String& _selectedMaterialName)
	{
		String name(_selectedMaterialName);
		if(!name.empty() && name[0] == '\x01')
			name = name.substr(1);
		return name;
	}
	//--------------------------------------------------------------------------------
	MaterialPtr MaterialUtil::getSelectedVersion(const MaterialPtr& _material)
	{
		const String& name = _material->getName();
		String selName = getSelectedVersionName(name);
		if(selName == name)
			return _material;
		
		MaterialPtr selectedMaterial = MaterialManager::getSingleton().getByName(selName);
		if(selectedMaterial.isNull())
		{
			selectedMaterial = MaterialManager::getSingleton().createManual(selName);
			updateSelectedVersion(_material);
		}
		return selectedMaterial;
	}
	//--------------------------------------------------------------------------------
	MaterialPtr MaterialUtil::getUnselectedVersion(const MaterialPtr& _selectedMaterial)
	{
		const String& selName = _selectedMaterial->getName();
		String name = getUnselectedVersionName(selName);
		if(selName == name)
			return _selectedMaterial;

		MaterialPtr material = MaterialManager::getSingleton().getByName(name);
		return material;
	}
	//--------------------------------------------------------------------------------
	void MaterialUtil::updateSelectedVersion(const MaterialPtr& _material)
	{
		String name = _material->getName();
		String selName = getSelectedVersionName(name);
		MaterialPtr material, selectedMaterial;
		if(selName == name)
		{
			name = getUnselectedVersionName(selName);
			selectedMaterial = _material;
			material = MaterialManager::getSingleton().getByName(name);
		}
		else
		{
			material = _material;
			selectedMaterial = MaterialManager::getSingleton().getByName(selName);
		}
		
		if(!material.isNull() && !selectedMaterial.isNull())
		{
			material->copyDetailsTo(selectedMaterial);
			Ogre::Technique* technique = selectedMaterial->getBestTechnique();
			Pass* pass0 = technique->getPass(0);
			Pass* passNew = technique->createPass();
			passNew->setDiffuse(1, 1, 1, 1);
			passNew->setPolygonMode(Ogre::PM_WIREFRAME);

			if(pass0->getNumTextureUnitStates() != 0)
			{
				Ogre::TextureUnitState* tu = pass0->getTextureUnitState(0);
				tu->setColourOperationEx(Ogre::LBX_MODULATE, 
					Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL,
					ColourValue::White, ColourValue(1, 0.6f, 0.6f, 1));
			}
		}
	}

} // namespace GothOgre
