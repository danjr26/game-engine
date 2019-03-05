#include "transform.h"
#include <vector>
#include "log.h"

template<class T, uint n>
Transform<T, n>::Transform() :
	mParent(nullptr),
	mTranslation(),
	mScale(),
	mRotation() {
	
	for (uint i = 0; i < n; i++) mScale[i] = 1;
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::getParent() {
	return mParent;
}

template<class T, uint n>
Transform<T, n> const* Transform<T, n>::getConstParent() const {
	return mParent;
}

template<class T, uint n>
void Transform<T, n>::setParent(Transform* in_parent) {
	mParent = in_parent;
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
void Transform<T, n>::setLocalRotation(const Rotation<T, n>& in_rotation) {
	mRotation = in_rotation;
}

template<class T, uint n>
void Transform<T, n>::setWorldRotation(const Rotation<T, n>& in_rotation) {
	if (mParent == nullptr) mRotation = in_rotation;
	else mRotation = worldToLocalRotation(in_rotation);
}

template<class T, uint n>
void Transform<T, n>::rotateLocal(const Rotation<T, n>& in_rotation) {
	mRotation = in_rotation.followedBy(mRotation);
}

template<class T, uint n>
void Transform<T, n>::rotateWorld(const Rotation<T, n>& in_rotation) {
	if (mParent == nullptr) mRotation = mRotation.followedBy(in_rotation);
	else mRotation = mRotation.followedBy(worldToLocalRotation(in_rotation));
}

template<class T, uint n>
void Transform<T, n>::rotateLocalAroundLocalPoint(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	mRotation = in_rotation.followedBy(mRotation);
	mTranslation = in_rotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateWorldAroundLocalPoint(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		in_rotation :
		worldToLocalRotation(in_rotation);

	mRotation = mRotation.followedBy(tempRotation);
	mTranslation = tempRotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateLocalAroundWorldPoint(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		in_point :
		worldToLocalPoint(in_point);

	mRotation = in_rotation.followedBy(mRotation);
	mTranslation = in_rotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::rotateWorldAroundWorldPoint(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		in_point :
		worldToLocalPoint(in_point);

	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		in_rotation :
		worldToLocalRotation(in_rotation);
		
	mRotation = mRotation.followedBy(tempRotation);
	mTranslation = tempRotation.applyTo(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::getLocalScale() const {
	return mScale;
}

template<class T, uint n>
void Transform<T, n>::setLocalScale(const Vector<T, n>& in_factor) {
	mScale = in_factor;
}

template<class T, uint n>
void Transform<T, n>::scaleLocal(const Vector<T, n>& in_factor) {
	mScale = mScale.compwise(in_factor);
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
void Transform<T, n>::setLocalPosition(const Vector<T, n>& in_position) {
	mTranslation = in_position;
}

template<class T, uint n>
void Transform<T, n>::translateLocal(const Vector<T, n>& in_translation) {
	mTranslation += in_translation;
}

template<class T, uint n>
void Transform<T, n>::translateWorld(const Vector<T, n>& in_translation) {
	mTranslation +=
		(mParent == nullptr) ?
		in_translation :
		worldToLocalVector(in_translation);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalPoint(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	tempPoint = tempPoint.compwise(mScale);
	tempPoint = mRotation.applyTo(tempPoint);
	tempPoint += mTranslation;
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldPoint(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = worldToLocalPoint(in_point);
	tempPoint = applyToLocalPoint(tempPoint);
	return localToWorldPoint(tempPoint);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalDirection(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	tempDirection = mRotation.applyTo(tempDirection);
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldDirection(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = worldToLocalDirection(in_direction);
	tempDirection = applyToLocalDirection(tempDirection);
	return localToWorldDirection(tempDirection);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToLocalVector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	tempVector = tempVector.compwise(mScale);
	tempVector = mRotation.applyTo(tempVector);
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::applyToWorldVector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = worldToLocalVector(in_vector);
	tempVector = applyToLocalVector(tempVector);
	return localToWorldVector(tempVector);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalPoint(const Vector<T, n>& in_point) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempPoint = in_point;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempPoint -= chain[i]->mTranslation;
		tempPoint = chain[i]->mRotation.getInverse().applyTo(tempPoint);
		tempPoint = tempPoint.compwise(chain[i]->mScale.componentInverted());
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldPoint(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempPoint = tempPoint.compwise(transform->mScale);
		tempPoint = transform->mRotation.applyTo(tempPoint);
		tempPoint += transform->mTranslation;
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalDirection(const Vector<T, n>& in_direction) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempDirection = in_direction;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempDirection = chain[i]->mRotation.getInverse().applyTo(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldDirection(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDirection = transform->mRotation.applyTo(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::worldToLocalVector(const Vector<T, n>& in_vector) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempVector = in_vector;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempVector = chain[i]->mRotation.getInverse().applyTo(tempVector);
		tempVector = tempVector.compwise(chain[i]->mScale.componentInverted());
	}
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::localToWorldVector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempVector = tempVector.compwise(transform->mScale);
		tempVector = transform->mRotation.applyTo(tempVector);
	}
	return tempVector;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::worldToLocalRotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = tempRotation.followedBy(transform->mRotation.getInverse());
	}
	return tempRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::localToWorldRotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = transform->mRotation.followedBy(tempRotation);
	}
	return tempRotation;
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::cloneChain() const {
	Transform<T, n>* out = new Transform<T, n>(*this);
	for (Transform<T, n>* it = out; it->mParent != nullptr; it = it->mParent) {
		it->mParent = new Transform<T, n>(*it->mParent);
	}
	return out;
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::lerpChain(const Transform<T, n>& in_other, T in_t) const {
	Transform<T, n>* out = cloneChain();
	Transform<T, n>* t1 = out;
	Transform<T, n> const* t2 = &in_other;
	while (t1 != nullptr && t2 != nullptr) {
		t1->mRotation = t1->mRotation.lerp(t2->mRotation, in_t);
		t1->mTranslation = t1->mTranslation.lerp(t2->mTranslation, in_t);
		t1->mScale = t1->mScale.lerp(t2->mScale, in_t);
		t1 = t1->mParent;
		t2 = t2->mParent;
	}
	if (t1 || t2) throw InvalidArgumentException();
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
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(mTranslation)) * mRotation.getMatrix() * Matrix<T, 4, 4>::scale(Vector<T, 3>(mScale));
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
	return Matrix<T, 4, 4>::translation(-Vector<T, 3>(mTranslation)) * mRotation.getInverse().getMatrix() * 
		Matrix<T, 4, 4>::scale(Vector<T, 3>(mScale.componentInverted()));
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