#include "Precompiled.h"
#include "SkyFileLoadContext.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------
	// Helper functions
	//---------------------------------------------------------------------------------
	template<typename T>
	void SkyFileLoadContext::getValueInParentheses( T& _x )
	{
		mParser.skipNextToken( "(" );
		getValue( _x );
		mParser.skipNextToken( ")" );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( int& _x )
	{
		_x = mParser.getNextInteger();
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( size_t& _x )
	{
		_x = (size_t) mParser.getNextInteger();
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( bool& _x )
	{
		_x = (mParser.getNextInteger() != 0);
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( Real& _x )
	{
		_x = mParser.getNextReal();
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue(Degree& _x)
	{
		Real f;
		getValue( f );
		_x = Degree( f );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( String& _x )
	{
		_x = mParser.getNextString();
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( Vector2& _x )
	{
		StrStream ss( mParser.getNextString(), StrStream::DONT_COPY );
		ss >> _x;
		if(ss.fail())
			mParser.throwException( "Expected a two-components vector", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue(Vector3& _x)
	{
		StrStream ss( mParser.getNextString(), StrStream::DONT_COPY );
		ss >> _x;
		if(ss.fail())
			mParser.throwException( "Expected a three-components vector", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( Time& _x )
	{
		StrStream ss( mParser.getNextString(), StrStream::DONT_COPY );
		ss >> _x;
		if(ss.fail())
			mParser.throwException( "Expected a three-components vector", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( ColourValue& _x )
	{
		StrStream ss( mParser.getNextString(), StrStream::DONT_COPY );
		ss >> _x;
		if(ss.fail())
			mParser.throwException( "Expected a colour value", 1 );
	}
	//---------------------------------------------------------------------------------
	template<typename T>
	void SkyFileLoadContext::getValue( SkyDef::Modifier<T>& _x )
	{
		_x = SkyDef::Modifier<T>::IDENTITY;

		while( mParser.peekNextToken() != ")" )
		{
			if( mParser.peekNextType() == Parser::IDENTIFIER )
			{
				const String& id = mParser.getNextIdentifier();
				if(id == "MUL")
				{
					mParser.skipNextToken(":");
					SkyDef::Modifier<T>::ValueType mul;
					getValue( mul );
					
					SkyDef::Modifier<T> transform = SkyDef::Modifier<T>::ZERO;
					transform.setMultiplier(mul);
					_x = transform * _x;
				}
				else if(id == "ADD")
				{
					mParser.skipNextToken(":");
					SkyDef::Modifier<T>::ValueType add;
					getValue( add );

					SkyDef::Modifier<T> transform = SkyDef::Modifier<T>::IDENTITY;
					transform.setAddend(add);
					_x = transform * _x;
				}
				else if(id == "SET")
				{
					mParser.skipNextToken(":");
					SkyDef::Modifier<T>::ValueType add;
					getValue( add );

					SkyDef::Modifier<T> transform = SkyDef::Modifier<T>::ZERO;
					transform.setAddend(add);
					_x = transform * _x;
				}
				else 
					mParser.throwException( "Expected ')' or 'MUL' or 'ADD' or 'SET' or color", 1 );
			}
			else
			{
				SkyDef::Modifier<T>::ValueType add;
				getValue( add );

				SkyDef::Modifier<T> transform = SkyDef::Modifier<T>::ZERO;
				transform.setAddend(add);
				_x = transform * _x;
			}
		}
	}




	//---------------------------------------------------------------------------------
	// Helper function of higher level
	//---------------------------------------------------------------------------------
	template<typename ASP>
	void SkyFileLoadContext::getAspect(ASP& _aspect)
	{
		getValueInParentheses( _aspect.name );

		mParser.skipNextToken( "{" );
		while( mParser.peekNextToken() != "}" )
		{
			getAspectSetting( _aspect );
		}
		mParser.skipNextToken( "}" );
	}
	//---------------------------------------------------------------------------------
	template<typename CTRL>
	void SkyFileLoadContext::getController( CTRL& _ctrl )
	{
		RefAndFunction raf;
		getValueInParentheses( raf );
		_ctrl.functionName = raf.functionName;
		getRefToNameOfControlledThing(_ctrl) = raf.refName;

		CTRL::Key defaultKey;

		mParser.skipNextToken( "{" );
		while( mParser.peekNextToken() != "}" )
		{
			const String& id = mParser.peekNextIdentifier();
			if(id == "AT")
			{
				mParser.skipNextIdentifier("AT");
				
				ValueAndPeriod vap;
				getValueInParentheses( vap );
				
				CTRL::Key key = defaultKey;
				mParser.skipNextToken("{");
				while( mParser.peekNextToken() != "}" )
				{
					getKeySetting( key );
				}
				mParser.skipNextToken("}");

				_ctrl.keys.push_back( key );
			}
			else
			{
				if(!_ctrl.keys.empty())
					mParser.throwException("Unconditional settings must be written first.", 1);

				getKeySetting( defaultKey );
			}
		}
		mParser.skipNextToken( "}" );

		if(_ctrl.keys.empty())
		{
			_ctrl.keys.push_back(defaultKey);
		}
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::BackgroundController& _ctrl )
	{
		return _ctrl.backgroundName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::FogController& _ctrl )
	{
		return _ctrl.fogName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::FarClippingPlaneController& _ctrl )
	{
		return _ctrl.farClippingPlaneName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::AtmosphereController& _ctrl )
	{
		return _ctrl.atmosphereName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::BodyController& _ctrl )
	{
		return _ctrl.bodyName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::CloudLayerController& _ctrl )
	{
		return _ctrl.cloudLayerName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::MaterialController& _ctrl )
	{
		return _ctrl.mtlName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::SoundController& _ctrl )
	{
		return _ctrl.soundName;
	}
	//---------------------------------------------------------------------------------
	String& SkyFileLoadContext::getRefToNameOfControlledThing( SkyDef::OverlayController& _ctrl )
	{
		return _ctrl.overlayName;
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( ValueAndPeriod& _vap )
	{
		_vap.value = 0;
		_vap.period = Math::POS_INFINITY;

		while( mParser.peekNextToken() != ")" )
		{
			if( mParser.peekNextType() == Parser::IDENTIFIER)
			{
				const String& id = mParser.getNextIdentifier();
				if(id == "PERIOD")
				{
					mParser.skipNextToken(":");
					getVapValue( _vap.period );
				}
				else
				{
					mParser.skipNextToken(":");
					getVapValue( _vap.value );
				}
			}
			else
			{
				getVapValue( _vap.value );
			}
		}
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getVapValue( Real& _x )
	{
		if( mParser.peekNextType() == Parser::STRING )
		{
			Time time;
			getValue( time );
			_x = (Real) time;
		}
		else
		{
			getValue( _x );
		}
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( RefAndFunction& _raf )
	{
		while( mParser.peekNextToken() != ")" )
		{
			const String& id = mParser.getNextToken();
			if(id == "REF")
			{
				mParser.skipNextToken(":");
				getValue( _raf.refName );
			}
			else if(id == "VAR")
			{
				mParser.skipNextToken(":");
				getFunctionName( _raf.functionName );
			}
			else
				mParser.throwException( "Unknown identifier", 1 );
		}
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getFunctionName( String& _functionName )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "WORLD_TIME" || id == "WORLD_TIME_REL"
			|| id == "SYSTEM_TIME" || id == "SYSTEM_TIME_REL")
		{
			if(!mSkyDef->hasFunction(id))
			{
				SkyDef::Function function;
				function.name = id;
				function.type = SkyDef::Function::LINEAR;
				function.initValue = 0;
				function.rate = 1;
				function.period = 0;
				if(id == "WORLD_TIME")
				{
					function.argument = SkyDef::Function::WORLD_TIME;
				}
				else if(id == "WORLD_TIME_REL")
				{
					function.argument = SkyDef::Function::WORLD_TIME_REL;
					mParser.skipNextToken(":");
					function.skyName = mParser.getNextString();
				}
				else if(id == "SYSTEM_TIME")
				{
					function.argument = SkyDef::Function::SYSTEM_TIME;
				}
				else if(id == "SYSTEM_TIME_REL")
				{
					function.argument = SkyDef::Function::SYSTEM_TIME_REL;
					mParser.skipNextToken(":");
					function.skyName = mParser.getNextString();
				}
				mSkyDef->insertFunction( function );
			}
			_functionName = id;
		}
		else if(id == "ALTITUDE_OF_BODY")
		{
			if(!mSkyDef->hasFunction(id))
			{
				SkyDef::Function function;
				function.name = id;
				function.type = SkyDef::Function::LINEAR;
				function.initValue = 0;
				function.rate = 1;
				function.period = 0;
				function.argument = SkyDef::Function::ALTITUDE_OF_BODY;
				mParser.skipNextToken(":");
				function.bodyName = mParser.getNextString();
				mSkyDef->insertFunction( function );
			}
			_functionName = id;
		}
		else if(id == "UNDERWATER_DEPTH" || id == "INDOOR_DEPTH")
		{
			if(!mSkyDef->hasFunction(id))
			{
				SkyDef::Function function;
				function.name = id;
				function.type = SkyDef::Function::LINEAR;
				function.initValue = 0;
				function.rate = 1;
				function.period = 0;
				if(id == "UNDERWATER_DEPTH")
				{
					function.argument = SkyDef::Function::UNDERWATER_DEPTH;
				}
				else if(id == "INDOOR_DEPTH")
				{
					function.argument = SkyDef::Function::INDOOR_DEPTH;
				}
				mSkyDef->insertFunction( function );
			}
			_functionName = id;
		}
		else if(id == "FUNCTION")
		{
			mParser.skipNextToken(":");
			_functionName = mParser.getNextString();
		}
		else
			mParser.throwException("Unknown identifier", 1);
	}




	//---------------------------------------------------------------------------------
	// Background and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createBackground( )
	{
		SkyDef::Background background;
		getAspect( background );
		mSkyDef->insertBackground( background );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Background& _background )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "COLOR")
		{
			getValueInParentheses( _background.colour );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createBackgroundController()
	{
		SkyDef::BackgroundController ctrl;
		getController( ctrl );
		mSkyDef->insertBackgroundController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::BackgroundController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "COLOR")
		{
			getValueInParentheses( _key.colourModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// Fog and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createFog( )
	{
		SkyDef::Fog fog;
		getAspect( fog );
		mSkyDef->insertFog( fog );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Fog& _fog )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "COLOR" )
		{
			getValueInParentheses( _fog.colour );
		}
		else if( id == "NEARDISTANCE" )
		{
			getValueInParentheses( _fog.nearDistance );
		}
		else if( id == "FARDISTANCE" )
		{
			getValueInParentheses( _fog.farDistance );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createFogController()
	{
		SkyDef::FogController ctrl;
		getController( ctrl );
		mSkyDef->insertFogController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::FogController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "COLOR")
		{
			getValueInParentheses( _key.colourModifier );
		}
		else if(id == "NEARDISTANCE")
		{
			getValueInParentheses( _key.nearDistanceModifier );
		}
		else if(id == "FARDISTANCE")
		{
			getValueInParentheses( _key.farDistanceModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}




	//---------------------------------------------------------------------------------
	// FarClippingPlane and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createFarClippingPlane( )
	{
		SkyDef::FarClippingPlane farClippingPlane;
		getAspect( farClippingPlane );
		mSkyDef->insertFarClippingPlane( farClippingPlane );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::FarClippingPlane& _farClippingPlane )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "DISTANCE" )
		{
			getValueInParentheses( _farClippingPlane.distance );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createFarClippingPlaneController()
	{
		SkyDef::FarClippingPlaneController ctrl;
		getController( ctrl );
		mSkyDef->insertFarClippingPlaneController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::FarClippingPlaneController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "DISTANCE")
		{
			getValueInParentheses( _key.distanceModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}




	//---------------------------------------------------------------------------------
	// Atmosphere and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createAtmosphere()
	{
		SkyDef::Atmosphere atmosphere;
		getAspect( atmosphere );
		mSkyDef->insertAtmosphere( atmosphere );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Atmosphere& _atm )
	{
		const String& id = mParser.getNextIdentifier();
		if(id == "GEOMETRY")
		{
			AtmosphereGeometry atmGeom;
			getValueInParentheses( atmGeom );
			_atm.radius = atmGeom.radius;
			_atm.minAltitude = atmGeom.minAltitude;
		}
		else if(id == "LAYER")
		{
			getValueInParentheses( _atm.layer );
		}
		else if(id == "COLOR")
		{
			getValueInParentheses( _atm.colour );
		}
		else if(id == "AMBIENTLIGHT")
		{
			getValueInParentheses( _atm.ambientLight );
		}
		else if(id == "AMBIENTLIGHTFILTER")
		{
			getValueInParentheses( _atm.ambientLightFilter );
		}
		else if(id == "DIFFUSELIGHTFILTER")
		{
			getValueInParentheses( _atm.diffuseLightFilter );
		}
		else if(id == "SPECULARLIGHTFILTER")
		{
			getValueInParentheses( _atm.specularLightFilter );
		}
		else
			mParser.throwException("Unknown identifier", 1);
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( AtmosphereGeometry& _atmGeom )
	{
		while( mParser.peekNextToken() != ")" )
		{
			const String& id = mParser.getNextIdentifier();
			if( id == "RADIUS" )
			{
				mParser.skipNextToken( ":" );
				getValue( _atmGeom.radius );
			}
			else if( id == "MINALTITUDE" )
			{
				mParser.skipNextToken( ":" );
				getValue( _atmGeom.minAltitude );
			}
			else
				mParser.throwException( "Expected RADIUS or DEPTH", 1 );
		}		
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createAtmosphereController()
	{
		SkyDef::AtmosphereController ctrl;
		getController( ctrl );
		mSkyDef->insertAtmosphereController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::AtmosphereController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "AMBIENTLIGHT" )
		{
			getValueInParentheses( _key.ambientLightModifier );
		}
		else if( id == "AMBIENTLIGHTFILTER" )
		{
			getValueInParentheses( _key.ambientLightFilterModifier );
		}
		else if( id == "DIFFUSELIGHTFILTER" )
		{
			getValueInParentheses( _key.diffuseLightFilterModifier );
		}
		else if( id == "SPECULARLIGHTFILTER" )
		{
			getValueInParentheses( _key.specularLightFilterModifier );
		}
		else if( id == "COLOR" )
		{
			if(!_key.angleColourModifierPairs.empty())
				mParser.throwException("Unconditional settings must be first", 1);

			getValueInParentheses( _key.colourModifier );
		}
		else if( id == "AT")
		{
			SkyDef::AtmosphereController::AngleColourModifierPair acm;
			ValueAndPeriod vap;
			getValueInParentheses( vap );
			acm.first = vap.value;
			
			mParser.skipNextToken("{");
			while( mParser.peekNextToken() != "}" )
			{
				const String& id = mParser.getNextIdentifier();
				if(id == "COLOR")
				{
					getValueInParentheses( acm.second );
				}
				else
					mParser.throwException("Unknown identifier", 1);
			}
			mParser.skipNextToken("}");
			_key.angleColourModifierPairs.push_back( acm );
		}
		else
			mParser.throwException("Unknown identifier", 1);
	}



	//---------------------------------------------------------------------------------
	// Body and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createBody( )
	{
		SkyDef::Body body;
		getAspect( body );
		mSkyDef->insertBody( body );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Body& _body )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "VISUAL" )
		{
			Visual visual;
			getValueInParentheses( visual );

			if(visual.resourceGroup.empty())
				_body.mesh = MeshManagerEx::getSingleton().create( visual.fileName );
			else
				_body.mesh = MeshManagerEx::getSingleton().create( visual.fileName, visual.resourceGroup );

			_body.mtlName = visual.mtlName;
		}
		else if( id == "LAYER" )
		{
			getValueInParentheses( _body.layer );
		}
		else if( id == "ALTITUDE" )
		{
			getValueInParentheses( _body.altitude );
		}
		else if( id == "AZIMUTH" )
		{
			getValueInParentheses( _body.azimuth );
		}
		else if( id == "DISTANCE" )
		{
			getValueInParentheses( _body.distance );
		}
		else if( id == "POSITION" )
		{
			getValueInParentheses( _body.position );
		}
		else if( id == "EULER_ANGLES" )
		{
			getValueInParentheses( _body.eulerAnglesDeg );
		}
		else if( id == "SCALE" )
		{
			getValueInParentheses( _body.scale );
		}
		else if( id == "AMBIENTLIGHT" )
		{
			getValueInParentheses( _body.ambientLight );
		}
		else if( id == "DIFFUSELIGHT" )
		{
			getValueInParentheses( _body.diffuseLight );
		}
		else if( id == "SPECULARLIGHT" )
		{
			getValueInParentheses( _body.specularLight );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( Visual& _visual )
	{
		while( mParser.peekNextToken() != ")" )
		{
			const String& id = mParser.getNextIdentifier();
			if( id == "FILE" )
			{
				mParser.skipNextToken( ":" );
				getValue( _visual.fileName );
			}
			else if( id == "RESOURCE_GROUP" )
			{
				mParser.skipNextToken( ":" );
				getValue( _visual.resourceGroup );
			}
			else if( id == "MATERIAL" )
			{
				mParser.skipNextToken( ":" );
				getValue( _visual.mtlName );
			}
			else
				mParser.throwException( "Unknown identifier", 1 );
		}
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createBodyController()
	{
		SkyDef::BodyController ctrl;
		getController( ctrl );
		mSkyDef->insertBodyController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::BodyController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "ALTITUDE" )
		{
			getValueInParentheses( _key.altitudeModifier );
		}
		else if( id == "AZIMUTH" )
		{
			getValueInParentheses( _key.azimuthModifier );
		}
		else if( id == "DISTANCE" )
		{
			getValueInParentheses( _key.distanceModifier );
		}
		else if( id == "POSITION" )
		{
			getValueInParentheses( _key.positionModifier );
		}
		else if( id == "EULER_ANGLES" )
		{
			getValueInParentheses( _key.eulerAnglesDegModifier );
		}
		else if( id == "SCALE" )
		{
			getValueInParentheses( _key.scaleModifier );
		}
		else if( id == "AMBIENTLIGHT" )
		{
			getValueInParentheses( _key.ambientLightModifier );
		}
		else if( id == "DIFFUSELIGHT" )
		{
			getValueInParentheses( _key.diffuseLightModifier );
		}
		else if( id == "SPECULARLIGHT" )
		{
			getValueInParentheses( _key.specularLightModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// CloudLayer and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createCloudLayer( )
	{
		SkyDef::CloudLayer cloudLayer;
		getAspect( cloudLayer );
		mSkyDef->insertCloudLayer( cloudLayer );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::CloudLayer& _cloudLayer )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "VISUAL" )
		{
			Visual visual;
			getValueInParentheses( visual );
			
			if(visual.resourceGroup.empty())
				_cloudLayer.mesh = MeshManagerEx::getSingleton().create( visual.fileName );
			else
				_cloudLayer.mesh = MeshManagerEx::getSingleton().create( visual.fileName, visual.resourceGroup );

			_cloudLayer.mtlName = visual.mtlName;
		}
		else if( id == "LAYER" )
		{
			getValueInParentheses( _cloudLayer.layer );
		}
		else if( id == "POSITION" )
		{
			getValueInParentheses( _cloudLayer.position );
		}
		else if( id == "EULER_ANGLES" )
		{
			getValueInParentheses( _cloudLayer.eulerAnglesDeg );
		}
		else if( id == "SCALE" )
		{
			getValueInParentheses( _cloudLayer.scale );
		}
		else if( id == "AMBIENTLIGHT" )
		{
			getValueInParentheses( _cloudLayer.ambientLight );
		}
		else if( id == "DIFFUSELIGHT" )
		{
			getValueInParentheses( _cloudLayer.diffuseLight );
		}
		else if( id == "SPECULARLIGHT" )
		{
			getValueInParentheses( _cloudLayer.specularLight );
		}
		else if( id == "AMBIENTLIGHTFILTER" )
		{
			getValueInParentheses( _cloudLayer.ambientLightFilter );
		}
		else if( id == "DIFFUSELIGHTFILTER" )
		{
			getValueInParentheses( _cloudLayer.diffuseLightFilter );
		}
		else if( id == "SPECULARLIGHTFILTER" )
		{
			getValueInParentheses( _cloudLayer.specularLightFilter );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createCloudLayerController()
	{
		SkyDef::CloudLayerController ctrl;
		getController( ctrl );
		mSkyDef->insertCloudLayerController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::CloudLayerController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "POSITION" )
		{
			getValueInParentheses( _key.positionModifier );
		}
		else if( id == "EULER_ANGLES" )
		{
			getValueInParentheses( _key.eulerAnglesDegModifier );
		}
		else if( id == "SCALE" )
		{
			getValueInParentheses( _key.scaleModifier );
		}
		else if( id == "AMBIENTLIGHT" )
		{
			getValueInParentheses( _key.ambientLightModifier );
		}
		else if( id == "DIFFUSELIGHT" )
		{
			getValueInParentheses( _key.diffuseLightModifier );
		}
		else if( id == "SPECULARLIGHT" )
		{
			getValueInParentheses( _key.specularLightModifier );
		}
		else if( id == "AMBIENTLIGHTFILTER" )
		{
			getValueInParentheses( _key.ambientLightFilterModifier );
		}
		else if( id == "DIFFUSELIGHTFILTER" )
		{
			getValueInParentheses( _key.diffuseLightFilterModifier );
		}
		else if( id == "SPECULARLIGHTFILTER" )
		{
			getValueInParentheses( _key.specularLightFilterModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// MaterialCpntroller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createMaterialController()
	{
		SkyDef::MaterialController ctrl;
		getController( ctrl );
		mSkyDef->insertMaterialController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::MaterialController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "FRAME" )
		{
			getValueInParentheses( _key.frameIndexModifier );
		}
		else if( id == "TEXANIMAPDIR" )
		{
			getValueInParentheses( _key.scrollSpeedModifier );
		}
		else if( id == "COLORFACTOR" )
		{
			getValueInParentheses( _key.colourFactorModifier );
		}
		else if( id == "SPECULARINTENSITYFACTOR" )
		{
			getValueInParentheses( _key.specularIntensityFactorModifier );
		}
		else if( id == "SPECULARCOLORFACTOR" )
		{
			getValueInParentheses( _key.specularColourFactorModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// Sound and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createSound( )
	{
		SkyDef::Sound sound;
		getAspect( sound );
		mSkyDef->insertSound( sound );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Sound& _sound )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "DATASOURCE" )
		{
			SoundDataSource sds;
			getValueInParentheses( sds );
			
			SoundDefPtr soundDef;
			
			if(!sds.soundDefName.empty())
				soundDef = SoundDefManagerEx::getSingleton().findByName( sds.soundDefName );

			if(soundDef.isNull())
			{
				if(sds.soundDefName.empty())
					sds.soundDefName = mSoundDefNameGenerator.generate();

				soundDef = SoundDefManagerEx::getSingleton().createManual( sds.soundDefName );
				SoundDef::TimeProperty timeProp;
				timeProp.fileName = sds.fileName;
				timeProp.resourceGroup = sds.resourceGroup;
				soundDef->insertTimeProperty(timeProp);
			}

			_sound.soundDef = soundDef;
		}
		else if( id == "LOOPING" )
		{
			getValueInParentheses( _sound.looping );
		}
		else if( id == "POSITION" )
		{
			getValueInParentheses( _sound.position );
		}
		else if( id == "MAXDISTANCE" )
		{
			getValueInParentheses( _sound.maxDistance );
		}
		else if( id == "VOLUME" )
		{
			getValueInParentheses( _sound.volumePerc );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( SoundDataSource& _sds )
	{
		while( mParser.peekNextToken() != ")" )
		{
			const String& id = mParser.getNextIdentifier();
			if( id == "SFX" )
			{
				mParser.skipNextToken( ":" );
				getValue( _sds.soundDefName );
			}
			else if( id == "FILE" )
			{
				mParser.skipNextToken( ":" );
				getValue( _sds.fileName );
			}
			else if( id == "RESOURCE_GROUP" )
			{
				mParser.skipNextToken( ":" );
				getValue( _sds.resourceGroup );
			}
			else
				mParser.throwException( "Unknown identifier", 1 );
		}		
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createSoundController()
	{
		SkyDef::SoundController ctrl;
		getController( ctrl );
		mSkyDef->insertSoundController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::SoundController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "VOLUME" )
		{
			getValueInParentheses( _key.volumePercModifier );
		}
		else if( id == "POSITION" )
		{
			getValueInParentheses( _key.positionModifier );
		}
		else if( id == "MAXDISTANCE" )
		{
			getValueInParentheses( _key.maxDistanceModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// Overlay and its controller
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createOverlay( )
	{
		SkyDef::Overlay overlay;
		getAspect( overlay );
		mSkyDef->insertOverlay( overlay );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Overlay& _overlay )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "SKY" )
		{
			getValueInParentheses( _overlay.skyName );
		}
		else if( id == "WEIGHT" )
		{
			getValueInParentheses( _overlay.weight );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createOverlayController()
	{
		SkyDef::OverlayController ctrl;
		getController( ctrl );
		mSkyDef->insertOverlayController( ctrl );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getKeySetting( SkyDef::OverlayController::Key& _key )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "WEIGHT" )
		{
			getValueInParentheses( _key.weightModifier );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}



	//---------------------------------------------------------------------------------
	// Function
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createFunction( )
	{
		SkyDef::Function function;
		getAspect( function );
		mSkyDef->insertFunction( function );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getAspectSetting( SkyDef::Function& _function )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "TYPE" )
		{
			getValueInParentheses( _function.type );
		}
		else if( id == "ARGUMENT" )
		{
			FunctionArgument fa;
			getValueInParentheses( fa );
			_function.argument = fa.argument;
			_function.skyName  = fa.skyName;
			_function.bodyName = fa.bodyName;
		}
		else if( id == "VALUE" )
		{
			getValueInParentheses( _function.value );
		}
		else if( id == "INITVALUE" )
		{
			getValueInParentheses( _function.initValue );
		}
		else if( id == "MINVALUE" )
		{
			getValueInParentheses( _function.minValue );
		}
		else if( id == "MAXVALUE" )
		{
			getValueInParentheses( _function.maxValue );
		}
		else if( id == "RATE" )
		{
			getValueInParentheses( _function.rate );
		}
		else if( id == "INITRATE" )
		{
			getValueInParentheses( _function.initRate );
		}
		else if( id == "MINRATE" )
		{
			getValueInParentheses( _function.minRate );
		}
		else if( id == "MAXRATE" )
		{
			getValueInParentheses( _function.maxRate );
		}
		else if( id == "RATE2" )
		{
			getValueInParentheses( _function.rate2 );
		}
		else if( id == "INITRATE2" )
		{
			getValueInParentheses( _function.initRate2 );
		}
		else if( id == "MINRATE2" )
		{
			getValueInParentheses( _function.minRate2 );
		}
		else if( id == "MAXRATE2" )
		{
			getValueInParentheses( _function.maxRate2 );
		}
		else if( id == "BOUNCE" )
		{
			getValueInParentheses( _function.bounce );
		}
		else if( id == "PERIOD" )
		{
			getValueInParentheses( _function.period );
		}
		else if( id == "INITPHASE" )
		{
			getValueInParentheses( _function.initPhase );
		}
		else if( id == "STEP" )
		{
			getValueInParentheses( _function.step );
		}
		else if( id == "STATE" )
		{
			SkyDef::Function::State state;
			getState( state );
			_function.states.push_back( state );
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( SkyDef::Function::Type& _functionType )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "CONSTANT" )
		{
			_functionType = SkyDef::Function::CONSTANT;
		}
		else if( id == "LINEAR" )
		{
			_functionType = SkyDef::Function::LINEAR;
		}
		else if( id == "QUADRATIC" )
		{
			_functionType = SkyDef::Function::QUADRATIC;
		}
		else if( id == "SIN" )
		{
			_functionType = SkyDef::Function::SIN;
		}
		else if( id == "RANDOM_VALUE" )
		{
			_functionType = SkyDef::Function::RANDOM_VALUE;
		}
		else if( id == "RANDOM_RATE" )
		{
			_functionType = SkyDef::Function::RANDOM_RATE;
		}
		else if( id == "RANDOM_RATE2" )
		{
			_functionType = SkyDef::Function::RANDOM_RATE2;
		}
		else if( id == "RANDOM_MANUAL" )
		{
			_functionType = SkyDef::Function::RANDOM_MANUAL;
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getValue( FunctionArgument& _fa )
	{
		const String& id = mParser.getNextIdentifier();
		if( id == "WORLD_TIME" )
		{
			_fa.argument = SkyDef::Function::WORLD_TIME;
		}
		else if( id == "WORLD_TIME_REL" )
		{
			_fa.argument = SkyDef::Function::WORLD_TIME_REL;
			mParser.skipNextToken( ":" );
			getValue( _fa.skyName );
		}
		else if( id == "SYSTEM_TIME" )
		{
			_fa.argument = SkyDef::Function::SYSTEM_TIME;
		}
		else if( id == "SYSTEM_TIME_REL" )
		{
			_fa.argument = SkyDef::Function::SYSTEM_TIME_REL;
			mParser.skipNextToken( ":" );
			getValue( _fa.skyName );
		}
		else if( id == "ALTITUDE_OF_BODY" )
		{
			_fa.argument = SkyDef::Function::ALTITUDE_OF_BODY;
			mParser.skipNextToken( ":" );
			getValue( _fa.bodyName );
		}
		else if( id == "UNDERWATER_DEPTH" )
		{
			_fa.argument = SkyDef::Function::UNDERWATER_DEPTH;
		}
		else if( id == "INDOOR_DEPTH" )
		{
			_fa.argument = SkyDef::Function::INDOOR_DEPTH;
		}
		else
			mParser.throwException( "Unknown identifier", 1 );
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::getState( SkyDef::Function::State& _state )
	{
		getValueInParentheses( _state.value );
		mParser.skipNextToken("{");
		while( mParser.peekNextToken() != "}" )
		{
			const String& id = mParser.getNextIdentifier();
			if( id == "STEP" )
			{
				getValueInParentheses( _state.step );
			}
			else if( id == "STAYCHANCE" )
			{
				getValueInParentheses( _state.stayChance );
			}
			else if( id == "UPCHANCE" )
			{
				getValueInParentheses( _state.upChance );
			}
			else if( id == "DOWNHANCE" )
			{
				getValueInParentheses( _state.downChance );
			}
			else
				mParser.throwException( "Unknown identifier", 1 );
		}		
		mParser.skipNextToken("}");
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::createSky()
	{
		String skyName;
		getValueInParentheses( skyName );

		mParser.skipNextToken("{");
		while( mParser.peekNextToken() != "}" )
		{
			const String& id = mParser.getNextIdentifier();
			if(id == "BACKGROUND")
			{
				createBackground();
			}
			else if(id == "BACKGROUNDCONTROLLER")
			{
				createBackgroundController();
			}
			else if(id == "FOG")
			{
				createFog();
			}
			else if(id == "FOGCONTROLLER")
			{
				createFogController();
			}
			else if(id == "FARCLIPPINGPLANE")
			{
				createFarClippingPlane();
			}
			else if(id == "FARCLIPPINGPLANECONTROLLER")
			{
				createFarClippingPlaneController();
			}
			else if(id == "ATMOSPHERE")
			{
				createAtmosphere();
			}
			else if(id == "ATMOSPHERECONTROLLER")
			{
				createAtmosphereController();
			}
			else if(id == "BODY")
			{
				createBody();
			}
			else if(id == "BODYCONTROLLER")
			{
				createBodyController();
			}
			else if(id == "CLOUDLAYER")
			{
				createCloudLayer();
			}
			else if(id == "CLOUDLAYERCONTROLLER")
			{
				createCloudLayerController();
			}
			else if(id == "MATERIALCONTROLLER")
			{
				createMaterialController();
			}
			else if(id == "SOUND")
			{
				createSound();
			}
			else if(id == "SOUNDCONTROLLER")
			{
				createSoundController();
			}
			else if(id == "Overlay")
			{
				createOverlay();
			}
			else if(id == "OVERLAYCONTROLLER")
			{
				createOverlayController();
			}
			else if(id == "FUNCTION")
			{
				createFunction();
			}
			else
				mParser.throwException( "Unknown identifier.", 1 );
		}
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::load(const DataStreamExPtr& _dataStream)
	{
		clear();
		mParser.loadScript(_dataStream);
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::apply(Resource* _destSkyDef)
	{
		mSkyDef = static_cast<SkyDef*>(_destSkyDef);
		
		mParser.skipNextToken("Sky");
		createSky();

		clear();
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::skip(const DataStreamExPtr& _dataStream)
	{
		clear();
		_dataStream->seekg(_dataStream->size());
	}
	//---------------------------------------------------------------------------------
	void SkyFileLoadContext::clear()
	{
		mSkyDef = nullptr;
		mParser.clear();
	}
	//---------------------------------------------------------------------------------
	SkyFileLoadContext::SkyFileLoadContext()
		: mSoundDefNameGenerator( "SkySound\x01" )
	{
		clear();
	}
	//---------------------------------------------------------------------------------
	SkyFileLoadContext::~SkyFileLoadContext()
	{
	}

}