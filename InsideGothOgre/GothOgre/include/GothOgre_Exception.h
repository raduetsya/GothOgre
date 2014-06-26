#ifndef GOTHOGRE_EXCEPTION_H
#define GOTHOGRE_EXCEPTION_H


namespace GothOgre
{
	class GOTHOGRE_EXPORT Exception : public std::exception
	{
	public:
		Exception();
		Exception(const String& _description, const String& _module, const char* _file, long _line);
		Exception(const Exception& src);

		Exception& operator =(const Exception& _src);

		const String& getDescription() const {return mDescription;}
		const String& getFile() const {return mFile;}
		long getLine() const {return mLine;}
		const String& getFullDescription() const;
		const char* what() const;

	private:
		String   mDescription;
		long     mLine;
		String   mFile;
		String   mModule;
		String   mFullDesc;
	};

} // namespace GothOgre


#endif // GOTHOGRE_EXCEPTION_H