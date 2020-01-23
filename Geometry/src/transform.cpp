#include "../include/internal/transform.h"

template<class T, uint n>
Transform<T, n>::Transform() :
	mParent(nullptr),
	mTranslation(),
	mScale(),
	mRotation() {
	
	for (uint i = 0; i < n; i++) mScale[i] = 1;
}

template<class T, uint n>
Transform<T, n>::Transform(const Transform<T, n>& i_other) :
	mParent(i_other.mParent),
	mTranslation(i_other.mTranslation),
	mScale(i_other.mScale),
	mRotation(i_other.mRotation) 
{}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::getParent() {
	return mParent;
}

template<class T, uint n>
Transform<T, n> const* Transform<T, n>::getConstParent() const {
	return mParent;
}

template<class T, uint n>
void Transform<T, n>::setParent(Transform<T, n>* i_parent) {
	mParent = i_parent;
}

template<class T, uint n>
bool Transform<T, n>::isLocalIdentity() const {
	return mTranslation.isZero() && mScale.isZero() && mRotation.isIdentity();
}

template<class T, uint n>
bool Transform<T, n>::isWorldIdentity() const {
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		if (!transform->isLocalIdentity()) return false;
	}
	return true;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::getLocalRotation() const {
	return mRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::getWorldRotation() const {
	if (mParent == nullptr) return mRotation;
	else return localToWorldRotation(mRotation);
}

template<class T, uint n>
void Transform<T, n>::setLocalRotation(const Rotation<T, n>& i_rotation) {
	mRotation = i_rotation;
}

template<class T, uint n>
void Transform<T, n>::setWorldRotation(const Rotation<T, n>& i_rotation) {
	if (mParent == nullptr) mRotation = i_rotation;
	else mRotation = worldToLocalRotation(i_rotation);
}

template<class T, uint n>
void Transform<T, n>::rotateLocal(const Rotation<T, n>& i_rotation) {
	mRotation = i_rotation.followedBy(mRotation);
}

template<class T, uint n>
void Transform<T, n>::rotateWorld(const Rotation<T, n>& i_rotation) {
	if (mParent == nullptr) mRotation = mRotation.followedBy(i_rotation);
	else mRotation = mRotation.followedBy(worldToLocalRotation(i_rotation));
}

template<class T, uint n>
void Transform<T, n>::rotateLocalAroundLocalPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point) {
	Vector<T, n> tempPoint = i_point;
	mRotation = i_rotation.followedBy(mRotation);
	mTranslation = i_rotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateWorldAroundLocalPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point) {
	Vector<T, n> tempPoint = i_point;
	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		i_rotation :
		worldToLocalRotation(i_rotation);

	mRotation = mRotation.followedBy(tempRotation);
	mTranslation = tempRotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateLocalAroundWorldPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		i_point :
		worldToLocalPoint(i_point);

	mRotation = i_rotation.followedBy(mRotation);
	mTranslation = i_rotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateWorldAroundWorldPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		i_point :
		worldToLocalPoint(i_point);

	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		i_rotation :
		worldToLocalRotation(i_rotation);
		
	mRotation = mRotation.followedBy(tempRotation);
	mTranslation = tempRotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::getLocalScale() const {
	return mScale;
}

template<class T, uint n>
void Transform<T, n>::setLocalScale(const Vector<T, n>& i_factor) {
	mScale = i_factor;
}

template<class T, uint n>
void Transform<T, n>::scaleLocal(const Vector<T, n>& i_factor) {
	mScale = mScale.compMult(i_factor);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::getLocalPosition() const {
	return mTranslation;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::getWorldPosition() const {
	return localToWorldPoint(mTranslation);
}

template<class T, uint n>
void Transform<T, n>::setLocalPosition(const Vector<T, n>& i_position) {
	mTranslation = i_position;
}

template<class T, uint n>
void Transform<T, n>::translateLocal(const Vector<T, n>& i_translation) {
	mTranslation += i_translation;
}

template<class T, uint n>
void Transform<T, n>::translateWorld(const Vector<T, n>& i_translation) {
	mTranslation +=
		(mParent == nullptr) ?
		i_translation :
		worldToLocalVector(i_translation);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalPoint(const Vector<T, n>& i_point) const {
	Vector<T, n> tempPoint = i_point;
	tempPoint = tempPoint.compMult(mScale);
	tempPoint = mRotation.applyTo(tempPoint);
	tempPoint += mTranslation;
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldPoint(const Vector<T, n>& i_point) const {
	Vector<T, n> tempPoint = worldToLocalPoint(i_point);
	tempPoint = applyToLocalPoint(tempPoint);
	return localToWorldPoint(tempPoint);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalDirection(const Vector<T, n>& i_direction) const {
	Vector<T, n> tempDirection = i_direction;
	tempDirection = mRotation.applyTo(tempDirection);
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldDirection(const Vector<T, n>& i_direction) const {
	Vector<T, n> tempDirection = worldToLocalDirection(i_direction);
	tempDirection = applyToLocalDirection(tempDirection);
	return localToWorldDirection(tempDirection);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalVector(const Vector<T, n>& i_vector) const {
	Vector<T, n> tempVector = i_vector;
	tempVector = tempVector.compMult(mScale);
	tempVector = mRotation.applyTo(tempVector);
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldVector(const Vector<T, n>& i_vector) const {
	Vector<T, n> tempVector = worldToLocalVector(i_vector);
	tempVector = applyToLocalVector(tempVector);
	return localToWorldVector(tempVector);
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::applyToLocalRotation(const Rotation<T, n>& i_rotation) const {
	return i_rotation.followedBy(mRotation);
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::applyToWorldRotation(const Rotation<T, n>& i_rotation) const {
	Rotation<T, n> tempRotation = worldToLocalRotation(i_rotation);
	tempRotation = applyToLocalRotation(tempRotation);
	return localToWorldRotation(tempRotation);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalPoint(const Vector<T, n>& i_point) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempPoint = i_point;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempPoint -= chain[i]->mTranslation;
		tempPoint = chain[i]->mRotation.getInverse().applyTo(tempPoint);
		tempPoint = tempPoint.compMult(chain[i]->mScale.compInverted());
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldPoint(const Vector<T, n>& i_point) const {
	Vector<T, n> tempPoint = i_point;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempPoint = tempPoint.compMult(transform->mScale);
		tempPoint = transform->mRotation.applyTo(tempPoint);
		tempPoint += transform->mTranslation;
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalDirection(const Vector<T, n>& i_direction) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempDirection = i_direction;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempDirection = chain[i]->mRotation.getInverse().applyTo(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldDirection(const Vector<T, n>& i_direction) const {
	Vector<T, n> tempDirection = i_direction;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDirection = transform->mRotation.applyTo(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalVector(const Vector<T, n>& i_vector) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempVector = i_vector;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempVector = chain[i]->mRotation.getInverse().applyTo(tempVector);
		tempVector = tempVector.compMult(chain[i]->mScale.compInverted());
	}
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldVector(const Vector<T, n>& i_vector) const {
	Vector<T, n> tempVector = i_vector;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempVector = tempVector.compMult(transform->mScale);
		tempVector = transform->mRotation.applyTo(tempVector);
	}
	return tempVector;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::worldToLocalRotation(const Rotation<T, n>& i_rotation) const {
	Rotation<T, n> tempRotation = i_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = tempRotation.followedBy(transform->mRotation.getInverse());
	}
	return tempRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::localToWorldRotation(const Rotation<T, n>& i_rotation) const {
	Rotation<T, n> tempRotation = i_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = transform->mRotation.followedBy(tempRotation);
	}
	return tempRotation;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyChainToLocalPoint(const Vector<T, n>& i_point) const {
	return localToWorldPoint(applyToLocalPoint(i_point));
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyChainToLocalDirection(const Vector<T, n>& i_point) const {
	return localToWorldDirection(applyToLocalDirection(i_point));
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyChainToLocalVector(const Vector<T, n>& i_point) const {
	return localToWorldVector(applyToLocalVector(i_point));
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::applyChainToLocalRotation(const Rotation<T, n>& i_rotation) const {
	return localToWorldRotation(applyToLocalRotation(i_rotation));
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::cloneChain() const {
	Transform<T, n>* newTransform = new Transform<T, n>(*this);
	for (Transform<T, n>* it = newTransform; it->mParent != nullptr; it = it->mParent) {
		it->mParent = new Transform<T, n>(*it->mParent);
	}
	return newTransform;
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::lerpChain(const Transform<T, n>& i_other, T i_t) const {
	Transform<T, n>* out = cloneChain();
	Transform<T, n>* t1 = out;
	Transform<T, n> const* t2 = &i_other;
	while (t1 != nullptr && t2 != nullptr) {
		t1->mRotation = t1->mRotation.lerp(t2->mRotation, i_t);
		t1->mTranslation = t1->mTranslation.lerp(t2->mTranslation, i_t);
		t1->mScale = t1->mScale.lerp(t2->mScale, i_t);
		t1 = t1->mParent;
		t2 = t2->mParent;
	}
	if (t1 || t2) fail();
	return out;
}

template<class T, uint n>
void Transform<T, n>::deleteChainParents() {
	Transform<T, n>* next;
	for (Transform<T, n>* it = mParent; it != nullptr; it = next) {
		next = it->mParent;
		delete it;
		it = next;
	}
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::getLocalMatrix() const {
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(0, mTranslation)) * mRotation.getMatrix() * Matrix<T, 4, 4>::scale(Vector<T, 3>(1, mScale));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::getWorldMatrix() const {
	Matrix<T, 4, 4> tempMatrix = getLocalMatrix();
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempMatrix = transform->getLocalMatrix() * tempMatrix;
	}
	return tempMatrix;
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::getLocalInverseMatrix() const {
	return Matrix<T, 4, 4>::translation(-Vector<T, 3>(0, mTranslation)) * mRotation.getInverse().getMatrix() * 
		Matrix<T, 4, 4>::scale(Vector<T, 3>(1, mScale.compInverted()));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::getWorldInverseMatrix() const {
	Matrix<T, 4, 4> tempMatrix = getLocalInverseMatrix();
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempMatrix = transform->getLocalInverseMatrix() * tempMatrix;
	}
	return tempMatrix;
}

template class Transform<float, 2>;
template class Transform<double, 2>;
template class Transform<float, 3>;
template class Transform<double, 3>;