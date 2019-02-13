#include "camera.h"
#include "game_engine.h"

Camera::Camera() {}

Projectiond Camera::getProjection() const {
	return mProjection;
}

void Camera::setProjection(const Projectiond& in_projection) {
	mProjection = in_projection;
}

Matrix4f Camera::getProjectionMatrix() {
	return mProjection.getMatrix();
}

Matrix4f Camera::getViewMatrix() {
	return mTransform.getWorldInverseMatrix();
}

Matrix4f Camera::getMatrix() {
	return getProjectionMatrix() * getViewMatrix();
}

void Camera::use() {
	GE.cameras().setActive(this);
}


