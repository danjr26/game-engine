#include "mesh_sphere_tree.h"
#include "game_engine.h"

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshVertexData& in_meshData) :
	mMeshData(in_meshData),
	mRootNode(nullptr) {
	
	if (in_meshData.Get_Member_Depth(MeshVertexData::MemberID::position) != n ||
		in_meshData.Get_Member_Type(MeshVertexData::MemberID::position) != MeshVertexData::To_Data_Type<T>() ||
		in_meshData.Get_Face_Mode() != MeshVertexData::FaceMode::triangles) {

		throw InvalidArgumentException();
	}

	Build_Tree();
}

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshSphereTree<T, n>& in_other) :
	mMeshData(in_other.mMeshData),
	mRootNode(nullptr) {

	*this = in_other;
}

template<class T, uint n>
MeshSphereTree<T, n>::~MeshSphereTree() {
	Destroy_Tree();
}

template<class T, uint n>
void MeshSphereTree<T, n>::operator=(const MeshSphereTree& in_other) {
	Destroy_Tree();

	mMeshData = in_other.mMeshData;

	Node* otherNode = in_other.mRootNode;
	if (otherNode == nullptr) return;

	Node* myNode = new Node;
	myNode->mSphere = otherNode->mSphere;
	myNode->mTriangles = otherNode->mTriangles;

	for (bool keepGoing = true; keepGoing;) {
		if (otherNode->mChildren[0] == nullptr && otherNode->mChildren[1] == nullptr) {
			if (myNode->mParent == nullptr) {
				keepGoing = false;
			}
			else {
				myNode = myNode->mParent;
				otherNode = otherNode->mParent;
			}
		}
		else if (myNode->mChildren[0] == nullptr) {
			myNode->mChildren[0] = new Node;
			myNode = myNode->mChildren[0];
			otherNode = otherNode->mChildren[0];
			myNode->mSphere = otherNode->mSphere;
			myNode->mTriangles = otherNode->mTriangles;
		}
		else if (myNode->mChildren[1] == nullptr) {
			myNode->mChildren[1] = new Node;
			myNode = myNode->mChildren[1];
			otherNode = otherNode->mChildren[1];
			myNode->mSphere = otherNode->mSphere;
			myNode->mTriangles = otherNode->mTriangles;
		}
		else if (myNode->mParent == nullptr) {
			keepGoing = false;
		}
		else {
			myNode = myNode->mParent;
			otherNode = otherNode->mParent;
		}
	}
}

template<class T, uint n>
const MeshVertexData& MeshSphereTree<T, n>::Get_Mesh_Data() const {
	return mMeshData;
}

template<class T, uint n>
void MeshSphereTree<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	mMeshData.Apply_Transform_Points(in_transform, MeshVertexData::MemberID::position);
	
	std::vector<Node*> nodeStack;
	nodeStack.reserve(mMeshData.Get_Number_Faces());
	nodeStack.push_back(mRootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		node->mSphere.Apply_Transform(in_transform);
		for (uint i = 0; i < 2; i++) {
			if (node->mChildren[i] != nullptr) {
				nodeStack.push_back(node->mChildren[i]);
			}
		}
	}
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::Get_Iterator() const {
	return Iterator(*this);
}

