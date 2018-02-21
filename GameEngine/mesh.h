#ifndef MESH_H
#define MESH_H

#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include "vectors.h"
#include "resources.h"
#include "shader.h"
#include "texture.h"

class Mesh;
class Texture2D;

struct MMLElement {
	string title;
	string body;
	std::map<uint, MMLElement> children;
	std::map<string, string> attributes;
};

void Parse_MML_String(MMLElement& inout_element, string in_string);

struct Material {
public:
	string					name;
	Color4f					ambient;	// alpha is (0.0) ? normal lighting : shadeless emission only
	Color4f					diffuse;	// alpha is material base alpha
	Color4f					specular;	// alpha is specular shininess
	Color4f					emission;	// alpha is useless for now
	ResponsibleArray
		<TextureInstance2D>	textures;
	BlendSettings			blendSettings;

	explicit	Material();
				Material(string in_name, Color4f in_ambient, Color4f in_diffuse, Color4f in_specular, 
		Color4f in_emission, BlendSettings in_blendSettings = BlendSettings());
};

template<class T>
class Plane {
public:
	enum Side {
		front,
		on,
		back
	};
private:
	T pointDot;
	Vector3<T> axis;

public:
	Plane(Vector3<T> in_point, Vector3<T> in_axis) :
	pointDot(in_point),
	axis	(in_axis)
	{}

	Side Which_Side(Vector3<T> in_point) {
		T thatDot = axis.Dot(in_point);

		if (thatDot > pointDot) {
			return Side::front;
		}
		if (thatDot < pointDot) {
			return Side::back;
		} 
		return Side::on;
	}

	T Signed_Distance_To(Vector3<T> in_point) {
		return axis.Dot(in_point) - pointDot;
	}

	T Unsigned_Distance_To(Vector3<T> in_point) {
		return abs(axis.Dot(in_point) - pointDot);
	}

	template<class T>
	Vector3<T> Coincidence(Plane<T> in_plane) {
		return axis.Cross(in_plane.axis);
	}
};

using Planef = Plane<float>;
using Planed = Plane<double>;

template<class T>
struct Sphere {
	Vector3<T>	center;
	T			radius;
};

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

class SphereNode {
private:
	Vector3f	localCenter;
	Vector3f	worldCenter;
	float		radius;

	uint		lastCenterUpdate;

	bool		isLeaf;

	union {
		struct {
			SphereNode*		child1;
			SphereNode*		child2;
		} branch;
		struct {
			uint		lastTriangleUpdate;
			Triangle3f	localTriangle;
			Triangle3f	worldTriangle;
		} leaf;
	} end;
	
public:
	SphereNode();
	SphereNode(Spheref in_sphere, SphereNode* in_child1, SphereNode* in_child2);
	SphereNode(Triangle3f& in_triangle);

	bool				Is_Leaf			() const;
	Vector3f const&		Local_Center	() const;
	Vector3f const&		World_Center	() const;
	float				Radius			() const;
	SphereNode*			Child_1			() const;
	SphereNode*			Child_2			() const;
	Triangle3f const&	Local_Triangle	() const;
	Triangle3f const&	World_Triangle	() const;

	void Update_Center	(Transform3d const& in_transform, uint in_uniqueCallID);
	void Update_Triangle(Transform3d const& in_transform, uint in_uniqueCallID);

	static CustomArray<SphereNode> Create_Tree(CompressedTriangleArray3f& in_triangles);
	static Spheref Welzl(CustomArray<Vector3f>& in_points, CustomArray<Vector3f>& in_borderPoints);
};

class Mesh : public Resource {
public:
	enum Buffers {
		vertex,
		normal,
		uv,
		materialIndex,
		vertexIndex,
		count
	};
private:
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint normalBufferID;
	GLuint uvBufferID;
	GLuint materialIndexBufferID;
	GLuint indexBufferID;
	CustomArray
		<Material> materials;
	uint nVertices;
	Flags renderFlags;
	
public:			
	Mesh(CustomArray<Vector3f>& in_vertices, CustomArray<Vector3f>& in_normals, CustomArray<Vector3f>& in_uvs, CustomArray<uchar>& in_materialIndices, 
		CustomArray<Triple<ushort>>& in_indices, CustomArray<Material>& in_materials, Flags in_renderFlags, string in_name);
	~Mesh();

	void Render (Flags callflags);

	static CustomArray<Mesh*> Load_Mesh_File(string in_filePath, string in_fileName);
};

struct RawMesh {
	Vector3f* vertices;
	Vector3f* norms;
	Vector2f* texcs;
	int ntris;
};

RawMesh	Ico_Sphere(int nsubdiv, float radius, bool calcnorms, bool calcuvs);

#endif
