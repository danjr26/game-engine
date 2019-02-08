#ifndef MESH_SPHERE_TREE_H
#define MESH_SPHERE_TREE_H

#include "mesh_sphere_tree.h"
#include "mesh_vertex_data.h"
#include "sphere.h"
#include "transform.h"
#include <vector>

template<class T, uint n>
class MeshSphereTree {
private:
	struct Node {
		Sphere<T, n> sphere;
		std::vector<uint> triangles;
		
		Node* parent;
		Node* children[2];
	};

	struct MiniNode {
		Sphere<T, n> sphere;
		union {
			MiniNode* children[2];
			uint triangle;
		};
		bool isLeaf;
	};

public:
	class Iterator {
		friend class MeshSphereTree;
	private:
		const MeshSphereTree<T, n>& tree;
		const Node* node;

	public:
		Iterator(const MeshSphereTree<T, n>& in_tree);

		void Go_Left();
		void Go_Right();
		Iterator Go_Both();

		const Sphere<T, n>& Get_Sphere() const;
		Triangle<T, n> Get_Triangle() const;
		bool Is_Leaf() const;
	};

private:
	MeshVertexData meshData;
	Node* rootNode;

public:
	explicit MeshSphereTree(const MeshVertexData& in_meshData);
	MeshSphereTree(const MeshSphereTree<T, n>& in_other);
	~MeshSphereTree();

	void operator=(const MeshSphereTree& in_other);

	const MeshVertexData& Get_Mesh_Data() const;

	void Apply_Transform(const Transform<T, n>& in_transform);

	Iterator Get_Iterator() const;

private:
	void Build_Tree();
	void Destroy_Tree();
};

using MeshSphereTree2f = MeshSphereTree<float, 2>;
using MeshSphereTree2d = MeshSphereTree<double, 2>;

#endif


