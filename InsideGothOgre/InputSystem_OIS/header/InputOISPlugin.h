#ifndef INPUTOIS_PLUGIN_H
#define INPUTOIS_PLUGIN_H


namespace GothOgre
{
	class InputOISPlugin : public Plugin
	{
	public:
		InputOISPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		InputSupport*     mInputSupport;
	};

} // namespace GothOgre

#endif // INPUTOIS_PLUGIN_H