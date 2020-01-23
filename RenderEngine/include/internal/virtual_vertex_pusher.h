#ifndef VIRTUAL_VERTEX_PUSHER_H
#define VIRTUAL_VERTEX_PUSHER_H

#include "vertex_pusher.h"

class VirtualVertexPusher {
friend class RenderEngine;
public:
using RenderCommand = VertexPusher::RenderCommand;

private:
	VertexPusher* mParent;
	uint mIndexBegin;
	uint mIndexLength;
	uint mVertexBegin;
	uint mVertexLength;
	std::unordered_map<RenderCommand*, uint> mNewCommands;

private:
	VirtualVertexPusher();

public:
	VertexPusher* getParent();

	void* editIndices();
	void commitIndices(void* i_ptr);

	void* editVertices(vertex_slot_t i_slot);
	void commitVertices(vertex_slot_t i_slot, void* i_ptr);

	RenderCommand* newCommands(uint i_n);
	void commitCommands(RenderCommand* i_ptr);
};

#endif