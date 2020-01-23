#include "../include/internal/gpu_buffer.h"

GpuBuffer::GpuBuffer() :
mId(0),
mSize(0),
mNCopies(0),
mCopyIndex(0),
mType(),
mUsage(),
mData(nullptr),
mEdits()
{}

GpuBuffer::GpuBuffer(size_t i_size, BufferType i_type, BufferUsage i_usage) :
mId(0),
mSize(i_size),
mNCopies(0),
mCopyIndex(0),
mType(i_type),
mUsage(i_usage),
mData(nullptr),
mEdits() {
	_init();
}

GpuBuffer::~GpuBuffer() {
	destroy();
}

size_t GpuBuffer::getSize() const {
	return mSize;
}

BufferType GpuBuffer::getType() const {
	return mType;
}

BufferUsage GpuBuffer::getUsage() const {
	return mUsage;
}

size_t GpuBuffer::getWriteOffset() const {
	return mCopyIndex * mSize;
}

size_t GpuBuffer::getDrawOffset() const {
	return getPrevCopyIndex() * mSize;
}

void GpuBuffer::init(size_t i_size, BufferType i_type, BufferUsage i_usage) {
	if (isValid()) fail();
	mSize = i_size;
	mType = i_type;
	mUsage = i_usage;
	_init();
}

void GpuBuffer::destroy() {
	if (!isValid()) return;
	if (!mEdits.empty()) fail();

	if (mData) {
		glBindBuffer(mType.toGL(), mId);
		glUnmapBuffer(mType.toGL());
		glBindBuffer(mType.toGL(), 0);
	}
	if (mId) {
		glDeleteBuffers(1, &mId);
	}

	*this = GpuBuffer();
}

bool GpuBuffer::isValid() const {
	return mId;
}

void* GpuBuffer::edit(size_t i_begin, size_t i_length) {
	if (!isValid()) fail();
	if (!mFences[mCopyIndex]) {
		size_t prevIndex = getPrevCopyIndex();
		GLsync& prevFence = mFences[prevIndex];
		if (prevFence) {
			glClientWaitSync(prevFence, GL_SYNC_FLUSH_COMMANDS_BIT, UINT64_MAX);
			glDeleteSync(prevFence);
			prevFence = 0;
		}
		mFences[mCopyIndex] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}

	void* editPtr = nullptr;
	Edit editData = { i_begin, i_length };
	for (auto it = mEdits.begin(); it != mEdits.end(); it++) {
		if (it->second.overlaps(editData)) return nullptr;
	}
	if (mData) {
		editPtr = (ubyte*)mData + i_begin;
	}
	else {
		editPtr = new ubyte[i_length];
	}
	mEdits.insert(std::make_pair(editPtr, editData));
}

void GpuBuffer::commit(void* i_ptr) {
	if (!isValid()) fail();
	auto result = mEdits.find(i_ptr);
	if (result == mEdits.end()) fail();
	if (!mData) {
		glBindBuffer(mType.toGL(), mId);
		glBufferSubData(mType.toGL(), result->second.mBegin, result->second.mLength, result->first);
		glBindBuffer(mType.toGL(), 0);
		delete[] i_ptr;
	}
	mEdits.erase(result);
}

void GpuBuffer::finalize() {
	if (!isValid()) fail();
	mCopyIndex = getNextCopyIndex();
}

void GpuBuffer::_init() {
	glCreateBuffers(1, &mId);
	mNCopies = (mUsage.mWriteFreq == BufferUsage::Frequency::often && mUsage.mWriteLocale == BufferUsage::DataLocale::user) ? 2 : 1;
	mFences.resize(mNCopies, 0);
	mCopyIndex = 0;
	glBindBuffer(mType.toGL(), mId);
	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(mType.toGL(), mSize * mNCopies, nullptr, mapFlags);
	mData = glMapBufferRange(mType.toGL(), 0, mSize * mNCopies, mapFlags);
	glBindBuffer(mType.toGL(), 0);
}

size_t GpuBuffer::getNextCopyIndex() const {
	return (mCopyIndex + 1) % mNCopies;
}

size_t GpuBuffer::getPrevCopyIndex() const {
	return (mCopyIndex + (mNCopies - 1)) % mNCopies;
}

bool GpuBuffer::Edit::overlaps(const Edit& i_other) const {
	return 
		(mBegin >= i_other.mBegin && mBegin < i_other.mBegin + i_other.mLength) ||
		(i_other.mBegin >= mBegin && i_other.mBegin < mBegin + mLength);
}
