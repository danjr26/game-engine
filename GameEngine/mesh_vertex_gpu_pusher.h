#ifndef MESH_VERTEX_GPU_PUSHER_H
#define MESH_VERTEX_GPU_PUSHER_H

#include "mesh_vertex_data.h"


class MeshVertexGPUPusher {
public:
	using DataType = MeshVertexData::DataType;

	enum class UseCase {
		changes_rarely = GL_STATIC_DRAW,
		changes_often = GL_DYNAMIC_DRAW,
		temporary_use = GL_STREAM_DRAW
	};

	struct ExtraParams {
		UseCase mUseCase;
		uint mNVerticesToReserve;
		uint mNFacesToReserve;
		ulong mMembersToIgnore;

		ExtraParams();
	};

private:
	struct Member {
		DataType mType;
		ubyte mDepth;
		GLuint mBufferID;

		ubyte getVertexSize() const;
	};

	MeshVertexData* mData;

	GLuint mVertexArrayID;

	std::unordered_map<uint, Member> mMembers;
	GLuint mIndexBufferID;

	uint mUsedVertices;
	uint mUsedFaceElements;

	uint mReservedVertices;
	uint mReservedFaceElements;

	UseCase mUseCase;

public:
	MeshVertexGPUPusher();
	~MeshVertexGPUPusher();

	void initialize(MeshVertexData* in_data, const ExtraParams& in_params = ExtraParams());

	MeshVertexData* getData();

	uint getNumberVertices() const;
	uint getNumberFaces() const;
	uint getNumberFaceElements() const;

	void reserveTotalVertices(uint in_nVertices);
	void reserveTotalFaces(uint in_nFaces);
	void reserveTotalFaceElements(uint in_nElements);

	void addMember(uint in_id);
	void removeMember(uint in_id);
	bool hasMember(uint in_id);

	void pushVertexCount();
	void pushFaceCount();

	void pushVertices();
	void pushVertices(uint in_start, uint in_length);
	void pushMember(uint in_id);
	void pushMember(uint in_id, uint in_start, uint in_length);
	void pushFaces();
	void pushFaces(uint in_start, uint in_length);
	void pushFaceElements();
	void pushFaceElements(uint in_start, uint in_length);

	bool isInitialized() const;

	void draw();
	void draw(uint in_elementIndex, uint in_nElements);

	void drawRaw();
	void drawRaw(uint in_elementIndex, uint in_nElements);
};

#endif