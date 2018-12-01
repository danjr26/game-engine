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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * nFaces, in_data->Get_Face_Pointer(), (GLuint)useCase);

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

MeshVertexGPUPusher::ExtraParams::ExtraParams() :
	useCase(UseCase::changes_rarely),
	nVerticesToReserve(0),
	nFacesToReserve(0),
	memberIndicesToIgnore(0)
{}

ubyte MeshVertexGPUPusher::Member::Get_Vertex_Size() const {
	return depth * MeshVertexData::Get_Data_Type_Size(type);
}

MeshVertexGPUPusher::MeshVertexGPUPusher() :
	data(nullptr),
	vertexArrayID(0),
	vertexMembers(),
	indexBufferID(0),
	reservedVertices(0),
	usedVertices(0),
	reservedFaces(0),
	usedFaces(0),
	useCase(UseCase::changes_rarely),
	idToIndex()
{}

MeshVertexGPUPusher::~MeshVertexGPUPusher() {
	if (vertexArrayID) glDeleteVertexArrays(1, &vertexArrayID);
	if (indexBufferID) glDeleteBuffers(1, &indexBufferID);
	for (uint i = 0; i < vertexMembers.size(); i++) {
		if (vertexMembers[i].bufferID) glDeleteBuffers(1, &vertexMembers[i].bufferID);
	}
}

void MeshVertexGPUPusher::Initialize(MeshVertexData* in_data, const ExtraParams& in_params) {
	if (Is_Initialized()) {
		throw InvalidArgumentException("gpu pusher already initialized");
	}

	if (in_data == nullptr) {
		throw InvalidArgumentException("mesh vertex data was null");
	}

	data = in_data;
	useCase = in_params.useCase;
	usedVertices = data->Get_Number_Vertices();
	usedFaces = data->Get_Number_Faces();
	reservedVertices = Max(in_params.nVerticesToReserve, usedVertices);
	reservedFaces = Max(in_params.nFacesToReserve, usedFaces);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	for (uint i = 0; i < data->Get_Number_Members(); i++) {
		if (!(in_params.memberIndicesToIgnore & 1ul << i)) {
			Add_Member(data->Get_Member_ID(i));
		}
	}

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, reservedFaces * data->Get_Face_Size(), nullptr, (GLenum)useCase);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, usedFaces * data->Get_Face_Size(), data->Get_Face_Pointer());

	glBindVertexArray(0);
}

uint MeshVertexGPUPusher::Get_Number_Vertices() const {
	return usedVertices;
}

uint MeshVertexGPUPusher::Get_Number_Faces() const {
	return usedFaces;
}

void MeshVertexGPUPusher::Reserve_Total(uint in_nVertices, uint in_nFaces) {
	glBindVertexArray(vertexArrayID);
	GLuint newBufferID = 0;
	if (in_nVertices > reservedVertices) {
		for (uint i = 0; i < vertexMembers.size(); i++) {
			glCreateBuffers(1, &newBufferID);
			glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
			glBufferData(GL_COPY_WRITE_BUFFER, in_nVertices * vertexMembers[i].Get_Vertex_Size(), nullptr, (GLuint)useCase);
			glBindBuffer(GL_COPY_READ_BUFFER, vertexMembers[i].bufferID);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, usedVertices * vertexMembers[i].Get_Vertex_Size());
			glDeleteBuffers(1, &vertexMembers[i].bufferID);
			vertexMembers[i].bufferID = newBufferID;
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glVertexAttribPointer(vertexMembers[i].id, vertexMembers[i].depth, vertexMembers[i].type, GL_FALSE, 0, nullptr);
		}
		reservedVertices = in_nVertices;
	}

	if (in_nFaces > reservedFaces) {
		glCreateBuffers(1, &newBufferID);
		glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
		glBufferData(GL_COPY_WRITE_BUFFER, in_nFaces * data->Get_Face_Size(), nullptr, (GLuint)useCase);
		glBindBuffer(GL_COPY_READ_BUFFER, indexBufferID);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, usedFaces * data->Get_Face_Size());
		glDeleteBuffers(1, &indexBufferID);
		indexBufferID = newBufferID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBufferID);
		reservedFaces = in_nFaces;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Reserve_Additional(uint in_nVertices, uint in_nFaces) {
	Reserve_Total(Get_Number_Vertices() + in_nVertices, Get_Number_Faces() + in_nFaces);
}

