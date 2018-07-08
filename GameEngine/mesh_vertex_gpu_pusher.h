#ifndef MESH_VERTEX_GPU_PUSHER_H
#define MESH_VERTEX_GPU_PUSHER_H

#include "mesh_vertex_data.h"
#include "extra_mesh_vertex_datum.h"

class MeshVertexGPUPusher {
public:
	using MemberIndex = MeshVertexData::MemberIndex;
	enum class UseCase {
		changes_rarely,
		changes_often,
		temporary_use
	};

private:
	MeshVertexData* data;

	GLuint vertexArrayID;
	GLuint indexBufferID;

	std::vector<GLuint> memberBufferIDs;
	std::vector<GLuint> extraBufferIDs;

	UseCase useCase;

public:
	MeshVertexGPUPusher();
	~MeshVertexGPUPusher();

	void Initialize(MeshVertexData* in_data, const std::vector<ExtraMeshVertexDatum>& in_extraData = std::vector<ExtraMeshVertexDatum>(), UseCase in_useCase = UseCase::changes_rarely);
	void Push_All();
	void Push_Vertices();
	void Push_Vertices(uint in_index, uint in_nToPush);
	void Push_Vertex_Member(MemberIndex in_member);
	void Push_Vertex_Member(MemberIndex in_member, uint in_index, uint in_nToPush);
	void Push_Faces();
	void Push_Faces(uint in_index, uint in_nToPush);

	bool Is_Initialized() const;

	void Draw();
};

#endif