#include "../include/internal/camera.h"

Camera::Camera() 
{}

Matrix4f Camera::getMatrix() {
	return mProjection.getMatrix() * mTransform.getWorldInverseMatrix();
}


