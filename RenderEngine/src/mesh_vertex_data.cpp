#include "mesh_vertex_data.h"
#include "game_engine.h"
#include <algorithm>



/*
MeshVertexData::MeshVertexData(ubyte i_nPositionComponents, ubyte i_nNormalComponents, ubyte i_nColorComponents, ubyte i_nUVComponents) :
	members(MemberIndex::count),
	componentCounts{ i_nPositionComponents, i_nNormalComponents, i_nColorComponents, i_nUVComponents } {

	bool hasComponents = false;
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			hasComponents = true;
			break;
		}
	}

	if (!hasComponents) {
		throw InvalidArgumentException("mesh vertices were given no components");
	}
}

ushort MeshVertexData::getNumberVertices() const {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			return componentCounts[i];
		}
	}
	throw InvalidArgumentException("mesh vertices have no components");
}

uint MeshVertexData::getNumberFaces() const {
	return indices.size() / 3;
}

bool MeshVertexData::hasMember(MemberIndex i_member) const {
	return componentCounts[i_member] != 0;
}

ubyte MeshVertexData::getMemberDepth(MemberIndex i_member) const {
	return componentCounts[i_member];
}

void MeshVertexData::Reserve_Total(ushort i_nVertices, uint i_nFaces) {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			members[i].reserve(i_nVertices * componentCounts[i]);
		}
	}

	indices.reserve(i_nFaces * 3);
}

void MeshVertexData::Reserve_Additional(ushort i_nVertices, uint i_nFaces) {
	Reserve_Total(getNumberVertices() + i_nVertices, getNumberFaces() + i_nFaces);
}

void MeshVertexData::addVertices(uint i_nVertices, const float* i_positions, const float* i_normals, const float* i_colors, const float* i_uvs) {
	Reserve_Additional(i_nVertices, 0);

	const float* pointers[MemberIndex::count] = {
		i_positions,
		i_normals,
		i_colors,
		i_uvs
	};

	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			if (pointers[i] == nullptr) {
				throw InvalidArgumentException("vertex data was null");
			}
			else {
				members[i].insert(members[i].end(), pointers[i], pointers[i] + i_nVertices * componentCounts[i]);
			}
		}
	}
}

void MeshVertexData::addFaces(uint i_nFaces, const ushort* i_indices) {
	if (i_indices == nullptr) {
		throw InvalidArgumentException("face data was null");
	}
	else {
		indices.insert(indices.end(), i_indices, i_indices + (i_nFaces * 3));
	}
}

void MeshVertexData::removeVertex(ushort i_index) {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			members[i].erase(
				members[i].begin() + i_index * componentCounts[i], 
				members[i].begin() + (i_index + 1) * componentCounts[i]
			);
		}
	}
}

void MeshVertexData::removeFace(uint i_index) {
	indices.erase(
		indices.begin() + i_index * 3,
		indices.begin() + (i_index + 1) * 3
	);
}

const float* MeshVertexData::getMemberValue(MemberIndex i_member, ushort i_index) const {
	return &members[i_member][i_index * componentCounts[i_member]];
}

ushort MeshVertexData::Get_Index(uint i_face, ubyte i_index) const {
	return indices[i_face * 3 + i_index];
}

void MeshVertexData::setMemberValue(MemberIndex i_member, ushort i_index, const float* i_value) {
	for (ubyte i = 0; i < componentCounts[i_member]; i++) {
		members[i_member][i_index * componentCounts[i_member] + i] = i_value[i];
	}
}

void MeshVertexData::setVertex(ushort i_index, const float* i_position, const float* i_normal, const float* i_color, const float* i_uv) {
	setMemberValue(MemberIndex::position, i_index, i_position);
	setMemberValue(MemberIndex::normal, i_index, i_normal);
	setMemberValue(MemberIndex::color, i_index, i_color);
	setMemberValue(MemberIndex::uv, i_index, i_uv);
}

const float* MeshVertexData::getMemberPointer(MemberIndex i_member) const {
	return members[i_member].data();
}

const ushort* MeshVertexData::getFacePointer() const {
	return indices.data();
}
*/

