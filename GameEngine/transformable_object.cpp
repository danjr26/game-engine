#include "transformable_object.h"

TransformableObject::TransformableObject() 
{}

TransformableObject::TransformableObject(Transform in_transform) :
	transform(in_transform)
{}

Transform& TransformableObject::Get_Transform() {
	return transform;
}
