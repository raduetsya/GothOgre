#ifndef SCRIPT_PROCESSING_PLUGIN_H
#define SCRIPT_PROCESSING_PLUGIN_H

namespace GothOgre
{
	class MusicDatLoader;
	class SfxDatLoader;

	//-------------------------------------------------------------------------
	class ScriptProcessingPlugin : public Plugin
	{
	public:
		ScriptProcessingPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		MusicDatLoader*	mMusicDatLoader;
		SfxDatLoader*   mSfxDatLoader;
	};

} // namespace GothOgre

#endif // SCRIPT_PROCESSING_PLUGIN_H