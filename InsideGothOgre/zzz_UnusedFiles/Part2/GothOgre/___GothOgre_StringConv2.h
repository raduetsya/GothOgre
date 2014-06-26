#ifndef GOTHOGRE_STRING_CONV_H
#define GOTHOGRE_STRING_CONV_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	template<typename T>
	struct StringConvOutput
	{
		static String toString(const T& _val)
		{
			return _val.toString();
		}
	};

	template<typename T>
	struct StringConvOutputP
	{
		static String toString(const T& _val, size_t _param)
		{
			return _val.toString(_param);
		}
	};

	template<typename T>
	struct StringConvInput
	{
		static T fromString(const String& _str, size_t _start, size_t& _readcount)
		{
			return T::fromString(_str, _start, _readcount);
		}
	};

	template<typename T>
	struct StringConvInputP
	{
		static T fromString(const String& _str, size_t _start, size_t& _readcount, size_t _param)
		{
			return T::fromString(_str, _start, _readcount, _param);
		}
	};


	//---------------------------------------------------------------------------
	struct GOTHOGRE_EXPORT StringConv
	{
		template<typename T>
		static T fromString(const String& _str)
		{
			size_t readcount;
			return fromString<T>(_str, 0, readcount);
		}

		template<typename T>
		static T fromString(const String& _str, size_t _start, size_t& _readcount)
		{
			return StringConvInput<T>::fromString(_str, _start, _readcount);
		}

		template<typename T>
		static T fromString(const String& _str, size_t _param)
		{
			size_t readcount;
			return fromString<T>(_str, 0, readcount, _param);
		}

		template<typename T>
		static T fromString(const String& _str, size_t _start, size_t& _readcount, size_t _param)
		{
			return StringConvInputP<T>::fromString(_str, _start, _readCount, _param);
		}

		template<typename T>
		static String toString(const T& _val)
		{
			return StringConvOutput<T>::toString(_val);
		}

		template<typename T>
		static String toString(const T& _val, size_t _param)
		{
			return StringConvOutputP<T>::toString(_val, _param);
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_STRING_CONV_H