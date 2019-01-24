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
		UseCase useCase;
		uint nVerticesToReserve;
		uint nFacesToReserve;
		ulong membersToIgnore;

		ExtraParams();
	};

private:
	struct Member {
		DataType type;
		ubyte depth;
		GLuint bufferID;

		ubyte Get_Vertex_Size() const;
	};

	MeshVertexData* data;

	GLuint vertexArrayID;

	std::unordered_map<uint, Member> members;
	GLuint indexBufferID;

	uint usedVertices;
	uint usedFaceElements;

	uint reservedVertices;
	uint reservedFaceElements;

	UseCase useCase;

public:
	MeshVertexGPUPusher();
	~MeshVertexGPUPusher();

	void Initialize(MeshVertexData* in_data, const ExtraParams& in_params = ExtraParams());

	MeshVertexData* Get_Data();

	uint Get_Number_Vertices() const;
	uint Get_Number_Faces() const;
	uint Get_Number_Face_Elements() const;

	void Reserve_Total_Vertices(uint in_nVertices);
	void Reserve_Total_Faces(uint in_nFaces);
	void Reserve_Total_Face_Elements(uint in_nElements);

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
	void Push_Face_Elements();
	void Push_Face_Elements(uint in_start, uint in_length);

	bool Is_Initialized() const;

	void Draw();
	void Draw(uint in_elementIndex, uint in_nElements);

	void Draw_Raw();
	void Draw_Raw(uint in_elementIndex, uint in_nElements);
};

#endif