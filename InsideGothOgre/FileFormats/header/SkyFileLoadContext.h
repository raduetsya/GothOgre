#ifndef SKY_FILE_LOAD_CONTEXT_H
#define SKY_FILE_LOAD_CONTEXT_H

#include "ModelScriptParser.h"


namespace GothOgre
{
	class SkyFileLoadContext : public ResourceLoadContext
	{
	public:
		SkyFileLoadContext();
		~SkyFileLoadContext();

		void load(const DataStreamExPtr& _stream);
		void apply(Resource* _resource);
		void skip(const DataStreamExPtr& _stream);

	private:
		void clear();
		void createSky();

	private:
		void createBackground();
		void getAspectSetting( SkyDef::Background& _background );
		void createBackgroundController();
		void getKeySetting( SkyDef::BackgroundController::Key& _key );

	private:
		void createFog();
		void getAspectSetting( SkyDef::Fog& _fog );
		void createFogController();
		void getKeySetting( SkyDef::FogController::Key& _key );

	private:
		void createFarClippingPlane();
		void getAspectSetting( SkyDef::FarClippingPlane& _farClippingPlane );
		void createFarClippingPlaneController();
		void getKeySetting( SkyDef::FarClippingPlaneController::Key& _key );

	private:
		void createAtmosphere();
		void getAspectSetting( SkyDef::Atmosphere& _atmosphere );
		
		struct AtmosphereGeometry
		{
			Real radius;
			Real minAltitude;
		};

		void getValue( AtmosphereGeometry& _atmGeom );
		
		void createAtmosphereController();
		void getKeySetting( SkyDef::AtmosphereController::Key& _key );

	private:
		void createBody();
		void getAspectSetting( SkyDef::Body& _body );

		struct Visual
		{
			String fileName;
			String resourceGroup;
			String mtlName;
		};
		
		void getValue( Visual& _visual );

		void createBodyController();
		void getKeySetting( SkyDef::BodyController::Key& _key );

	private:
		void createCloudLayer();
		void getAspectSetting( SkyDef::CloudLayer& _cloudLayer );
		void createCloudLayerController();
		void getKeySetting( SkyDef::CloudLayerController::Key& _key );

	private:
		void createMaterialController();
		void getKeySetting( SkyDef::MaterialController::Key& _key );

	private:
		void createSound();
		void getAspectSetting( SkyDef::Sound& _sound );

		struct SoundDataSource
		{
			String soundDefName;
			String fileName;
			String resourceGroup;
		};

		void getValue( SoundDataSource& _sds );
		void createSoundController();
		void getKeySetting( SkyDef::SoundController::Key& _key );

	private:
		void createOverlay();
		void getAspectSetting( SkyDef::Overlay& _overlay );
		void createOverlayController();
		void getKeySetting( SkyDef::OverlayController::Key& _key );

	private:
		void createFunction();
		void getAspectSetting( SkyDef::Function& _function );

		struct FunctionArgument
		{
			SkyDef::Function::Argument  argument;
			String                      skyName;
			String                      bodyName;
		};

		void getValue( SkyDef::Function::Type& _functionType );
		void getValue( FunctionArgument& _fa );
		void getState( SkyDef::Function::State& _state );

	private:
		template<typename T>
		void getValueInParentheses( T& _x );

		void getValue( int& _x );
		void getValue( size_t& _x );
		void getValue( bool& _x );
		void getValue( Real& _x );
		void getValue( Degree& _x );
		void getValue( String& _x );
		void getValue( Vector2& _x );
		void getValue( Vector3& _x );
		void getValue( Time& _x );
		void getValue( ColourValue& _x );

		template<typename T>
		void getValue( SkyDef::Modifier<T>& _x );

		template<typename ASP>
		void getAspect(ASP& _aspect);

		template<typename CTRL>
		void getController( CTRL& _ctrl );

		String& getRefToNameOfControlledThing( SkyDef::BackgroundController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::FogController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::FarClippingPlaneController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::AtmosphereController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::BodyController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::CloudLayerController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::MaterialController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::SoundController& _ctrl );
		String& getRefToNameOfControlledThing( SkyDef::OverlayController& _ctrl );

		struct ValueAndPeriod
		{
			Real value;
			Real period;
		};

		void getValue( ValueAndPeriod& _vap );
		void getVapValue( Real& _x );

		struct RefAndFunction
		{
			String  refName;
			String  functionName;
		};

		void getValue( RefAndFunction& _raf );
		void getFunctionName( String& _functionName );

	private:
		typedef ModelScriptParser         Parser;

		Parser           mParser;
		SkyDef*			 mSkyDef;
		NameGenerator    mSoundDefNameGenerator;
	};
}

#endif // SKY_FILE_LOAD_CONTEXT_H