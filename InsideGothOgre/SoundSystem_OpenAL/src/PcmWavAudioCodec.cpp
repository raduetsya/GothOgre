#include "Precompiled.h"
#include "PcmWavAudioCodec.h"

	
namespace GothOgre
{
	//--------------------------------------------------------------------------
	PcmWavAudioCodec::PcmWavAudioCodec()
	{
	}
	//--------------------------------------------------------------------------
	PcmWavAudioCodec::~PcmWavAudioCodec()
	{
	}
	//--------------------------------------------------------------------------
	void PcmWavAudioCodec::readHeader()
	{
		Chunk riff;
		mDataStream->readData(riff);
		if(riff.type[0] != 'R' || riff.type[1] != 'I' || riff.type[2] != 'F' || riff.type[3] != 'F')
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - is not a wav file.");

		char wave[4];
		mDataStream->readData(wave);
		if(wave[0] != 'W' || wave[1] != 'A' || wave[2] != 'V' || wave[3] != 'E')
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - is not a wav file.");

		bool fmtChunkFound = false;
		bool dataChunkFound = false;

		size_t restSize = riff.size - 4;
		while(restSize != 0)
		{
			Chunk chunk;
			mDataStream->readData(chunk);
			size_t chunkPos = mDataStream->tell();

			if(chunk.type[0] == 'f' && chunk.type[1] == 'm' && chunk.type[2] == 't' && chunk.type[3] == ' ')
			{
				fmtChunkFound = true;
				Fmt fmt;
				mDataStream->readData(fmt);
				if(fmt.format != FORMAT_PCM)
					GOTHOGRE_EXCEPT(mDataStream->getName() << " - Cannot process compressed audio files.");

				mSoundInfo.channels = (uchar) fmt.channels;
				mBytesPerSampleInFile = fmt.bitsPerSample / 8;
				mSoundInfo.framesPerSec = fmt.samplesPerSec;
			}
			else if(chunk.type[0] == 'd' && chunk.type[1] == 'a' && chunk.type[2] == 't' && chunk.type[3] == 'a')
			{
				dataChunkFound = true;
				mStartOffset = chunkPos;
				mEndOffset = chunkPos + chunk.size;
			}

			mDataStream->seekg(chunkPos + chunk.size);
			restSize -= chunk.size + 8;
		}
		
		if(!fmtChunkFound)
			GOTHOGRE_EXCEPT(mDataStream->getName() << " -  Chunk 'fmt' not found. The file seems to be invalid.");
		if(!dataChunkFound)
			GOTHOGRE_EXCEPT(mDataStream->getName() << " -  Chunk 'data' not found. The file seems to be invalid.");

		mSoundInfo.frames = (mEndOffset - mStartOffset) / mBytesPerSampleInFile / mSoundInfo.channels;
	}
	//--------------------------------------------------------------------------
	size_t PcmWavAudioCodec::readData(int16* _data, size_t _maxFrames)
	{
		size_t offset = mDataStream->tellg();
		if(offset >= mEndOffset)
			return false;

		size_t frames = (mEndOffset - offset) / mBytesPerSampleInFile / mSoundInfo.channels;
		if(frames > _maxFrames)
			frames = _maxFrames;
		if(frames == 0)
			return 0;

		size_t sizeInBytes = frames * mBytesPerSampleInFile * mSoundInfo.channels;
		mDataStream->read( (void*) _data, sizeInBytes);

		if(mBytesPerSampleInFile == 1)
		{
			// in-place conversion 8-bit -> 16-bit
			// (waveform data range for 8-bit is [0..255], 
			// for 16-bit is [-32768..32767]
			int8* srcptr = (int8*) _data + sizeInBytes;
			int16* destptr = _data + sizeInBytes;
			while(destptr != _data)
				*(--destptr) = int16(*(--srcptr)) * 256 - 32768;
		}
		return frames;
	}
	//--------------------------------------------------------------------------
	size_t PcmWavAudioCodec::getOffset()
	{
		size_t offset = mDataStream->tellg();
		return (offset - mStartOffset) / mBytesPerSampleInFile / mSoundInfo.channels;
	}
	//--------------------------------------------------------------------------
	void PcmWavAudioCodec::setOffset(size_t _frames)
	{
		size_t pos = _frames * mBytesPerSampleInFile * mSoundInfo.channels + mStartOffset;
		mDataStream->seekg(pos);
	}

}