#include "mesh_vertex_data.h"
#include "game_engine.h"
#include <algorithm>



/*
MeshVertexData::MeshVertexData(ubyte in_nPositionComponents, ubyte in_nNormalComponents, ubyte in_nColorComponents, ubyte in_nUVComponents) :
	members(MemberIndex::count),
	componentCounts{ in_nPositionComponents, in_nNormalComponents, in_nColorComponents, in_nUVComponents } {

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

bool MeshVertexData::hasMember(MemberIndex in_member) const {
	return componentCounts[in_member] != 0;
}

ubyte MeshVertexData::getMemberDepth(MemberIndex in_member) const {
	return componentCounts[in_member];
}

void MeshVertexData::Reserve_Total(ushort in_nVertices, uint in_nFaces) {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			members[i].reserve(in_nVertices * componentCounts[i]);
		}
	}

	indices.reserve(in_nFaces * 3);
}

void MeshVertexData::Reserve_Additional(ushort in_nVertices, uint in_nFaces) {
	Reserve_Total(getNumberVertices() + in_nVertices, getNumberFaces() + in_nFaces);
}

void MeshVertexData::addVertices(uint in_nVertices, const float* in_positions, const float* in_normals, const float* in_colors, const float* in_uvs) {
	Reserve_Additional(in_nVertices, 0);

	const float* pointers[MemberIndex::count] = {
		in_positions,
		in_normals,
		in_colors,
		in_uvs
	};

	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			if (pointers[i] == nullptr) {
				throw InvalidArgumentException("vertex data was null");
			}
			else {
				members[i].insert(members[i].end(), pointers[i], pointers[i] + in_nVertices * componentCounts[i]);
			}
		}
	}
}

void MeshVertexData::addFaces(uint in_nFaces, const ushort* in_indices) {
	if (in_indices == nullptr) {
		throw InvalidArgumentException("face data was null");
	}
	else {
		indices.insert(indices.end(), in_indices, in_indices + (in_nFaces * 3));
	}
}

void MeshVertexData::removeVertex(ushort in_index) {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			members[i].erase(
				members[i].begin() + in_index * componentCounts[i], 
				members[i].begin() + (in_index + 1) * componentCounts[i]
			);
		}
	}
}

void MeshVertexData::removeFace(uint in_index) {
	indices.erase(
		indices.begin() + in_index * 3,
		indices.begin() + (in_index + 1) * 3
	);
}

const float* MeshVertexData::getMemberValue(MemberIndex in_member, ushort in_index) const {
	return &members[in_member][in_index * componentCounts[in_member]];
}

ushort MeshVertexData::Get_Index(uint in_face, ubyte in_index) const {
	return indices[in_face * 3 + in_index];
}

void MeshVertexData::setMemberValue(MemberIndex in_member, ushort in_index, const float* in_value) {
	for (ubyte i = 0; i < componentCounts[in_member]; i++) {
		members[in_member][in_index * componentCounts[in_member] + i] = in_value[i];
	}
}

void MeshVertexData::setVertex(ushort in_index, const float* in_position, const float* in_normal, const float* in_color, const float* in_uv) {
	setMemberValue(MemberIndex::position, in_index, in_position);
	setMemberValue(MemberIndex::normal, in_index, in_normal);
	setMemberValue(MemberIndex::color, in_index, in_color);
	setMemberValue(MemberIndex::uv, in_index, in_uv);
}

const float* MeshVertexData::getMemberPointer(MemberIndex in_member) const {
	return members[in_member].data();
}

const ushort* MeshVertexData::getFacePointer() const {
	return indices.data();
}
*/

