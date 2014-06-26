#include "Precompiled.h"
#include "MatLibIniLoader.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	void MatLibIniLoader::parseScript( DataStreamExPtr& _dataStream, const String& _resourceGroup )
	{
		// The first line must be equal "[MATERIALFILTER]"
		EncodingSettings::getSingleton().readLineAndGuessEncoding(_dataStream, 
			"[MATERIALFILTER]", EOL::CRLF);

		// Read the rest lines with names of *.pml files
		while(!_dataStream->eof())
		{
			String line = _dataStream->readLine(EOL::CRLF);
			if(!line.empty())
			{
				String filename = line;
				size_t equalPos = filename.rfind('=');
				if(equalPos != String::npos)
					filename.resize(equalPos);
				filename += ".pml";
				readPmlFile(filename, _resourceGroup);
			}
		}
	}
	//---------------------------------------------------------------------------
	void MatLibIniLoader::readPmlFile(const String& _filename, const String& _resourceGroup)
	{
		ZenArchive za;
		za.readHeader(_filename, _resourceGroup);
		
		size_t numMaterials = za.getNumObjects();
		for(size_t i = 0; i != numMaterials; ++i)
			za.readObject();

		za.readFooter();
	}
	//---------------------------------------------------------------------------
	MatLibIniLoader::MatLibIniLoader()
		: ScriptLoaderEx("matlib.ini")
	{
	}

} // namespace GothOgre