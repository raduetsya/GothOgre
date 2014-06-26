#ifndef RECURSIVE_ARCHIVE_H
#define RECURSIVE_ARCHIVE_H

namespace GothOgre 
{
	/** This is a "flat" archive. A flat archive is based on normal archive.
	However a flat archive does not recognize subdirectories
	but it can see all files in them.
	Searching in the flat archive is always recursive. */
	template<typename T>
	class RecursiveArchive : public T
    {
    public:
		/** Constructor - don't call direct, used by FlatArchiveFactory. */
		RecursiveArchive(const String& _name, const String& _archType);

		/** Destructor. */
		virtual ~RecursiveArchive();

		/** Find out if the named file exists (note: only base filename is used,
		path is ignored). */
        bool exists(const String& _filename);

		/** Retrieve the modification time of a given file */
		time_t getModifiedTime(const String& _filename);

		/** Open a stream on a given file. 
        @note
            There is no equivalent 'close' method; the returned stream
            controls the lifecycle of this file operation.
        @param filename name of the file (note: only base filename is used,
		path is ignored)
        @returns A shared pointer to a DataStream which can be used to 
            read / write the file. If the file is not present, returns a null
			shared pointer. */
		Ogre::DataStreamPtr open(const String& _filename, bool _readOnly) const;

		Ogre::DataStreamPtr create(const String& _filename) const;

		void remove(const String& _filename) const;

    protected:
		/** Find path to a specified file */
		String getPath(const String& _filename) const;

	private:
		/** Find path to a specified file */
		String _getPath(const String& _filename);
    };
	//-----------------------------------------------------------------------------------
	template<typename T>
	RecursiveArchive<T>::RecursiveArchive(const String& _name, const String& _archType )
		: T(_name, _archType)
	{
	}
	//-----------------------------------------------------------------------------------
	template<typename T>
	RecursiveArchive<T>::~RecursiveArchive()
	{
	}
	//-----------------------------------------------------------------------------------
	template<typename T>
	inline String RecursiveArchive<T>::getPath(const String& _filename) const
	{
		return const_cast<RecursiveArchive<T>*>(this)->_getPath(_filename);
	}
	//-----------------------------------------------------------------------------------
	template<typename T>
	String RecursiveArchive<T>::_getPath(const String& _filename)
	{
		String filepath = _filename;
		StringVectorPtr vecptr = T::find(_filename, true, false);
		if(!vecptr->empty())
		{
			filepath = (*vecptr)[0];
			if(vecptr->size() != 1)
			{
				GOTHOGRE_WARNING(_filename << " - Found two files"
					" with the same name: '" << (*vecptr)[0] << "' and '" << (*vecptr)[1] << "'. "
					<< "One of these files will be unavailable.");
			}
		}
		return filepath;
	}
    //-----------------------------------------------------------------------
	template<typename T>
	bool RecursiveArchive<T>::exists(const String& _filename)
	{
		return T::exists(getPath(_filename));
	}
	//---------------------------------------------------------------------
	template<typename T>
	time_t RecursiveArchive<T>::getModifiedTime(const String& _filename)
	{
		return T::getModifiedTime(getPath(_filename));
	}
    //-----------------------------------------------------------------------
	template<typename T>
	Ogre::DataStreamPtr RecursiveArchive<T>::open(const String& _filename, bool _readOnly) const
    {
		return T::open(getPath(_filename), _readOnly);
    }
    //-----------------------------------------------------------------------
	template<typename T>
	Ogre::DataStreamPtr RecursiveArchive<T>::create(const String& _filename) const
	{
		return T::create(getPath(_filename));
	}
    //-----------------------------------------------------------------------
	template<typename T>
	void RecursiveArchive<T>::remove(const String& _filename) const
    {
		return T::remove(getPath(_filename));
	}

} // namespace GothOgre

#endif // RECURSIVE_ARCHIVE_H
