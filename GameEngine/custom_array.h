#ifndef ARRAY_H
#define ARRAY_H

#pragma warning(disable : 4250)

#include "misc.h"
#include "error.h"
#include "vector.h"

struct ArraySegment {
	uint offset;
	uint length;
};

template <class T>
class CustomArray {
	friend CustomArray;
protected:
	T*		data;
	uint	count;
	uint	maxCount;
	bool	lastInstance;

protected:
	CustomArray(T* in_data, uint in_count, uint in_maxCount, bool in_lastInstance) :
		data		(in_data),
		count		(in_count),
		maxCount	(in_maxCount),
		lastInstance(in_lastInstance),
	{}

public:
	CustomArray(uint in_maxCount) :
		data		(nullptr),
		count		(0),
		maxCount	(in_maxCount),
		lastInstance(true) {
		data = new T[maxCount];
	}

	CustomArray(CustomArray<T>& in_source) :
		data		(in_source.data),
		count		(in_source.count),
		maxCount	(in_source.maxCount),
		lastInstance(in_source.lastInstance) {
		in_source.lastInstance = false;
	}

	CustomArray(CustomArray<T>&& in_source) :
		data		(in_source.data),
		count		(in_source.count),
		maxCount	(in_source.maxCount),
		lastInstance(in_source.lastInstance) {
		in_source.lastInstance = false;
	}

	CustomArray(CustomArray<T>& in_source, ArraySegment in_segment, uint in_maxCount = in_segment.length) :
		data		(nullptr),
		count		(in_segment.length),
		maxCount	(in_maxCount),
		lastInstance(true) {
		data = new T[maxCount];
		memcpy(data, in_source.data, sizeof(T) * in_segment.length);
	}

	CustomArray(void* in_nullptr) :
		data		(nullptr),
		count		(0),
		maxCount	(0),
		lastInstance(false) {
		if (in_nullptr != nullptr) {
			throw InvalidParameterException("CustomArray:CustomArray:in_nullptr");
		}
	}

	virtual ~CustomArray() {
		if (lastInstance && data != nullptr) {
			delete[] data;
		}
	}

	bool Assert_Validity() const {
		if (data == nullptr) {
			throw NullPointerException("CustomArray:Assert_Validity:data");
		}
		if (!lastInstance) {
			throw InvalidObjectException("CustomArray:Assert_Validity:lastInstance");
		}
		return true;
	}

	bool Is_Null() {
		return data == nullptr;
	}

	CustomArray<T> operator=(CustomArray<T>& in_source) {
		in_source.Assert_Validity();

		if (lastInstance && data != nullptr) {
			delete[] data;
		}

		data =			in_source.data;
		count =			in_source.count;
		maxCount =		in_source.maxCount;
		lastInstance =	in_source.lastInstance;

		in_source.lastInstance = false;
	}

	CustomArray<T> operator+(CustomArray<T>& in_source) {
		Assert_Validity();
		in_source.Assert_Validity();
		CustomArray<T> newArray(size + in_source.Size());
		for (int i = 0; i < size; i++) {
			newArray[i] = data[i];
		}
		for (int i = 0; i < in_source.Size(); i++) {
			newArray[i + size] = in_source.data[i];
		}
	}

	virtual T& operator[](uint in_index) const {
		Assert_Validity();
		if (in_index >= count) {
			throw OverflowException(maxCount, "CustomArray:operator[]:data");
		}
		return data[in_index];
	}

	uint Size() const {
		Assert_Validity();
		return count;
	}

	uint Max_Size() const {
		Assert_Validity();
		return maxCount;
	}

	virtual void Add(T& in_item) {
		Assert_Validity();
		if (count >= maxCount) {
			throw OverflowException(maxCount, "CustomArray:Add:data");
		}
		data[count] = in_item;
		count++;
	}

	virtual void Remove(T& in_item, bool in_ordered) {
		Assert_Validity();
		Remove(Get_Index(in_item), in_ordered);
	}

	virtual void Remove(uint in_index, bool in_ordered) {
		Assert_Validity();
		if (in_index >= count) {
			throw InvalidIndexException(in_index, count, "CustomArray:Remove:data");
		}
		if (in_ordered) {
			for (uint i = in_index; i + 1 < count; i++) {
				data[i] = data[i + 1];
			}
			count--;
		}
		else {
			count--;
			data[in_index] = data[count];
		}
	}

