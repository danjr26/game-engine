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

	static ubyte getDataTypeSize(DataType in_type);
	static ubyte getFaceModeRank(FaceMode in_faceMode);
	static ubyte getFaceModeBase(FaceMode in_faceMode);

protected:
	struct Member {
		DataType mType;
		ubyte mDepth;
		std::vector<ubyte> mData;

		Member();
		Member(DataType in_type, ubyte in_depth, uint in_nVertices, const void* in_data);
		Member(DataType in_type, ubyte in_depth, uint in_nVertices);

		ubyte getVertexSize() const;
	};

	std::unordered_map<ubyte, Member> mMembers;
	std::vector<ubyte> mIndices;
	DataType mIndexType;
	FaceMode mFaceMode;
	uint mNVertices;

public:
	MeshVertexData(DataType in_indexType = DataType::_uint, FaceMode in_faceMode = FaceMode::triangles);
	MeshVertexData(const MeshVertexData& in_other);
	~MeshVertexData();

	void operator=(const MeshVertexData& in_other);

	template<class T, uint n>
	void applyTransformPoints(const Transform<T, n>& in_transform, ubyte in_id);
	template<class T, uint n>
	void applyTransformDirections(const Transform<T, n>& in_transform, ubyte in_id);
	template<class T, uint n>
	void applyTransformVectors(const Transform<T, n>& in_transform, ubyte in_id);

	uint getNumberVertices() const;
	uint getNumberFaces() const;
	uint getNumberFaceElements() const;
	uint getNumberMembers() const;

	bool isValidNumberElements(uint in_nElements) const;

	void getMemberIDs(std::vector<ubyte>& out_ids);
	void addMember(ubyte in_id, DataType in_type, ubyte in_depth, const void* in_data);
	void removeMember(ubyte in_member);
	void setMemberValue(ubyte in_member, uint in_index, const void* in_value);
	void setMemberValues(ubyte in_member, uint in_index, uint in_nValues, const void* in_values);
	void getMemberValue(ubyte in_member, uint in_index, void* out_value) const;
	void expandMember(ubyte in_member, void* out_values);
	bool hasMember(ubyte in_id) const;
	DataType getMemberType(ubyte in_member) const;
	ubyte getMemberDepth(ubyte in_member) const;

	void reserveTotalVertices(uint in_nVertices);
	void reserveTotalFaces(uint in_nFaces);
	void reserveTotalFaceElements(uint in_nElements);

	void addVertices(uint in_nVertices, const std::unordered_map<ubyte, const void*>& in_data);
	void removeVertex(uint in_index);
	void setVertex(uint in_index, std::unordered_map<ubyte, const void*> in_data);
	void setVertices(uint in_index, uint in_nVertices, std::unordered_map<ubyte, const void*> in_data);
	void swapVertices(uint in_index1, uint in_index2);

	template<class T>
	void addFacesPolygon();
	template<class T>
	void addFacesPolygon(uint in_index, uint in_nVertices);
	template<class T>
	void addFacesDelaunay();
	template<class T>
	void addFacesDelaunay(uint in_index, uint in_nVertices);

	void addFaces(uint in_nFaces, const void* in_indices);
	void addFaceElements(uint in_nElements, const void* in_indices);
	void removeFace(uint in_index);
	void removeFaceElement(uint in_index);
	const void* getFace(uint in_index) const;
	const void* getFaceElement(uint in_index) const;
	uint getStandardFaceElement(uint in_index) const;
	void setFace(uint in_faceIndex, const void* in_indices);
	void setFaceElement(uint in_faceElement, const void* in_index);
	void setFaces(uint in_faceIndex, uint in_nFaces, const void* in_indices);
	void setFaceElements(uint in_faceElement, uint in_nFaceElements, const void* in_indices);
	void setFaceMode(FaceMode in_faceMode);
	DataType setFaceType() const;
	FaceMode getFaceMode() const;
	ubyte getFaceElementSize() const;

	const void* getMemberPointer(ubyte in_member) const;
	const void* getFacePointer() const;

	uint elementToFaceIndex(uint in_elementIndex) const;
	uint faceToElementIndex(uint in_faceIndex) const;
	uint faceToElementCount(uint in_faceIndex, uint in_nFaces) const;
	uint elementToFaceCount(uint in_elementIndex, uint in_nElements) const;

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
