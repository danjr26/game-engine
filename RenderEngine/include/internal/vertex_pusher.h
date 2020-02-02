#ifndef VERTEX_PUSHER_H
#define VERTEX_PUSHER_H

#include "gpu_buffer.h"
#include <unordered_map>
#include <functional>

using vertex_slot_t = uint;

struct IndexLayout {
	PushableType mType;
	BufferUsage::Frequency mFreq;

	ubyte getSize() const;
	BufferUsage getUsage() const;

	static size_t hash(const IndexLayout& i_object) {
		std::hash<size_t> hasher;
		return 
			hasher((size_t)i_object.mType.mValue) + 
			hasher((size_t)i_object.mFreq);
	}
};

struct VertexLayout {
	PushableType mType;
	ubyte mDepth;
	BufferUsage::Frequency mFreq;

	ubyte getSize() const;
	BufferUsage getUsage() const;

	static size_t hash(const VertexLayout& i_object) {
		std::hash<size_t> hasher;
		return 
			hasher((size_t)i_object.mType.mValue) +
			hasher((size_t)i_object.mDepth) +
			hasher((size_t)i_object.mFreq);
	}
};

struct VertexPusherLayout {
	IndexLayout mIndex;
	std::unordered_map<vertex_slot_t, VertexLayout> mVertex;
	Topology mTopology;

	static size_t hash(const VertexPusherLayout& i_object) {
		std::hash<size_t> hasher;
		size_t out =
			IndexLayout::hash(i_object.mIndex) +
			hasher((size_t)i_object.mTopology);
		for (auto it = i_object.mVertex.cbegin(); it != i_object.mVertex.cend(); it++) {
			out += hasher((size_t)it->first) + VertexLayout::hash(it->second);
		}
		return out;
	}
};

class VertexPusher {
	friend class RenderEngine;
	friend class VirtualVertexPusher;

public:
	struct RenderCommand {
		uint mNIndices;
		uint mNInstances;
		uint mIndexBegin;
		uint mVertexBegin;
		uint mInstanceBegin;

		RenderCommand() :
		mNIndices(0),
		mNInstances(1),
		mIndexBegin(0),
		mVertexBegin(0),
		mInstanceBegin(0)
		{}
	};

private:
	GLuint mArrayId;

	VertexPusherLayout mLayout;

	GpuBuffer mIndices;
	uint mMaxIndices;

	std::unordered_map<vertex_slot_t, GpuBuffer> mVertices;
	uint mMaxVertices;

	GpuBuffer mCommands;
	uint mMaxCommands;
	uint mNCommands;

public:
	~VertexPusher();

private:
	VertexPusher();
	VertexPusher(const VertexPusherLayout& i_layout, uint i_maxIndices, uint i_maxVertices, uint i_maxCommands);

	void init(const VertexPusherLayout& i_layout, uint i_maxIndices, uint i_maxVertices, uint i_maxCommands);
	void _init();
	void destroy();
	bool isValid() const;

	RenderCommand* newCommands(uint i_n);
	void commitCommands(RenderCommand* i_ptr);

	void finalize();
	void render();
};

#endif