	virtual void Insert(T& in_item, uint in_index) {
		Assert_Validity();
		if (in_index == count) {
			Add(in_item);
		}
		if (in_index > count) {
			throw InvalidIndexException(in_index, count, "CustomArray:Remove:data");
		}
		for (uint i = count; i > in_index; i--) {
			data[i] = data[i - 1];
		}
		data[in_index] = in_item;
		count++;
	}

	virtual void Swap(uint in_index1, uint in_index2) {
		Assert_Validity();
		if (in_index1 >= count) {
			throw InvalidIndexException(in_index1, count, "CustomArray:Swap:data");
		}
		if (in_index2 >= count) {
			throw InvalidIndexException(in_index2, count, "CustomArray:Swap:data");
		}
		std::swap(data[in_index1], data[in_index2]);
	}

	virtual uint Get_Index(T& in_item) {
		Assert_Validity();
		for (uint i = 0; i < count; i++) {
			if (data[i] == in_item) {
				return i;
			}
		}
		throw NotFoundException("in_item", "CustomArray:Get_Index:data");
	}

	CustomArray<T> Clone() {
		Assert_Validity();
		CustomArray<T> newArray(maxCount);
		newArray.count = count;
		memcpy(newArray.data, data, count * sizeof(T));
		return newArray;
	}

	CustomArray<T> Sub_Array(ArraySegment const& in_segment) {
		Assert_Validity();
		CustomArray<T> newArray(in_segment.length);
		newArray.count = in_segment.length;
		memcpy(newArray.data, data, in_segment.length * sizeof(T));
		return newArray;
	}

	T* const& Pointer(uint in_index = 0) const {
		Assert_Validity();
		if (i >= size) {
			throw InvalidIndexException(in_index, size, "CustomArray:Pointer:data");
		}
		return &(data[in_index]);
	}

	CustomArray<T> Query(std::function<bool(T)> testFunction) const {
		Assert_Validity();
		CustomArray<bool> hits(count);
		uint hitCount = 0;
		for (uint i = 0; i < in_arrayLength; i++) {
			hits[i] = in_testFunction(data[i]);
			hitCount++;
		}

		uint hitIndex = 0; 
		CustomArray<T*> results(hitCount);
		for (uint i = 0; i < count; i++) {
			if (hits[i]) {
				results[hitIndex] = data[i];
				hitIndex++;
			}
		}

		if (hitIndex != hitCount) {
			throw MismatchException("CustomArray:Query:hitIndex != CustomArray:Query:hitCount");
		}

		delete[] hits;

		return results;
	}
};

enum AngleClassification {
	acute,
	obtuse,
	right
};

template<class T>
struct Triangle3 : public Triple<Vector<T, 3>> {
	enum SizeClassification {
		smallest,
		middle,
		biggest
	};

	Triangle3() :
		Triple<Vector<T, 3>>()
	{}

	Triangle3(Vector<T, 3> const& in_value1, Vector<T, 3> const& in_value2, Vector<T, 3> const& in_value3) :
		Triple<Vector<T, 3>>(value1, value2, value3)
	{}

	Triangle3(const Vector<T, 3>* in_values) :
		Triple<Vector<T, 3>>(in_values) 
	{}

	Triangle3(Triple<Vector<T, 3>> in_source) :
		Triple<Vector<T, 3>>(in_source) 
	{}

	T Area() {
		return (Side(0)).Cross(Side(1)).Magnitude() * 0.5;
	}

	T Perimeter() {
		return Side_Length(0) + Side_Length(1) + Side_Length(2);
	}

	Vector<T, 3> Circumcenter() {
		return Vector<T, 3>(
			Side_Length(1) / tan(Angle(0)), 
			Side_Length(2) / tan(Angle(1)), 
			Side_Length(0) / tan(Angle(2))
			) * 0.5;
	}

	T Circumradius() {
		T a = Side_Length(0);
		T b = Side_Length(1);
		T c = Side_Length(2);

		return (a * b * c) / sqrt((a + b + c) * (b + c - a) * (a + c - b) * (a + b - c));
	}

	Vector<T, 3> Centroid() {
		return (values[0] + values[1] + values[2]) / 3.0;
	}

