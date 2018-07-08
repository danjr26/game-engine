#include "camera.h"
#include "game_engine.h"

Camera::Camera() {}

Camera::Camera(Transform in_transform, Projection in_projection) :
	transform(in_transform),
	projection(in_projection) 
{}

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


