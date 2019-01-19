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

ushort MeshVertexData::Get_Number_Vertices() const {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			return componentCounts[i];
		}
	}
	throw InvalidArgumentException("mesh vertices have no components");
}

uint MeshVertexData::Get_Number_Faces() const {
	return indices.size() / 3;
}

bool MeshVertexData::Has_Member(MemberIndex in_member) const {
	return componentCounts[in_member] != 0;
}

ubyte MeshVertexData::Get_Member_Depth(MemberIndex in_member) const {
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
	Reserve_Total(Get_Number_Vertices() + in_nVertices, Get_Number_Faces() + in_nFaces);
}

void MeshVertexData::Add_Vertices(uint in_nVertices, const float* in_positions, const float* in_normals, const float* in_colors, const float* in_uvs) {
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

void MeshVertexData::Add_Faces(uint in_nFaces, const ushort* in_indices) {
	if (in_indices == nullptr) {
		throw InvalidArgumentException("face data was null");
	}
	else {
		indices.insert(indices.end(), in_indices, in_indices + (in_nFaces * 3));
	}
}

void MeshVertexData::Remove_Vertex(ushort in_index) {
	for (uint i = 0; i < members.size(); i++) {
		if (componentCounts[i] > 0) {
			members[i].erase(
				members[i].begin() + in_index * componentCounts[i], 
				members[i].begin() + (in_index + 1) * componentCounts[i]
			);
		}
	}
}

void MeshVertexData::Remove_Face(uint in_index) {
	indices.erase(
		indices.begin() + in_index * 3,
		indices.begin() + (in_index + 1) * 3
	);
}

const float* MeshVertexData::Get_Member_Value(MemberIndex in_member, ushort in_index) const {
	return &members[in_member][in_index * componentCounts[in_member]];
}

ushort MeshVertexData::Get_Index(uint in_face, ubyte in_index) const {
	return indices[in_face * 3 + in_index];
}

void MeshVertexData::Set_Member_Value(MemberIndex in_member, ushort in_index, const float* in_value) {
	for (ubyte i = 0; i < componentCounts[in_member]; i++) {
		members[in_member][in_index * componentCounts[in_member] + i] = in_value[i];
	}
}

void MeshVertexData::Set_Vertex(ushort in_index, const float* in_position, const float* in_normal, const float* in_color, const float* in_uv) {
	Set_Member_Value(MemberIndex::position, in_index, in_position);
	Set_Member_Value(MemberIndex::normal, in_index, in_normal);
	Set_Member_Value(MemberIndex::color, in_index, in_color);
	Set_Member_Value(MemberIndex::uv, in_index, in_uv);
}

const float* MeshVertexData::Get_Member_Pointer(MemberIndex in_member) const {
	return members[in_member].data();
}

const ushort* MeshVertexData::Get_Face_Pointer() const {
	return indices.data();
}
*/

ubyte MeshVertexData::Get_Data_Type_Size(DataType in_type) {
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

MeshVertexData::MeshVertexData(DataType in_indexType) :
	members(),
	indices(),
	indexType(in_indexType),
	idToIndex(),
	nVertices(0)
{}

template<class T, uint n>
void MeshVertexData::Apply_Transform_Points(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = members[in_member];
	if (member.type != To_Data_Type<T>() || member.depth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.data.data();
	for (uint i = 0; i < member.data.size() / member.Get_Vertex_Size(); i++) {
		data[i] = in_transform.Local_To_World_Point(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::Apply_Transform_Directions(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = members[in_member];
	if (member.type != To_Data_Type<T>() || member.depth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.data.data();
	for (uint i = 0; i < member.data.size() / member.Get_Vertex_Size(); i++) {
		data[i] = in_transform.Local_To_World_Direction(data[i]);
	}
}

template<class T, uint n>
void MeshVertexData::Apply_Transform_Vectors(const Transform<T, n>& in_transform, ubyte in_member) {
	Member& member = members[in_member];
	if (member.type != To_Data_Type<T>() || member.depth != n) {
		throw InvalidArgumentException();
	}

	Vector<T, n>* data = (Vector<T, n>*)member.data.data();
	for (uint i = 0; i < member.data.size() / member.Get_Vertex_Size(); i++) {
		data[i] = in_transform.Local_To_World_Vector(data[i]);
	}
}

template void MeshVertexData::Apply_Transform_Points<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::Apply_Transform_Points<double, 2>(const Transform2d& in_transform, ubyte in_member);
template void MeshVertexData::Apply_Transform_Directions<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::Apply_Transform_Directions<double, 2>(const Transform2d& in_transform, ubyte in_member);
template void MeshVertexData::Apply_Transform_Vectors<float, 2>(const Transform2f& in_transform, ubyte in_member);
template void MeshVertexData::Apply_Transform_Vectors<double, 2>(const Transform2d& in_transform, ubyte in_member);

uint MeshVertexData::Get_Number_Vertices() const {
	return nVertices;
}

uint MeshVertexData::Get_Number_Faces() const {
	return (uint)indices.size() / Get_Data_Type_Size(indexType) / 3;
}

uint MeshVertexData::Get_Number_Members() const {
	return (uint)members.size();
}

void MeshVertexData::Add_Member(ubyte in_id, DataType in_type, ubyte in_depth, const void* in_data) {
	if (!idToIndex.insert(std::pair<ubyte, ubyte>(in_id, (ubyte)members.size())).second) {
		throw InvalidArgumentException("duplicate mesh member id");
	}

	if (in_data == nullptr) {
		members.emplace_back(in_id, in_type, in_depth, nVertices);
	}
	else {
		members.emplace_back(in_id, in_type, in_depth, nVertices, in_data);
	}
}

void MeshVertexData::Remove_Member(ubyte in_member) {
	if (in_member >= members.size()) {
		throw InvalidArgumentException("invalid index");
	}

	idToIndex.erase(members[in_member].id);
	members.erase(members.begin() + in_member);

	for (uint i = in_member; i < members.size(); i++) {
		idToIndex[members[i].id] = i;
	}
}

void MeshVertexData::Set_Member_Value(ubyte in_member, uint in_index, const void* in_value) {
	Member& member = members[in_member];
	for (uint i = 0; i < member.depth; i++) {
		member.data[in_index * member.Get_Vertex_Size() + i] = ((const char*)in_value)[i];
	}
}

void MeshVertexData::Set_Member_Values(ubyte in_member, uint in_index, uint in_nValues, const void* in_values) {
	Member& member = members[in_member];
	memcpy(&member.data[in_index * member.Get_Vertex_Size()], in_values, in_nValues * member.Get_Vertex_Size());
}

bool MeshVertexData::Has_Member(ubyte in_id) const {
	return idToIndex.find(in_id) != idToIndex.end();
}

ubyte MeshVertexData::Get_Member_Index_By_ID(ubyte in_id) const {
	if (!Has_Member(in_id)) {
		throw InvalidArgumentException("invalid id");
	}
	return idToIndex.at(in_id);
}

ubyte MeshVertexData::Get_Member_ID(ubyte in_member) const {
	return members[in_member].id;
}

MeshVertexData::DataType MeshVertexData::Get_Member_Type(ubyte in_member) const {
	return members[in_member].type;
}

ubyte MeshVertexData::Get_Member_Depth(ubyte in_member) const {
	return members[in_member].depth;
}

void MeshVertexData::Reserve_Total(uint in_nVertices, uint in_nFaces) {
	for (uint i = 0; i < members.size(); i++) {
		members[i].data.reserve(in_nVertices * members[i].Get_Vertex_Size());
	}
	indices.reserve(in_nFaces * 3);
}

void MeshVertexData::Reserve_Additional(uint in_nVertices, uint in_nFaces) {
	Reserve_Total(Get_Number_Vertices() + in_nVertices, Get_Number_Faces() * in_nFaces);
}

void MeshVertexData::Add_Vertices(uint in_nVertices, std::initializer_list<const void*> in_data) {
	if (in_data.size() != members.size()) {
		throw InvalidArgumentException("vertex data of wrong dimension");
	}
	for (uint i = 0; i < members.size(); i++) {
		const ubyte* ptr = (const ubyte*) *(in_data.begin() + i);
		if(ptr == nullptr) members[i].data.resize(members[i].data.size() + in_nVertices * members[i].Get_Vertex_Size());
		else members[i].data.insert(members[i].data.end(), ptr, ptr + in_nVertices * members[i].Get_Vertex_Size());
	}

	nVertices += in_nVertices;
}

void MeshVertexData::Add_Vertices(uint in_nVertices, const std::vector<const void*>& in_data) {
	if (in_data.size() != members.size()) {
		throw InvalidArgumentException("vertex data of wrong dimension");
	}
	for (uint i = 0; i < members.size(); i++) {
		const ubyte* ptr = (const ubyte*) *(in_data.begin() + i);
		if (ptr == nullptr) members[i].data.resize(members[i].data.size() + in_nVertices * members[i].Get_Vertex_Size());
		else members[i].data.insert(members[i].data.end(), ptr, ptr + in_nVertices * members[i].Get_Vertex_Size());
	}

	nVertices += in_nVertices;
}

void MeshVertexData::Remove_Vertex(uint in_index) {
	if (in_index >= nVertices) {
		throw InvalidArgumentException("invalid index");
	}

	for (uint i = 0; i < members.size(); i++) {
		auto begin = members[i].data.begin() + (in_index * members[i].Get_Vertex_Size());
		auto end = begin + members[i].Get_Vertex_Size();
		members[i].data.erase(begin, end);
	}

	nVertices--;
}

void MeshVertexData::Set_Vertex(uint in_index, std::initializer_list<const void*> in_data) {
	if (in_data.size() != members.size()) {
		throw InvalidArgumentException("vertex data of wrong dimension");
	}
	for (uint i = 0; i < members.size(); i++) {
		Set_Member_Value(i, in_index, *(in_data.begin() + i));
	}
}

void MeshVertexData::Set_Vertices(uint in_index, uint in_nVertices, std::initializer_list<const void*> in_data) {
	if (in_data.size() != members.size()) {
		throw InvalidArgumentException("vertex data of wrong dimension");
	}
	for (uint i = 0; i < members.size(); i++) {
		Set_Member_Values(i, in_index, in_nVertices, *(in_data.begin() + i));
	}
}

void MeshVertexData::Add_Faces(uint in_nFaces, const void* in_indices) {
	uint length = in_nFaces * 3 * Get_Data_Type_Size(indexType);
	const ubyte* ptr = (const ubyte*)in_indices;
	indices.insert(indices.end(), ptr, ptr + length);
}

template<class T>
void MeshVertexData::Add_Faces_Polygon() {
	Add_Faces_Polygon<T>(0, nVertices);
}

template void MeshVertexData::Add_Faces_Polygon<float>();
template void MeshVertexData::Add_Faces_Polygon<double>();

template<class T>
void MeshVertexData::Add_Faces_Polygon(uint in_index, uint in_nVertices) {
	if (!Has_Member(MemberID::position) || Get_Member_Type(MemberID::position) != To_Data_Type<T>() || in_nVertices < 3) {
		throw InvalidArgumentException();
	}

	struct PointData {
		uint index;
		PointData* next;
		PointData* prev;
	};

	const Vector<T, 2>* values = (const Vector<T, 2>*)this->Get_Member_Pointer(MemberID::position);

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
			return values[p1->index].X() < values[p2->index].X();
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

		const bool isLine = point.X() == prevPoint.X() && point.X() == nextPoint.X();
		const bool isSplit = !isLine && point.X() <= prevPoint.X() && point.X() <= nextPoint.X() && nextPoint.Y() > prevPoint.Y();
		const bool isMerge = !isLine && point.X() >= prevPoint.X() && point.X() >= nextPoint.X() && nextPoint.Y() < prevPoint.Y();

		if (isSplit || isMerge) {
			PointData* anchorPoint = nullptr;
			std::pair<PointData*, PointData*> lowEdge(nullptr, nullptr);
			std::pair<PointData*, PointData*> highEdge(nullptr, nullptr);
			T lowY, highY;

			for (auto it = activeEdges.begin(); it != activeEdges.end(); it++) {
				const Vector<T, 2>& edgePoint1 = values[it->first->index];
				const Vector<T, 2>& edgePoint2 = values[it->second->index];
				T interpolatedY = Lerp(edgePoint1.Y(), edgePoint2.Y(), point.X(), edgePoint1.X(), edgePoint2.X());
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

				T interpolatedY1 = Lerp(lowEdgePoint1.Y(), lowEdgePoint2.Y(), testPoint.X(), lowEdgePoint1.X(), lowEdgePoint2.X());
				T interpolatedY2 = Lerp(highEdgePoint1.Y(), highEdgePoint2.Y(), testPoint.X(), highEdgePoint1.X(), highEdgePoint2.X());

				if (pointData[j] == lowEdge.first || pointData[j] == lowEdge.second || 
					pointData[j] == highEdge.first || pointData[j] == highEdge.second || 
					Is_Between_Inc<T>(testPoint.Y(), interpolatedY1, interpolatedY2)) {

					for (int k = i; values[pointData[k]->index].X() == point.X(); k = pointData[k]->next->index) {
						newEdges.insert(std::pair<PointData*, PointData*>(pointData[k], pointData[j]));
						newEdges.insert(std::pair<PointData*, PointData*>(pointData[j], pointData[k]));
					}
					break;
				}
			}
		}

		// add back edge
		if (point.X() < prevPoint.X()) {
			activeEdges.insert(std::pair<PointData*, PointData*>(pointData[i]->prev, pointData[i]));
		}

		// add forward edge
		if (point.X() < nextPoint.X()) {
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

		switch (indexType) {
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

		Add_Faces(1, &indices);
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

		if ((thisPoint - prevPoint).Orthogonal().Dot(nextPoint - thisPoint) > 0 ||
			!(isRepeating || (thisPoint - prevPoint).Dot(nextPoint - thisPoint) <= 0)) {
			
			continue;
		}

		bool isEar = true;
		for (PointData* testNode = currentNode->next->next; testNode != currentNode->prev; testNode = testNode->next) {
			const Vector<T, 2>& testPoint = values[testNode->index];
			if ((thisPoint - prevPoint).Orthogonal().Dot(testPoint - prevPoint) <= 0 &&
				(nextPoint - thisPoint).Orthogonal().Dot(testPoint - thisPoint) <= 0 &&
				(prevPoint - nextPoint).Orthogonal().Dot(testPoint - nextPoint) <= 0) {

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
void MeshVertexData::Add_Faces_Delaunay() {
	Add_Faces_Delaunay(0, nVertices);
}

void MeshVertexData::Remove_Face(uint in_index) {
	uint faceSize = 3 * Get_Data_Type_Size(indexType);
	indices.erase(indices.begin() + in_index * faceSize, indices.begin() + (in_index + 1) * faceSize);
}

const void* MeshVertexData::Get_Face(uint in_index) const {
	return indices.data() + in_index * 3 * Get_Data_Type_Size(indexType);
}

void MeshVertexData::Set_Face(uint in_faceIndex, const void* in_indices) {
	const ubyte* ptr = (const ubyte*)in_indices;
	for (uint i = 0; i < 3u * Get_Data_Type_Size(indexType); i++) {
		indices[in_faceIndex * 3 * Get_Data_Type_Size(indexType) + i] = ptr[i];
	}
}

void MeshVertexData::Set_Faces(uint in_faceIndex, uint in_nFaces, const void* in_indices) {
	memcpy(&indices[in_faceIndex * 3 * Get_Data_Type_Size(indexType)], in_indices, in_nFaces * 3 * Get_Data_Type_Size(indexType));
}

ubyte MeshVertexData::Get_Face_Size() const {
	return Get_Data_Type_Size(indexType) * 3;
}

MeshVertexData::DataType MeshVertexData::Get_Face_Type() const {
	return indexType;
}

const void* MeshVertexData::Get_Member_Pointer(ubyte in_member) const {
	return members[in_member].data.data();
}

const void* MeshVertexData::Get_Face_Pointer() const {
	return indices.data();
}

MeshVertexData::Member::Member(ubyte in_id, DataType in_type, ubyte in_depth, uint in_nVertices, const void* in_data) :
	id(in_id),
	type(in_type),
	depth(in_depth),
	data(((const ubyte*)in_data), ((const ubyte*)in_data) + (in_nVertices * in_depth * Get_Data_Type_Size(in_type)))
{}

MeshVertexData::Member::Member(ubyte in_id, DataType in_type, ubyte in_depth, uint in_nVertices) :
	id(in_id),
	type(in_type),
	depth(in_depth),
	data(in_nVertices * in_depth * Get_Data_Type_Size(in_type))
{}

ubyte MeshVertexData::Member::Get_Vertex_Size() const {
	return depth * MeshVertexData::Get_Data_Type_Size(type);
}
