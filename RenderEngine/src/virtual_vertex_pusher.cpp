#include "../include/internal/virtual_vertex_pusher.h"

VirtualVertexPusher::VirtualVertexPusher() :
mParent(nullptr),
mIndexBegin(0),
mIndexLength(0),
mVertexBegin(0),
mVertexLength(0)
{}

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

void VirtualVertexPusher::translate(const RenderCommand& i_command, VertexPusher::RenderCommand& o_command) const {
	o_command.mIndexBegin = i_command.mIndexBegin + mIndexBegin;
	o_command.mVertexBegin = i_command.mVertexBegin + mVertexBegin;
	o_command.mInstanceBegin = i_command.mInstanceBegin;
	o_command.mNIndices = i_command.mNIndices;
	o_command.mNInstances = i_command.mNInstances;
}
