#include "mesh_sphere_tree.h"
#include "game_engine.h"

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshVertexData& in_meshData) :
	mMeshData(in_meshData),
	mRootNode(nullptr) {
	
	if (in_meshData.getMemberDepth(MeshVertexData::MemberID::position) != n ||
		in_meshData.getMemberType(MeshVertexData::MemberID::position) != MeshVertexData::toDataType<T>() ||
		in_meshData.getFaceMode() != MeshVertexData::FaceMode::triangles) {

		throw InvalidArgumentException();
	}

	buildTree();
}

template<class T, uint n>
MeshSphereTree<T, n>::MeshSphereTree(const MeshSphereTree<T, n>& in_other) :
	mMeshData(in_other.mMeshData),
	mRootNode(nullptr) {

	*this = in_other;
}

template<class T, uint n>
MeshSphereTree<T, n>::~MeshSphereTree() {
	destroyTree();
}

template<class T, uint n>
void MeshSphereTree<T, n>::operator=(const MeshSphereTree& in_other) {
	destroyTree();

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
const MeshVertexData& MeshSphereTree<T, n>::getMeshData() const {
	return mMeshData;
}

template<class T, uint n>
void MeshSphereTree<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	mMeshData.applyTransformPoints(in_transform, MeshVertexData::MemberID::position);
	
	std::vector<Node*> nodeStack;
	nodeStack.reserve(mMeshData.getNumberFaces());
	nodeStack.push_back(mRootNode);
	while (!nodeStack.empty()) {
		Node* node = nodeStack.back();
		nodeStack.pop_back();

		node->mSphere.applyTransform(in_transform);
		for (uint i = 0; i < 2; i++) {
			if (node->mChildren[i] != nullptr) {
				nodeStack.push_back(node->mChildren[i]);
			}
		}
	}
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::getIterator() const {
	return Iterator(*this);
}

template<class T, uint n>
void MeshSphereTree<T, n>::buildTree() {
	if (mMeshData.getNumberFaces() == 0) {
		throw InvalidArgumentException();
	}

	destroyTree();

	uint nFaces = mMeshData.getNumberFaces();

	Triangle<T, n>* triangles = new Triangle<T, n>[nFaces];
	mMeshData.expandMember(MeshVertexData::MemberID::position, triangles);

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
			node->mSphere = Sphere<T, n>::fromBoundedTriangle(triangles[node->mTriangles[0]]);
		}
		else {
			std::vector<Vector<T, n>> boundedPoints;
			boundedPoints.reserve(node->mTriangles.size() * 3);
			for (auto it = node->mTriangles.begin(); it != node->mTriangles.end(); it++) {
				for (uint i = 0; i < 3; i++) {
					boundedPoints.push_back(triangles[*it][i]);
				}
			}
			node->mSphere = Sphere<T, n>::fromBoundedPoints(boundedPoints.data(), boundedPoints.size());

			Range<T> extremes[n];
			for (uint i = 0; i < n; i++) {
				extremes[i] = Range<T>(triangles[node->mTriangles[0]][0][i]);
			}
			for (uint i = 0; i < n; i++) {
				for (uint j = 0; j < node->mTriangles.size(); j++) {
					extremes[i].expandTo(means[node->mTriangles[j]][i]);
				}
			}
			uint dimension = 0;
			T maxSpan = 0;
			for (uint i = 0; i < n; i++) {
				if (extremes[i].getSpan() > maxSpan) {
					maxSpan = extremes[i].getSpan();
					dimension = i;
				}
			}
			T midValue = extremes[dimension].getMean();

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
void MeshSphereTree<T, n>::destroyTree() {
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
void MeshSphereTree<T, n>::Iterator::goLeft() {
	if (isLeaf()) {
		throw InvalidArgumentException();
	}

	mNode = mNode->mChildren[0];
}

template<class T, uint n>
inline void MeshSphereTree<T, n>::Iterator::goRight() {
	if (isLeaf()) {
		throw InvalidArgumentException();
	}

	mNode = mNode->mChildren[1];
}

template<class T, uint n>
typename MeshSphereTree<T, n>::Iterator MeshSphereTree<T, n>::Iterator::goBoth() {
	Iterator other = *this;
	this->goLeft();
	other.goRight();
	return other;
}

template<class T, uint n>
const Sphere<T, n>& MeshSphereTree<T, n>::Iterator::getSphere() const {
	return mNode->mSphere;
}

template<class T, uint n>
Triangle<T, n> MeshSphereTree<T, n>::Iterator::getTriangle() const {
	if (!isLeaf()) {
		throw InvalidArgumentException();
	}

	uint faceElementIndex = mTree.mMeshData.faceToElementIndex(mNode->mTriangles[0]);
	uint vertexIndex = mTree.mMeshData.getStandardFaceElement(faceElementIndex);

	Triangle<T, n> out;

	for (uint i = 0; i < 3; i++) {
		mTree.mMeshData.getMemberValue(MeshVertexData::MemberID::position, vertexIndex++, &out[i]);
	}

	return out;
}

template<class T, uint n>
bool MeshSphereTree<T, n>::Iterator::isLeaf() const {
	return mNode->mChildren[0] == nullptr && mNode->mChildren[0] == nullptr;
}

template class MeshSphereTree<float, 2>;
template class MeshSphereTree<double, 2>;