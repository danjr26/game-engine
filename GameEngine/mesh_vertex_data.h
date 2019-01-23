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

	static ubyte Get_Data_Type_Size(DataType in_type);

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
	uint nVertices;

public:
	MeshVertexData(DataType in_indexType);

	template<class T, uint n>
	void Apply_Transform_Points(const Transform<T, n>& in_transform, ubyte in_id);

	template<class T, uint n>
	void Apply_Transform_Directions(const Transform<T, n>& in_transform, ubyte in_id);

	template<class T, uint n>
	void Apply_Transform_Vectors(const Transform<T, n>& in_transform, ubyte in_id);

	uint Get_Number_Vertices() const;
	uint Get_Number_Faces() const;
	uint Get_Number_Members() const;

	void Get_Member_IDs(std::vector<ubyte>& out_ids);
	void Add_Member(ubyte in_id, DataType in_type, ubyte in_depth, const void* in_data);
	void Remove_Member(ubyte in_member);
	void Set_Member_Value(ubyte in_member, uint in_index, const void* in_value);
	void Set_Member_Values(ubyte in_member, uint in_index, uint in_nValues, const void* in_values);
	bool Has_Member(ubyte in_id) const;
	DataType Get_Member_Type(ubyte in_member) const;
	ubyte Get_Member_Depth(ubyte in_member) const;

	void Reserve_Total(uint in_nVertices, uint in_nFaces);
	void Reserve_Additional(uint in_nVertices, uint in_nFaces);

	void Add_Vertices(uint in_nVertices, std::initializer_list<const void*> in_data);
	void Add_Vertices(uint in_nVertices, const std::vector<const void*>& in_data);
	void Remove_Vertex(uint in_index);
	void Set_Vertex(uint in_index, std::initializer_list<const void*> in_data);
	void Set_Vertices(uint in_index, uint in_nVertices, std::initializer_list<const void*> in_data);

	void Add_Faces(uint in_nFaces, const void* in_indices);

	template<class T>
	void Add_Faces_Polygon();

	template<class T>
	void Add_Faces_Polygon(uint in_index, uint in_nVertices);

	template<class T>
	void Add_Faces_Delaunay();

	template<class T>
	void Add_Faces_Delaunay(uint in_index, uint in_nVertices);

	void Remove_Face(uint in_index);
	const void* Get_Face(uint in_index) const;
	void Set_Face(uint in_faceIndex, const void* in_indices);
	void Set_Faces(uint in_faceIndex, uint in_nFaces, const void* in_indices);
	DataType Get_Face_Type() const;
	ubyte Get_Face_Size() const;

	const void* Get_Member_Pointer(ubyte in_member) const;
	const void* Get_Face_Pointer() const;

protected:
	template<class T> inline DataType To_Data_Type();
	template<> inline DataType To_Data_Type<ubyte>() { return DataType::_ubyte; }
	template<> inline DataType To_Data_Type<char>() { return DataType::_byte; }
	template<> inline DataType To_Data_Type<ushort>() { return DataType::_ushort; }
	template<> inline DataType To_Data_Type<short>() { return DataType::_short; }
	template<> inline DataType To_Data_Type<uint>() { return DataType::_uint; }
	template<> inline DataType To_Data_Type<int>() { return DataType::_int; }
	template<> inline DataType To_Data_Type<float>() { return DataType::_float; }
	template<> inline DataType To_Data_Type<double>() { return DataType::_double; }
};


#endif
