#include "Precompiled.h"
#include "MaterialGeneratorImpl.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	MaterialGeneratorImpl::MaterialGeneratorImpl( SceneManagerExImpl* _sceneManager )
		: MaterialGenerator( _sceneManager )
	{
	}
	//--------------------------------------------------------------------------
	MaterialGeneratorImpl::~MaterialGeneratorImpl()
	{
	}
	//--------------------------------------------------------------------------
	void MaterialGeneratorImpl::setupMaterial( Material& _rMaterial, 
		const MtlPtr& _mtl, const MatGenParams& _matGenParams )
	{
		setNumTechniques( _rMaterial, 1 );
		Technique* technique = _rMaterial.getTechnique(0);

		// Setup first pass
		setNumPasses( technique, 1, OR_GREATER );
		Pass* pass = technique->getPass(0);

		// Reject fully transparent pixels
		pass->setAlphaRejectFunction( CMPF_GREATER_EQUAL );
		pass->setAlphaRejectValue( 220 );

		if(_mtl->getTexture().empty())
		{
			// no texture, only colour
			setNumTextureUnitStates(pass, 0);
			pass->setDiffuse( _mtl->getColour() );
			pass->setAmbient( _mtl->getColour() );
		}
		else
		{
			setNumTextureUnitStates(pass, 1);
			TextureUnitState* tus = pass->getTextureUnitState(0);
			if(_mtl->getFPS() == 0)
			{
				// texture, not animated
				tus->setTextureName( getRealTextureName( _mtl->getTexture() ));
			}
			else
			{
				// animated texture
				StringVector frames = findAnimatedTextureNames( _mtl->getTexture() );
				Real duration = frames.size() / _mtl->getFPS();
				tus->setAnimatedTextureName(&frames[0], frames.size(), duration);
			}
		}

		// Setup second pass
		if( _matGenParams.isSelected() )
		{
			setNumPasses( technique, 2 );
			Pass* pass = technique->getPass(1);
			pass->setPolygonMode( PM_WIREFRAME );
			pass->setAmbient( 1.0f, 1.0f, 1.0f );
		}
		else
		{
			setNumPasses( technique, 1 );
		}
	}
	//--------------------------------------------------------------------------
	void MaterialGeneratorImpl::setNumTechniques( Material& _rMaterial, unsigned short _numTechniques, SettingNumberOption _opt )
	{
		if( _rMaterial.getNumTechniques() != _numTechniques )
		{
			if( _rMaterial.getNumTechniques() < _numTechniques )
			{
				unsigned short delta = _numTechniques - _rMaterial.getNumTechniques();
				while( delta-- )
					_rMaterial.createTechnique();
			}
			else if( _opt == EXACTLY )
			{
				unsigned short delta = _rMaterial.getNumTechniques() - _numTechniques;
				while( delta-- )
					_rMaterial.removeTechnique( _rMaterial.getNumTechniques() - 1 );
			}
		}
	}
	//--------------------------------------------------------------------------
	void MaterialGeneratorImpl::setNumPasses( Technique* _technique, unsigned short _numPasses, SettingNumberOption _opt )
	{
		if( _technique->getNumPasses() != _numPasses )
		{
			if( _technique->getNumPasses() < _numPasses )
			{
				unsigned short delta = _numPasses - _technique->getNumPasses();
				while( delta-- )
					_technique->createPass();
			}
			else if( _opt == EXACTLY )
			{
				unsigned short delta = _technique->getNumPasses() - _numPasses;
				while( delta-- )
					_technique->removePass( _technique->getNumPasses() - 1 );
			}
		}
	}
	//--------------------------------------------------------------------------
	void MaterialGeneratorImpl::setNumTextureUnitStates( Pass* _pass, unsigned short _numTextureUnitStates, SettingNumberOption _opt )
	{
		if( _pass->getNumTextureUnitStates() != _numTextureUnitStates )
		{
			if( _pass->getNumTextureUnitStates() < _numTextureUnitStates )
			{
				unsigned short delta = _numTextureUnitStates - _pass->getNumTextureUnitStates();
				while( delta-- )
					_pass->createTextureUnitState();
			}
			else if( _opt == EXACTLY )
			{
				unsigned short delta = _pass->getNumTextureUnitStates() - _numTextureUnitStates;
				while( delta-- )
					_pass->removeTextureUnitState( _pass->getNumTextureUnitStates() - 1 );
			}
		}
	}
	//----------------------------------------------------------------------------
	String MaterialGeneratorImpl::getRealTextureName(const String& _filename)
	{
		TexturePtr texture = TextureManagerEx::getSingleton().create(_filename);
		return texture->getName();
	}
	//----------------------------------------------------------------------------
	StringVector MaterialGeneratorImpl::findAnimatedTextureNames(const String& _texname)
	{
		StringVector vec;
		vec.push_back(getRealTextureName(_texname));

		if(TextureManagerEx::getSingleton().canLoad(_texname))
		{
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
				int index = StrConv::fromString<int>(digits);

				while(true)
				{
					++index;
					digits = StrConv::toString<int>(index);
					String newtexname = basename + digits + fileext;
					if(!TextureManagerEx::getSingleton().canLoad(newtexname))
						break;

					vec.push_back(getRealTextureName(newtexname));
				}
			}		
		}
		return vec;
	}

} // namespace GothOgre