#include "GothOgre_Precompiled.h"
#include "GothOgre_Material.h"
#include "GothOgre_MaterialPtr.h"
#include "GothOgre_StringUtil.h"
#include "GothOgre_TextureManager.h"
#include "GothOgre_TexturePtr.h"
#include "GothOgre_MaterialManager.h"
#include "GothOgre_StringConverter.h"

namespace GothOgre
{
	Material::Material() 
		: Ogre::Material(nullptr, "", 0, "") 
	{
		GOTHOGRE_CRITICAL("Material::Material never should be called!");
	}
	//----------------------------------------------------------------------------
	Material::~Material() 
	{
		GOTHOGRE_CRITICAL("Material::~Material() never should be called!");
	}
	//----------------------------------------------------------------------------
	bool Material::isPortal() const
	{
		const String& name = getName();
		if(name.length() >= 2)
		{
			if(tolower(name[0]) == 'p' && name[1] == ':')
				return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------
	std::pair<String, String> Material::getPortalRooms() const
	{
		const String& name = getName();
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
	//----------------------------------------------------------------------------
	bool Material::isGhostOccluder() const
	{
		return StringUtil::equalsIgnoreCase(getName(), "ghostoccluder");
	}
	//----------------------------------------------------------------------------
	void Material::setMatGroup(MatGroup::Enum _matGroup) const
	{
		getExtra()->matGroup = _matGroup;
	}
	//----------------------------------------------------------------------------
	MatGroup::Enum Material::getMatGroup() const
	{
		return getExtra()->matGroup;
	}
	//----------------------------------------------------------------------------
	void Material::setSmoothAngle(Degree _smoothAngle)
	{
		getExtra()->smoothAngle = _smoothAngle;
	}
	//----------------------------------------------------------------------------
	Degree Material::getSmoothAngle() const
	{
		return getExtra()->smoothAngle;
	}
	//----------------------------------------------------------------------------
	void Material::setColor(const ColourValue& _color)
	{
		setDiffuse(_color);
		setAmbient(_color);
	}
	//----------------------------------------------------------------------------
	const ColourValue& Material::getColor() const
	{
		Technique* technique = const_cast<Material*>(this)->getTechnique(0);
		Pass* pass = technique->getPass(0);
		return pass->getDiffuse();
	}
	//----------------------------------------------------------------------------
	void Material::setTexture(const String& _texture)
	{
		Technique* technique = getTechnique(0);
		Pass* pass = technique->getPass(0);
		TextureUnitState* tus = pass->getNumTextureUnitStates() ? pass->getTextureUnitState(0) : pass->createTextureUnitState();
		TexturePtr ogreTexture = TextureManager::getSingleton().create(_texture);
		const String& ogreTexName = ogreTexture->getName();
		tus->setTextureName(ogreTexName);
	}
	//----------------------------------------------------------------------------
	const String& Material::getTexture() const
	{
		Technique* technique = const_cast<Material*>(this)->getTechnique(0);
		Pass* pass = technique->getPass(0);
		if(pass->getNumTextureUnitStates() != 0)
		{
			TextureUnitState* tus = pass->getTextureUnitState(0);
			if(tus->getNumFrames() == 0)
			{
				return tus->getTextureName();
			}
			else
			{
				return tus->getFrameTextureName(0);
			}
		}
		return StringUtil::BLANK;
	}
	//----------------------------------------------------------------------------
	void Material::setTexAniFPS(Real _texAniFPS)
	{
		Technique* technique = getTechnique(0);
		Pass* pass = technique->getPass(0);
		TextureUnitState* tus = pass->getNumTextureUnitStates() ? pass->getTextureUnitState(0) : pass->createTextureUnitState();
		String texture = getTexture();

		if(_texAniFPS == 0)
		{
			tus->setAnimatedTextureName(&texture, 1, 0);
		}
		else
		{
			vector<String>::type ogreTexNames = findAnimatedTextureNames(getTexture());
			Real duration = ogreTexNames.size() / _texAniFPS;

			tus->setAnimatedTextureName(&ogreTexNames[0], ogreTexNames.size(), duration);
		}
	}
	//----------------------------------------------------------------------------
	vector<String>::type Material::findAnimatedTextureNames(const String& _texname)
	{
		vector<String>::type ogreTexNames;

		TextureManager& tmgr = TextureManager::getSingleton();
		TexturePtr ogreTexture = tmgr.create(_texname);
		ogreTexNames.push_back(ogreTexture->getName());

		size_t dotpos = _texname.rfind('.');
		if(dotpos != String::npos && dotpos >= 1 && isdigit(_texname[dotpos-1]))
		{
			size_t digpos = dotpos - 1;
			while(digpos >= 0 && isdigit(_texname[digpos]))
				--digpos;
			++digpos;

			String basename = _texname.substr(0, digpos);
			String digits = _texname.substr(digpos, dotpos - digpos);
			String fileext = _texname.substr(dotpos);
			int index = StringConverter::fromString<int>(digits);

			while(true)
			{
				++index;
				digits = StringConverter::toString<int>(index);
				String newtexname = basename + digits + fileext;
				if(!tmgr.canLoad(newtexname))
					break;

				TexturePtr newtex = tmgr.create(newtexname);
				ogreTexNames.push_back(newtex->getName());
			}
		}		
		return ogreTexNames;
	}
	//----------------------------------------------------------------------------
	class TextureUnitState_Duration : public TextureUnitState
	{
	public:
		// Access to protected member TextureUnitState::mAnimDuration
		Real getAnimDuration() const 
		{
			return mAnimDuration;
		}
	};
	//----------------------------------------------------------------------------
	Real Material::getTexAniFPS() const
	{
		Technique* technique = const_cast<Material*>(this)->getTechnique(0);
		Pass* pass = technique->getPass(0);
		if(pass->getNumTextureUnitStates() != 0)
		{
			TextureUnitState* tus = pass->getTextureUnitState(0);
			Real duration = static_cast<TextureUnitState_Duration*>(tus)->getAnimDuration();
			if(duration != 0)
				return tus->getNumFrames() / duration;
		}
		return 0;
	}
	//----------------------------------------------------------------------------
	void Material::update()
	{
		correctAlpha();
		setReceiveShadows(true);
		updateSelectedMaterial();
	}
	//----------------------------------------------------------------------------
	void Material::correctAlpha()
	{
		if(isPortal() || isGhostOccluder())
		{
			// create fully transparent material
			Material::TechniqueIterator it = getTechniqueIterator();
			while(it.hasMoreElements())
			{
				Technique* technique = it.getNext();
				Technique::PassIterator it2 = technique->getPassIterator();
				while(it2.hasMoreElements())
				{
					Pass* pass = it2.getNext();
					pass->setAlphaRejectFunction(CMPF_ALWAYS_FAIL);
				}
			}
		}
		else
		{
			Material::TechniqueIterator it = getTechniqueIterator();
			while(it.hasMoreElements())
			{
				Technique* technique = it.getNext();
				Technique::PassIterator it2 = technique->getPassIterator();
				while(it2.hasMoreElements())
				{
					Pass* pass = it2.getNext();
					pass->setAlphaRejectFunction(CMPF_GREATER);
					pass->setAlphaRejectValue(220);
				}
			}
		}
	}
	//----------------------------------------------------------------------------
	void Material::updateSelectedMaterial()
	{
		if(!isSelectedVersion())
		{
			String selVerName = getSelectedVersionName();
			MaterialPtr selVer = MaterialManager::getSingleton().findByName(selVerName);
			if(!selVer.isNull())
			{
				initialiseSelectedVersion(selVer);
			}
		}
	}
	//----------------------------------------------------------------------------
	bool Material::isSelectedVersion() const
	{
		const String& name = getName();
		return (!name.empty() && name[0] == '\x01');
	}
	//----------------------------------------------------------------------------
	String Material::getSelectedVersionName() const
	{
		String name = getName();
		if(name.empty() || name[0] != '\x01')
			name = '\x01' + name;
		return name;
	}
	//----------------------------------------------------------------------------
	String Material::getUnselectedVersionName() const
	{
		String name = getName();
		if(!name.empty() && name[0] == '\x01')
			name = name.substr(1);
		return name;
	}
	//----------------------------------------------------------------------------
	void Material::initialiseSelectedVersion(MaterialPtr& _selVer) const
	{
		copyDetailsTo(_selVer);
		Technique* technique = _selVer->getTechnique(0);
		Pass* pass0 = technique->getPass(0);
		Pass* passNew = technique->createPass();
		passNew->setDiffuse(1, 1, 1, 1);
		passNew->setPolygonMode(PM_WIREFRAME);

		if(pass0->getNumTextureUnitStates() != 0)
		{
			TextureUnitState* tu = pass0->getTextureUnitState(0);
			tu->setColourOperationEx(LBX_MODULATE, 
				LBS_TEXTURE, LBS_MANUAL,
				ColourValue::White, ColourValue(1, 0.6f, 0.6f, 1));
		}
	}
	//----------------------------------------------------------------------------
	MaterialPtr Material::getSelectedVersion() const
	{
		String selVerName = getSelectedVersionName();
		MaterialPtr selVer = MaterialManager::getSingleton().findByName(selVerName);
		if(selVer.isNull())
		{
			selVer = MaterialManager::getSingleton().createManual(selVerName);
			initialiseSelectedVersion(selVer);
		}
		return selVer;
	}
	//----------------------------------------------------------------------------
	MaterialPtr Material::getUnselectedVersion() const
	{
		String unselVerName = getUnselectedVersionName();
		MaterialPtr unselVer = MaterialManager::getSingleton().findByName(unselVerName);
		return unselVer;
	}
	//----------------------------------------------------------------------------
	Material::Extra* Material::getExtra() const
	{
		Technique* technique = const_cast<Material*>(this)->getTechnique(0);
		UserObjectBindings& uob = technique->getUserObjectBindings();
		const Any& any = uob.getUserAny();
		if(any.isEmpty())
		{
			Extra* ret = new Extra;
			uob.setUserAny(Any(ExtraPtr(ret)));
			return ret;
		}
		ExtraPtr ptr = any.operator ()<ExtraPtr>();
		Extra* ret = ptr.get();
		return ret;		
	}
	//----------------------------------------------------------------------------
	Material::Extra::Extra()
	{
		matGroup = MatGroup::UNDEF;
		smoothAngle = Degree(60);
	}

} // namespace GothOgre