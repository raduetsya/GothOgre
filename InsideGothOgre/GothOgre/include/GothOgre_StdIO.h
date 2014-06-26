#ifndef GOTHOGRE_STD_IO_H
#define GOTHOGRE_STD_IO_H

#include "GothOgre_StrStream.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// std::pair
	//---------------------------------------------------------------------------
	template<typename T1, typename T2>
	StrStream& operator <<(StrStream& _ss, const std::pair<T1, T2>& _pair)
	{
		_ss << _pair.first;
		_ss << ' ';
		_ss << _pair.second;
		return _ss;
	}
	//---------------------------------------------------------------------------
	template<typename T1, typename T2>
	StrStream& operator >>(StrStream& _ss, std::pair<T1, T2>& _pair)
	{
		if(_ss.fail())
			return _ss;

		_ss >> _pair.first;
		if(_ss.fail())
			return _ss;

		_ss >> _pair.second;
		return _ss;
	}


	//---------------------------------------------------------------------------
	// std::vector, std::list - sequence containers
	//---------------------------------------------------------------------------
	template<typename ContainerType>
	void outputSequenceContainer(StrStream& _ss, const ContainerType& _container)
	{
		for(ContainerType::const_iterator it = _container.begin();
			it != _container.end(); ++it)
		{
			if(it != _container.begin())
				_ss << ' ';
			_ss << *it;
		}
	}

	template<typename ContainerType>
	void inputSequenceContainer(StrStream& _ss, ContainerType& _container)
	{
		if(_ss.fail())
			return;

		_container.clear();
		
		while(true)
		{
			ContainerType::value_type value;
			size_t pos = _ss.tellg();
			_ss >> value;
			if(_ss.fail())
			{
				_ss.clear();
				_ss.seekg(pos);
				break;
			}
			_container.push_back(value);
		}
	}

	template<typename T, typename A>
	StrStream& operator <<(StrStream& _ss, const std::vector<T, A>& _vector)
	{
		outputSequenceContainer(_ss, _vector);
		return _ss;
	}

	template<typename T, typename A>
	StrStream& operator >>(StrStream& _ss, std::vector<T, A>& _vector)
	{
		inputSequenceContainer(_ss, _vector);
		return _ss;
	}

	template<typename T, typename A>
	StrStream& operator <<(StrStream& _ss, const std::list<T, A>& _list)
	{
		outputSequenceContainer(_ss, _list);
		return _ss;
	}

	template<typename T, typename A>
	StrStream& operator >>(StrStream& _ss, std::list<T, A>& _list)
	{
		inputSequenceContainer(_ss, _list);
		return _ss;
	}

} // namespace GothOgre

#endif // GOTHOGRE_STD_IO_H