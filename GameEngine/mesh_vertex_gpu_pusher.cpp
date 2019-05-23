#include "mesh_vertex_gpu_pusher.h"
#include "game_engine.h"

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

void MeshVertexGPUPusher::initialize(MeshVertexData* in_data, const ExtraParams& in_params) {
	if (isInitialized()) {
		throw InvalidArgumentException("gpu pusher already initialized");
	}

	if (in_data == nullptr) {
		throw InvalidArgumentException("mesh vertex data was null");
	}

	mData = in_data;
	mUseCase = in_params.mUseCase;
	mUsedVertices = mData->getNumberVertices();
	mUsedFaceElements = mData->getNumberFaceElements();
	mReservedVertices = GEUtil::max(in_params.mNVerticesToReserve, mUsedVertices);
	mReservedFaceElements = GEUtil::max(in_params.mNFacesToReserve, mUsedFaceElements);

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	std::vector<ubyte> ids;
	mData->getMemberIDs(ids);

	for (auto it = ids.begin(); it < ids.end(); it++) {
		if (!(in_params.mMembersToIgnore & 1ul << (*it))) {
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

void MeshVertexGPUPusher::reserveTotalVertices(uint in_nVertices) {
	glBindVertexArray(mVertexArrayID);
	GLuint newBufferID = 0;
	if (in_nVertices > mReservedVertices) {
		for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
			glCreateBuffers(1, &newBufferID);
			if (mReservedVertices != 0) {
				glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferID);
				glBufferData(GL_COPY_WRITE_BUFFER, in_nVertices * it->second.getVertexSize(), nullptr, (GLuint)mUseCase);
				glBindBuffer(GL_COPY_READ_BUFFER, it->second.mBufferID);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mUsedVertices * it->second.getVertexSize());
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
				glBufferData(GL_ARRAY_BUFFER, in_nVertices * it->second.getVertexSize(), nullptr, (GLuint)mUseCase);
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

void MeshVertexGPUPusher::reserveTotalFaces(uint in_nFaces) {
	reserveTotalFaceElements(mData->faceToElementCount(0, in_nFaces));
}

void MeshVertexGPUPusher::reserveTotalFaceElements(uint in_nElements) {
	uint faceElementSize = mData->getFaceElementSize();

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
		else {
			glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
			glBufferData(GL_ARRAY_BUFFER, in_nElements * faceElementSize, nullptr, (GLuint)mUseCase);
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

void MeshVertexGPUPusher::addMember(uint in_id) {
	if (!mData->hasMember(in_id)) {
		throw InvalidArgumentException("invalid vertex data member id");
	}

	if (!mMembers.insert(std::pair<uint, Member>(in_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	Member& newMember = mMembers[in_id];
	newMember.mType = mData->getMemberType(in_id);
	newMember.mDepth = mData->getMemberDepth(in_id);
	newMember.mDefaultValue = nullptr;

	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &newMember.mBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newMember.mBufferID);
	glBufferData(GL_ARRAY_BUFFER, mReservedVertices * newMember.getVertexSize(), nullptr, (GLenum)mUseCase);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0, mUsedVertices * newMember.getVertexSize(),
		mData->getMemberPointer(in_id)
	);
	glEnableVertexAttribArray(in_id);
	glVertexAttribPointer(in_id, newMember.mDepth, newMember.mType, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

void MeshVertexGPUPusher::removeMember(uint in_id) {
	Member& member = mMembers[in_id];

	glBindVertexArray(mVertexArrayID);
	glDisableVertexAttribArray(in_id);
	glDeleteBuffers(1, &member.mBufferID);
	glBindVertexArray(0);

	if (member.mDefaultValue) delete[] member.mDefaultValue;

	mMembers.erase(in_id);
}

bool MeshVertexGPUPusher::hasMember(uint in_id) {
	return mMembers.find(in_id) != mMembers.end();
}

void MeshVertexGPUPusher::setMemberDefault(uint in_id, const void* in_value, bool in_realInts) {
	Member& member = mMembers[in_id];
	if (in_value == nullptr) throw InvalidArgumentException();
	if (member.mDefaultValue != nullptr) {
		delete[] member.mDefaultValue;
	}
	member.mDefaultValue = new ubyte[member.getVertexSize()];
	memcpy(member.mDefaultValue, in_value, member.getVertexSize());
	member.mRealInts = in_realInts;
	glBindVertexArray(mVertexArrayID);
	glDisableVertexAttribArray(in_id);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::clearMemberDefault(uint in_id) {
	Member& member = mMembers[in_id];
	if (member.mDefaultValue != nullptr) {
		delete[] member.mDefaultValue;
		member.mDefaultValue = nullptr;
	}
	glBindVertexArray(mVertexArrayID);
	glEnableVertexAttribArray(in_id);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::pushDefault(uint in_id) {
	Member& member = mMembers[in_id];
	if (member.mDefaultValue == nullptr) return;
	const void* value = member.mDefaultValue;
	switch (member.mType) {
	case DataType::_byte:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4bv(in_id, (GLbyte*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nbv(in_id, (GLbyte*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_ubyte:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4ubv(in_id, (GLubyte*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nubv(in_id, (GLubyte*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_short:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4sv(in_id, (GLshort*)value);
			else throw InvalidArgumentException();
		}
		else {
			switch (member.mDepth) {
			case 1: glVertexAttrib1sv(in_id, (GLshort*)value); break;
			case 2: glVertexAttrib2sv(in_id, (GLshort*)value); break;
			case 3: glVertexAttrib3sv(in_id, (GLshort*)value); break;
			case 4: glVertexAttrib4sv(in_id, (GLshort*)value); break;
			default: throw InvalidArgumentException();
			}
		}
		break;
	case DataType::_ushort:
		if (member.mRealInts) {
			if (member.mDepth == 4) glVertexAttribI4usv(in_id, (GLushort*)value);
			else throw InvalidArgumentException();
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nusv(in_id, (GLushort*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_int:
		switch (member.mDepth) {
		case 1: glVertexAttribI1iv(in_id, (GLint*)value); break;
		case 2: glVertexAttribI2iv(in_id, (GLint*)value); break;
		case 3: glVertexAttribI3iv(in_id, (GLint*)value); break;
		case 4: glVertexAttribI4iv(in_id, (GLint*)value); break;
		}
		break;
	case DataType::_uint:
		if (member.mRealInts) {
			switch (member.mDepth) {
			case 1: glVertexAttribI1uiv(in_id, (GLuint*)value); break;
			case 2: glVertexAttribI2uiv(in_id, (GLuint*)value); break;
			case 3: glVertexAttribI3uiv(in_id, (GLuint*)value); break;
			case 4: glVertexAttribI4uiv(in_id, (GLuint*)value); break;
			}
		}
		else {
			if (member.mDepth == 4) glVertexAttrib4Nuiv(in_id, (GLuint*)value);
			else throw InvalidArgumentException();
		}
		break;
	case DataType::_float:
		switch (member.mDepth) {
		case 1: glVertexAttrib1fv(in_id, (GLfloat*)value); break;
		case 2: glVertexAttrib2fv(in_id, (GLfloat*)value); break;
		case 3: glVertexAttrib3fv(in_id, (GLfloat*)value); break;
		case 4: glVertexAttrib4fv(in_id, (GLfloat*)value); break;
		default: throw InvalidArgumentException();
		}
		break;
	case DataType::_double:
		switch (member.mDepth) {
		case 1: glVertexAttrib1dv(in_id, (GLdouble*)value); break;
		case 2: glVertexAttrib2dv(in_id, (GLdouble*)value); break;
		case 3: glVertexAttrib3dv(in_id, (GLdouble*)value); break;
		case 4: glVertexAttrib4dv(in_id, (GLdouble*)value); break;
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

void MeshVertexGPUPusher::pushVertices(uint in_start, uint in_length) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		pushMember(it->first, in_start, in_length);
	}
}

void MeshVertexGPUPusher::pushMember(uint in_id) {
	pushMember(in_id, 0, mUsedVertices);
}

void MeshVertexGPUPusher::pushMember(uint in_id, uint in_start, uint in_length) {
	if (in_start + in_length > mUsedVertices) {
		throw InvalidArgumentException("invalid vertex indices");
	}
	const Member& member = mMembers[in_id];
	const uint vertexSize = member.getVertexSize();
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, member.mBufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		in_start * vertexSize, 
		in_length * vertexSize, 
		mData->getMemberPointer(in_id)
	);
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::pushFaces() {
	pushFaceElements();
}

void MeshVertexGPUPusher::pushFaces(uint in_start, uint in_length) {
	pushFaceElements(mData->faceToElementIndex(in_start), mData->faceToElementCount(0, in_length));
}

void MeshVertexGPUPusher::pushFaceElements() {
	pushFaceElements(0, mUsedFaceElements);
}

void MeshVertexGPUPusher::pushFaceElements(uint in_start, uint in_length) {
	uint faceElementSize = mData->getFaceElementSize();

	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		in_start * faceElementSize,
		in_length * faceElementSize,
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

void MeshVertexGPUPusher::draw(uint in_elementIndex, uint in_nElements) {
	pushDefaults();
	glBindVertexArray(mVertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glDrawElements(mData->getFaceMode(), in_nElements, mData->setFaceType(), (const void*)(in_elementIndex * mData->getFaceElementSize()));
	glBindVertexArray(0);
}

void MeshVertexGPUPusher::drawRaw() {
	drawRaw(0, mUsedVertices);
}

void MeshVertexGPUPusher::drawRaw(uint in_elementIndex, uint in_nElements) {
	pushDefaults();
	glBindVertexArray(mVertexArrayID);
	glDrawArrays(mData->getFaceMode(), in_elementIndex, in_nElements);
	glBindVertexArray(0);
}
