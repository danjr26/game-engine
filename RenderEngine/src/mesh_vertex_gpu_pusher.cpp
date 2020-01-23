#include "../include/internal/mesh_vertex_gpu_pusher.h"

MeshVertexGPUPusher::ExtraParams::ExtraParams() :
	mUseCase(UseCase::changes_rarely),
	mNVerticesToReserve(0),
	mNFacesToReserve(0),
	mMembersToIgnore(0ul)
{}

ubyte MeshVertexGPUPusher::Member::getVertexSize() const {
	return mDepth * MeshVertexData::getDataTypeSize(mType);
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
	if (!_CrtCheckMemory()) {
		Log::main("err");
	}
	if (mVertexArrayID) glDeleteVertexArrays(1, &mVertexArrayID);
	if (mIndexBufferID) glDeleteBuffers(1, &mIndexBufferID);
	for (uint i = 0; i < mMembers.size(); i++) {
		if (mMembers[i].mBufferID) glDeleteBuffers(1, &mMembers[i].mBufferID);
	}
}

void MeshVertexGPUPusher::initialize(MeshVertexData* i_data, const ExtraParams& i_params) {
	if (isInitialized()) {
		throw InvalidArgumentException("gpu pusher already initialized");
	}

	if (i_data == nullptr) {
		throw InvalidArgumentException("mesh vertex data was null");
	}

	mData = i_data;
	mUseCase = i_params.mUseCase;
	mUsedVertices = mData->getNumberVertices();
	mUsedFaceElements = mData->getNumberFaceElements();
	mReservedVertices = geutil::max(i_params.mNVerticesToReserve, mUsedVertices);
	mReservedFaceElements = geutil::max(i_params.mNFacesToReserve, mUsedFaceElements);

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	std::vector<ubyte> ids;
	mData->getMemberIDs(ids);

	for (auto it = ids.begin(); it < ids.end(); it++) {
		if (!(i_params.mMembersToIgnore & 1ul << (*it))) {
			addMember(*it);
		}
	}

	uint faceElementSize = mData->getFaceElementSize();

	glGenBuffers(1, &mIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mReservedFaceElements * faceElementSize, nullptr, (GLenum)mUseCase);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mUsedFaceElements * faceElementSize, mData->getFacePointer());

	glBindVertexArray(0);
}

MeshVertexData* MeshVertexGPUPusher::getData() {
	return mData;
}

uint MeshVertexGPUPusher::getNumberVertices() const {
	return mUsedVertices;
}

uint MeshVertexGPUPusher::getNumberFaces() const {
	return mData->elementToFaceCount(0, mUsedFaceElements);
}

uint MeshVertexGPUPusher::getNumberFaceElements() const {
	return mUsedFaceElements;
}

void MeshVertexGPUPusher::reserveTotalVertices(uint i_nVertices) {
	glBindVertexArray(mVertexArrayID);
	GLuint newBufferID = 0;
	if (i_nVertices > mReservedVertices) {
		for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
			glCreateBuffers(1, &newBufferID);
			if (mReservedVertices != 0) {
				glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
				glBufferData(GL_COPY_WRITE_BUFFER, i_nVertices * it->second.getVertexSize(), nullptr, (GLuint)mUseCase);
				glBindBuffer(GL_COPY_READ_BUFFER, it->second.mBufferID);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mUsedVertices * it->second.getVertexSize());
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
				glBufferData(GL_ARRAY_BUFFER, i_nVertices * it->second.getVertexSize(), nullptr, (GLuint)mUseCase);
			}
			glDeleteBuffers(1, &it->second.mBufferID);
			it->second.mBufferID = newBufferID;
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glVertexAttribPointer(it->first, it->second.mDepth, it->second.mType, GL_FALSE, 0, nullptr);
		}
		mReservedVertices = i_nVertices;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::reserveTotalFaces(uint i_nFaces) {
	reserveTotalFaceElements(mData->faceToElementCount(0, i_nFaces));
}

