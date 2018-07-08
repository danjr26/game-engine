#ifndef MESH_VERTEX_DATA_H
#define MESH_VERTEX_DATA_H

#include <vector>
#include <utility>
#include <tuple>
#include <initializer_list>
#include <variant>
#include <map>
#include "vector.h"
#include "color.h"

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

	struct Member {
		ubyte id;
		DataType type;
		ubyte depth;
		std::vector<ubyte> data;

		ubyte Get_Vertex_Size() const;
	};

protected:
	std::vector<Member> members;
	std::vector<ubyte> indices;
	DataType indexType;
	std::map<ubyte, ubyte> idToIndex;
	uint nVertices;

	static ubyte Get_Data_Type_Size(DataType in_type);

public:
	MeshVertexData(DataType in_indexType);

	uint Get_Number_Vertices() const;
	uint Get_Number_Faces() const;

	void Add_Member(const Member& in_member);
	void Remove_Member(ubyte in_member);
	void Set_Member_Value(ubyte in_member, uint in_index, const void* in_value);
	bool Has_Member(ubyte in_id) const;
	ubyte Get_Member_Index_By_ID(ubyte in_id) const;
	DataType Get_Member_Type(ubyte in_member) const;
	ubyte Get_Member_Depth(ubyte in_member) const;

	void Reserve_Total(uint in_nVertices, uint in_nFaces);
	void Reserve_Additional(uint in_nVertices, uint in_nFaces);

	void Add_Vertices(uint in_nVertices, std::initializer_list<const void*> in_data);
	void Remove_Vertex(uint in_index);
	void Set_Vertex(uint in_index, std::initializer_list<const void*> in_data);

	void Add_Faces(uint in_nFaces, const void* in_indices);
	void Remove_Face(uint in_index);
	const void* Get_Face(uint in_index) const;
	void Set_Face(uint in_faceIndex, const void* in_indices);


	const void* Get_Member_Pointer(ubyte in_member) const;
	const void* Get_Index_Pointer() const;

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
