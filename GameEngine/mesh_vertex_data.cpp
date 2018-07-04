#include "mesh_vertex_data.h"
#include "game_engine.h"

const float* MeshVertexData::Get_Component(MemberIndex in_member, uint in_index) const {
	return &members[in_member][in_index * componentCounts[in_member]];
}

void MeshVertexData::Set_Component(MemberIndex in_member, uint in_index, const float* in_values) {
	for (ubyte i = 0; i < componentCounts[in_member]; i++) {
		members[in_member][in_index * componentCounts[in_member] + i] = in_values[i];
	}
}

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

const float* MeshVertexData::Get_Position(ushort in_index) const {
	return Get_Component(MemberIndex::position, in_index);
}

const float* MeshVertexData::Get_Normal(ushort in_index) const {
	return Get_Component(MemberIndex::normal, in_index);
}

const float* MeshVertexData::Get_Color(ushort in_index) const {
	return Get_Component(MemberIndex::color, in_index);
}

const float* MeshVertexData::Get_UV(ushort in_index) const {
	return Get_Component(MemberIndex::uv, in_index);
}

ushort MeshVertexData::Get_Index(uint in_face, ubyte in_index) const {
	return indices[in_face * 3 + in_index];
}

void MeshVertexData::Set_Position(ushort in_index, const float* in_position) {
	Set_Component(MemberIndex::position, in_index, in_position);
}

void MeshVertexData::Set_Normal(ushort in_index, const float* in_normal) {
	Set_Component(MemberIndex::normal, in_index, in_normal);
}

void MeshVertexData::Set_Color(ushort in_index, const float* in_color) {
	Set_Component(MemberIndex::color, in_index, in_color);
}

void MeshVertexData::Set_UV(ushort in_index, const float* in_uv) {
	Set_Component(MemberIndex::uv, in_index, in_uv);
}

void MeshVertexData::Set_Vertex(ushort in_index, const float* in_position, const float* in_normal, const float* in_color, const float* in_uv) {
	Set_Position(in_index, in_position);
	Set_Normal(in_index, in_normal);
	Set_Color(in_index, in_color);
	Set_UV(in_index, in_uv);
}

const float* MeshVertexData::Get_Position_Pointer() const {
	return members[MemberIndex::position].data();
}

const float* MeshVertexData::Get_Normal_Pointer() const {
	return members[MemberIndex::normal].data();
}

const float* MeshVertexData::Get_Color_Pointer() const {
	return members[MemberIndex::color].data();
}

const float* MeshVertexData::Get_UV_Pointer() const {
	return members[MemberIndex::uv].data();
}

const ushort* MeshVertexData::Get_Index_Pointer() const {
	return indices.data();
}



