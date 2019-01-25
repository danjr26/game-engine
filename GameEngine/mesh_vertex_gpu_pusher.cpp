#include "mesh_vertex_gpu_pusher.h"
#include "game_engine.h"

MeshVertexGPUPusher::ExtraParams::ExtraParams() :
	useCase(UseCase::changes_rarely),
	nVerticesToReserve(0),
	nFacesToReserve(0),
	membersToIgnore(0ul)
{}

ubyte MeshVertexGPUPusher::Member::Get_Vertex_Size() const {
	return depth * MeshVertexData::Get_Data_Type_Size(type);
}

MeshVertexGPUPusher::MeshVertexGPUPusher() :
	data(nullptr),
	vertexArrayID(0),
	members(),
	indexBufferID(0),
	usedVertices(0),
	usedFaceElements(0),
	reservedVertices(0),
	reservedFaceElements(0),
	useCase(UseCase::changes_rarely)
{}

MeshVertexGPUPusher::~MeshVertexGPUPusher() {
	if (vertexArrayID) glDeleteVertexArrays(1, &vertexArrayID);
	if (indexBufferID) glDeleteBuffers(1, &indexBufferID);
	for (uint i = 0; i < members.size(); i++) {
		if (members[i].bufferID) glDeleteBuffers(1, &members[i].bufferID);
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
	usedFaceElements = data->Get_Number_Face_Elements();
	reservedVertices = Max(in_params.nVerticesToReserve, usedVertices);
	reservedFaceElements = Max(in_params.nFacesToReserve, usedFaceElements);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	std::vector<ubyte> ids;
	data->Get_Member_IDs(ids);

	for (auto it = ids.begin(); it < ids.end(); it++) {
		if (!(in_params.membersToIgnore & 1ul << (*it))) {
			Add_Member(*it);
		}
	}

	uint faceElementSize = data->Get_Face_Element_Size();

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, reservedFaceElements * faceElementSize, nullptr, (GLenum)useCase);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, usedFaceElements * faceElementSize, data->Get_Face_Pointer());

	glBindVertexArray(0);
}

MeshVertexData* MeshVertexGPUPusher::Get_Data() {
	return data;
}

uint MeshVertexGPUPusher::Get_Number_Vertices() const {
	return usedVertices;
}

uint MeshVertexGPUPusher::Get_Number_Faces() const {
	return data->Element_To_Face_Count(0, usedFaceElements);
}

uint MeshVertexGPUPusher::Get_Number_Face_Elements() const {
	return usedFaceElements;
}