void MeshVertexGPUPusher::Add_Member(uint in_id) {
	if (!data->Has_Member(in_id)) {
		throw InvalidArgumentException("invalid vertex data member id");
	}

	if (!idToIndex.insert(std::pair<ubyte, ubyte>(in_id, (ubyte)vertexMembers.size())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	uint index = data->Get_Member_Index_By_ID(in_id);
	Member newMember = {
		in_id,
		data->Get_Member_Type(index),
		(ubyte)data->Get_Member_Depth(index),
		0
	};

	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &newMember.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newMember.bufferID);
	glBufferData(GL_ARRAY_BUFFER, reservedVertices * newMember.Get_Vertex_Size(), nullptr, (GLenum)useCase);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0, usedVertices * newMember.Get_Vertex_Size(),
		data->Get_Member_Pointer(data->Get_Member_Index_By_ID(in_id))
	);
	glEnableVertexAttribArray(newMember.id);
	glVertexAttribPointer(newMember.id, newMember.depth, newMember.type, GL_FALSE, 0, nullptr);

	idToIndex.insert(std::pair<ubyte, ubyte>(newMember.id, (ubyte)vertexMembers.size()));
	vertexMembers.push_back(newMember);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Remove_Member(uint in_id) {
	uint memberIndex = idToIndex.at(in_id);
	Member& member = vertexMembers[memberIndex];

	glBindVertexArray(vertexArrayID);
	glDisableVertexAttribArray(in_id);
	glDeleteBuffers(1, &member.bufferID);
	glBindVertexArray(0);

	idToIndex.erase(in_id);
	vertexMembers.erase(vertexMembers.begin() + memberIndex);

	for (uint i = memberIndex; i < vertexMembers.size(); i++) {
		idToIndex[vertexMembers[i].id] = i;
	}
}

bool MeshVertexGPUPusher::Has_Member(uint in_id) {
	return idToIndex.find(in_id) != idToIndex.end();
}

void MeshVertexGPUPusher::Push_Vertex_Count() {
	const uint nVertices = data->Get_Number_Vertices();
	if (nVertices > reservedVertices) {
		Reserve_Total(nVertices, 0);
	}
	usedVertices = nVertices;
}

void MeshVertexGPUPusher::Push_Face_Count() {
	const uint nFaces = data->Get_Number_Faces();
	if (nFaces > reservedFaces) {
		Reserve_Total(0, nFaces);
	}
	usedFaces = nFaces;
}

void MeshVertexGPUPusher::Push_Vertices() {
	Push_Vertices(0, usedVertices);
}

void MeshVertexGPUPusher::Push_Vertices(uint in_start, uint in_length) {
	for (uint i = 0; i < vertexMembers.size(); i++) {
		Push_Member(vertexMembers[i].id, in_start, in_length);
	}
}

void MeshVertexGPUPusher::Push_Member(uint in_id) {
	Push_Member(in_id, 0, usedVertices);
}

void MeshVertexGPUPusher::Push_Member(uint in_id, uint in_start, uint in_length) {
	if (in_start + in_length > usedVertices) {
		throw InvalidArgumentException("invalid vertex indices");
	}
	uint memberIndex = idToIndex.at(in_id);
	const Member& member = vertexMembers[memberIndex];
	const uint vertexSize = member.Get_Vertex_Size();
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, member.bufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		in_start * vertexSize, 
		in_length * vertexSize, 
		data->Get_Member_Pointer(data->Get_Member_Index_By_ID(member.id))
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Push_Faces() {
	Push_Faces(0, usedFaces);
}

void MeshVertexGPUPusher::Push_Faces(uint in_start, uint in_length) {
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		in_start * data->Get_Face_Size(),
		in_length * data->Get_Face_Size(),
		data->Get_Member_Pointer(data->Get_Member_Index_By_ID(indexBufferID))
	);
	glBindVertexArray(0);
}

bool MeshVertexGPUPusher::Is_Initialized() const {
	return data != nullptr;
}

void MeshVertexGPUPusher::Draw() {
	Draw(0, usedFaces);
}

void MeshVertexGPUPusher::Draw(uint in_start, uint in_length) {
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, in_length * 3, data->Get_Face_Type(), (const void*)(in_start * data->Get_Face_Size()));
	glBindVertexArray(0);
}
