#ifndef EXTARCHIVES_PLUGIN_H
#define EXTARCHIVES_PLUGIN_H


namespace GothOgre
{
	class RFileSystemArchiveFactory;
	class RZipArchiveFactory;

	/** Plugin instance for EditorTools */
	class ExtArchivesPlugin : public Plugin
	{
	public:
		ExtArchivesPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		RFileSystemArchiveFactory*		mRFileSystemArchiveFactory;
		RZipArchiveFactory*				mRZipArchiveFactory;
	};

} // namespace GothOgre

#endif // EXTARCHIVES_PLUGIN_H