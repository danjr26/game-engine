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
Transform<T, n>* Transform<T, n>::Get_Parent() {
	return mParent;
}

template<class T, uint n>
Transform<T, n> const* Transform<T, n>::Get_Const_Parent() const {
	return mParent;
}

template<class T, uint n>
void Transform<T, n>::Set_Parent(Transform* in_parent) {
	mParent = in_parent;
}

template<class T, uint n>
bool Transform<T, n>::Is_Local_Identity() const {
	return mTranslation.Is_Zero() && mScale.Is_Zero() && mRotation.Is_Identity();
}

template<class T, uint n>
bool Transform<T, n>::Is_World_Identity() const {
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		if (!transform->Is_Local_Identity()) return false;
	}
	return true;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Get_Local_Rotation() const {
	return mRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Get_World_Rotation() const {
	if (mParent == nullptr) return mRotation;
	else return Local_To_World_Rotation(mRotation);
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Rotation(const Rotation<T, n>& in_rotation) {
	mRotation = in_rotation;
}

template<class T, uint n>
void Transform<T, n>::Set_World_Rotation(const Rotation<T, n>& in_rotation) {
	if (mParent == nullptr) mRotation = in_rotation;
	else mRotation = World_To_Local_Rotation(in_rotation);
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local(const Rotation<T, n>& in_rotation) {
	mRotation = in_rotation.Followed_By(mRotation);
}

template<class T, uint n>
void Transform<T, n>::Rotate_World(const Rotation<T, n>& in_rotation) {
	if (mParent == nullptr) mRotation = mRotation.Followed_By(in_rotation);
	else mRotation = mRotation.Followed_By(World_To_Local_Rotation(in_rotation));
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	mRotation = in_rotation.Followed_By(mRotation);
	mTranslation = in_rotation.Apply_To(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_World_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		in_rotation :
		World_To_Local_Rotation(in_rotation);

	mRotation = mRotation.Followed_By(tempRotation);
	mTranslation = tempRotation.Apply_To(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		in_point :
		World_To_Local_Point(in_point);

	mRotation = in_rotation.Followed_By(mRotation);
	mTranslation = in_rotation.Apply_To(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_World_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(mParent == nullptr) ?
		in_point :
		World_To_Local_Point(in_point);

	Rotation<T, n> tempRotation =
		(mParent == nullptr) ?
		in_rotation :
		World_To_Local_Rotation(in_rotation);
		
	mRotation = mRotation.Followed_By(tempRotation);
	mTranslation = tempRotation.Apply_To(mTranslation - tempPoint) + tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_Local_Scale() const {
	return mScale;
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Scale(const Vector<T, n>& in_factor) {
	mScale = in_factor;
}

template<class T, uint n>
void Transform<T, n>::Scale_Local(const Vector<T, n>& in_factor) {
	mScale = mScale.Compwise(in_factor);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_Local_Position() const {
	return mTranslation;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_World_Position() const {
	return Local_To_World_Point(mTranslation);
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Position(const Vector<T, n>& in_position) {
	mTranslation = in_position;
}

template<class T, uint n>
void Transform<T, n>::Translate_Local(const Vector<T, n>& in_translation) {
	mTranslation += in_translation;
}

template<class T, uint n>
void Transform<T, n>::Translate_World(const Vector<T, n>& in_translation) {
	mTranslation +=
		(mParent == nullptr) ?
		in_translation :
		World_To_Local_Vector(in_translation);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	tempPoint = tempPoint.Compwise(mScale);
	tempPoint = mRotation.Apply_To(tempPoint);
	tempPoint += mTranslation;
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = World_To_Local_Point(in_point);
	tempPoint = Apply_To_Local_Point(tempPoint);
	return Local_To_World_Point(tempPoint);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	tempDirection = mRotation.Apply_To(tempDirection);
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = World_To_Local_Direction(in_direction);
	tempDirection = Apply_To_Local_Direction(tempDirection);
	return Local_To_World_Direction(tempDirection);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	tempVector = tempVector.Compwise(mScale);
	tempVector = mRotation.Apply_To(tempVector);
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = World_To_Local_Vector(in_vector);
	tempVector = Apply_To_Local_Vector(tempVector);
	return Local_To_World_Vector(tempVector);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Point(const Vector<T, n>& in_point) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempPoint = in_point;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempPoint -= chain[i]->mTranslation;
		tempPoint = chain[i]->mRotation.Get_Inverse().Apply_To(tempPoint);
		tempPoint = tempPoint.Compwise(chain[i]->mScale.Component_Inverted());
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempPoint = tempPoint.Compwise(transform->mScale);
		tempPoint = transform->mRotation.Apply_To(tempPoint);
		tempPoint += transform->mTranslation;
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Direction(const Vector<T, n>& in_direction) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempDirection = in_direction;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempDirection = chain[i]->mRotation.Get_Inverse().Apply_To(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDirection = transform->mRotation.Apply_To(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Vector(const Vector<T, n>& in_vector) const {
	std::vector<Transform*> chain;
	for (Transform* transform = mParent; transform != nullptr; transform = transform->mParent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempVector = in_vector;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempVector = chain[i]->mRotation.Get_Inverse().Apply_To(tempVector);
		tempVector = tempVector.Compwise(chain[i]->mScale.Component_Inverted());
	}
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempVector = tempVector.Compwise(transform->mScale);
		tempVector = transform->mRotation.Apply_To(tempVector);
	}
	return tempVector;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::World_To_Local_Rotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = tempRotation.Followed_By(transform->mRotation.Get_Inverse());
	}
	return tempRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Local_To_World_Rotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempRotation = transform->mRotation.Followed_By(tempRotation);
	}
	return tempRotation;
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_Local_Matrix() const {
	return Matrix<T, 4, 4>::Translation(Vector<T, 3>(mTranslation)) * mRotation.Get_Matrix() * Matrix<T, 4, 4>::Scale(Vector<T, 3>(mScale));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_World_Matrix() const {
	Matrix<T, 4, 4> tempMatrix = Get_Local_Matrix();
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempMatrix = transform->Get_Local_Matrix() * tempMatrix;
	}
	return tempMatrix;
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_Local_Inverse_Matrix() const {
	return Matrix<T, 4, 4>::Translation(-Vector<T, 3>(mTranslation)) * mRotation.Get_Inverse().Get_Matrix() * 
		Matrix<T, 4, 4>::Scale(Vector<T, 3>(mScale.Component_Inverted()));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_World_Inverse_Matrix() const {
	Matrix<T, 4, 4> tempMatrix = Get_Local_Inverse_Matrix();
	for (Transform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempMatrix = transform->Get_Local_Inverse_Matrix() * tempMatrix;
	}
	return tempMatrix;
}

template class Transform<float, 2>;
template class Transform<double, 2>;
template class Transform<float, 3>;
template class Transform<double, 3>;