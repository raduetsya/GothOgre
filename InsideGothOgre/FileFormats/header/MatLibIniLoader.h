#ifndef MAT_LIB_INI_LOADER_H
#define MAT_LIB_INI_LOADER_H


namespace GothOgre
{
	// Loader for file "matlib.ini"
	class MatLibIniLoader : public ScriptLoaderEx
	{
	public:
		MatLibIniLoader();

		/** Parse a script file. */
		void parseScript( DataStreamExPtr& _dataStream, const String& _resourceGroup );

	private:
		/** Read a *.pml file with materials. */
		void readPmlFile( const String& _filename, const String& _resourceGroup );
	};
}

#endif // MAT_LIB_INI_LOADER_H