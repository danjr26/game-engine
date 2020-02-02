#include "../include/internal/vertex_pusher.h"

VertexPusher::VertexPusher() :
mArrayId(0),
mLayout(),
mIndices(),
mMaxIndices(0),
mVertices(),
mMaxVertices(0),
mCommands(),
mMaxCommands(0),
mNCommands(0)
{}

VertexPusher::VertexPusher(const VertexPusherLayout& i_layout, uint i_maxIndices, uint i_maxVertices, uint i_maxCommands) :
mArrayId(0),
mLayout(i_layout),
mIndices(),
mMaxIndices(i_maxIndices),
mVertices(),
mMaxVertices(i_maxVertices),
mCommands(),
mMaxCommands(i_maxCommands),
mNCommands(0) {
	_init();
}

VertexPusher::~VertexPusher() {
	destroy();
}

void VertexPusher::init(const VertexPusherLayout& i_layout, uint i_maxIndices, uint i_maxVertices, uint i_maxCommands) {
	if (isValid()) fail();
	mLayout = i_layout;
	mMaxIndices = i_maxIndices;
	mMaxVertices = i_maxVertices;
	mMaxCommands = i_maxCommands;
	_init();
}

void VertexPusher::_init() {
	glCreateVertexArrays(1, &mArrayId);
	glBindVertexArray(mArrayId);
	{
		mIndices.init(
			mMaxIndices * (size_t)mLayout.mIndex.getSize(), 
			BufferType::Value::vertex_indices, 
			mLayout.mIndex.getUsage(), 
			ReadWrite(false, true)
		);
		for (auto it = mLayout.mVertex.begin(); it != mLayout.mVertex.end(); it++) {
			auto kt = mVertices.insert(std::make_pair(it->first, GpuBuffer())).first;
			kt->second.init(
				mMaxVertices * (size_t)it->second.getSize(), 
				BufferType::Value::vertex_attributes, 
				it->second.getUsage(),
				ReadWrite(false, true)
			);
			glEnableVertexAttribArray(it->first);
			kt->second.activate();
			glVertexAttribPointer(it->first, it->second.mDepth, it->second.mType.toGL(), false, 0, nullptr);
			kt->second.deactivate();
		}
		BufferUsage commandUsage(BufferUsage::Frequency::often, BufferUsage::DataLocale::renderer, BufferUsage::Frequency::often, BufferUsage::DataLocale::user);
		mCommands.init(
			mMaxCommands * sizeof(RenderCommand), 
			BufferType::Value::draw_command, 
			commandUsage,
			ReadWrite(false, true)
		);
		mIndices.activate();
		mCommands.activate();
	}
	glBindVertexArray(0);
}

void VertexPusher::destroy() {
	if (!isValid()) return;

	if (mArrayId) {
		glDeleteVertexArrays(1, &mArrayId);
	}

	mIndices.destroy();
	for (auto it = mVertices.begin(); it != mVertices.end(); it++) {
		it->second.destroy();
	}
	mCommands.destroy();

	*this = VertexPusher();
}

bool VertexPusher::isValid() const {
	return mArrayId;
}

VertexPusher::RenderCommand* VertexPusher::newCommands(uint i_n) {
	if (mNCommands + i_n > mMaxCommands) return nullptr;
	void * editPtr = mCommands.edit(mNCommands * sizeof(RenderCommand), i_n * sizeof(RenderCommand));
	mNCommands += i_n;
	return (RenderCommand*)editPtr;
}

void VertexPusher::commitCommands(RenderCommand* i_ptr) {
	mCommands.commit(i_ptr);
}

void VertexPusher::finalize() {
	mIndices.finalize();
	mCommands.finalize();
	for (auto it = mVertices.begin(); it != mVertices.end(); it++) {
		it->second.finalize();
	}
}

void VertexPusher::render() {
	glBindVertexArray(mArrayId);
	glMultiDrawElementsIndirect(mLayout.mTopology.toGL(), mLayout.mIndex.mType.toGL(), (void*)mCommands.getDrawOffset(), mNCommands, sizeof(RenderCommand));
	glBindVertexArray(0);
}

ubyte IndexLayout::getSize() const {
	return mType.getSize();
}

BufferUsage IndexLayout::getUsage() const {
	return BufferUsage(BufferUsage::Frequency::often, BufferUsage::DataLocale::renderer, mFreq, BufferUsage::DataLocale::user);
}

ubyte VertexLayout::getSize() const {
	return mType.getSize() * mDepth;
}

BufferUsage VertexLayout::getUsage() const {
	return BufferUsage(BufferUsage::Frequency::often, BufferUsage::DataLocale::renderer, mFreq, BufferUsage::DataLocale::user);
}
