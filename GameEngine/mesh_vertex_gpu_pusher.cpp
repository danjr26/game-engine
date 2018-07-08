#include "mesh_vertex_gpu_pusher.h"
#include "game_engine.h"
/*
MeshVertexGPUPusher::MeshVertexGPUPusher() :
	data(nullptr),
	vertexArrayID(0),
	indexBufferID(0),
	memberBufferIDs(MemberIndex::count, 0),
	extraBufferIDs(),
	useCase(UseCase::changes_rarely) 
{}

MeshVertexGPUPusher::~MeshVertexGPUPusher() {
	for (uint i = 0; i < MemberIndex::count; i++) {
		if (memberBufferIDs[i] != 0) {
			glDeleteBuffers(1, &memberBufferIDs[i]);
		}
	}
	if(indexBufferID != 0) glDeleteBuffers(1, &indexBufferID);
	if(vertexArrayID != 0) glDeleteVertexArrays(1, &vertexArrayID);
}

void MeshVertexGPUPusher::Initialize(MeshVertexData* in_data, const std::vector<ExtraMeshVertexDatum>& in_extraData, UseCase in_useCase) {
	if (in_data == nullptr) {
		throw InvalidArgumentException("mesh vertex data was null");
	}

	const uint nVertices = in_data->Get_Number_Vertices();
	const uint nFaces = in_data->Get_Number_Faces();

	data = in_data;
	useCase = in_useCase;
	extraBufferIDs.resize(in_extraData.size(), 0);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	for (uint i = 0; i < MemberIndex::count; i++) {
		MemberIndex memberIndex = (MemberIndex)i;
		if (in_data->Has_Member(memberIndex)) {
			glGenBuffers(1, &memberBufferIDs[i]);
			glBindBuffer(GL_ARRAY_BUFFER, memberBufferIDs[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices * in_data->Get_Member_Depth(memberIndex), in_data->Get_Member_Pointer(memberIndex), (GLuint)useCase);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, in_data->Get_Member_Depth(memberIndex), GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		else {
			memberBufferIDs[i] = 0;
		}
	}

	for (uint i = 0; i < in_extraData.size(); i++) {
		glGenBuffers(1, &extraBufferIDs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, extraBufferIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, in_extraData[i].Get_Data_Type_Size() * nVertices * in_extraData[i].nComponents, in_extraData[i].data, (GLuint)useCase);
		glEnableVertexAttribArray(in_extraData[i].attributeIndex);
		glVertexAttribPointer(in_extraData[i].attributeIndex, in_extraData[i].nComponents, (GLenum)in_extraData[i].dataType, GL_FALSE, 0, nullptr);
	}

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * nFaces, in_data->Get_Index_Pointer(), (GLuint)useCase);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Push_All() {
	Push_Vertices();
	Push_Faces();
}

void MeshVertexGPUPusher::Push_Vertices() {
	Push_Vertices(0, data->Get_Number_Vertices());
}

void MeshVertexGPUPusher::Push_Vertices(uint in_index, uint in_nToPush) {
	for (uint i = 0; i < MemberIndex::count; i++) {
		Push_Vertex_Member((MemberIndex)i, in_index, in_nToPush);
	}
}

void MeshVertexGPUPusher::Push_Vertex_Member(MemberIndex in_member) {
	Push_Vertex_Member(in_member, 0, data->Get_Number_Vertices());
}

void MeshVertexGPUPusher::Push_Vertex_Member(MemberIndex in_member, uint in_index, uint in_nToPush) {
	glBindBuffer(GL_ARRAY_BUFFER, memberBufferIDs[in_member]);
	glBufferSubData(GL_ARRAY_BUFFER, in_index * sizeof(float), in_nToPush * sizeof(float), data->Get_Member_Pointer(in_member));
}

void MeshVertexGPUPusher::Push_Faces() {
	Push_Faces(0, data->Get_Number_Faces());
}

bool MeshVertexGPUPusher::Is_Initialized() const {
	return data != nullptr;
}

void MeshVertexGPUPusher::Draw() {
	if (!Is_Initialized()) {
		throw InvalidArgumentException("could not draw; data was null");
	}
	glDrawElements(GL_TRIANGLES, data->Get_Number_Vertices(), GL_UNSIGNED_SHORT, 0);
}
*/