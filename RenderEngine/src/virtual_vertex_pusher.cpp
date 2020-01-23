#include "../include/internal/virtual_vertex_pusher.h"

VirtualVertexPusher::VirtualVertexPusher() :
mParent(nullptr),
mIndexBegin(0),
mIndexLength(0),
mVertexBegin(0),
mVertexLength(0),
mNewCommands()
{}

VertexPusher* VirtualVertexPusher::getParent() {
	return mParent;
}

void* VirtualVertexPusher::editIndices() {
	if (!mParent) fail();
	return mParent->mIndices.edit(mIndexBegin, mIndexLength);
}

void VirtualVertexPusher::commitIndices(void* i_ptr) {
	if (!mParent) fail();
	mParent->mIndices.commit(i_ptr);
}

void* VirtualVertexPusher::editVertices(vertex_slot_t i_slot) {
	if (!mParent) fail();
	auto result = mParent->mVertices.find(i_slot);
	if (result == mParent->mVertices.end()) fail();
	return result->second.edit(mVertexBegin, mVertexLength);
}

void VirtualVertexPusher::commitVertices(vertex_slot_t i_slot, void* i_ptr) {
	if (!mParent) fail();
	auto result = mParent->mVertices.find(i_slot);
	if (result == mParent->mVertices.end()) fail();
	result->second.commit(i_ptr);
}

VirtualVertexPusher::RenderCommand* VirtualVertexPusher::newCommands(uint i_n) {
	if (!mParent) fail();
	RenderCommand* ptr = new RenderCommand[i_n];
	mNewCommands[ptr] = i_n;
	return ptr;
}

void VirtualVertexPusher::commitCommands(RenderCommand* i_ptr) {
	if (!mParent) fail();
	auto result = mNewCommands.find(i_ptr);
	if (result == mNewCommands.end()) fail();
	for (size_t i = 0; i < result->second; i++) {
		i_ptr[i].mVertexBegin += mVertexBegin;
		i_ptr[i].mIndexBegin += mIndexBegin;
	}
	void* edit = mParent->mCommands.edit(mParent->mNCommands, result->second);
	memcpy(edit, i_ptr, result->second * sizeof(RenderCommand));
	mParent->mCommands.commit(edit);
	mParent->mNCommands += result->second;
	if (mParent->mNCommands > mParent->mMaxCommands) fail();
}
