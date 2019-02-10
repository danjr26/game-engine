#include "mesh_vertex_gpu_pusher.h"
#include "game_engine.h"

MeshVertexGPUPusher::ExtraParams::ExtraParams() :
	mUseCase(UseCase::changes_rarely),
	mNVerticesToReserve(0),
	mNFacesToReserve(0),
	mMembersToIgnore(0ul)
{}

ubyte MeshVertexGPUPusher::Member::Get_Vertex_Size() const {
	return mDepth * MeshVertexData::Get_Data_Type_Size(mType);
}

MeshVertexGPUPusher::MeshVertexGPUPusher() :
	mData(nullptr),
	mVertexArrayID(0),
	mMembers(),
	mIndexBufferID(0),
	mUsedVertices(0),
	mUsedFaceElements(0),
	mReservedVertices(0),
	mReservedFaceElements(0),
	mUseCase(UseCase::changes_rarely)
{}

MeshVertexGPUPusher::~MeshVertexGPUPusher() {
	if (mVertexArrayID) glDeleteVertexArrays(1, &mVertexArrayID);
	if (mIndexBufferID) glDeleteBuffers(1, &mIndexBufferID);
	for (uint i = 0; i < mMembers.size(); i++) {
		if (mMembers[i].mBufferID) glDeleteBuffers(1, &mMembers[i].mBufferID);
	}
}

void MeshVertexGPUPusher::Initialize(MeshVertexData* in_data, const ExtraParams& in_params) {
	if (Is_Initialized()) {
		throw InvalidArgumentException("gpu pusher already initialized");
	}

	if (in_data == nullptr) {
		throw InvalidArgumentException("mesh vertex data was null");
	}

	mData = in_data;
	mUseCase = in_params.mUseCase;
	mUsedVertices = mData->Get_Number_Vertices();
	mUsedFaceElements = mData->Get_Number_Face_Elements();
	mReservedVertices = Max(in_params.mNVerticesToReserve, mUsedVertices);
	mReservedFaceElements = Max(in_params.mNFacesToReserve, mUsedFaceElements);

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	std::vector<ubyte> ids;
	mData->Get_Member_IDs(ids);

	for (auto it = ids.begin(); it < ids.end(); it++) {
		if (!(in_params.mMembersToIgnore & 1ul << (*it))) {
			Add_Member(*it);
		}
	}

	uint faceElementSize = mData->Get_Face_Element_Size();

	glGenBuffers(1, &mIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mReservedFaceElements * faceElementSize, nullptr, (GLenum)mUseCase);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mUsedFaceElements * faceElementSize, mData->Get_Face_Pointer());

	glBindVertexArray(0);
}

MeshVertexData* MeshVertexGPUPusher::Get_Data() {
	return mData;
}

uint MeshVertexGPUPusher::Get_Number_Vertices() const {
	return mUsedVertices;
}

uint MeshVertexGPUPusher::Get_Number_Faces() const {
	return mData->Element_To_Face_Count(0, mUsedFaceElements);
}

uint MeshVertexGPUPusher::Get_Number_Face_Elements() const {
	return mUsedFaceElements;
}

void MeshVertexGPUPusher::Reserve_Total_Vertices(uint in_nVertices) {
	glBindVertexArray(mVertexArrayID);
	GLuint newBufferID = 0;
	if (in_nVertices > mReservedVertices) {
		for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
			glCreateBuffers(1, &newBufferID);
			if (mReservedVertices != 0) {
				glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
				glBufferData(GL_COPY_WRITE_BUFFER, in_nVertices * it->second.Get_Vertex_Size(), nullptr, (GLuint)mUseCase);
				glBindBuffer(GL_COPY_READ_BUFFER, it->second.mBufferID);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mUsedVertices * it->second.Get_Vertex_Size());
			}
			glDeleteBuffers(1, &it->second.mBufferID);
			it->second.mBufferID = newBufferID;
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glVertexAttribPointer(it->first, it->second.mDepth, it->second.mType, GL_FALSE, 0, nullptr);
		}
		mReservedVertices = in_nVertices;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Reserve_Total_Faces(uint in_nFaces) {
	Reserve_Total_Face_Elements(mData->Face_To_Element_Count(0, in_nFaces));
}

