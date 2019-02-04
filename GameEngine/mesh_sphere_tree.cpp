#include "mesh_sphere_tree.h"
#include "game_engine.h"

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshVertexData& in_meshData) :
	meshData(in_meshData),
	rootNode(nullptr) {
	
	if (in_meshData.Get_Member_Depth(MeshVertexData::MemberID::position) != n ||
		in_meshData.Get_Member_Type(MeshVertexData::MemberID::position) != MeshVertexData::To_Data_Type<T>() ||
		in_meshData.Get_Face_Mode() != MeshVertexData::FaceMode::triangles) {

		throw InvalidArgumentException();
	}

	Build_Tree();
}

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshSphereTree<T, n>& in_other) :
	meshData(in_other.meshData),
	rootNode(nullptr) {

	*this = in_other;
}

template<class T, uint n>
MeshSphereTree<T, n>::~MeshSphereTree() {
	Destroy_Tree();
}

template<class T, uint n>
void MeshSphereTree<T, n>::operator=(const MeshSphereTree& in_other) {
	Destroy_Tree();

	meshData = in_other.meshData;

	Node* otherNode = in_other.rootNode;
	if (otherNode == nullptr) return;

	Node* myNode = new Node;
	myNode->sphere = otherNode->sphere;
	myNode->triangles = otherNode->triangles;

	for (bool keepGoing = true; keepGoing;) {
		if (otherNode->children[0] == nullptr && otherNode->children[1] == nullptr) {
			if (myNode->parent == nullptr) {
				keepGoing = false;
			}
			else {
				myNode = myNode->parent;
				otherNode = otherNode->parent;
			}
		}
		else if (myNode->children[0] == nullptr) {
			myNode->children[0] = new Node;
			myNode = myNode->children[0];
			otherNode = otherNode->children[0];
			myNode->sphere = otherNode->sphere;
			myNode->triangles = otherNode->triangles;
		}
		else if (myNode->children[1] == nullptr) {
			myNode->children[1] = new Node;
			myNode = myNode->children[1];
			otherNode = otherNode->children[1];
			myNode->sphere = otherNode->sphere;
			myNode->triangles = otherNode->triangles;
		}
		else if (myNode->parent == nullptr) {
			keepGoing = false;
		}
		else {
			myNode = myNode->parent;
			otherNode = otherNode->parent;
		}
	}
}

template<class T, uint n>
void MeshSphereTree<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	meshData.Apply_Transform_Points(in_transform, MeshVertexData::MemberID::position);
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::Get_Iterator() const {
	return Iterator(*this);
}

template<class T, uint n>
void MeshSphereTree<T, n>::Build_Tree() {
	if (meshData.Get_Number_Faces() == 0) {
		throw InvalidArgumentException();
	}

	Destroy_Tree();

	uint nFaces = meshData.Get_Number_Faces();

	Triangle<T, n>* triangles = new Triangle<T, n>[nFaces];
	meshData.Expand_Member(MeshVertexData::MemberID::position, triangles);

	Vector<T, n>* means = new Vector<T, n>[nFaces];
	for (uint i = 0; i < nFaces; i++) {
		for (uint k = 0; k < 3; k++) {
			means[i] += triangles[i][k];
		}
		means[i] /= (T)3;
	}

	rootNode = new Node;
	rootNode->triangles.reserve(nFaces);
	for (uint i = 0; i < nFaces; i++) {
		rootNode->triangles.push_back(i);
	}

	std::vector<Node*> nodeStack;
	nodeStack.push_back(rootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		if (node->triangles.size() == 1) {
			node->sphere = Sphere<T, n>::From_Bounded_Triangle(triangles[node->triangles[0]]);
		}
		else {
			std::vector<Vector<T, n>> boundedPoints;
			boundedPoints.reserve(node->triangles.size() * 3);
			for (auto it = node->triangles.begin(); it != node->triangles.end(); it++) {
				for (uint i = 0; i < 3; i++) {
					boundedPoints.push_back(triangles[*it][i]);
				}
			}
			node->sphere = Sphere<T, n>::From_Bounded_Points(boundedPoints.data(), boundedPoints.size());

			Range<T> extremes[n];
			for (uint i = 0; i < n; i++) {
				extremes[i] = Range<T>(triangles[node->triangles[0]][0][i]);
			}
			for (uint i = 0; i < n; i++) {
				for (uint j = 0; j < node->triangles.size(); j++) {
					extremes[i].Expand_To(means[node->triangles[j]][i]);
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
				nodeStack.push_back(node->children[i]);
			}

			for (auto it = node->triangles.begin(); it != node->triangles.end(); it++) {
				if (means[*it][dimension] < midValue) {
					node->children[0]->triangles.push_back(*it);
				}
				else {
					node->children[1]->triangles.push_back(*it);
				}
			}

			if (node->children[0]->triangles.empty()) {
				node->children[0]->triangles.push_back(node->children[1]->triangles.back());
				node->children[1]->triangles.pop_back();
			}
			else if (node->children[1]->triangles.empty()) {
				node->children[1]->triangles.push_back(node->children[0]->triangles.back());
				node->children[0]->triangles.pop_back();
			}
		}
	}

	nodeStack.push_back(rootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		if (node->triangles.size() != 1) {
			node->triangles.clear();
			node->triangles.shrink_to_fit();
			for (uint i = 0; i < 2; i++) {
				nodeStack.push_back(node->children[i]);
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

	std::vector<Node*> nodeStack;
	nodeStack.push_back(rootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		for (uint i = 0; i < 2; i++) {
			if (node->children[i] != nullptr) {
				nodeStack.push_back(node->children[i]);
			}
		}
		
		delete node;
	}

	rootNode = nullptr;
}

template<class T, uint n>
MeshSphereTree<T, n>::Iterator::Iterator(const MeshSphereTree<T, n>& in_tree) :
	tree(in_tree),
	node(in_tree.rootNode) 
{}

template<class T, uint n>
void MeshSphereTree<T, n>::Iterator::Go_Left() {
	if (Is_Leaf()) {
		throw InvalidArgumentException();
	}

	node = node->children[0];
}

template<class T, uint n>
inline void MeshSphereTree<T, n>::Iterator::Go_Right() {
	if (Is_Leaf()) {
		throw InvalidArgumentException();
	}

	node = node->children[1];
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::Iterator::Go_Both() {
	Iterator other = *this;
	this->Go_Left();
	other.Go_Right();
	return other;
}

template<class T, uint n>
Sphere<T, n> MeshSphereTree<T, n>::Iterator::Get_Sphere() const {
	return node->sphere;
}

template<class T, uint n>
Triangle<T, n> MeshSphereTree<T, n>::Iterator::Get_Triangle() const {
	if (!Is_Leaf()) {
		throw InvalidArgumentException();
	}

	uint faceElementIndex = tree.meshData.Face_To_Element_Index(node->triangles[0]);
	uint vertexIndex = tree.meshData.Get_Standard_Face_Element(faceElementIndex);

	Triangle<T, n> out;

	for (uint i = 0; i < 3; i++) {
		tree.meshData.Get_Member_Value(MeshVertexData::MemberID::position, vertexIndex++, &out[i]);
	}

	return out;
}

template<class T, uint n>
bool MeshSphereTree<T, n>::Iterator::Is_Leaf() const {
	return node->children[0] == nullptr && node->children[0] == nullptr;
}

template class MeshSphereTree<float, 2>;
template class MeshSphereTree<double, 2>;