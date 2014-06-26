#ifndef GOTHOGRE_STR_CONV_H
#define GOTHOGRE_STR_CONV_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	struct GOTHOGRE_EXPORT StrConv
	{
		template<typename T>
		static String toString(const T& _value)
		{
			StrStream ss;
			ss << _value;
			return ss.str();
		}
		
		template<typename T>
		static T fromString(const String& _str)
		{
			StrStream ss(_str, StrStream::DONT_COPY);
			T value;
			ss >> value;
			return value;
		}

		template<typename T>
		static T fromString(const String& _str, bool* pSuccess)
		{
			StrStream ss(_str, StrStream::DONT_COPY);
			T value;
			ss >> value;
			*pSuccess = !ss.fail();
			return value;
		}
	};

}; // namespace GothOgre

#endif // GOTHOGRE_STR_CONV_H
