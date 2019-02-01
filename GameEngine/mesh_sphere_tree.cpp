#include "mesh_sphere_tree.h"
#include "game_engine.h"

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree() :
	meshData(nullptr),
	rootNode(nullptr)
{}

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(MeshVertexData& in_meshData) :
	meshData(nullptr),
	rootNode(nullptr) {
	
	Build_Around(in_meshData);
}

template<class T, uint n>
MeshSphereTree<T, n>::~MeshSphereTree() {
	Destroy_Tree();
}

template<class T, uint n>
void MeshSphereTree<T, n>::Build_Around(MeshVertexData& in_meshData) {
	if (in_meshData.Get_Member_Depth(MeshVertexData::MemberID::position) != n ||
		in_meshData.Get_Member_Type(MeshVertexData::MemberID::position) != MeshVertexData::To_Data_Type<T>() ||
		in_meshData.Get_Face_Mode() != MeshVertexData::FaceMode::triangles) {

		throw InvalidArgumentException();
	}

	meshData = &in_meshData;

	Build_Tree();
}

template<class T, uint n>
void MeshSphereTree<T, n>::Build_Tree() {
	if (meshData == nullptr || meshData->Get_Number_Faces() == 0) {
		throw InvalidArgumentException();
	}

	Destroy_Tree();

	uint nFaces = meshData->Get_Number_Faces();

	Triangle<T, n>* triangles = new Triangle<T, n>[nFaces];
	meshData->Expand_Member(MeshVertexData::MemberID::position, triangles);

	Vector<T, n>* means = new Vector<T, n>[nFaces];
	for (uint i = 0; i < nFaces; i++) {
		for (uint k = 0; k < 3; k++) {
			means[i] += triangles[i][k];
		}
		means[i] /= (T)3;
	}

	Node* rootNode = new Node;
	rootNode->triangles.reserve(nFaces);
	for (uint i = 0; i < nFaces; i++) {
		rootNode->triangles.push_back(i);
	}

	Node* node = rootNode;
	while (node->children[0] == nullptr || node->children[1] == nullptr || node->parent != nullptr) {
		if (node->triangles.size() == 1) {
			rootNode->sphere = Sphere<T, n>::From_Bounded_Triangle(triangles[node->triangles[0]]);
			node = node->parent;
		}
		else {
			Range<T> extremes[n];
			for (uint i = 0; i < n; i++) {
				extremes[i] = Range<T>(triangles[node->triangles[0]][0][i]);
			}
			for (uint i = 0; i < n; i++) {
				for (uint j = 0; j < nFaces; j++) {
					extremes[i].Expand_To(means[node->triangles[j][i]);
				}
			}
			uint dimension = 0;
			T maxSpan = 0;
			for (uint i = 0; i < n; i++) {
				if (extremes[i].Get_Span() > maxSpan) {
					maxSpan = extremes[i].Get_Span();
					dimension = i;
				}
			}
			T midValue = extremes[dimension].Get_Mean();

			for (uint i = 0; i < 2; i++) {
				node->children[i] = new Node;
				node->children[i]->parent = node;
			}

			for (auto it = node->triangles.begin(); it != node->triangles.end(); it++) {

			}
		}
	}

	delete[] triangles;
	delete[] means;
}

template<class T, uint n>
void MeshSphereTree<T, n>::Destroy_Tree() {
	if (rootNode == nullptr) {
		return;
	}


}

template class MeshSphereTree<float, 2>;
template class MeshSphereTree<double, 2>;