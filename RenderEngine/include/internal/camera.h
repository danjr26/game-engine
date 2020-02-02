#ifndef CAMERA_H
#define CAMERA_H

#include "projection.h"

struct Camera {
	Transform3d mTransform;
	Projectiond mProjection;

	Camera();

	Matrix4f getMatrix();
};

#endif
