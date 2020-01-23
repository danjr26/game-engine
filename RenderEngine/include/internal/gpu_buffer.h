#ifndef GPU_BUFFER_H
#define GPU_BUFFER_H

#include "enum_wrappers.h"
#include <unordered_map>

class GpuBuffer {
private:
	struct Edit {
		size_t mBegin;
		size_t mLength;

		bool overlaps(const Edit& i_other) const;
	};

	GLuint mId;
	size_t mSize;
	size_t mNCopies;
	size_t mCopyIndex;
	std::vector<GLsync> mFences;

	BufferType mType;
	BufferUsage mUsage;

	void* mData;

	std::unordered_map<void*, Edit> mEdits;

public:
	GpuBuffer();
	GpuBuffer(size_t i_size, BufferType i_type, BufferUsage i_usage);
	~GpuBuffer();

	size_t getSize() const;
	BufferType getType() const;
	BufferUsage getUsage() const;

	size_t getWriteOffset() const;
	size_t getDrawOffset() const;

	void init(size_t i_size, BufferType i_type, BufferUsage i_usage);
	void destroy();
	bool isValid() const;

	void* edit(size_t i_begin, size_t i_length);
	void commit(void* i_ptr);
	void finalize();

private:
	void _init();
	size_t getNextCopyIndex() const;
	size_t getPrevCopyIndex() const;
};

#endif