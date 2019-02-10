#ifndef CAMERA_H
#define CAMERA_H

#include "transformable_object.h"
#include "projection.h"

class Camera : public TransformableObject3d {
private:
	Projectiond mProjection;

public:
	Camera();

	Projectiond getProjection() const;
	void setProjection(const Projectiond& in_projection);

	Matrix4f getProjectionMatrix();
	Matrix4f getViewMatrix();
	Matrix4f getMatrix();

	void use();
};

#endif
