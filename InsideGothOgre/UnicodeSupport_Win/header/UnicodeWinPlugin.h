#ifndef UNICODEWIN_PLUGIN_H
#define UNICODEWIN_PLUGIN_H


namespace GothOgre
{
	class WinUnicodeSupport;

	/** Plugin instance */
	class UnicodeWinPlugin : public Plugin
	{
	public:
		UnicodeWinPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		WinUnicodeSupport*	mWinUnicodeSupport;
	};

} // namespace GothOgre

#endif // UNICODEWIN_PLUGIN_H