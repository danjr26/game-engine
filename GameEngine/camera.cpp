#include "camera.h"
#include "game_engine.h"

Camera::Camera() {}

Projectiond Camera::Get_Projection() const {
	return projection;
}

void Camera::Set_Projection(const Projectiond& in_projection) {
	projection = in_projection;
}

Matrix4f Camera::Get_Projection_Matrix() {
	return projection.Get_Matrix();
}

Matrix4f Camera::Get_View_Matrix() {
	return transform.Get_World_Inverse_Matrix();
}

Matrix4f Camera::Get_Matrix() {
	return Get_Projection_Matrix() * Get_View_Matrix();
}

void Camera::Use() {
	GE.Cameras().active = this;
}


