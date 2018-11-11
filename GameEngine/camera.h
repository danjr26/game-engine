#ifndef CAMERA_H
#define CAMERA_H

#include "transformable_object.h"
#include "projection.h"

class Camera : public TransformableObject3d {
private:
	Projectiond projection;

public:
	Camera();

	Projectiond Get_Projection() const;
	void Set_Projection(const Projectiond& in_projection);

	Matrix4f Get_Projection_Matrix();
	Matrix4f Get_View_Matrix();
	Matrix4f Get_Matrix();

	void Use();
};

#endif