ubyte MeshVertexData::getDataTypeSize(DataType i_type) {
	switch (i_type) {
	case _byte: return sizeof(char);
	case _ubyte: return sizeof(ubyte);
	case _short: return sizeof(short);
	case _ushort: return sizeof(ushort);
	case _int: return sizeof(int);
	case _uint: return sizeof(uint);
	case _float: return sizeof(float);
	case _double: return sizeof(double);
	default: throw InvalidArgumentException("invalid data type");
	}
}

MeshVertexData::MeshVertexData(DataType i_indexType, FaceMode i_faceMode) :
	mMembers(),
	mIndices(),
	mIndexType(i_indexType),
	mNVertices(0),
	mFaceMode(i_faceMode)
{}

MeshVertexData::MeshVertexData(const MeshVertexData& i_other) {
	*this = i_other;
}

MeshVertexData::~MeshVertexData() 
{}

void MeshVertexData::operator=(const MeshVertexData& i_other) {
	mMembers = i_other.mMembers;
	mIndices = i_other.mIndices;
	mIndexType = i_other.mIndexType;
	mFaceMode = i_other.mFaceMode;
	mNVertices = i_other.mNVertices;
}

template<class T, uint n>
void MeshVertexData::applyTransformPoints(const Transform<T, n>& i_transform, ubyte i_member) {
	Member& member = mMembers[i_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = i_transform.localToWorldPoint(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::applyTransformDirections(const Transform<T, n>& i_transform, ubyte i_member) {
	Member& member = mMembers[i_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = i_transform.localToWorldDirection(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::applyTransformVectors(const Transform<T, n>& i_transform, ubyte i_member) {
	Member& member = mMembers[i_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = i_transform.localToWorldVector(data[i]);
	}
}

template void MeshVertexData::applyTransformPoints<float, 2>(const Transform2f& i_transform, ubyte i_member);
template void MeshVertexData::applyTransformPoints<double, 2>(const Transform2d& i_transform, ubyte i_member);
template void MeshVertexData::applyTransformDirections<float, 2>(const Transform2f& i_transform, ubyte i_member);
template void MeshVertexData::applyTransformDirections<double, 2>(const Transform2d& i_transform, ubyte i_member);
template void MeshVertexData::applyTransformVectors<float, 2>(const Transform2f& i_transform, ubyte i_member);
template void MeshVertexData::applyTransformVectors<double, 2>(const Transform2d& i_transform, ubyte i_member);

ubyte MeshVertexData::getFaceModeRank(FaceMode i_faceMode) {
	switch (i_faceMode) {
	case MeshVertexData::points:
		return 1;
		break;
	case MeshVertexData::line_strip:
		return 1;
		break;
	case MeshVertexData::lines:
		return 2;
		break;
	case MeshVertexData::triangle_strip:
		return 1;
		break;
	case MeshVertexData::triangle_fan:
		return 1;
		break;
	case MeshVertexData::triangles:
		return 3;
		break;
	default:
		throw InvalidArgumentException();
		return 0;
		break;
	}
}

ubyte MeshVertexData::getFaceModeBase(FaceMode i_faceMode) {
	switch (i_faceMode) {
	case MeshVertexData::points:
		return 0;
		break;
	case MeshVertexData::line_strip:
		return 1;
		break;
	case MeshVertexData::lines:
		return 0;
		break;
	case MeshVertexData::triangle_strip:
		return 2;
		break;
	case MeshVertexData::triangle_fan:
		return 2;
		break;
	case MeshVertexData::triangles:
		return 0;
		break;
	default:
		throw InvalidArgumentException();
		return 0;
		break;
	}
}

uint MeshVertexData::getNumberVertices() const {
	return mNVertices;
}

uint MeshVertexData::getNumberFaces() const {
	return (getNumberFaceElements() - getFaceModeBase(mFaceMode)) / getFaceModeRank(mFaceMode);
}

uint MeshVertexData::getNumberFaceElements() const {
	return (uint)mIndices.size() / getDataTypeSize(mIndexType);
}

uint MeshVertexData::getNumberMembers() const {
	return (uint)mMembers.size();
}

bool MeshVertexData::isValidNumberElements(uint i_nElements) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (i_nElements - base) % rank == 0;
}

void MeshVertexData::getMemberIDs(std::vector<ubyte>& o_ids) {
	o_ids.reserve(o_ids.size() + mMembers.size());
	for (auto it = mMembers.cbegin(); it != mMembers.cend(); it++) {
		o_ids.push_back(it->first);
	}
}

void MeshVertexData::addMember(ubyte i_id, DataType i_type, ubyte i_depth, const void* i_data) {
	if (!mMembers.insert(std::pair<ubyte, Member>(i_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	if (i_data == nullptr) {
		mMembers[i_id] = Member(i_type, i_depth, mNVertices);
	}
	else {
		mMembers[i_id] = Member(i_type, i_depth, mNVertices, i_data);
	}
}

void MeshVertexData::removeMember(ubyte i_member) {
	mMembers.erase(i_member);
}

void MeshVertexData::setMemberValue(ubyte i_member, uint i_index, const void* i_value) {
	Member& member = mMembers[i_member];
	uint size = member.getVertexSize();
	memcpy(&member.mData[i_index * size], i_value, size);
}

void MeshVertexData::setMemberValues(ubyte i_member, uint i_index, uint i_nValues, const void* i_values) {
	Member& member = mMembers[i_member];
	uint size = member.getVertexSize();
	memcpy(&member.mData[i_index * size], i_values, i_nValues * size);
}

void MeshVertexData::getMemberValue(ubyte i_member, uint i_index, void* o_value) const {
	const Member& member = mMembers.at(i_member);
	uint size = member.getVertexSize();
	memcpy(o_value, &member.mData[i_index * size], size);
}

void MeshVertexData::expandMember(ubyte i_member, void* o_values) {
	Member& member = mMembers[i_member];
	uint size = member.getVertexSize();
	ubyte* outPtr = (ubyte*)o_values;
	for (uint i = 0; i < getNumberFaceElements(); i++) {
		uint index = getStandardFaceElement(i);
		memcpy(&outPtr[i * size], &member.mData[index * size], size);
	}
}

bool MeshVertexData::hasMember(ubyte i_id) const {
	return mMembers.find(i_id) != mMembers.end();
}

MeshVertexData::DataType MeshVertexData::getMemberType(ubyte i_member) const {
	return mMembers.at(i_member).mType;
}

ubyte MeshVertexData::getMemberDepth(ubyte i_member) const {
	return mMembers.at(i_member).mDepth;
}

void MeshVertexData::reserveTotalVertices(uint i_nVertices) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		member.mData.reserve(i_nVertices * member.getVertexSize());
	}
}

void MeshVertexData::reserveTotalFaces(uint i_nFaces) {
	reserveTotalFaceElements(faceToElementCount(0, i_nFaces));
}

void MeshVertexData::reserveTotalFaceElements(uint i_nElements) {
	mIndices.reserve(i_nElements * getDataTypeSize(mIndexType));
}

void MeshVertexData::addVertices(uint i_nVertices, const std::unordered_map<ubyte, const void*>& i_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		auto search = i_data.find(it->first);
		if (search == i_data.end() || search->second == nullptr) {
			member.mData.resize(member.mData.size() + i_nVertices * member.getVertexSize());
		}
		else {
			const ubyte* ptr = (const ubyte*)search->second;
			member.mData.insert(member.mData.end(), ptr, ptr + i_nVertices * member.getVertexSize());
		}
	}

	mNVertices += i_nVertices;
}

void MeshVertexData::removeVertex(uint i_index) {
	if (i_index >= mNVertices) {
		throw InvalidArgumentException("invalid index");
	}

	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		member.mData.erase(
			member.mData.begin() + i_index * member.getVertexSize(), 
			member.mData.begin() + (i_index + 1) * member.getVertexSize()
		);
	}

	mNVertices--;
}

void MeshVertexData::setVertex(uint i_index, std::unordered_map<ubyte, const void*> i_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		auto search = i_data.find(it->first);
		if (search == i_data.end() || search->second == nullptr) {
			setMemberValue(it->first, i_index, search->second);
		}
	}
}

void MeshVertexData::setVertices(uint i_index, uint i_nVertices, std::unordered_map<ubyte, const void*> i_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		auto search = i_data.find(it->first);
		if (search == i_data.end() || search->second == nullptr) {
			setMemberValues(it->first, i_index, i_nVertices, search->second);
		}
	}
}

void MeshVertexData::swapVertices(uint i_index1, uint i_index2) {
	if (i_index1 == i_index2) return;
	if (i_index1 >= mNVertices || i_index2 >= mNVertices) {
		throw InvalidArgumentException("invalid index");
	}

	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		uint size = member.getVertexSize();
		std::swap_ranges(
			member.mData.begin() + i_index1 * size,
			member.mData.begin() + (i_index1 + 1) * size,
			member.mData.begin() + i_index2 * size
			);
	}
}

void MeshVertexData::addFaces(uint i_nFaces, const void* i_indices) {
	addFaceElements(faceToElementCount(getNumberFaces(), i_nFaces), i_indices);
}

void MeshVertexData::addFaceElements(uint i_nElements, const void* i_indices) {
	if (!isValidNumberElements(getNumberFaceElements() + i_nElements)) {
		throw InvalidArgumentException();
	}
	uint length = i_nElements * getDataTypeSize(mIndexType);
	const ubyte* ptr = (const ubyte*)i_indices;

	mIndices.insert(mIndices.end(), ptr, ptr + length);
}

template<class T>
void MeshVertexData::addFacesPolygon() {
	addFacesPolygon<T>(0, mNVertices);
}

template void MeshVertexData::addFacesPolygon<float>();
template void MeshVertexData::addFacesPolygon<double>();

template<class T>
void MeshVertexData::addFacesPolygon(uint i_index, uint i_nVertices) {
	if (mFaceMode != FaceMode::triangles || !hasMember(MemberID::position) || 
		getMemberType(MemberID::position) != toDataType<T>() || i_nVertices < 3) {

		throw InvalidArgumentException();
	}

	struct PointData {
		uint index;
		PointData* next;
		PointData* prev;
	};

	const Vector<T, 2>* values = (const Vector<T, 2>*)this->getMemberPointer(MemberID::position);

	// expand points
	std::vector<PointData*> pointData;
	pointData.reserve(i_nVertices);
	for (uint i = 0; i < i_nVertices; i++) {
		pointData.push_back(new PointData());
	}

	// connect points
	for (uint i = 0; i < i_nVertices; i++) {
		const Vector<T, 2>& point = values[i];
		pointData[i]->index = i;
		pointData[i]->next = pointData[(i == i_nVertices - 1) ? (0) : (i + 1)];
		pointData[i]->prev = pointData[(i == 0) ? (i_nVertices - 1) : (i - 1)];
	}

	/*
	// sort points by x value
	std::sort(
		pointData.begin(), 
		pointData.end(), 
		[values](PointData*& p1, PointData*& p2) {
			return values[p1->index].x() < values[p2->index].x();
		}
	);

	// split into monotone polygons
	std::unordered_multimap<PointData*, PointData*> activeEdges;
	std::unordered_multimap<PointData*, PointData*> newEdges;
	for (uint i = 0; i < pointData.size(); i++) {
		const Vector<T, 2>& point = values[pointData[i]->index];
		const Vector<T, 2>& prevPoint = values[pointData[i]->prev->index];
		const Vector<T, 2>& nextPoint = values[pointData[i]->next->index];

		// delete edges that end at this point
		auto searchRange = activeEdges.equal_range(pointData[i]);
		activeEdges.erase(searchRange.first, searchRange.second);

		const bool isLine = point.x() == prevPoint.x() && point.x() == nextPoint.x();
		const bool isSplit = !isLine && point.x() <= prevPoint.x() && point.x() <= nextPoint.x() && nextPoint.y() > prevPoint.y();
		const bool isMerge = !isLine && point.x() >= prevPoint.x() && point.x() >= nextPoint.x() && nextPoint.y() < prevPoint.y();

		if (isSplit || isMerge) {
			PointData* anchorPoint = nullptr;
			std::pair<PointData*, PointData*> lowEdge(nullptr, nullptr);
			std::pair<PointData*, PointData*> highEdge(nullptr, nullptr);
			T lowY, highY;

			for (auto it = activeEdges.begin(); it != activeEdges.end(); it++) {
				const Vector<T, 2>& edgePoint1 = values[it->first->index];
				const Vector<T, 2>& edgePoint2 = values[it->second->index];
				T interpolatedY = lerp(edgePoint1.y(), edgePoint2.y(), point.x(), edgePoint1.x(), edgePoint2.x());
				if (lowEdge.first == nullptr || interpolatedY > lowY) {
					lowY = interpolatedY;
					lowEdge = *it;
				}
				if (highEdge.first == nullptr || interpolatedY < highY) {
					highY = interpolatedY;
					highEdge = *it;
				}
			}

			for (int j = (isSplit) ? i - 1 : i + 1; (isSplit) ? (j >= 0) : (j < pointData.size()); j += (isSplit) ? -1 : 1) {
				const Vector<T, 2>& testPoint = values[pointData[j]->index];
				const Vector<T, 2>& lowEdgePoint1 = values[lowEdge.first->index];
				const Vector<T, 2>& lowEdgePoint2 = values[lowEdge.second->index];
				const Vector<T, 2>& highEdgePoint1 = values[highEdge.first->index];
				const Vector<T, 2>& highEdgePoint2 = values[highEdge.second->index];

				T interpolatedY1 = lerp(lowEdgePoint1.y(), lowEdgePoint2.y(), testPoint.x(), lowEdgePoint1.x(), lowEdgePoint2.x());
				T interpolatedY2 = lerp(highEdgePoint1.y(), highEdgePoint2.y(), testPoint.x(), highEdgePoint1.x(), highEdgePoint2.x());

				if (pointData[j] == lowEdge.first || pointData[j] == lowEdge.second || 
					pointData[j] == highEdge.first || pointData[j] == highEdge.second || 
					betwInc<T>(testPoint.y(), interpolatedY1, interpolatedY2)) {

					for (int k = i; values[pointData[k]->index].x() == point.x(); k = pointData[k]->next->index) {
						newEdges.insert(std::pair<PointData*, PointData*>(pointData[k], pointData[j]));
						newEdges.insert(std::pair<PointData*, PointData*>(pointData[j], pointData[k]));
					}
					break;
				}
			}
		}

		// add back edge
		if (point.x() < prevPoint.x()) {
			activeEdges.insert(std::pair<PointData*, PointData*>(pointData[i]->prev, pointData[i]));
		}

		// add forward edge
		if (point.x() < nextPoint.x()) {
			activeEdges.insert(std::pair<PointData*, PointData*>(pointData[i]->next, pointData[i]));
		}
	}

	// triangulate monotone polygons
	std::vector<std::pair<PointData*, PointData*>> edgesLeft;
	edgesLeft.push_back(std::pair<PointData*, PointData*>(pointData[0], pointData[0]->next));
	while (!edgesLeft.empty()) {
		const Vector<T, 2>& point1 = values[edgesLeft.back().first->index];
		const Vector<T, 2>& point2 = values[edgesLeft.back().second->index];
		auto search = newEdges.equal_range(edgesLeft.back().second);
		
	}
	*/

	auto addEar = [this](PointData* earNode) {
		union {
			uchar bytes[3];
			ushort shorts[3];
			uint ints[3];
		} indices;

		switch (mIndexType) {
		case DataType::_byte:
		case DataType::_ubyte:
			indices.bytes[0] = earNode->prev->index;
			indices.bytes[1] = earNode->index;
			indices.bytes[2] = earNode->next->index;
			break;
		case DataType::_short:
		case DataType::_ushort:
			indices.shorts[0] = earNode->prev->index;
			indices.shorts[1] = earNode->index;
			indices.shorts[2] = earNode->next->index;
			break;
		case DataType::_int:
		case DataType::_uint:
			indices.ints[0] = earNode->prev->index;
			indices.ints[1] = earNode->index;
			indices.ints[2] = earNode->next->index;
			break;
		default:
			throw InvalidArgumentException();
		}

		addFaces(1, &indices);
		earNode->prev->next = earNode->next;
		earNode->next->prev = earNode->prev;
	};

	PointData* currentNode = pointData[0];
	PointData* repeatNode = pointData[0];
	bool isRepeating = false;
	for (; currentNode->next->next->next != currentNode; currentNode = currentNode->next) {
		const Vector<T, 2>& thisPoint = values[currentNode->index];
		const Vector<T, 2>& nextPoint = values[currentNode->next->index];
		const Vector<T, 2>& prevPoint = values[currentNode->prev->index];
		
		if (currentNode == repeatNode) {
			isRepeating = true;
		}

		if ((thisPoint - prevPoint).orthogonal().dot(nextPoint - thisPoint) > 0 ||
			!(isRepeating || (thisPoint - prevPoint).dot(nextPoint - thisPoint) <= 0)) {
			
			continue;
		}

		bool isEar = true;
		for (PointData* testNode = currentNode->next->next; testNode != currentNode->prev; testNode = testNode->next) {
			const Vector<T, 2>& testPoint = values[testNode->index];
			if ((thisPoint - prevPoint).orthogonal().dot(testPoint - prevPoint) <= 0 &&
				(nextPoint - thisPoint).orthogonal().dot(testPoint - thisPoint) <= 0 &&
				(prevPoint - nextPoint).orthogonal().dot(testPoint - nextPoint) <= 0) {

				isEar = false;
				break;
			}
		}

		if (isEar) {
			addEar(currentNode);
			repeatNode = currentNode->prev;
			isRepeating = false;
		}
	}

	addEar(currentNode);

	// clean up
	for (uint i = 0; i < i_nVertices; i++) {
		delete pointData[i];
	}
}

template<class T>
void MeshVertexData::addFacesDelaunay() {
	addFacesDelaunay(0, mNVertices);
}

template<class T>
void MeshVertexData::addFacesDelaunay(uint i_index, uint i_nVertices) {
	throw NotImplementedException();
}

void MeshVertexData::removeFace(uint i_index) {
	uint size = getDataTypeSize(mIndexType);
	uint index1 = faceToElementIndex(i_index);
	uint index2 = faceToElementIndex(i_index + 1);
	mIndices.erase(mIndices.begin() + index1 * size, mIndices.begin() + index2 * size);
}

void MeshVertexData::removeFaceElement(uint i_index) {
	uint size = getDataTypeSize(mIndexType);
	mIndices.erase(mIndices.begin() + i_index * size);
}

const void* MeshVertexData::getFace(uint i_index) const {
	return getFaceElement(faceToElementIndex(i_index));
}

const void* MeshVertexData::getFaceElement(uint i_index) const {
	uint size = getDataTypeSize(mIndexType);
	return mIndices.data() + i_index * size;
}

uint MeshVertexData::getStandardFaceElement(uint i_index) const {
	uint index = 0;
	switch (mIndexType) {
	case DataType::_byte:
		index = ((byte*)mIndices.data())[i_index];
		break;
	case DataType::_ubyte:
		index = ((ubyte*)mIndices.data())[i_index];
		break;
	case DataType::_short:
		index = ((short*)mIndices.data())[i_index];
		break;
	case DataType::_ushort:
		index = ((ushort*)mIndices.data())[i_index];
		break;
	case DataType::_int:
		index = ((int*)mIndices.data())[i_index];
		break;
	case DataType::_uint:
		index = ((uint*)mIndices.data())[i_index];
		break;
	default:
		throw InvalidArgumentException();
	}
	return index;
}

void MeshVertexData::setFace(uint i_faceIndex, const void* i_indices) {
	setFaces(i_faceIndex, 1, i_indices);
}

void MeshVertexData::setFaceElement(uint i_faceElement, const void* i_index) {
	setFaceElements(i_faceElement, 1, i_index);
}

void MeshVertexData::setFaces(uint i_faceIndex, uint i_nFaces, const void* i_indices) {
	setFaceElements(faceToElementIndex(i_faceIndex), faceToElementCount(i_faceIndex, i_nFaces), i_indices);
}

void MeshVertexData::setFaceElements(uint i_faceElement, uint i_nFaceElements, const void* i_indices) {
	uint size = getDataTypeSize(mIndexType);
	const ubyte* ptr = (const ubyte*)i_indices;
	memcpy(&mIndices[i_faceElement * size], i_indices, i_nFaceElements * size);
}

void MeshVertexData::setFaceMode(FaceMode i_faceMode) {
	if (!mIndices.empty()) {
		throw InvalidArgumentException();
	}

	mFaceMode = i_faceMode;
}

MeshVertexData::FaceMode MeshVertexData::getFaceMode() const {
	return mFaceMode;
}

ubyte MeshVertexData::getFaceElementSize() const {
	return getDataTypeSize(mIndexType);
}

MeshVertexData::DataType MeshVertexData::setFaceType() const {
	return mIndexType;
}

const void* MeshVertexData::getMemberPointer(ubyte i_member) const {
	return mMembers.at(i_member).mData.data();
}

const void* MeshVertexData::getFacePointer() const {
	return mIndices.data();
}

uint MeshVertexData::elementToFaceIndex(uint i_elementIndex) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (i_elementIndex <= base) ? (0) : ((i_elementIndex - base) / rank);
}

