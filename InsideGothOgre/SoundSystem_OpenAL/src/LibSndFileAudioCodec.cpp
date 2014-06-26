#include "Precompiled.h"
#include "LibSndFileAudioCodec.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// Virtual I/O functionality to allow libsndfile 
	// reading from Ogre's data stream.
	static sf_count_t vio_get_filelen(void* _userdata)
	{
		DataStreamEx* dataStream = (DataStreamEx*) _userdata;
		return dataStream->size();
	}

	static sf_count_t vio_seek(sf_count_t _offset, int _whence, void* _userdata)
	{
		DataStreamEx* dataStream = (DataStreamEx*) _userdata;
		size_t newofs;
		switch(_whence)
		{
			case SEEK_SET: newofs = (size_t) _offset; break;
			case SEEK_CUR: newofs = dataStream->tellg() + (size_t) _offset; break;
			case SEEK_END: newofs = dataStream->size(); break;
			default: GOTHOGRE_EXCEPT("Unknown whence: " << _whence);
		}
		dataStream->seekg(newofs);
		return newofs;
	}

	static sf_count_t vio_read(void* _ptr, sf_count_t _count, void* _userdata)
	{
		DataStreamEx* dataStream = (DataStreamEx*) _userdata;
		return dataStream->read(_ptr, (size_t) _count);
	}

	static sf_count_t vio_write(const void* _ptr, sf_count_t _count, void* _userdata)
	{
		return 0; // I'm not going to write sound files.
	}

	static sf_count_t vio_tell(void* _userdata)
	{
		DataStreamEx* dataStream = (DataStreamEx*) _userdata;
		return dataStream->tellg();
	}

	static SF_VIRTUAL_IO* getVirtualIO()
	{
		static SF_VIRTUAL_IO sVirtualIO;
		if(!sVirtualIO.get_filelen)
		{
			sVirtualIO.get_filelen = vio_get_filelen;
			sVirtualIO.seek        = vio_seek;
			sVirtualIO.read        = vio_read;
			sVirtualIO.write       = vio_write;
			sVirtualIO.tell        = vio_tell;
		}
		return &sVirtualIO;
	}
		




	//-------------------------------------------------------------------------
	LibSndFileAudioCodec::LibSndFileAudioCodec()
	{
		mSndFile = nullptr;
	}
	//-------------------------------------------------------------------------
	LibSndFileAudioCodec::~LibSndFileAudioCodec()
	{
		if(mSndFile)
			sf_close(mSndFile);
	}
	//-------------------------------------------------------------------------
	void LibSndFileAudioCodec::readHeader()
	{
		SF_INFO sfinfo;
		sfinfo.format = 0;
		mSndFile = sf_open_virtual(getVirtualIO(), SFM_READ, &sfinfo, mDataStream.getPointer());
		if(!mSndFile)
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - Unsupported format or the file is corrupted.");

		mSoundInfo.frames = (size_t) sfinfo.frames;
		mSoundInfo.framesPerSec = sfinfo.samplerate;
		mSoundInfo.channels = sfinfo.channels;
	}
	//-------------------------------------------------------------------------
	void LibSndFileAudioCodec::readFooter()
	{
		if(mSndFile)
		{
			sf_close(mSndFile);
			mSndFile = nullptr;
		}
	}
	//-------------------------------------------------------------------------
	size_t LibSndFileAudioCodec::readData(int16* _data, size_t _maxFrames)
	{
		return (size_t) sf_readf_short(mSndFile, _data, _maxFrames);
	}
	//--------------------------------------------------------------------------
	size_t LibSndFileAudioCodec::getOffset()
	{
		return (size_t) sf_seek(mSndFile, 0, SEEK_CUR);
	}
	//--------------------------------------------------------------------------
	void LibSndFileAudioCodec::setOffset(size_t _frames)
	{
		sf_seek(mSndFile, _frames, SEEK_SET);
	}
}