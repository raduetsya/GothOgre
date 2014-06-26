#include "Precompiled.h"
#include "ZenObjectPropertiesUtil.h"
#include "ZenObjectPropertiesParamViews1.h"
#include "ZenObjectPropertiesParamViews2.h"

namespace GUISystem
{
	namespace ZenObjectProperties
	{
		const String Util::MULTIPLE_VALUES = "<<Multiple Values>>";
		//----------------------------------------------------------------------------
		ParamView* Util::createParamView(ZenParamDef* _paramDef, Group* _group)
		{
			const String& paramType = _paramDef->getType();
			const String& paramSubType = _paramDef->getSubType();
			ParamView* p = nullptr;

			if(paramType == ZenParamBool::TYPE)
				p = new BoolView;
			else if(paramType == ZenParamReal::TYPE)
				p = new RealView;
			else if(paramType == ZenParamInt::TYPE)
				p = new IntView;
			else if(paramType == ZenParamEnum::TYPE)
				p = new EnumView;
			else if(paramType == ZenParamString::TYPE)
			{
				if(paramSubType == ZenParamString::SubType::VISUAL_NAME)
					p = new VisualNameView;
				else if(paramSubType == ZenParamString::SubType::SOUND_NAME)
					p = new SoundNameView;
				else
					p = new StringView;
			}
			else if(paramType == ZenParamVector3::TYPE)
				p = new Vector3View;
			else if(paramType == ZenParamQuaternion::TYPE)
				p = new QuaternionView;
			else if(paramType == ZenParamAxisAlignedBox::TYPE)
				p = new AxisAlignedBoxView;
			else if(paramType == ZenParamTime::TYPE)
				p = new TimeView;
			else if(paramType == ZenParamColour::TYPE)
				p = new ColourView;
			else
			{
				GOTHOGRE_ERROR("Illegal param type: " << paramType);
				return nullptr;
			}

			p->baseinit(_paramDef, _group);
			return p;
		}
		//----------------------------------------------------------------------------
		bool Util::equalsParamValues(ZenParamDef* _paramDef, ZenStringInterface* _obj1, ZenStringInterface* _obj2)
		{
			const String& paramName = _paramDef->getName();
			const String& paramType = _paramDef->getType();
			bool equals;

			if(paramType == ZenParamBool::TYPE)
			{
				equals = (_obj1->getParameter<bool>(paramName) 
				       == _obj2->getParameter<bool>(paramName));
			}
			else if(paramType == ZenParamReal::TYPE)
			{
				equals = (_obj1->getParameter<Real>(paramName) 
				       == _obj2->getParameter<Real>(paramName));
			}
			else if(paramType == ZenParamInt::TYPE)
			{
				equals = (_obj1->getParameter<int>(paramName) 
				       == _obj2->getParameter<int>(paramName));
			}
			else if(paramType == ZenParamEnum::TYPE)
			{
				equals = (_obj1->getParameter<Real>(paramName) 
				       == _obj2->getParameter<Real>(paramName));
			}
			else if(paramType == ZenParamString::TYPE)
			{
				equals = (_obj1->getParameter<String>(paramName) 
				       == _obj2->getParameter<String>(paramName));
			}
			else if(paramType == ZenParamVector3::TYPE)
			{
				equals = (_obj1->getParameter<Vector3>(paramName) 
				       == _obj2->getParameter<Vector3>(paramName));
			}
			else if(paramType == ZenParamQuaternion::TYPE)
			{
				equals = (_obj1->getParameter<Quaternion>(paramName) 
				       == _obj2->getParameter<Quaternion>(paramName));
			}
			else if(paramType == ZenParamAxisAlignedBox::TYPE)
			{
				equals = (_obj1->getParameter<AxisAlignedBox>(paramName) 
				       == _obj2->getParameter<AxisAlignedBox>(paramName));
			}
			else if(paramType == ZenParamColour::TYPE)
			{
				equals = (_obj1->getParameter<ColourValue>(paramName) 
				       == _obj2->getParameter<ColourValue>(paramName));
			}
			else if(paramType == ZenParamTime::TYPE)
			{
				equals = (_obj1->getParameter<Time>(paramName) 
				       == _obj2->getParameter<Time>(paramName));
			}
			else
			{
				GOTHOGRE_ERROR("Could not compare two values of type: " << paramType);
				equals = false;
			}
			return equals;
		}
	}
}