void MeshVertexGPUPusher::Reserve_Total_Face_Elements(uint in_nElements) {
	uint faceElementSize = mData->Get_Face_Element_Size();

	glBindVertexArray(mVertexArrayID);
	GLuint newBufferID = 0;
	if (in_nElements > mReservedFaceElements) {
		glCreateBuffers(1, &newBufferID);
		if (mReservedFaceElements != 0) {
			glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
			glBufferData(GL_COPY_WRITE_BUFFER, in_nElements * faceElementSize, nullptr, (GLuint)mUseCase);
			glBindBuffer(GL_COPY_READ_BUFFER, mIndexBufferID);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mUsedFaceElements * faceElementSize);
		}
		glDeleteBuffers(1, &mIndexBufferID);
		mIndexBufferID = newBufferID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBufferID);
		mReservedFaceElements = in_nElements;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Add_Member(uint in_id) {
	if (!mData->Has_Member(in_id)) {
		throw InvalidArgumentException("invalid vertex data member id");
	}

	if (!mMembers.insert(std::pair<uint, Member>(in_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	Member& newMember = mMembers[in_id];
	newMember.mType = mData->Get_Member_Type(in_id);
	newMember.mDepth = mData->Get_Member_Depth(in_id);

	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &newMember.mBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newMember.mBufferID);
	glBufferData(GL_ARRAY_BUFFER, mReservedVertices * newMember.Get_Vertex_Size(), nullptr, (GLenum)mUseCase);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0, mUsedVertices * newMember.Get_Vertex_Size(),
		mData->Get_Member_Pointer(in_id)
	);
	glEnableVertexAttribArray(in_id);
	glVertexAttribPointer(in_id, newMember.mDepth, newMember.mType, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Remove_Member(uint in_id) {
	Member& member = mMembers[in_id];

	glBindVertexArray(mVertexArrayID);
	glDisableVertexAttribArray(in_id);
	glDeleteBuffers(1, &member.mBufferID);
	glBindVertexArray(0);

	mMembers.erase(in_id);
}

bool MeshVertexGPUPusher::Has_Member(uint in_id) {
	return mMembers.find(in_id) != mMembers.end();
}

void MeshVertexGPUPusher::Push_Vertex_Count() {
	const uint nVertices = mData->Get_Number_Vertices();
	if (nVertices > mReservedVertices) {
		Reserve_Total_Vertices(nVertices);
	}
	mUsedVertices = nVertices;
}

void MeshVertexGPUPusher::Push_Face_Count() {
	const uint nFaceElements = mData->Get_Number_Face_Elements();
	if (nFaceElements > mReservedFaceElements) {
		Reserve_Total_Face_Elements(nFaceElements);
	}
	mUsedFaceElements = nFaceElements;
}

void MeshVertexGPUPusher::Push_Vertices() {
	Push_Vertices(0, mUsedVertices);
}

void MeshVertexGPUPusher::Push_Vertices(uint in_start, uint in_length) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Push_Member(it->first, in_start, in_length);
	}
}

void MeshVertexGPUPusher::Push_Member(uint in_id) {
	Push_Member(in_id, 0, mUsedVertices);
}

void MeshVertexGPUPusher::Push_Member(uint in_id, uint in_start, uint in_length) {
	if (in_start + in_length > mUsedVertices) {
		throw InvalidArgumentException("invalid vertex indices");
	}
	const Member& member = mMembers[in_id];
	const uint vertexSize = member.Get_Vertex_Size();
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, member.mBufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		in_start * vertexSize, 
		in_length * vertexSize, 
		mData->Get_Member_Pointer(in_id)
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Push_Faces() {
	Push_Face_Elements();
}

void MeshVertexGPUPusher::Push_Faces(uint in_start, uint in_length) {
	Push_Face_Elements(mData->Face_To_Element_Index(in_start), mData->Face_To_Element_Count(0, in_length));
}

void MeshVertexGPUPusher::Push_Face_Elements() {
	Push_Face_Elements(0, mUsedFaceElements);
}

void MeshVertexGPUPusher::Push_Face_Elements(uint in_start, uint in_length) {
	uint faceElementSize = mData->Get_Face_Element_Size();

	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		in_start * faceElementSize,
		in_length * faceElementSize,
		mData->Get_Face_Pointer()
	);
	glBindVertexArray(0);
}

bool MeshVertexGPUPusher::Is_Initialized() const {
	return mData != nullptr;
}

void MeshVertexGPUPusher::Draw() {
	Draw(0, mUsedFaceElements);
}

void MeshVertexGPUPusher::Draw(uint in_elementIndex, uint in_nElements) {
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glDrawElements(mData->Get_Face_Mode(), in_nElements, mData->Get_Face_Type(), (const void*)(in_elementIndex * mData->Get_Face_Element_Size()));
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::Draw_Raw() {
	Draw_Raw(0, mUsedVertices);
}

void MeshVertexGPUPusher::Draw_Raw(uint in_elementIndex, uint in_nElements) {
	glBindVertexArray(mVertexArrayID);
	glDrawArrays(mData->Get_Face_Mode(), in_elementIndex, in_nElements);
	glBindVertexArray(0);
}
