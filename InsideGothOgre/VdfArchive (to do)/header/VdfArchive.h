#ifndef VDFS_ARCHIVE_H
#define VDFS_ARCHIVE_H


// Один Vdfs-архив описывается объектом этого класса
class VdfsArchive : public Ogre::Archive 
{
public:
	// конструктор, непосредственно вызываться не должен,
	// этим занимается VdfsArchiveFactory
	VdfsArchive( const String& name, const String& archType );
	~VdfsArchive();

	// Чувствительность к регистру, когда ищем файлы
	bool isCaseSensitive() const { return false; }

	// Загрузить архив
	void load();
	
	// Выгрузить архив
	void unload();

	// Открыть файл в архиве
	DataStreamPtr open(const String& filename) const;

	// Получить список файлов в архиве
	StringVectorPtr list(bool recursive = true, bool dirs = false);

	// Получить список файлов в архиве с дополнительной информацией
	FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

	// Найти файл в архиве
	StringVectorPtr find(const String& pattern, bool recursive = true,
		bool dirs = false);

	// Найти файл в архиве и получить дополнительную информацию
	FileInfoListPtr findFileInfo(const String& pattern, bool recursive = true,
		bool dirs = false);

	// Проверить существование файла
	bool exists(const String& filename);

	// Получить дату последнего изменения файла
	time_t getModifiedTime(const String& filename);
};


// Фабрика VDFS-архивов - существует в единственном числе
class VdfsArchiveFactory : public ArchiveFactory
{
public:
	virtual ~VdfsArchiveFactory();
	const String& getType(void) const;
	Archive *createInstance( const String& name );
	void destroyInstance( Archive* arch);
};


// Поток данных из открытого файла в VDFS-архиве
class VdfsDataStream : public DataStream
{
public:
	VdfsDataStream(...);
	~VdfsDataStream();
	size_t read(void* buf, size_t count);
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
};


#endif // VDFS_ARCHIVE_H