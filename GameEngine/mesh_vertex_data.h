#ifndef MESH_VERTEX_DATA_H
#define MESH_VERTEX_DATA_H

#include <vector>
#include <utility>
#include "vector.h"
#include "color.h"

class MeshVertexData {
protected:
	enum MemberIndex : uint {
		position,
		normal,
		color,
		uv,
		count
	};

	std::vector<ubyte> componentCounts;
	std::vector<std::vector<float>> members;
	std::vector<ushort> indices;

protected:
	const float* Get_Component(MemberIndex in_member, uint in_index) const;
	void Set_Component(MemberIndex in_member, uint in_index, const float* in_values);

public:
	MeshVertexData(ubyte in_nPositionComponents, ubyte in_nNormalComponents, ubyte in_nColorComponents, ubyte in_nUVComponents);
	ushort Get_Number_Vertices() const;
	uint Get_Number_Faces() const;
	void Reserve_Total(ushort in_nVertices, uint in_nFaces);
	void Reserve_Additional(ushort in_nVertices, uint in_nFaces);
	void Add_Vertices(uint in_nVertices, const float* in_positions, const float* in_normals, const float* in_colors, const float* in_uvs);
	void Add_Faces(uint in_nFaces, const ushort* in_indices);
	void Remove_Vertex(ushort in_index);
	void Remove_Face(uint in_index);
	const float* Get_Position(ushort in_index) const;
	const float* Get_Normal(ushort in_index) const;
	const float* Get_Color(ushort in_index) const;
	const float* Get_UV(ushort in_index) const;
	ushort Get_Index(uint in_face, ubyte in_index) const;
	void Set_Position(ushort in_index, const float* in_position);
	void Set_Normal(ushort in_index, const float* in_normal);
	void Set_Color(ushort in_index, const float* in_color);
	void Set_UV(ushort in_index, const float* in_uv);
	void Set_Vertex(ushort in_index, const float* in_position, const float* in_normal, const float* in_color, const float* in_uv);
	const float* Get_Position_Pointer() const;
	const float* Get_Normal_Pointer() const;
	const float* Get_Color_Pointer() const;
	const float* Get_UV_Pointer() const;
	const ushort* Get_Index_Pointer() const;
};

#endif