ubyte MeshVertexData::getDataTypeSize(DataType in_type) {
	switch (in_type) {
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

MeshVertexData::MeshVertexData(DataType in_indexType, FaceMode in_faceMode) :
	mMembers(),
	mIndices(),
	mIndexType(in_indexType),
	mNVertices(0),
	mFaceMode(in_faceMode)
{}

MeshVertexData::MeshVertexData(const MeshVertexData& in_other) {
	*this = in_other;
}

MeshVertexData::~MeshVertexData() 
{}

void MeshVertexData::operator=(const MeshVertexData& in_other) {
	mMembers = in_other.mMembers;
	mIndices = in_other.mIndices;
	mIndexType = in_other.mIndexType;
	mFaceMode = in_other.mFaceMode;
	mNVertices = in_other.mNVertices;
}

template<class T, uint n>
void MeshVertexData::applyTransformPoints(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = mMembers[in_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = in_transform.localToWorldPoint(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::applyTransformDirections(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = mMembers[in_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = in_transform.localToWorldDirection(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::applyTransformVectors(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = mMembers[in_member];
	if (member.mType != toDataType<T>() || member.mDepth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.mData.data();
	for (uint i = 0; i < member.mData.size() / member.getVertexSize(); i++) {
		data[i] = in_transform.localToWorldVector(data[i]);
	}
}

template void MeshVertexData::applyTransformPoints<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::applyTransformPoints<double, 2>(const Transform2d& in_transform, ubyte in_member);
template void MeshVertexData::applyTransformDirections<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::applyTransformDirections<double, 2>(const Transform2d& in_transform, ubyte in_member);
template void MeshVertexData::applyTransformVectors<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::applyTransformVectors<double, 2>(const Transform2d& in_transform, ubyte in_member);

ubyte MeshVertexData::getFaceModeRank(FaceMode in_faceMode) {
	switch (in_faceMode) {
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

ubyte MeshVertexData::getFaceModeBase(FaceMode in_faceMode) {
	switch (in_faceMode) {
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

bool MeshVertexData::isValidNumberElements(uint in_nElements) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (in_nElements - base) % rank == 0;
}

void MeshVertexData::getMemberIDs(std::vector<ubyte>& out_ids) {
	out_ids.reserve(out_ids.size() + mMembers.size());
	for (auto it = mMembers.cbegin(); it != mMembers.cend(); it++) {
		out_ids.push_back(it->first);
	}
}

void MeshVertexData::addMember(ubyte in_id, DataType in_type, ubyte in_depth, const void* in_data) {
	if (!mMembers.insert(std::pair<ubyte, Member>(in_id, Member())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	if (in_data == nullptr) {
		mMembers[in_id] = Member(in_type, in_depth, mNVertices);
	}
	else {
		mMembers[in_id] = Member(in_type, in_depth, mNVertices, in_data);
	}
}

void MeshVertexData::removeMember(ubyte in_member) {
	mMembers.erase(in_member);
}

void MeshVertexData::setMemberValue(ubyte in_member, uint in_index, const void* in_value) {
	Member& member = mMembers[in_member];
	uint size = member.getVertexSize();
	memcpy(&member.mData[in_index * size], in_value, size);
}

void MeshVertexData::setMemberValues(ubyte in_member, uint in_index, uint in_nValues, const void* in_values) {
	Member& member = mMembers[in_member];
	uint size = member.getVertexSize();
	memcpy(&member.mData[in_index * size], in_values, in_nValues * size);
}

void MeshVertexData::getMemberValue(ubyte in_member, uint in_index, void* out_value) const {
	const Member& member = mMembers.at(in_member);
	uint size = member.getVertexSize();
	memcpy(out_value, &member.mData[in_index * size], size);
}

void MeshVertexData::expandMember(ubyte in_member, void* out_values) {
	Member& member = mMembers[in_member];
	uint size = member.getVertexSize();
	ubyte* outPtr = (ubyte*)out_values;
	for (uint i = 0; i < getNumberFaceElements(); i++) {
		uint index = getStandardFaceElement(i);
		memcpy(&outPtr[i * size], &member.mData[index * size], size);
	}
}

bool MeshVertexData::hasMember(ubyte in_id) const {
	return mMembers.find(in_id) != mMembers.end();
}

MeshVertexData::DataType MeshVertexData::getMemberType(ubyte in_member) const {
	return mMembers.at(in_member).mType;
}

ubyte MeshVertexData::getMemberDepth(ubyte in_member) const {
	return mMembers.at(in_member).mDepth;
}

void MeshVertexData::reserveTotalVertices(uint in_nVertices) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		member.mData.reserve(in_nVertices * member.getVertexSize());
	}
}

void MeshVertexData::reserveTotalFaces(uint in_nFaces) {
	reserveTotalFaceElements(faceToElementCount(0, in_nFaces));
}

void MeshVertexData::reserveTotalFaceElements(uint in_nElements) {
	mIndices.reserve(in_nElements * getDataTypeSize(mIndexType));
}

void MeshVertexData::addVertices(uint in_nVertices, const std::unordered_map<ubyte, const void*>& in_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		auto search = in_data.find(it->first);
		if (search == in_data.end() || search->second == nullptr) {
			member.mData.resize(member.mData.size() + in_nVertices * member.getVertexSize());
		}
		else {
			const ubyte* ptr = (const ubyte*)search->second;
			member.mData.insert(member.mData.end(), ptr, ptr + in_nVertices * member.getVertexSize());
		}
	}

	mNVertices += in_nVertices;
}

void MeshVertexData::removeVertex(uint in_index) {
	if (in_index >= mNVertices) {
		throw InvalidArgumentException("invalid index");
	}

	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		member.mData.erase(
			member.mData.begin() + in_index * member.getVertexSize(), 
			member.mData.begin() + (in_index + 1) * member.getVertexSize()
		);
	}

	mNVertices--;
}

void MeshVertexData::setVertex(uint in_index, std::unordered_map<ubyte, const void*> in_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		auto search = in_data.find(it->first);
		if (search == in_data.end() || search->second == nullptr) {
			setMemberValue(it->first, in_index, search->second);
		}
	}
}

void MeshVertexData::setVertices(uint in_index, uint in_nVertices, std::unordered_map<ubyte, const void*> in_data) {
	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		auto search = in_data.find(it->first);
		if (search == in_data.end() || search->second == nullptr) {
			setMemberValues(it->first, in_index, in_nVertices, search->second);
		}
	}
}

void MeshVertexData::swapVertices(uint in_index1, uint in_index2) {
	if (in_index1 == in_index2) return;
	if (in_index1 >= mNVertices || in_index2 >= mNVertices) {
		throw InvalidArgumentException("invalid index");
	}

	for (auto it = mMembers.begin(); it != mMembers.end(); it++) {
		Member& member = it->second;
		uint size = member.getVertexSize();
		std::swap_ranges(
			member.mData.begin() + in_index1 * size,
			member.mData.begin() + (in_index1 + 1) * size,
			member.mData.begin() + in_index2 * size
			);
	}
}

void MeshVertexData::addFaces(uint in_nFaces, const void* in_indices) {
	addFaceElements(faceToElementCount(getNumberFaces(), in_nFaces), in_indices);
}

void MeshVertexData::addFaceElements(uint in_nElements, const void* in_indices) {
	if (!isValidNumberElements(getNumberFaceElements() + in_nElements)) {
		throw InvalidArgumentException();
	}
	uint length = in_nElements * getDataTypeSize(mIndexType);
	const ubyte* ptr = (const ubyte*)in_indices;

	mIndices.insert(mIndices.end(), ptr, ptr + length);
}

template<class T>
void MeshVertexData::addFacesPolygon() {
	addFacesPolygon<T>(0, mNVertices);
}

template void MeshVertexData::addFacesPolygon<float>();
template void MeshVertexData::addFacesPolygon<double>();

template<class T>
void MeshVertexData::addFacesPolygon(uint in_index, uint in_nVertices) {
	if (mFaceMode != FaceMode::triangles || !hasMember(MemberID::position) || 
		getMemberType(MemberID::position) != toDataType<T>() || in_nVertices < 3) {

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
	pointData.reserve(in_nVertices);
	for (uint i = 0; i < in_nVertices; i++) {
		pointData.push_back(new PointData());
	}

	// connect points
	for (uint i = 0; i < in_nVertices; i++) {
		const Vector<T, 2>& point = values[i];
		pointData[i]->index = i;
		pointData[i]->next = pointData[(i == in_nVertices - 1) ? (0) : (i + 1)];
		pointData[i]->prev = pointData[(i == 0) ? (in_nVertices - 1) : (i - 1)];
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
	for (uint i = 0; i < in_nVertices; i++) {
		delete pointData[i];
	}
}

template<class T>
void MeshVertexData::addFacesDelaunay() {
	addFacesDelaunay(0, mNVertices);
}

template<class T>
void MeshVertexData::addFacesDelaunay(uint in_index, uint in_nVertices) {
	throw NotImplementedException();
}

void MeshVertexData::removeFace(uint in_index) {
	uint size = getDataTypeSize(mIndexType);
	uint index1 = faceToElementIndex(in_index);
	uint index2 = faceToElementIndex(in_index + 1);
	mIndices.erase(mIndices.begin() + index1 * size, mIndices.begin() + index2 * size);
}

void MeshVertexData::removeFaceElement(uint in_index) {
	uint size = getDataTypeSize(mIndexType);
	mIndices.erase(mIndices.begin() + in_index * size);
}

const void* MeshVertexData::getFace(uint in_index) const {
	return getFaceElement(faceToElementIndex(in_index));
}

const void* MeshVertexData::getFaceElement(uint in_index) const {
	uint size = getDataTypeSize(mIndexType);
	return mIndices.data() + in_index * size;
}

uint MeshVertexData::getStandardFaceElement(uint in_index) const {
	uint index = 0;
	switch (mIndexType) {
	case DataType::_byte:
		index = ((byte*)mIndices.data())[in_index];
		break;
	case DataType::_ubyte:
		index = ((ubyte*)mIndices.data())[in_index];
		break;
	case DataType::_short:
		index = ((short*)mIndices.data())[in_index];
		break;
	case DataType::_ushort:
		index = ((ushort*)mIndices.data())[in_index];
		break;
	case DataType::_int:
		index = ((int*)mIndices.data())[in_index];
		break;
	case DataType::_uint:
		index = ((uint*)mIndices.data())[in_index];
		break;
	default:
		throw InvalidArgumentException();
	}
	return index;
}

void MeshVertexData::setFace(uint in_faceIndex, const void* in_indices) {
	setFaces(in_faceIndex, 1, in_indices);
}

void MeshVertexData::setFaceElement(uint in_faceElement, const void* in_index) {
	setFaceElements(in_faceElement, 1, in_index);
}

void MeshVertexData::setFaces(uint in_faceIndex, uint in_nFaces, const void* in_indices) {
	setFaceElements(faceToElementIndex(in_faceIndex), faceToElementCount(in_faceIndex, in_nFaces), in_indices);
}

void MeshVertexData::setFaceElements(uint in_faceElement, uint in_nFaceElements, const void* in_indices) {
	uint size = getDataTypeSize(mIndexType);
	const ubyte* ptr = (const ubyte*)in_indices;
	memcpy(&mIndices[in_faceElement * size], in_indices, in_nFaceElements * size);
}

void MeshVertexData::setFaceMode(FaceMode in_faceMode) {
	if (!mIndices.empty()) {
		throw InvalidArgumentException();
	}

	mFaceMode = in_faceMode;
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

const void* MeshVertexData::getMemberPointer(ubyte in_member) const {
	return mMembers.at(in_member).mData.data();
}

const void* MeshVertexData::getFacePointer() const {
	return mIndices.data();
}

uint MeshVertexData::elementToFaceIndex(uint in_elementIndex) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (in_elementIndex <= base) ? (0) : ((in_elementIndex - base) / rank);
}

uint MeshVertexData::faceToElementIndex(uint in_faceIndex) const {
	uint rank = getFaceModeRank(mFaceMode);
	uint base = getFaceModeBase(mFaceMode);
	return (in_faceIndex == 0) ? (0) : in_faceIndex * rank + base;
}

uint MeshVertexData::faceToElementCount(uint in_faceIndex, uint in_nFaces) const {
	return faceToElementIndex(in_faceIndex + in_nFaces) - faceToElementIndex(in_faceIndex);
}

uint MeshVertexData::elementToFaceCount(uint in_elementIndex, uint in_nElements) const {
	return faceToElementIndex(in_elementIndex + in_nElements) - faceToElementIndex(in_elementIndex);
}

MeshVertexData::Member::Member() 
{}

MeshVertexData::Member::Member(DataType in_type, ubyte in_depth, uint in_nVertices, const void* in_data) :
	mType(in_type),
	mDepth(in_depth),
	mData(((const ubyte*)in_data), ((const ubyte*)in_data) + (in_nVertices * in_depth * getDataTypeSize(in_type)))
{}

MeshVertexData::Member::Member(DataType in_type, ubyte in_depth, uint in_nVertices) :
	mType(in_type),
	mDepth(in_depth),
	mData(in_nVertices * in_depth * getDataTypeSize(in_type))
{}

ubyte MeshVertexData::Member::getVertexSize() const {
	return mDepth * MeshVertexData::getDataTypeSize(mType);
}
