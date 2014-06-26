#ifndef GOTHOGRE_CODEPAGE_CONVERTER_MANAGER_H
#define GOTHOGRE_CODEPAGE_CONVERTER_MANAGER_H


namespace GothOgre
{
	class CodepageConverter;


	//--------------------------------------------------------------------------
	class GOTHOGRE_EXPORT CodepageConverterManager : public Singleton<CodepageConverterManager>
	{
	public:
		CodepageConverterManager();
		~CodepageConverterManager();

		/** Returns a list of all available encodings. */
		const StringVector& getAllEncodings();

		/** Opens a converter to convert strings after. */
		CodepageConverter* openConverter(const String& _encoding);
	};

} // namespace GothOgre

#endif // GOTHOGRE_CODEPAGE_CONVERTER_MANAGER_H