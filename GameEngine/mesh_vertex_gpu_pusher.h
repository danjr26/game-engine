#ifndef MESH_VERTEX_GPU_PUSHER_H
#define MESH_VERTEX_GPU_PUSHER_H

#include "mesh_vertex_data.h"


class MeshVertexGPUPusher {
public:
	using DataType = MeshVertexData::DataType;

	enum class UseCase {
		changes_rarely,
		changes_often,
		temporary_use
	};

	struct ExtraParams {
		UseCase useCase;
		uint nVerticesToReserve;
		uint nFacesToReserve;
		ulong memberIndicesToIgnore;
		bool shouldPushData;

		ExtraParams();
	};

private:
	struct Member {
		ubyte id;
		DataType type;
		ubyte depth;
		GLuint bufferID;

		ubyte Get_Vertex_Size() const;
	};

	MeshVertexData* data;

	GLuint vertexArrayID;

	std::vector<Member> vertexMembers;
	Member indexMember;

	uint reservedVertices;
	uint usedVertices;

	uint reservedFaces;
	uint usedFaces;

	UseCase useCase;

	std::map<ubyte, ubyte> idToIndex;

public:
	MeshVertexGPUPusher();
	~MeshVertexGPUPusher();

	void Initialize(MeshVertexData* in_data, const ExtraParams& in_params);

	uint Get_Number_Vertices() const;
	uint Get_Number_Faces() const;

	void Reserve_Total(uint in_nVertices, uint in_nFaces);
	void Reserve_Additional(uint in_nVertices, uint in_nFaces);

	void Add_Member(uint in_id);
	void Remove_Member(uint in_id);
	bool Has_Member(uint in_id);

	void Push_Vertex_Count();
	void Push_Face_Count();

	void Push_Vertices();
	void Push_Vertices(uint in_start, uint in_length);
	void Push_Member(uint in_id);
	void Push_Member(uint in_id, uint in_start, uint in_length);
	void Push_Faces();
	void Push_Faces(uint in_start, uint in_length);

	bool Is_Initialized() const;

	void Draw();
	void Draw(uint in_start, uint in_length);
};

#endif