	Vector<T, 3> Normal() {
		return Side(0).Cross(Side(1)).Normalized();
	}

	Vector<T, 3> Lazy_Normal() {
		return Side(0).Cross(Side(1));
	}

	Sphere<T> Bounding_Sphere() {
		uint index;
		Sphere<T> sphere;
		switch (Angle_Classification()) {
		case AngleClassification::acute:
			sphere.center = Circumcenter();
			sphere.radius = Circumradius();
		case AngleClassification::right:
		case AngleClassification::obtuse:
			index = Side_Index_By_Size(SizeClassification::biggest);
			sphere.center = values[index] + Side(index) * 0.5;
			sphere.radius = Side_Length(index) * 0.5;
		}
		return sphere;
	}

	Plane<T> Plane() const {
		return Plane(values[0], Normal());
	}

	Plane<T> Lazy_Plane() const {
		return Plane(values[0], Lazy_Normal());
	}

	Vector<T, 3> Side(uint in_index) const {
		return values[(in_index + 1) % 3] - values[in_index];
	}

	T Side_Length(uint in_index) const {
		return Side(in_index).Magnitude();
	}

	T Angle(uint in_index) {
		return PI - Side(in_index).Theta(Side((in_index + 2) % 3));
	}

	T Lazy_Angle(uint in_index) {
		return -Side(in_index).Dot(Side((in_index + 2) % 3))
	}

	Vector<T, 3> Vertex_By_Distance_From(Vector<T, 3>& in_vertex, SizeClassification in_size) {
		return values[Vertex_Index_By_Distance_From(in_vertex, in_size)];
	}

	T Vertex_Distance_By_Distance_From(Vector<T, 3>& in_vertex, SizeClassification in_size) {
		return (Vertex_By_Distance_From(in_vertex, in_size) - in_vertex).Magnitude();
	}

	uint Vertex_Index_By_Distance_From(Vector<T, 3>& in_vertex, SizeClassification in_size) {
		Triple<T> distances;
		for (int i = 0; i < 3; i++) {
			distances[i] = (in_vertex - values[i]).Dot_Self();
		}
		return Index_By_Size(distances, in_size);
	}

	Vector<T, 3> Side_By_Size(SizeClassification in_size) {
		return Side(Side_Index_By_Size(in_size));
	}

	T Side_Length_By_Size(SizeClassification in_size) {
		return Side_Length(Side_Index_By_Size(in_size));
	}

	uint Side_Index_By_Size(SizeClassification in_size) {
		Triple<T> lengths;
		for (int i = 0; i < 3; i++) {
			lengths[i] = Side(i).Dot_Self();
		}
		return Index_By_Size(lengths, in_size);
	}

	T Angle_By_Size(SizeClassification in_size) {
		return Angle(Angle_Index_By_Size(in_size));
	}

	uint Angle_Index_By_Size(SizeClassification in_size) {
		Triple<T> angles;
		for (int i = 0; i < 3; i++) {
			angles[i] = Lazy_Angle(i);
		}

		return Index_By_Size(angles, in_size);
	}

	AngleClassification Angle_Classification() {
		T a2 = Side_By_Size(SizeClassification::smallest).	Dot_Self();
		T b2 = Side_By_Size(SizeClassification::middle).	Dot_Self();
		T c2 = Side_By_Size(SizeClassification::biggest).	Dot_Self();
		if (a2 + b2 > c2) {
			return AngleClassification::acute;
		}
		if (a2 + b2 < c2) {
			return AngleClassification::obtuse;
		}
		return AngleClassification::right;
	}