void MeshVertexGPUPusher::reserveTotalFaceElements(uint i_nElements) {
	uint faceElementSize = mData->getFaceElementSize();

	glBindVertexArray(mVertexArrayID);
	GLuint newBufferID = 0;
	if (i_nElements > mReservedFaceElements) {
		glCreateBuffers(1, &newBufferID);
		if (mReservedFaceElements != 0) {
			glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
			glBufferData(GL_COPY_WRITE_BUFFER, i_nElements * faceElementSize, nullptr, (GLuint)mUseCase);
			glBindBuffer(GL_COPY_READ_BUFFER, mIndexBufferID);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mUsedFaceElements * faceElementSize);
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glBufferData(GL_ARRAY_BUFFER, i_nElements * faceElementSize, nullptr, (GLuint)mUseCase);
		}
		glDeleteBuffers(1, &mIndexBufferID);
		mIndexBufferID = newBufferID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBufferID);
		mReservedFaceElements = i_nElements;
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::addMember(uint i_id) {
	if (!mData->hasMember(i_id)) {
		throw InvalidArgumentException("invalid vertex data member id");
	}

	if (!mMembers.insert(std::pair<uint, Member>(i_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	Member& newMember = mMembers[i_id];
	newMember.mType = mData->getMemberType(i_id);
	newMember.mDepth = mData->getMemberDepth(i_id);
	newMember.mDefaultValue = nullptr;

	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &newMember.mBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newMember.mBufferID);
	glBufferData(GL_ARRAY_BUFFER, mReservedVertices * newMember.getVertexSize(), nullptr, (GLenum)mUseCase);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0, mUsedVertices * newMember.getVertexSize(),
		mData->getMemberPointer(i_id)
	);
	glEnableVertexAttribArray(i_id);
	glVertexAttribPointer(i_id, newMember.mDepth, newMember.mType, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::removeMember(uint i_id) {
	Member& member = mMembers[i_id];

	glBindVertexArray(mVertexArrayID);
	glDisableVertexAttribArray(i_id);
	glDeleteBuffers(1, &member.mBufferID);
	glBindVertexArray(0);

	if (member.mDefaultValue) delete[] member.mDefaultValue;

	mMembers.erase(i_id);
}

bool MeshVertexGPUPusher::hasMember(uint i_id) {
	return mMembers.find(i_id) != mMembers.end();
}

void MeshVertexGPUPusher::setMemberDefault(uint i_id, const void* i_value, bool i_realInts) {
	Member& member = mMembers[i_id];
	if (i_value == nullptr) throw InvalidArgumentException();
	if (member.mDefaultValue != nullptr) {
		delete[] member.mDefaultValue;
	}
	member.mDefaultValue = new ubyte[member.getVertexSize()];
	memcpy(member.mDefaultValue, i_value, member.getVertexSize());
	member.mRealInts = i_realInts;
	glBindVertexArray(mVertexArrayID);
	glDisableVertexAttribArray(i_id);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::clearMemberDefault(uint i_id) {
	Member& member = mMembers[i_id];
	if (member.mDefaultValue != nullptr) {
		delete[] member.mDefaultValue;
		member.mDefaultValue = nullptr;
	}
	glBindVertexArray(mVertexArrayID);
	glEnableVertexAttribArray(i_id);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::pushDefault(uint i_id) {
	Member& member = mMembers[i_id];
	if (member.mDefaultValue == nullptr) return;
	const void* value = member.mDefaultValue;
	switch (member.mType) {
	case DataType::_byte:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4bv(i_id, (GLbyte*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nbv(i_id, (GLbyte*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_ubyte:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4ubv(i_id, (GLubyte*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nubv(i_id, (GLubyte*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_short:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4sv(i_id, (GLshort*)value);
			else throw InvalidArgumentException();
		}
		else {
			switch (member.mDepth) {
			case 1: glVertexAttrib1sv(i_id, (GLshort*)value); break;
			case 2: glVertexAttrib2sv(i_id, (GLshort*)value); break;
			case 3: glVertexAttrib3sv(i_id, (GLshort*)value); break;
			case 4: glVertexAttrib4sv(i_id, (GLshort*)value); break;
			default: throw InvalidArgumentException();
			}
		}
		break;
	case DataType::_ushort:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4usv(i_id, (GLushort*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nusv(i_id, (GLushort*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_int:
		switch (member.mDepth) {
		case 1: glVertexAttribI1iv(i_id, (GLint*)value); break;
		case 2: glVertexAttribI2iv(i_id, (GLint*)value); break;
		case 3: glVertexAttribI3iv(i_id, (GLint*)value); break;
		case 4: glVertexAttribI4iv(i_id, (GLint*)value); break;
		}
		break;
	case DataType::_uint:
		if (member.mRealInts) {
			switch (member.mDepth) {
			case 1: glVertexAttribI1uiv(i_id, (GLuint*)value); break;
			case 2: glVertexAttribI2uiv(i_id, (GLuint*)value); break;
			case 3: glVertexAttribI3uiv(i_id, (GLuint*)value); break;
			case 4: glVertexAttribI4uiv(i_id, (GLuint*)value); break;
			}
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nuiv(i_id, (GLuint*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_float:
		switch (member.mDepth) {
		case 1: glVertexAttrib1fv(i_id, (GLfloat*)value); break;
		case 2: glVertexAttrib2fv(i_id, (GLfloat*)value); break;
		case 3: glVertexAttrib3fv(i_id, (GLfloat*)value); break;
		case 4: glVertexAttrib4fv(i_id, (GLfloat*)value); break;
		default: throw InvalidArgumentException();
		}
		break;
	case DataType::_double:
		switch (member.mDepth) {
		case 1: glVertexAttrib1dv(i_id, (GLdouble*)value); break;
		case 2: glVertexAttrib2dv(i_id, (GLdouble*)value); break;
		case 3: glVertexAttrib3dv(i_id, (GLdouble*)value); break;
		case 4: glVertexAttrib4dv(i_id, (GLdouble*)value); break;
		default: throw InvalidArgumentException();
		}
		break;
	}
}

void MeshVertexGPUPusher::pushDefaults() {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		pushDefault(it->first);
	}
}

void MeshVertexGPUPusher::pushVertexCount() {
	const uint nVertices = mData->getNumberVertices();
	if (nVertices > mReservedVertices) {
		reserveTotalVertices(nVertices);
	}
	mUsedVertices = nVertices;
}

void MeshVertexGPUPusher::pushFaceCount() {
	const uint nFaceElements = mData->getNumberFaceElements();
	if (nFaceElements > mReservedFaceElements) {
		reserveTotalFaceElements(nFaceElements);
	}
	mUsedFaceElements = nFaceElements;
}

void MeshVertexGPUPusher::pushVertices() {
	pushVertices(0, mUsedVertices);
}

void MeshVertexGPUPusher::pushVertices(uint i_start, uint i_length) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		pushMember(it->first, i_start, i_length);
	}
}

void MeshVertexGPUPusher::pushMember(uint i_id) {
	pushMember(i_id, 0, mUsedVertices);
}

void MeshVertexGPUPusher::pushMember(uint i_id, uint i_start, uint i_length) {
	if (i_start + i_length > mUsedVertices) {
		throw InvalidArgumentException("invalid vertex indices");
	}
	const Member& member = mMembers[i_id];
	const uint vertexSize = member.getVertexSize();
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, member.mBufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		i_start * vertexSize, 
		i_length * vertexSize, 
		mData->getMemberPointer(i_id)
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::pushFaces() {
	pushFaceElements();
}

void MeshVertexGPUPusher::pushFaces(uint i_start, uint i_length) {
	pushFaceElements(mData->faceToElementIndex(i_start), mData->faceToElementCount(0, i_length));
}

void MeshVertexGPUPusher::pushFaceElements() {
	pushFaceElements(0, mUsedFaceElements);
}

void MeshVertexGPUPusher::pushFaceElements(uint i_start, uint i_length) {
	uint faceElementSize = mData->getFaceElementSize();

	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		i_start * faceElementSize,
		i_length * faceElementSize,
		mData->getFacePointer()
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::pushAll() {
	pushVertexCount();
	pushFaceCount();
	pushVertices();
	pushFaces();
}

bool MeshVertexGPUPusher::isInitialized() const {
	return mData != nullptr;
}

void MeshVertexGPUPusher::draw() {
	draw(0, mUsedFaceElements);
}

void MeshVertexGPUPusher::draw(uint i_elementIndex, uint i_nElements) {
	pushDefaults();
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glDrawElements(mData->getFaceMode(), i_nElements, mData->setFaceType(), (const void*)(i_elementIndex * mData->getFaceElementSize()));
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::drawRaw() {
	drawRaw(0, mUsedVertices);
}

void MeshVertexGPUPusher::drawRaw(uint i_elementIndex, uint i_nElements) {
	pushDefaults();
	glBindVertexArray(mVertexArrayID);
	glDrawArrays(mData->getFaceMode(), i_elementIndex, i_nElements);
	glBindVertexArray(0);
}
