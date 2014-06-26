#ifndef OPENAL_PLUGIN_H
#define OPENAL_PLUGIN_H

namespace GothOgre
{
	class OpenALPlayer;

	//-------------------------------------------------------------------------
	class OpenALPlugin : public Plugin
	{
	public:
		OpenALPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		OpenALPlayer*	mOpenALPlayer;
	};

} // namespace GothOgre

#endif // OPENAL_PLUGIN_H