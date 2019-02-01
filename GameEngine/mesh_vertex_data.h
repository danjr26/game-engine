#ifndef MESH_VERTEX_DATA_H
#define MESH_VERTEX_DATA_H

#include <vector>
#include <utility>
#include <tuple>
#include <initializer_list>
#include <variant>
#include <map>
#include <unordered_map>
#include "vector.h"
#include "color.h"
#include "transform.h"

class MeshVertexData {
public:
	enum MemberID : ubyte {
		position,
		normal,
		color,
		uv,
		extra
	};

	enum DataType : GLenum {
		_byte = GL_BYTE,
		_ubyte = GL_UNSIGNED_BYTE,
		_short = GL_SHORT,
		_ushort = GL_UNSIGNED_SHORT,
		_int = GL_INT,
		_uint = GL_UNSIGNED_INT,
		_float = GL_FLOAT,
		_double = GL_DOUBLE
	};

	enum FaceMode : GLenum {
		points = GL_POINTS,
		line_strip = GL_LINE_STRIP,
		lines = GL_LINES,
		triangle_strip = GL_TRIANGLE_STRIP,
		triangle_fan = GL_TRIANGLE_FAN,
		triangles = GL_TRIANGLES
	};

	static ubyte Get_Data_Type_Size(DataType in_type);
	static ubyte Get_Face_Mode_Rank(FaceMode in_faceMode);
	static ubyte Get_Face_Mode_Base(FaceMode in_faceMode);

protected:
	struct Member {
		DataType type;
		ubyte depth;
		std::vector<ubyte> data;

		Member();
		Member(DataType in_type, ubyte in_depth, uint in_nVertices, const void* in_data);
		Member(DataType in_type, ubyte in_depth, uint in_nVertices);

		ubyte Get_Vertex_Size() const;
	};

	std::unordered_map<ubyte, Member> members;
	std::vector<ubyte> indices;
	DataType indexType;
	FaceMode faceMode;
	uint nVertices;

public:
	MeshVertexData(DataType in_indexType = DataType::_uint, FaceMode in_faceMode = FaceMode::triangles);

	template<class T, uint n>
	void Apply_Transform_Points(const Transform<T, n>& in_transform, ubyte in_id);
	template<class T, uint n>
	void Apply_Transform_Directions(const Transform<T, n>& in_transform, ubyte in_id);
	template<class T, uint n>
	void Apply_Transform_Vectors(const Transform<T, n>& in_transform, ubyte in_id);

	uint Get_Number_Vertices() const;
	uint Get_Number_Faces() const;
	uint Get_Number_Face_Elements() const;
	uint Get_Number_Members() const;

	bool Is_Valid_Number_Elements(uint in_nElements) const;

	void Get_Member_IDs(std::vector<ubyte>& out_ids);
	void Add_Member(ubyte in_id, DataType in_type, ubyte in_depth, const void* in_data);
	void Remove_Member(ubyte in_member);
	void Set_Member_Value(ubyte in_member, uint in_index, const void* in_value);
	void Set_Member_Values(ubyte in_member, uint in_index, uint in_nValues, const void* in_values);
	void Get_Member_Value(ubyte in_member, uint in_index, void* out_value);
	void Expand_Member(ubyte in_member, void* out_values);
	bool Has_Member(ubyte in_id) const;
	DataType Get_Member_Type(ubyte in_member) const;
	ubyte Get_Member_Depth(ubyte in_member) const;

	void Reserve_Total_Vertices(uint in_nVertices);
	void Reserve_Total_Faces(uint in_nFaces);
	void Reserve_Total_Face_Elements(uint in_nElements);

	void Add_Vertices(uint in_nVertices, const std::unordered_map<ubyte, const void*>& in_data);
	void Remove_Vertex(uint in_index);
	void Set_Vertex(uint in_index, std::unordered_map<ubyte, const void*> in_data);
	void Set_Vertices(uint in_index, uint in_nVertices, std::unordered_map<ubyte, const void*> in_data);
	void Swap_Vertices(uint in_index1, uint in_index2);

	template<class T>
	void Add_Faces_Polygon();
	template<class T>
	void Add_Faces_Polygon(uint in_index, uint in_nVertices);
	template<class T>
	void Add_Faces_Delaunay();
	template<class T>
	void Add_Faces_Delaunay(uint in_index, uint in_nVertices);

	void Add_Faces(uint in_nFaces, const void* in_indices);
	void Add_Face_Elements(uint in_nElements, const void* in_indices);
	void Remove_Face(uint in_index);
	void Remove_Face_Element(uint in_index);
	const void* Get_Face(uint in_index) const;
	const void* Get_Face_Element(uint in_index) const;
	void Set_Face(uint in_faceIndex, const void* in_indices);
	void Set_Face_Element(uint in_faceElement, const void* in_index);
	void Set_Faces(uint in_faceIndex, uint in_nFaces, const void* in_indices);
	void Set_Face_Elements(uint in_faceElement, uint in_nFaceElements, const void* in_indices);
	void Set_Face_Mode(FaceMode in_faceMode);
	DataType Get_Face_Type() const;
	FaceMode Get_Face_Mode() const;
	ubyte Get_Face_Element_Size() const;

	const void* Get_Member_Pointer(ubyte in_member) const;
	const void* Get_Face_Pointer() const;

	uint Element_To_Face_Index(uint in_elementIndex) const;
	uint Face_To_Element_Index(uint in_faceIndex) const;
	uint Face_To_Element_Count(uint in_faceIndex, uint in_nFaces) const;
	uint Element_To_Face_Count(uint in_elementIndex, uint in_nElements) const;

	template<class T> inline static DataType To_Data_Type();
	template<> inline static DataType To_Data_Type<ubyte>() { return DataType::_ubyte; }
	template<> inline static DataType To_Data_Type<char>() { return DataType::_byte; }
	template<> inline static DataType To_Data_Type<ushort>() { return DataType::_ushort; }
	template<> inline static DataType To_Data_Type<short>() { return DataType::_short; }
	template<> inline static DataType To_Data_Type<uint>() { return DataType::_uint; }
	template<> inline static DataType To_Data_Type<int>() { return DataType::_int; }
	template<> inline static DataType To_Data_Type<float>() { return DataType::_float; }
	template<> inline static DataType To_Data_Type<double>() { return DataType::_double; }
};


#endif