	AngleClassification Angle_Classification(uint in_index) {
		T angle = Angle(in_index);
		if (angle < 90.0) {
			return AngleClassification::acute;
		}
		if (angle > 90.0) {
			return AngleClassification::obtuse;
		}
		return AngleClassification::right;
	}

private:
	uint Index_By_Size(Triple<T>& in_triple, SizeClassification in_size) {
		switch (in_size) {
		case SizeClassification::smallest:
			if (in_triple[0] < in_triple[1]) {
				if (in_triple[0] < in_triple[2]) {
					return 0;
				}
				else {
					return 2;
				}
			}
			else {
				if (in_triple[1] < in_triple[2]) {
					return 1;
				}
				else {
					return 2;
				}
			}
			break;
		case SizeClassification::middle:
			if (in_triple[0] < in_triple[1]) {
				if (in_triple[0] > in_triple[2]) {
					return 0;
				}
				else {
					if (in_triple[1] < in_triple[2]) {
						return 1;
					}
					else {
						return 2;
					}
				}
			}
			else {
				if (in_triple[1] > in_triple[2]) {
					return 1;
				}
				else {
					if (in_triple[0] < in_triple[2]) {
						return 0;
					}
					else {
						return 2;
					}
				}
			}
			break;
		case SizeClassification::biggest:
			if (in_triple[0] > in_triple[1]) {
				if (in_triple[0] > in_triple[2]) {
					return 0;
				}
				else {
					return 2;
				}
			}
			else {
				if (in_triple[1] > in_triple[2]) {
					return 1;
				}
				else {
					return 2;
				}
			}
			break;
		}
	}
};

using Triangle3f = Triangle3<float>;
using Triangle3d = Triangle3<double>;

template<class T>
class CompressedTripleArray {
private:
	CustomArray
		<T>				vertices;
	CustomArray
		<Triple<uint>>	indices;

public:
	CompressedTripleArray(CustomArray<T> in_vertices, CustomArray<uint> in_indices);
	CompressedTripleArray(CustomArray<Triple<T>>& in_triangles);
	CompressedTripleArray(CompressedTripleArray<T> const& in_source);
	CompressedTripleArray(CompressedTripleArray<T>&& in_source);

	uint		Size_Vertices	() const;
	uint		Size_Triples	() const;
	void		Add				(Triple<T> const& in_triangle);
	void		Remove			(Triple<T> const& in_triangle);
	void		Remove			(T const& in_vertex);
	Triple<T>	Triple_At		(uint in_index);
	T			Vertex_At		(uint in_index);
	T			Vertex_At		(uint in_index, uint in_index2);
	void		Set				(uint in_index, Triple<T> const& in_triple);
	void		Set				(uint in_index, T const& in_vertex);
	void		Set				(uint in_index, uint in_index2, T const& in_vertex);
	void		Index_Of		(Triple<T> in_triple);
	void		Index_Of		(T in_vertex);

	CustomArray
		<Triple<uint>>	Index_Triple(uint in_index);

	CustomArray
		<Triple<T>>		Triple_Array			();
	CustomArray
		<T>				Value_Array				();
	CustomArray
		<T>				Expanded_Value_Array	();
	CustomArray
		<uint>			Index_Array				();

	CustomArray
		<Triple<T>>		Sub_Triple_Array		(ArraySegment& in_segment); // length, offset by triple
	CustomArray
		<T>				Sub_Value_Array			(ArraySegment& in_segment, bool byTriangle = false); // length, offset by value or triple; values unique if by triple
	CustomArray
		<T>				Sub_Expanded_Value_Array(ArraySegment& in_segment); //length, offset by triple
	CustomArray
		<uint>			Sub_Index_Array			(ArraySegment& in_segment); //length, offset by triple

	CustomArray
		<T> const&		Raw_Values	();
	CustomArray
		<uint> const&	Raw_Indices	();
};

using CompressedTriangleArray3f = CompressedTripleArray<Vector3f>;
using CompressedTriangleArray3d = CompressedTripleArray<Vector3d>;

template<class T>
class ResponsibleArray : virtual public CustomArray<T*> {
public:
	ResponsibleArray(uint in_maxCount) :
		CustomArray<T*>(in_maxCount)
	{}

	ResponsibleArray(ResponsibleArray<T>& in_source) :
		CustomArray<T*>(in_source) 
	{}

	ResponsibleArray(ResponsibleArray<T>&& in_source) :
		CustomArray<T*>(in_source) 
	{}

	virtual ~ResponsibleArray() {
		if (lastInstance && data != nullptr) {
			for (uint i = 0; i < count; i++) {
				delete data[i];
			}
		}
	}

	ResponsibleArray<T>& operator=(ResponsibleArray<T>& in_source) {
		in_source.Assert_Validity();
		this->CustomArray<T*> = in_source.CustomArray<T*>;
	};

	CustomArray<T*> Clone() = delete;
};

