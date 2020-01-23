#ifndef MESH_SPHERE_TREE_H
#define MESH_SPHERE_TREE_H

#include "mesh_sphere_tree.h"
#include "mesh_vertex_data.h"
#include "sphere.h"
#include "collider_basis.h"
#include <vector>

template<class T, uint n>
class MeshSphereTree : public ColliderBasis<T, n> {
private:
	struct Node {
		Sphere<T, n> mSphere;
		std::vector<uint> mTriangles;
		
		Node* mParent;
		Node* mChildren[2];
	};

	struct MiniNode {
		Sphere<T, n> mSphere;
		union {
			MiniNode* mChildren[2];
			uint mTriangle;
		};
		bool mIsLeaf;
	};

public:
	class Iterator {
		friend class MeshSphereTree;
	private:
		const MeshSphereTree<T, n>& mTree;
		const Node* mNode;

	public:
		Iterator(const MeshSphereTree<T, n>& i_tree);

		void goLeft();
		void goRight();
		Iterator goBoth();

		const Sphere<T, n>& getSphere() const;
		Triangle<T, n> getTriangle() const;
		bool isLeaf() const;
	};

private:
	MeshVertexData mMeshData;
	Node* mRootNode;

public:
	explicit MeshSphereTree(const MeshVertexData& i_meshData);
	MeshSphereTree(const MeshSphereTree<T, n>& i_other);
	~MeshSphereTree();

	void operator=(const MeshSphereTree& i_other);

	const MeshVertexData& getMeshData() const;

	void applyTransform(const Transform<T, n>& i_transform);

	Iterator getIterator() const;

private:
	void buildTree();
	void destroyTree();
};

using MeshSphereTree2f = MeshSphereTree<float, 2>;
using MeshSphereTree2d = MeshSphereTree<double, 2>;

#endif


