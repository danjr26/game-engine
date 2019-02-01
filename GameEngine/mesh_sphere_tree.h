#ifndef MESH_SPHERE_TREE_H
#define MESH_SPHERE_TREE_H

#include "mesh_sphere_tree.h"
#include "mesh_vertex_data.h"
#include "sphere.h"
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

private:
	MeshVertexData* meshData;
	Node* rootNode;

public:
	MeshSphereTree();
	MeshSphereTree(MeshVertexData& in_meshData);
	MeshSphereTree(MeshSphereTree& in_other);
	~MeshSphereTree();

	void Build_Around(MeshVertexData& in_meshData);

private:
	void Build_Tree();
	void Destroy_Tree();
};

#endif


