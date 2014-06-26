#include "GothOgre_Precompiled.h"
#include "GothOgre_MemoryPool.h"

namespace GothOgre
{

	MemoryPool::MemoryPool(size_t _blockSize, size_t _numBlocksPerAllocate)
	{
		mRefCount = 0;
		mNumChunks = 0;
		mNumAllocatedBlocks = 0;
		mNumDeallocatedBlocks = 0;
		mFirstChunk = nullptr;
		mFirstFreeBlock = nullptr;
		mBlockSize = _blockSize;
		mNumBlocksPerAllocate = _numBlocksPerAllocate;
		recalcChunkSize();
	}
	//-------------------------------------------------------------------------------------------------------------------
	MemoryPool::~MemoryPool()
	{
		while(mFirstChunk)
		{
			Chunk* nextChunk = mFirstChunk->nextChunk;
			delete[] reinterpret_cast<uint8*>( mFirstChunk );
			mFirstChunk = nextChunk;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------
	void MemoryPool::setBlockSize(size_t _blockSize)
	{
		if(mBlockSize != _blockSize)
		{
			GOTHOGRE_ASSERT(mFirstChunk == nullptr, "Cannot change block's size because this memory pool is already in use.");
			mBlockSize = _blockSize;
			recalcChunkSize();
		}
	}
	//-------------------------------------------------------------------------------------------------------------------
	void MemoryPool::setNumBlocksPerAllocate(size_t _numBlocksPerAllocate)
	{
		if(mNumBlocksPerAllocate != _numBlocksPerAllocate)
		{
			mNumBlocksPerAllocate = _numBlocksPerAllocate;
			recalcChunkSize();
		}
	}
	//-------------------------------------------------------------------------------------------------------------------
	void MemoryPool::recalcChunkSize()
	{
		size_t offsetOfBlocks = (size_t) &(((Chunk*)0)->blocks[0]);
		mRealBlockSize = std::max(mBlockSize, sizeof(Block*));
		mChunkSize = offsetOfBlocks + mRealBlockSize * mNumBlocksPerAllocate;
	}
	//-------------------------------------------------------------------------------------------------------------------
	void* MemoryPool::allocate()
	{
		// If there are no free blocks...
		if(!mFirstFreeBlock)
		{
			// then allocate a new chunk,
			Chunk* newChunk = reinterpret_cast<Chunk*>( new uint8[mChunkSize] );
			newChunk->nextChunk = mFirstChunk;
			newChunk->numBlocks = mNumBlocksPerAllocate;
			++mNumChunks;
			mFirstChunk = newChunk;

			// and add blocks in the new chunk to list of free chunk.
			size_t offsetOfBlocks = (size_t) &(newChunk->blocks[0]);
			for(size_t i = 0; i != newChunk->numBlocks; ++i)
			{
				Block* block = (Block*) (offsetOfBlocks + mRealBlockSize * i);
				block->nextFreeBlock = mFirstFreeBlock;
				mFirstFreeBlock = block;
			}
		}

		// Use the first free block.
		void* data = mFirstFreeBlock;
		mFirstFreeBlock = mFirstFreeBlock->nextFreeBlock;
		++mNumAllocatedBlocks;
		return data;
	}
	//-------------------------------------------------------------------------------------------------------------------
	void MemoryPool::deallocate(void* _p)
	{
		// Add the deallocated block to list of free blocks.
		Block* newFreeBlock = reinterpret_cast<Block*>( _p );
		newFreeBlock->nextFreeBlock = mFirstFreeBlock;
		mFirstFreeBlock = newFreeBlock;
		++mNumDeallocatedBlocks;
	}
	//-------------------------------------------------------------------------------------------------------------------
	MemoryPool::Chunk* MemoryPool::findChunkByPointer(void* _p) const
	{
		Chunk* chunk = mFirstChunk;
		while(chunk)
		{
			void* beginBlocks = &(chunk->blocks[0]);
			void* endBlocks = (void*) ((size_t) beginBlocks + chunk->numBlocks * mRealBlockSize);
			if(beginBlocks <= _p && _p < endBlocks)
				return chunk;
			chunk = chunk->nextChunk;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------------------------------------------------------
	bool MemoryPool::isFreeBlock(Block* _block) const
	{
		Block* block = mFirstFreeBlock;
		while(block)
		{
			if(block == _block)
				return true;
			block = block->nextFreeBlock;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------------------
	bool MemoryPool::isAllocated(void* _p) const
	{
		Chunk* chunk = findChunkByPointer(_p);
		return (chunk != nullptr);
	}
	//-------------------------------------------------------------------------------------------------------------------
	void* MemoryPool::findAllocated(void* _start) const
	{
		// Determine start chunk and start block for searching
		// by the specified "_start" pointer.
		Chunk* chunk;
		Block* block;
		if(_start)
		{
			// Continue searching.
			chunk = findChunkByPointer(_start);
			void* beginBlocks = &(chunk->blocks[0]);
			size_t blockIndex = ((size_t) _start - (size_t) beginBlocks) / mRealBlockSize;
			++blockIndex;
			block = (Block*) ((size_t) beginBlocks + mRealBlockSize * blockIndex);
		}
		else
		{
			// Start searching.
			if(!mFirstChunk)
				return nullptr;
			chunk = mFirstChunk;;
			block = &(mFirstChunk->blocks[0]);
		}

		// Search next allocated block.
		while(chunk)
		{
			// Check blocks in the current chunk
			void* beginBlocks = &(chunk->blocks[0]);
			void* endBlocks = (void*) ((size_t) beginBlocks + chunk->numBlocks * mRealBlockSize);
			while( (void*) block < endBlocks)
			{
				if(!isFreeBlock(block))
					return block;
				block = (Block*) ((size_t) block + mRealBlockSize);
			}

			// Go to the next chunk.
			chunk = chunk->nextChunk;
			if(chunk)
				block = &(chunk->blocks[0]);
		}

		// Not found.
		return nullptr;
	}


} // namespace GothOgre