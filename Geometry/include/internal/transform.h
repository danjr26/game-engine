#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "unclamped_rotation.h"
#include "rotation.h"

template<class T, uint n>
class Transform {
	friend class Transform;
	// applies components in this order:
	// scale, rotation, translation
private:
	Transform<T, n>* mParent;
	Vector<T, n> mTranslation;
	Vector<T, n> mScale;
	Rotation<T, n> mRotation;

public:
	Transform();
	Transform(const Transform<T, n>& i_other);
	template<class T2, std::enable_if_t<!std::is_same_v<T, T2>>>
	Transform(const Transform<T2, n>& i_other) : 
		mParent(nullptr),
		mTranslation(i_other.mTranslation),
		mScale(i_other.mScale),
		mRotation(i_other.mRotation)
	{}

	Transform<T, n>* getParent();
	Transform<T, n> const* getConstParent() const;
	void setParent(Transform<T, n>* i_parent);

	bool isLocalIdentity() const;
	bool isWorldIdentity() const;

	Rotation<T, n> getLocalRotation() const;
	Rotation<T, n> getWorldRotation() const;
	void setLocalRotation(const Rotation<T, n>& i_rotation);
	void setWorldRotation(const Rotation<T, n>& i_rotation);
	void rotateLocal(const Rotation<T, n>& i_rotation);
	void rotateWorld(const Rotation<T, n>& i_rotation);
	void rotateLocalAroundLocalPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point);
	void rotateWorldAroundLocalPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point);
	void rotateLocalAroundWorldPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point);
	void rotateWorldAroundWorldPoint(const Rotation<T, n>& i_rotation, const Vector<T, n>& i_point);

	Vector<T, n> getLocalScale() const;
	void setLocalScale(const Vector<T, n>& i_factor);
	void scaleLocal(const Vector<T, n>& i_factor);

	Vector<T, n> getLocalPosition() const;
	Vector<T, n> getWorldPosition() const;
	void setLocalPosition(const Vector<T, n>& i_position);
	void translateLocal(const Vector<T, n>& i_translation);
	void translateWorld(const Vector<T, n>& i_translation);

	Matrix<T, 4, 4> getLocalMatrix() const;
	Matrix<T, 4, 4> getWorldMatrix() const;
	Matrix<T, 4, 4> getLocalInverseMatrix() const;
	Matrix<T, 4, 4> getWorldInverseMatrix() const;

	Vector<T, n> applyToLocalPoint(const Vector<T, n>& i_point) const;
	Vector<T, n> applyToWorldPoint(const Vector<T, n>& i_point) const;
	Vector<T, n> applyToLocalDirection(const Vector<T, n>& i_direction) const;
	Vector<T, n> applyToWorldDirection(const Vector<T, n>& i_direction) const;
	Vector<T, n> applyToLocalVector(const Vector<T, n>& i_vector) const;
	Vector<T, n> applyToWorldVector(const Vector<T, n>& i_vector) const;
	Rotation<T, n> applyToLocalRotation(const Rotation<T, n>& i_rotation) const;
	Rotation<T, n> applyToWorldRotation(const Rotation<T, n>& i_rotation) const;

	Vector<T, n> worldToLocalPoint(const Vector<T, n>& i_point) const;
	Vector<T, n> localToWorldPoint(const Vector<T, n>& i_point) const;
	Vector<T, n> worldToLocalDirection(const Vector<T, n>& i_direction) const;
	Vector<T, n> localToWorldDirection(const Vector<T, n>& i_direction) const;
	Vector<T, n> worldToLocalVector(const Vector<T, n>& i_vector) const;
	Vector<T, n> localToWorldVector(const Vector<T, n>& i_vector) const;
	Rotation<T, n> worldToLocalRotation(const Rotation<T, n>& i_rotation) const;
	Rotation<T, n> localToWorldRotation(const Rotation<T, n>& i_rotation) const;

	Vector<T, n> applyChainToLocalPoint(const Vector<T, n>& i_point) const;
	Vector<T, n> applyChainToLocalDirection(const Vector<T, n>& i_point) const;
	Vector<T, n> applyChainToLocalVector(const Vector<T, n>& i_point) const;
	Rotation<T, n> applyChainToLocalRotation(const Rotation<T, n>& i_rotation) const;

	Transform<T, n>* cloneChain() const;
	Transform<T, n>* lerpChain(const Transform<T, n>& i_other, T i_t) const;
	void deleteChainParents();
};

using Transform2f = Transform<float, 2>;
using Transform2d = Transform<double, 2>;
using Transform3f = Transform<float, 3>;
using Transform3d = Transform<double, 3>;

#endif

