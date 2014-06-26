#ifndef DIRECT_MUSIC_PLUGIN_H
#define DIRECT_MUSIC_PLUGIN_H

namespace GothOgre
{
	class DirectMusicPlayer;

	//-------------------------------------------------------------------------
	class DirectMusicPlugin : public Plugin
	{
	public:
		DirectMusicPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		DirectMusicPlayer*	mDirectMusicPlayer;
	};

} // namespace GothOgre

#endif // DIRECT_MUSIC_PLUGIN_H