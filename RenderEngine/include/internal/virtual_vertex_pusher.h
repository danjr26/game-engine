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

private:
	VirtualVertexPusher();

public:
	void* editIndices();
	void commitIndices(void* i_ptr);

	void* editVertices(vertex_slot_t i_slot);
	void commitVertices(vertex_slot_t i_slot, void* i_ptr);

private:
	void translate(const RenderCommand& i_command, VertexPusher::RenderCommand& o_command) const;
};

#endif