void MeshVertexGPUPusher::Reserve_Total_Vertices(uint in_nVertices) {
	glBindVertexArray(vertexArrayID);
	GLuint newBufferID = 0;
	if (in_nVertices > reservedVertices) {
		for (auto it = members.begin(); it != members.end(); it++) {
			glCreateBuffers(1, &newBufferID);
			glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
			glBufferData(GL_COPY_WRITE_BUFFER, in_nVertices * it->second.Get_Vertex_Size(), nullptr, (GLuint)useCase);
			glBindBuffer(GL_COPY_READ_BUFFER, it->second.bufferID);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, usedVertices * it->second.Get_Vertex_Size());
			glDeleteBuffers(1, &it->second.bufferID);
			it->second.bufferID = newBufferID;
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glVertexAttribPointer(it->first, it->second.depth, it->second.type, GL_FALSE, 0, nullptr);
		}
		reservedVertices = in_nVertices;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Reserve_Total_Faces(uint in_nFaces) {
	Reserve_Total_Face_Elements(data->Face_To_Element_Count(0, in_nFaces));
}

void MeshVertexGPUPusher::Reserve_Total_Face_Elements(uint in_nElements) {
	uint faceElementSize = data->Get_Face_Element_Size();

	glBindVertexArray(vertexArrayID);
	GLuint newBufferID = 0;
	if (in_nElements > reservedFaceElements) {
		glCreateBuffers(1, &newBufferID);
		glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
		glBufferData(GL_COPY_WRITE_BUFFER, in_nElements * faceElementSize, nullptr, (GLuint)useCase);
		glBindBuffer(GL_COPY_READ_BUFFER, indexBufferID);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, usedFaceElements * faceElementSize);
		glDeleteBuffers(1, &indexBufferID);
		indexBufferID = newBufferID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBufferID);
		reservedFaceElements = in_nElements;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Add_Member(uint in_id) {
	if (!data->Has_Member(in_id)) {
		throw InvalidArgumentException("invalid vertex data member id");
	}

	if (!members.insert(std::pair<uint, Member>(in_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	Member& newMember = members[in_id];
	newMember.type = data->Get_Member_Type(in_id);
	newMember.depth = data->Get_Member_Depth(in_id);

	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &newMember.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newMember.bufferID);
	glBufferData(GL_ARRAY_BUFFER, reservedVertices * newMember.Get_Vertex_Size(), nullptr, (GLenum)useCase);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0, usedVertices * newMember.Get_Vertex_Size(),
		data->Get_Member_Pointer(in_id)
	);
	glEnableVertexAttribArray(in_id);
	glVertexAttribPointer(in_id, newMember.depth, newMember.type, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Remove_Member(uint in_id) {
	Member& member = members[in_id];

	glBindVertexArray(vertexArrayID);
	glDisableVertexAttribArray(in_id);
	glDeleteBuffers(1, &member.bufferID);
	glBindVertexArray(0);

	members.erase(in_id);
}

bool MeshVertexGPUPusher::Has_Member(uint in_id) {
	return members.find(in_id) != members.end();
}

void MeshVertexGPUPusher::Push_Vertex_Count() {
	const uint nVertices = data->Get_Number_Vertices();
	if (nVertices > reservedVertices) {
		Reserve_Total_Vertices(nVertices);
	}
	usedVertices = nVertices;
}

void MeshVertexGPUPusher::Push_Face_Count() {
	const uint nFaceElements = data->Get_Number_Face_Elements();
	if (nFaceElements > reservedFaceElements) {
		Reserve_Total_Face_Elements(nFaceElements);
	}
	usedFaceElements = nFaceElements;
}

void MeshVertexGPUPusher::Push_Vertices() {
	Push_Vertices(0, usedVertices);
}

void MeshVertexGPUPusher::Push_Vertices(uint in_start, uint in_length) {
	for (auto it = members.begin(); it != members.end(); it++) {
		Push_Member(it->first, in_start, in_length);
	}
}

void MeshVertexGPUPusher::Push_Member(uint in_id) {
	Push_Member(in_id, 0, usedVertices);
}

void MeshVertexGPUPusher::Push_Member(uint in_id, uint in_start, uint in_length) {
	if (in_start + in_length > usedVertices) {
		throw InvalidArgumentException("invalid vertex indices");
	}
	const Member& member = members[in_id];
	const uint vertexSize = member.Get_Vertex_Size();
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, member.bufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		in_start * vertexSize, 
		in_length * vertexSize, 
		data->Get_Member_Pointer(in_id)
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Push_Faces() {
	Push_Face_Elements();
}

void MeshVertexGPUPusher::Push_Faces(uint in_start, uint in_length) {
	Push_Face_Elements(data->Face_To_Element_Index(in_start), data->Face_To_Element_Count(0, in_length));
}

void MeshVertexGPUPusher::Push_Face_Elements() {
	Push_Face_Elements(0, usedFaceElements);
}

void MeshVertexGPUPusher::Push_Face_Elements(uint in_start, uint in_length) {
	uint faceElementSize = data->Get_Face_Element_Size();

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		in_start * faceElementSize,
		in_length * faceElementSize,
		data->Get_Member_Pointer(indexBufferID)
	);
	glBindVertexArray(0);
}

bool MeshVertexGPUPusher::Is_Initialized() const {
	return data != nullptr;
}

void MeshVertexGPUPusher::Draw() {
	Draw(0, usedFaceElements);
}

void MeshVertexGPUPusher::Draw(uint in_elementIndex, uint in_nElements) {
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(data->Get_Face_Mode(), in_nElements, data->Get_Face_Type(), (const void*)(in_elementIndex * data->Get_Face_Element_Size()));
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Draw_Raw() {
	Draw_Raw(0, usedFaceElements);
}

void MeshVertexGPUPusher::Draw_Raw(uint in_elementIndex, uint in_nElements) {
	glBindVertexArray(vertexArrayID);
	glDrawArrays(data->Get_Face_Mode(), in_elementIndex, in_nElements);
	glBindVertexArray(0);
}