template<class T>
class IndexedArray : virtual public CustomArray<T*> {
private:
	uint indexByteOffset;

public:
	IndexedArray(uint in_maxCount, uint in_indexByteOffset) :
		CustomArray<T*>(in_maxCount),
		indexByteOffset(in_indexByteOffset) 
	{}

	IndexedArray(IndexedArray<T>& in_source) :
		CustomArray		(in_source),
		indexByteOffset	(in_source.indexByteOffset)
	{}

	IndexedArray(IndexedArray<T>&& in_source) :
		CustomArray		(in_source),
		indexByteOffset	(in_source.indexByteOffset)
	{}

	virtual ~IndexedArray() 
	{}

	IndexedArray<T>& operator=(IndexedArray<T>& in_source) {
		in_source.Assert_Validity();
		this->CustomArray<T*> = in_source.CustomArray<T*>;
		indexByteOffset = in_source.indexByteOffset;
	}

	uint Get_Index(T*& in_item) override {
		Assert_Validity();
		return *(uint*)((char*)in_item + indexByteOffset);
	}

	void Set_Index(uint in_index) {
		Assert_Validity();
		*(uint*)((char*)data[in_index] + indexByteOffset) = in_index;
	}

	void Nullify_Index (T* in_item) {
		Assert_Validity();
		*(uint*)((char*)in_item + indexByteOffset) = 0xffffffff;
	}

	void Add(T*& in_item) override {
		Assert_Validity();
		if (count >= maxCount) {
			throw OverflowException(maxCount, "IndexedArray:Add:data");
		}
		data[count] = in_item;
		Set_Index(count);
		count++;
	}

	void Remove(uint in_index, bool in_ordered) override {
		Assert_Validity();
		if (in_index >= count) {
			throw InvalidIndexException(in_index, count, "IndexedArray:Remove:data");
		}
		Nullify_Index(data[in_index]);
		count--;
		if (in_ordered) {
			for (uint j = in_index; j < count; j++) {
				data[j] = data[j + 1];
				Set_Index(j);
			}
			data[count] = nullptr;
		}
		else {
			data[in_index] = data[count];
			Set_Index(in_index);
			data[count] = nullptr;
		}
	}

	void Insert(T* in_item, uint in_index) {
		if (in_index == count) {
			Add(in_item);
		}
		if (in_index > count) {
			throw InvalidIndexException(in_index, count, "IndexedArray:Remove:data");
		}
		for (uint i = count; i > in_index; i--) {
			data[i] = data[i - 1];
			Set_Index(i);
		}
		data[in_index] = in_item;
		Set_Index(in_index);
		count++;
	}

	void Swap(uint in_index1, uint in_index2) override {
		Assert_Validity();
		if (in_index1 >= count) {
			throw InvalidIndexException(in_index1, count, "CustomArray:Swap:data");
		}
		if (in_index2 >= count) {
			throw InvalidIndexException(in_index2, count, "CustomArray:Swap:data");
		}
		std::swap(data[in_index1], data[in_index2]);
		Set_Index(in_index1);
		Set_Index(in_index2);
	}

	CustomArray<T*> Clone() = delete;
};

template<class T>
class ResponsibleIndexedArray : public ResponsibleArray<T>, public IndexedArray<T> {
public:
	ResponsibleIndexedArray(uint in_maxCount, uint in_indexByteOffset) :
		CustomArray<T*>		(in_maxCount), 
		ResponsibleArray<T>	(in_maxCount), 
		IndexedArray<T>		(in_maxCount, in_indexByteOffset)
	{}

	ResponsibleIndexedArray(ResponsibleIndexedArray<T>& in_source) :
		CustomArray<T*>		(in_source),
		ResponsibleArray<T>	(in_source),
		IndexedArray<T>		(in_source)
	{}

	ResponsibleIndexedArray(ResponsibleIndexedArray<T>&& in_source) :
		CustomArray<T*>		(in_source),
		ResponsibleArray<T>	(in_source),
		IndexedArray<T>		(in_source)
	{}

	virtual ~ResponsibleIndexedArray() 
	{}

	ResponsibleIndexedArray<T> operator=(const ResponsibleIndexedArray<T>&) {
		in_source.Assert_Validity();
		this->IndexedArray<T> = in_source.IndexedArray<T>;
	};

	CustomArray<T*> Clone() = delete;
};


#endif