template<class T, uint n>
void MeshSphereTree<T, n>::Build_Tree() {
	if (mMeshData.Get_Number_Faces() == 0) {
		throw InvalidArgumentException();
	}

	Destroy_Tree();

	uint nFaces = mMeshData.Get_Number_Faces();

	Triangle<T, n>* triangles = new Triangle<T, n>[nFaces];
	mMeshData.Expand_Member(MeshVertexData::MemberID::position, triangles);

	Vector<T, n>* means = new Vector<T, n>[nFaces];
	for (uint i = 0; i < nFaces; i++) {
		for (uint k = 0; k < 3; k++) {
			means[i] += triangles[i][k];
		}
		means[i] /= (T)3;
	}

	mRootNode = new Node;
	mRootNode->mTriangles.reserve(nFaces);
	for (uint i = 0; i < nFaces; i++) {
		mRootNode->mTriangles.push_back(i);
	}

	std::vector<Node*> nodeStack;
	nodeStack.push_back(mRootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		if (node->mTriangles.size() == 1) {
			node->mSphere = Sphere<T, n>::From_Bounded_Triangle(triangles[node->mTriangles[0]]);
		}
		else {
			std::vector<Vector<T, n>> boundedPoints;
			boundedPoints.reserve(node->mTriangles.size() * 3);
			for (auto it = node->mTriangles.begin(); it != node->mTriangles.end(); it++) {
				for (uint i = 0; i < 3; i++) {
					boundedPoints.push_back(triangles[*it][i]);
				}
			}
			node->mSphere = Sphere<T, n>::From_Bounded_Points(boundedPoints.data(), boundedPoints.size());

			Range<T> extremes[n];
			for (uint i = 0; i < n; i++) {
				extremes[i] = Range<T>(triangles[node->mTriangles[0]][0][i]);
			}
			for (uint i = 0; i < n; i++) {
				for (uint j = 0; j < node->mTriangles.size(); j++) {
					extremes[i].Expand_To(means[node->mTriangles[j]][i]);
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
				node->mChildren[i] = new Node;
				node->mChildren[i]->mParent = node;
				nodeStack.push_back(node->mChildren[i]);
			}

			for (auto it = node->mTriangles.begin(); it != node->mTriangles.end(); it++) {
				if (means[*it][dimension] < midValue) {
					node->mChildren[0]->mTriangles.push_back(*it);
				}
				else {
					node->mChildren[1]->mTriangles.push_back(*it);
				}
			}

			if (node->mChildren[0]->mTriangles.empty()) {
				node->mChildren[0]->mTriangles.push_back(node->mChildren[1]->mTriangles.back());
				node->mChildren[1]->mTriangles.pop_back();
			}
			else if (node->mChildren[1]->mTriangles.empty()) {
				node->mChildren[1]->mTriangles.push_back(node->mChildren[0]->mTriangles.back());
				node->mChildren[0]->mTriangles.pop_back();
			}
		}
	}

	nodeStack.push_back(mRootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		if (node->mTriangles.size() != 1) {
			node->mTriangles.clear();
			node->mTriangles.shrink_to_fit();
			for (uint i = 0; i < 2; i++) {
				nodeStack.push_back(node->mChildren[i]);
			}
		}
	}

	delete[] triangles;
	delete[] means;
}

template<class T, uint n>
void MeshSphereTree<T, n>::Destroy_Tree() {
	if (mRootNode == nullptr) {
		return;
	}

	std::vector<Node*> nodeStack;
	nodeStack.push_back(mRootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		for (uint i = 0; i < 2; i++) {
			if (node->mChildren[i] != nullptr) {
				nodeStack.push_back(node->mChildren[i]);
			}
		}
		
		delete node;
	}

	mRootNode = nullptr;
}

template<class T, uint n>
MeshSphereTree<T, n>::Iterator::Iterator(const MeshSphereTree<T, n>& in_tree) :
	mTree(in_tree),
	mNode(in_tree.mRootNode) 
{}

template<class T, uint n>
void MeshSphereTree<T, n>::Iterator::Go_Left() {
	if (Is_Leaf()) {
		throw InvalidArgumentException();
	}

	mNode = mNode->mChildren[0];
}

template<class T, uint n>
inline void MeshSphereTree<T, n>::Iterator::Go_Right() {
	if (Is_Leaf()) {
		throw InvalidArgumentException();
	}

	mNode = mNode->mChildren[1];
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::Iterator::Go_Both() {
	Iterator other = *this;
	this->Go_Left();
	other.Go_Right();
	return other;
}

template<class T, uint n>
const Sphere<T, n>& MeshSphereTree<T, n>::Iterator::Get_Sphere() const {
	return mNode->mSphere;
}

template<class T, uint n>
Triangle<T, n> MeshSphereTree<T, n>::Iterator::Get_Triangle() const {
	if (!Is_Leaf()) {
		throw InvalidArgumentException();
	}

	uint faceElementIndex = mTree.mMeshData.Face_To_Element_Index(mNode->mTriangles[0]);
	uint vertexIndex = mTree.mMeshData.Get_Standard_Face_Element(faceElementIndex);

	Triangle<T, n> out;

	for (uint i = 0; i < 3; i++) {
		mTree.mMeshData.Get_Member_Value(MeshVertexData::MemberID::position, vertexIndex++, &out[i]);
	}

	return out;
}

template<class T, uint n>
bool MeshSphereTree<T, n>::Iterator::Is_Leaf() const {
	return mNode->mChildren[0] == nullptr && mNode->mChildren[0] == nullptr;
}

template class MeshSphereTree<float, 2>;
template class MeshSphereTree<double, 2>;