uint MeshVertexData::faceToElementIndex(uint i_faceIndex) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (i_faceIndex == 0) ? (0) : i_faceIndex * rank + base;
}

uint MeshVertexData::faceToElementCount(uint i_faceIndex, uint i_nFaces) const {
	return faceToElementIndex(i_faceIndex + i_nFaces) - faceToElementIndex(i_faceIndex);
}

uint MeshVertexData::elementToFaceCount(uint i_elementIndex, uint i_nElements) const {
	return faceToElementIndex(i_elementIndex + i_nElements) - faceToElementIndex(i_elementIndex);
}

MeshVertexData::Member::Member() 
{}

MeshVertexData::Member::Member(DataType i_type, ubyte i_depth, uint i_nVertices, const void* i_data) :
	mType(i_type),
	mDepth(i_depth),
	mData(((const ubyte*)i_data), ((const ubyte*)i_data) + (i_nVertices * i_depth * getDataTypeSize(i_type)))
{}

MeshVertexData::Member::Member(DataType i_type, ubyte i_depth, uint i_nVertices) :
	mType(i_type),
	mDepth(i_depth),
	mData(i_nVertices * i_depth * getDataTypeSize(i_type))
{}

ubyte MeshVertexData::Member::getVertexSize() const {
	return mDepth * MeshVertexData::getDataTypeSize(mType);
}
