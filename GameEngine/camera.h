#ifndef CAMERA_H
#define CAMERA_H

#include "projection.h"

class Camera {
public:
	Transform transform;
	Projection projection;

public:
	Camera();
	Camera(Transform in_transform, Projection in_projection);
	Matrix4f Get_Projection_Matrix();
	Matrix4f Get_View_Matrix();
	Matrix4f Get_Matrix();
	void Use();
};

#endif
