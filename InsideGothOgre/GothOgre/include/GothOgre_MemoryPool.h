#ifndef GOTHOGRE_MEMORY_POOL_H
#define GOTHOGRE_MEMORY_POOL_H

namespace GothOgre
{
	/// Class to operate with memory pool.
	/// This class is designed to speed up allocation small fixed-sized blocks of storage.
	class GOTHOGRE_EXPORT MemoryPool
	{
	public:
		/// Constuctor.
		/// @param _blockSize Size of one block. MemoryPool::allocate returns a block of this size.
		/// @param _numBlockPerAllocate Number of blocks which must be internally allocated from
		/// operation system in one time.
		MemoryPool(size_t _blockSize, size_t _numBlocksPerAllocate);

		/// Destructor.
		~MemoryPool();

		/// Sets block's size.
		/// It's possible to change size of a block specified in the constructor by calling this function
		/// if there are not allocated blocks yet.
		void setBlockSize(size_t _blockSize);

		/// Returns block's size.
		size_t getBlockSize() const {return mBlockSize;}

		/// Sets number of blocks which must be internally allocated from operation system in one time.
		void setNumBlocksPerAllocate(size_t _numBlocksPerAllocate);

		/// Returns number of blocks which must be internally allocated from operation system in one time.
		size_t getNumBlocksPerAllocate() const {return mNumBlocksPerAllocate;}

		/// Allocate a block of storage from this pool.
		/// Size of this block is set by function MemoryPool::setBlockSize and in constructor.
		void* allocate();

		/// Deallocate a block of storage (this will get back a block to this pool).
		void deallocate(void* _p);

		/// Returns number of chunks. It's can be used for statistics.
		size_t getNumChunks() const {return mNumChunks;}

		/// Returns number of allocated blocks. It's can be used for statistics.
		size_t getNumAllocatedBlocks() const {return mNumAllocatedBlocks;}

		/// Returns number of deallocated blocks. It's can be used for statistics.
		size_t getNumDeallocatedBlocks() const {return mNumDeallocatedBlocks;}

		/// Whether the specified block of storage is allocated with the memory pool?
		/// @note This function works slowly - use it for debugging only!
		bool isAllocated(void* _p) const;

		/// Find an allocated (and not deallocated) block of storage.
		/// @param _start Address to start searching from.
		/// This address can be nullptr to find the first allocated block,
		/// either it can be a result of previous search to find next allocated block.
		/// @return Pointer to allocated block of storage or nullptr if not found.
		/// @note This function works very slowly - use it for debugging only!
		void* findAllocated(void* _start) const;

	private:
		/// Block of storage. 
		/// Allocated block stores data, deallocated blocks stores a pointer to next deallocated block.
		/// (Deallocated blocks are stored as one-directional list of free blocks.)
		struct Block
		{
			union
			{
				Block*  nextFreeBlock;

				// Data can have any size. The digit "1" here is only for C++ syntax.
				uint8   data[1];
			};
		};

		/// Chunk. Memory allocated from operation system by large chunks. A chunk contains lots of blocks.
		/// Chunks are stored as one-directional list of chunks.
		struct Chunk
		{
			Chunk*     nextChunk;

			/// Number of blocks in this chunk
			size_t     numBlocks;

			/// Any number of blocks can be here. The digit "1" here is only for C++ syntax.
			Block      blocks[1];
		};

		/// Recalculate mRealBlockSize and mChunkSize
		/// based on mBlockSize and mNumBlocksPerAllocate.
		void recalcChunkSize();

		/// Searchs a chunk by pointer to block of storage;
		/// the function returns nullptr if not found.
		Chunk* findChunkByPointer(void* _p) const;

		/// Whether the specified block of storage is free block?
		bool isFreeBlock(Block* _block) const;

	private:
		/// Number of blocks in one chunk.
		size_t				mNumBlocksPerAllocate;

		/// Size of a block specified in constructor.
		size_t				mBlockSize;

		/// Real size of a block which is really used.
		/// (mRealBlockSize = std::max(mBlockSize, sizeof(Block*) )
		size_t				mRealBlockSize;

		/// Size of chunk is calculated based on mRealBlockSize and mNumBlocksPerAllocate.
		size_t				mChunkSize;

		/// The first chunk. This is a first element of one-directional list of chunks.
		Chunk*				mFirstChunk;

		/// The first free chunk. This is a first element of one-directional list of free blocks.
		Block*				mFirstFreeBlock;

		/// Number of chunks. This can be used for statistics.
		size_t				mNumChunks;

		/// Number of allocated blocks. This can be used for statistics.
		size_t				mNumAllocatedBlocks;

		/// Number of deallocated blocks. This can be used for statistics.
		size_t				mNumDeallocatedBlocks;

		/// Reference counter. Used by the "MemoryPoolPtr" class.
		size_t				mRefCount;
		friend class MemoryPoolPtr;
	};	

}; // namespace GothOgre

#endif // GOTHOGRE_MEMORY_POOL_H
