#ifndef TRANSFORMABLE_OBJECT_H
#define TRANSFORMABLE_OBJECT_H

#include "transform.h"

class TransformableObject {
protected:
	Transform transform;
public:
	TransformableObject();
	TransformableObject(Transform in_transform);
	Transform& Get_Transform();
};

#endif