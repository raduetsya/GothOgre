#ifndef GOTHOGRE_ENCODING_SETTINGS_H
#define GOTHOGRE_ENCODING_SETTINGS_H

#include "GothOgre_ConfigSettings.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------
	/** Class to keep encoding settings.
	@remark
	The class is designed to operate with map of encodings.
	@par
	Once an instance has been created, the same instance is accessible throughout 
	the life of that object by using EncodingSettings::getSingleton (as a reference) 
	or EncodingSettings::getSingletonPtr (as a pointer). */
	class GOTHOGRE_EXPORT EncodingSettings : public Singleton<EncodingSettings>, public ConfigSettings
	{
	public:
		EncodingSettings(const String& _filename);

		// The default encoding is "windows-1251".
		static const String DEFAULT_ENCODING;

		/** Searchs a encoding by filename in the configuration file;
		returns a found encoding or the default encoding if not found. */
		String getEncoding(const String& _filename, const String& _defaultEncoding = DEFAULT_ENCODING);

		/** Sets a encoding of the DataStream, if it's not set before. */
		void setupStreamEncoding(const DataStreamExPtr& _dataStream, const String& _defaultEncoding = DEFAULT_ENCODING);

		/** Read specified number of bytes from data stream, compare the result 
		with a specified string, and find a encoding in which 
		these strings are equal. */
		void readStringAndGuessEncoding(const DataStreamExPtr& _dataStream, const String& _str, size_t _sizeInBytes);

		/** Read a line from data stream, compare the result 
		with a specified string, and find a encoding in which 
		these strings are equal. */
		void readLineAndGuessEncoding(const DataStreamExPtr& _dataStream, const String& _line, EOL _eol = EOL::CRLF);
	};

	// Macro to inject config settings' listening functionality to any class.
#	define GOTHOGRE_DECLARE_ENCODING_SETTINGS_CHANGED_EX( \
		encodingSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			encodingSettingsChanged, \
			registerFunc, unregisterFunc, \
			classname, EncodingSettings)

#	define GOTHOGRE_DECLARE_ENCODING_SETTINGS_CHANGED(classname) \
		GOTHOGRE_DECLARE_ENCODING_SETTINGS_CHANGED_EX( \
			encodingSettingsChanged, \
			registerEncodingSettingsListener, unregisterEncodingSettingsListener, \
			classname)

	//------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass();

	private:
		GOTHOGRE_DECLARE_ENCODING_SETTINGS_CHANGED(MyClass)
	};
	
	MyClass::MyClass()
	{
		registerEncodingSettingsListener(); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterEncodingSettingsListener
		// - it's called automatically
	}

	void MyClass::encodingSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// my event-handling code
	}

	*/

} // namespace GothOgre

#endif // GOTHOGRE_ENCODING_SETTINGS_H