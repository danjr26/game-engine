#ifndef CAMERA_H
#define CAMERA_H

#include "projection.h"

class Camera {
private:
	Transform3d mTransform;
	Projectiond mProjection;

public:
	Camera();

	Transform3d& getTransform();
	const Transform3d& getTransform() const;

	Projectiond getProjection() const;
	void setProjection(const Projectiond& i_projection);

	Rectangled getRectangle() const;
	Rectangled getTransformedRectangle() const;

	Matrix4f getProjectionMatrix();
	Matrix4f getViewMatrix();
	Matrix4f getMatrix();
};

#endif
