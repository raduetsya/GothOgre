#ifndef THIS_PLUGIN_H
#define THIS_PLUGIN_H


namespace GothOgre
{
	class ICUUnicodeSupport;

	/** Plugin instance */
	class UnicodeICUPlugin : public Plugin
	{
	public:
		UnicodeICUPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		ICUUnicodeSupport*	mICUUnicodeSupport;
	};

} // namespace GothOgre

#endif // THIS_PLUGIN_H