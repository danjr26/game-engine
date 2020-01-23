#include "../include/internal/vertex_pusher.h"

VertexPusher::VertexPusher() :
mArrayId(0),
mLayout(),
mIndices(),
mMaxIndices(0),
mNIndices(0),
mVertices(),
mMaxVertices(0),
mNVertices(0),
mCommands(),
mMaxCommands(0),
mNCommands(0)
{}

VertexPusher::VertexPusher(const VertexPusherLayout& i_layout, size_t i_maxIndices, size_t i_maxVertices, size_t i_maxCommands) :
mArrayId(0),
mLayout(i_layout),
mIndices(),
mMaxIndices(i_maxIndices),
mNIndices(0),
mVertices(),
mMaxVertices(i_maxVertices),
mNVertices(0),
mCommands(),
mMaxCommands(i_maxCommands),
mNCommands(0) {
	_init();
}

VertexPusher::~VertexPusher() {
	destroy();
}

void VertexPusher::init(const VertexPusherLayout& i_layout, size_t i_maxIndices, size_t i_maxVertices, size_t i_maxCommands) {
	if (isValid()) fail();
	mLayout = i_layout;
	mMaxIndices = i_maxIndices;
	mMaxVertices = i_maxVertices;
	mMaxCommands = i_maxCommands;
	_init();
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

void VertexPusher::getLayout(VertexPusherLayout& o_layout) {
	if (!isValid()) fail();
	o_layout = mLayout;
}

void VertexPusher::render() {
	glBindVertexArray(mArrayId);
	glMultiDrawElementsIndirect(mLayout.mTopology.toGL(), mLayout.mIndex.mType.toGL(), (void*)mCommands.getDrawOffset(), mNCommands, sizeof(RenderCommand));
	glBindVertexArray(0);
}

/*
VertexPusher::IndexData* VertexPusher::addIndexData() {
	if (mIndices.first == nullptr) {
		mIndices.first = new IndexData;
	}
	return mIndices.first;
}

void VertexPusher::removeIndexData() {
	if (mIndices.first != nullptr) {
		delete mIndices.first;
		mIndices.first = nullptr;
	}
	if (mIndices.second != nullptr) {
		delete mIndices.second;
		mIndices.second = nullptr;

		glBindVertexArray(mArrayID);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
		}
		glBindVertexArray(0);
	}
}

VertexPusher::IndexData* VertexPusher::getIndexData() {
	return mIndices.first;
}

void VertexPusher::pushIndexData() {
	glBindVertexArray(mArrayID);
	{
		// delete old gpu-side container if is now not valid
		if (mIndices.second && *mIndices.first != (IndexData)*mIndices.second) {
			delete mIndices.second;
			mIndices.second = nullptr;
		}

		// create new gpu-side container if needed
		if (!mIndices.second) {
			mIndices.second = new _IndexData;
			*(IndexData*)mIndices.second = *mIndices.first;

			glCreateBuffers(1, &mIndices.second->mBufferID);

			// bind it to this vao
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices.second->mBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.first->mCount * mIndices.first->mType.getSize(), nullptr, mIndices.first->mUsage.toGL());
		}
	}
	glBindVertexArray(0);
}

void VertexPusher::pushIndices() {
	if (!mArrayID || !mIndices.first || !mIndices.first->mData || !mIndices.second) fail();

	mIndices.second->mData = mIndices.first->mData;
	glBindVertexArray(mArrayID);
	{
		glBufferSubData(
			GL_ELEMENT_ARRAY_BUFFER, 
			0, mIndices.second->mCount * mIndices.second->mType.getSize(), 
			mIndices.second->mData
		);
	}
	glBindVertexArray(0);
}

VertexPusher::VertexData* VertexPusher::addVertexData(slot_t i_slot) {
	auto result = mVertices.find(i_slot);
	if (result == mVertices.end()) {
		auto newData = new VertexData;
		mVertices.insert(std::make_pair(i_slot, std::make_pair(newData, nullptr)));
		return newData;
	}
	else {
		return result->second.first;
	}
}

void VertexPusher::removeVertexData(slot_t i_slot) {
	auto result = mVertices.find(i_slot);
	if (result == mVertices.end()) return;
	auto vertexData = result->second;

	if (vertexData.first != nullptr) {
		delete vertexData.first;
		vertexData.first = nullptr;
	}
	if (vertexData.second != nullptr) {
		delete vertexData.second;
		vertexData.second = nullptr;
	}
}

VertexPusher::VertexData* VertexPusher::getVertexData(slot_t i_slot) {
	auto result = mVertices.find(i_slot);
	return (result == mVertices.end()) ? nullptr : result->second.first;
}

void VertexPusher::pushVertexData(slot_t i_slot) {
	auto result = mVertices.find(i_slot);
	if (result == mVertices.end()) fail();
	auto vertexData = result->second;

	// create and bind vao
	glBindVertexArray(mArrayID);
	{
		// delete old gpu-side container if is now not valid
		if (vertexData.second && *vertexData.first != *vertexData.second) {
			delete vertexData.second;
			vertexData.second = nullptr;
		}

		// create new gpu-side container if needed
		if (!vertexData.second) {
			vertexData.second = new _VertexData;
			*(VertexData*)vertexData.second = *vertexData.first;
			glCreateBuffers(1, &vertexData.second->mBufferID);

			// bind it to this vao
			glBindBuffer(GL_ARRAY_BUFFER, vertexData.second->mBufferID);
			{
				glBufferData(GL_ARRAY_BUFFER, vertexData.first->mCount * vertexData.first->mDepth * vertexData.first->mType.getSize(), nullptr, vertexData.first->mUsage.toGL());
				glEnableVertexAttribArray(i_slot);
				glVertexAttribPointer(i_slot, vertexData.first->mDepth, vertexData.first->mType.toGL(), GL_FALSE, 0, nullptr);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	glBindVertexArray(0);
}

void VertexPusher::pushVertices(slot_t i_slot) {
	auto result = mVertices.find(i_slot);
	if (result == mVertices.end()) fail();
	auto vertexData = result->second;

	if (!mArrayID || !vertexData.first || !vertexData.first->mData || !vertexData.second) fail();

	vertexData.second->mData = vertexData.first->mData;
	glBindVertexArray(mArrayID);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexData.second->mBufferID);
		{
			glBufferSubData(
				GL_ELEMENT_ARRAY_BUFFER,
				0, vertexData.second->mCount * (uint)vertexData.second->mDepth * (uint)vertexData.second->mType.getSize(),
				vertexData.second->mData
			);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void VertexPusher::render() {
	if (mVertices.empty()) return;
	glBindVertexArray(mArrayID);
	{
		if (mIndices.second) {
			glDrawElements(mTopology.toGL(), mIndices.second->mCount, mIndices.second->mType.toGL(), nullptr);
		}
		else {
			uint nVertices = UINT_MAX;
			for (auto it = mVertices.begin(); it != mVertices.end(); it++) {
				nVertices = std::min(nVertices, it->second.second->mCount);
			}
			glDrawArrays(mTopology.toGL(), 0, nVertices);
		}
	}
	glBindVertexArray(0);
}
*/
void VertexPusher::_init() {
	glCreateVertexArrays(1, &mArrayId);
	glBindVertexArray(mArrayId);
	{
		mIndices.init(mMaxIndices * mLayout.mIndex.getSize(), BufferType::Value::vertex_indices, mLayout.mIndex.getUsage());
		for (auto it = mLayout.mVertex.begin(); it != mLayout.mVertex.end(); it++) {
			auto kt = mVertices.insert(std::make_pair(it->first, GpuBuffer())).first;
			kt->second.init(mMaxVertices * it->second.getSize(), BufferType::Value::vertex_attributes, it->second.getUsage());
			glEnableVertexAttribArray(it->first);
			glVertexAttribPointer(it->first, it->second.mDepth, it->second.mType.toGL(), false, 0, nullptr);
		}
		BufferUsage commandUsage(BufferUsage::Frequency::often, BufferUsage::DataLocale::renderer, BufferUsage::Frequency::often, BufferUsage::DataLocale::user);
		mCommands.init(mMaxCommands * sizeof(RenderCommand), BufferType::Value::draw_command, commandUsage);
	}
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
