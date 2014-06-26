#ifndef FILEFORMATS_PLUGIN_H
#define FILEFORMATS_PLUGIN_H

namespace GothOgre
{
	class _3dsFileLoader;
	class MrmFileLoader;
	class MshFileLoader;
	class MeshAndBspLoader;
	class TgaFileLoader;
	class MatLibIniLoader;
	class SkyFileLoader;

	/** Plugin instance for EditorTools */
	class FileFormatsPlugin : public Plugin
	{
	public:
		FileFormatsPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		_3dsFileLoader*		m3dsFileLoader;
		MrmFileLoader*		mMrmFileLoader;
		MshFileLoader*		mMshFileLoader;
		MeshAndBspLoader*	mMeshAndBspLoader;
		TgaFileLoader*		mTgaFileLoader;
		MatLibIniLoader*    mMatLibIniLoader; 
		SkyFileLoader*      mSkyFileLoader;
	};

} // namespace GothOgre

#endif // FILEFORMATS_PLUGIN_H