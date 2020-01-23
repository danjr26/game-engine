#include "../include/internal/camera.h"

Camera::Camera() {}

Transform3d& Camera::getTransform() {
	return mTransform;
}

const Transform3d& Camera::getTransform() const {
	return mTransform;
}

Projectiond Camera::getProjection() const {
	return mProjection;
}

void Camera::setProjection(const Projectiond& i_projection) {
	mProjection = i_projection;
}

Rectangled Camera::getRectangle() const {
	if (mProjection.type != Projectiond::Type::orthographic) fail();
	Vector2d origin = Vector2d(mProjection.mMinima);
	Vector2d axes[2] = {
		Vector2d(mProjection.mMaxima.x() - mProjection.mMinima.x(), 0),
		Vector2d(0, mProjection.mMaxima.y() - mProjection.mMinima.y())
	};
	return Rectangled::fromOriginAxes(origin, axes);
}

Rectangled Camera::getTransformedRectangle() const {
	Rectangled rect = getRectangle();
	Vector2d origin = Vector2d(mTransform.applyToLocalPoint(Vector3d(rect.getOrigin(), 0)));
	Vector2d axes[2];
	rect.getAxes(axes);
	for (uint i = 0; i < 2; i++) {
		axes[i] = Vector2d(mTransform.applyToLocalVector(Vector3d(axes[i], 0)));
	}
	return Rectangled::fromOriginAxes(origin, axes);
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


