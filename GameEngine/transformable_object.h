#ifndef TRANSFORMABLE_OBJECT_H
#define TRANSFORMABLE_OBJECT_H

#include "transform.h"
#include "misc.h"

template<class T, uint n>
class TransformableObject {
private:
	neuterable_ptr<Transform<T, n>> mTransform;

public:
	TransformableObject();
	TransformableObject(Transform<T, n>* in_transform);
	TransformableObject(const TransformableObject<T, n>& in_other);

	TransformableObject<T, n>& operator=(const TransformableObject<T, n>& in_other);

	Transform<T, n>& getTransform();
	Transform<T, n> const& getTransform() const;
	void subTransform(Transform<T, n>* in_transform);
};

using TransformableObject2f = TransformableObject<float, 2>;
using TransformableObject2d = TransformableObject<double, 2>;
using TransformableObject3f = TransformableObject<float, 3>;
using TransformableObject3d = TransformableObject<double, 3>;

#endif


