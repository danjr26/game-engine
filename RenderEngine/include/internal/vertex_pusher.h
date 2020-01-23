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
friend class VirtualVertexPusher;
private:
	struct RenderCommand {
		uint mNIndices;
		uint mNInstances;
		uint mIndexBegin;
		uint mVertexBegin;
		uint mInstanceBegin;

		RenderCommand() :
		mNIndices(0),
		mNInstances(0),
		mIndexBegin(0),
		mVertexBegin(0),
		mInstanceBegin(0)
		{}
	};

private:
	GLuint mArrayId;

	VertexPusherLayout mLayout;

	GpuBuffer mIndices;
	size_t mMaxIndices;
	size_t mNIndices;

	std::unordered_map<vertex_slot_t, GpuBuffer> mVertices;
	size_t mMaxVertices;
	size_t mNVertices;

	GpuBuffer mCommands;
	size_t mMaxCommands;
	size_t mNCommands;

public:
	VertexPusher();
	VertexPusher(const VertexPusherLayout& i_layout, size_t i_maxIndices, size_t i_maxVertices, size_t i_maxCommands);
	~VertexPusher();

	void init(const VertexPusherLayout& i_layout, size_t i_maxIndices, size_t i_maxVertices, size_t i_maxCommands);
	void destroy();
	bool isValid() const;

	void getLayout(VertexPusherLayout& o_layout);

	void render();

private:
	void _init();
};

#endif
