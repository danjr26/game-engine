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

	static ubyte getDataTypeSize(DataType i_type);
	static ubyte getFaceModeRank(FaceMode i_faceMode);
	static ubyte getFaceModeBase(FaceMode i_faceMode);
		
protected:
	struct Member {
		DataType mType;
		ubyte mDepth;
		std::vector<ubyte> mData;

		Member();
		Member(DataType i_type, ubyte i_depth, uint i_nVertices, const void* i_data);
		Member(DataType i_type, ubyte i_depth, uint i_nVertices);

		ubyte getVertexSize() const;
	};

	std::unordered_map<ubyte, Member> mMembers;
	std::vector<ubyte> mIndices;
	DataType mIndexType;
	FaceMode mFaceMode;
	uint mNVertices;

public:
	MeshVertexData(DataType i_indexType = DataType::_uint, FaceMode i_faceMode = FaceMode::triangles);
	MeshVertexData(const MeshVertexData& i_other);
	~MeshVertexData();

	void operator=(const MeshVertexData& i_other);

	template<class T, uint n>
	void applyTransformPoints(const Transform<T, n>& i_transform, ubyte i_id);
	template<class T, uint n>
	void applyTransformDirections(const Transform<T, n>& i_transform, ubyte i_id);
	template<class T, uint n>
	void applyTransformVectors(const Transform<T, n>& i_transform, ubyte i_id);

	uint getNumberVertices() const;
	uint getNumberFaces() const;
	uint getNumberFaceElements() const;
	uint getNumberMembers() const;

	bool isValidNumberElements(uint i_nElements) const;

	void getMemberIDs(std::vector<ubyte>& o_ids);
	void addMember(ubyte i_id, DataType i_type, ubyte i_depth, const void* i_data);
	void removeMember(ubyte i_member);
	void setMemberValue(ubyte i_member, uint i_index, const void* i_value);
	void setMemberValues(ubyte i_member, uint i_index, uint i_nValues, const void* i_values);
	void getMemberValue(ubyte i_member, uint i_index, void* o_value) const;
	void expandMember(ubyte i_member, void* o_values);
	bool hasMember(ubyte i_id) const;
	DataType getMemberType(ubyte i_member) const;
	ubyte getMemberDepth(ubyte i_member) const;

	void reserveTotalVertices(uint i_nVertices);
	void reserveTotalFaces(uint i_nFaces);
	void reserveTotalFaceElements(uint i_nElements);

	void addVertices(uint i_nVertices, const std::unordered_map<ubyte, const void*>& i_data);
	void removeVertex(uint i_index);
	void setVertex(uint i_index, std::unordered_map<ubyte, const void*> i_data);
	void setVertices(uint i_index, uint i_nVertices, std::unordered_map<ubyte, const void*> i_data);
	void swapVertices(uint i_index1, uint i_index2);

	template<class T>
	void addFacesPolygon();
	template<class T>
	void addFacesPolygon(uint i_index, uint i_nVertices);
	template<class T>
	void addFacesDelaunay();
	template<class T>
	void addFacesDelaunay(uint i_index, uint i_nVertices);

	void addFaces(uint i_nFaces, const void* i_indices);
	void addFaceElements(uint i_nElements, const void* i_indices);
	void removeFace(uint i_index);
	void removeFaceElement(uint i_index);
	const void* getFace(uint i_index) const;
	const void* getFaceElement(uint i_index) const;
	uint getStandardFaceElement(uint i_index) const;
	void setFace(uint i_faceIndex, const void* i_indices);
	void setFaceElement(uint i_faceElement, const void* i_index);
	void setFaces(uint i_faceIndex, uint i_nFaces, const void* i_indices);
	void setFaceElements(uint i_faceElement, uint i_nFaceElements, const void* i_indices);
	void setFaceMode(FaceMode i_faceMode);
	DataType setFaceType() const;
	FaceMode getFaceMode() const;
	ubyte getFaceElementSize() const;

	const void* getMemberPointer(ubyte i_member) const;
	const void* getFacePointer() const;

	uint elementToFaceIndex(uint i_elementIndex) const;
	uint faceToElementIndex(uint i_faceIndex) const;
	uint faceToElementCount(uint i_faceIndex, uint i_nFaces) const;
	uint elementToFaceCount(uint i_elementIndex, uint i_nElements) const;

	template<class T> inline static DataType toDataType();
	template<> inline static DataType toDataType<ubyte>() { return DataType::_ubyte; }
	template<> inline static DataType toDataType<char>() { return DataType::_byte; }
	template<> inline static DataType toDataType<ushort>() { return DataType::_ushort; }
	template<> inline static DataType toDataType<short>() { return DataType::_short; }
	template<> inline static DataType toDataType<uint>() { return DataType::_uint; }
	template<> inline static DataType toDataType<int>() { return DataType::_int; }
	template<> inline static DataType toDataType<float>() { return DataType::_float; }
	template<> inline static DataType toDataType<double>() { return DataType::_double; }
};


#endif
