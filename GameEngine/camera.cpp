#include "camera.h"

Projection::Projection(float in_close, float in_distant, float in_horizontalAngle) {
	close =		in_close;
	distant =	in_distant;
	right =		tanf(in_horizontalAngle * (float)DEG_TO_RAD) * in_close;
	left =		-right;
	top =		right * (0.75f);
	bottom =	-top;
}

Projection::Projection(float in_left, float in_right, float in_bottom, float in_top, float in_close, float in_distant) :
left	(in_left),
right	(in_right),
bottom	(in_bottom),
top		(in_top),
close	(in_close),
distant	(in_distant)
{}

void Projection::Apply(Type in_type) {
	projectionStack.Push();
	if(in_type == orthographic)
		projectionStack.Top() *= Orthographic_Matrix();
	if(in_type == perspective)
		projectionStack.Top() *= Perspective_Matrix();
}

void Projection::Unapply() {
	projectionStack.Pop();
}

double Projection::View_Angle() {
	return atan(right / close) * RAD_TO_DEG;
}

Matrix4f Projection::Orthographic_Matrix() {
	float projectionMatrix[16] = {
		2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
		0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
		0.0f, 0.0f, -2.0f / (distant - close), -(distant + close) / (distant - close),
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return Matrix4f(projectionMatrix);
}

Matrix4f Projection::Perspective_Matrix() {
	float projectionMatrix[16] = {
		2.0f * close / (right - left), 0, (right + left) / (right - left), 0.0f,
		0.0f, 2.0f * close / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, -(distant + close) / (distant - close), -(2 * distant * close) / (distant - close),
		0.0f, 0.0f, -1.0f, 0.0f
	};

	return Matrix4f(projectionMatrix);
}


Camera::Camera(Transform3d in_transform, Projection in_projection) :
transform	(in_transform),
projection	(in_projection) 
{}

void Camera::View() {
	Clear();
	projection.Apply(Projection::perspective);
	transform.Apply_Camera();
}

void Camera::Clear() {
	viewStack.Clear();
	projectionStack.Clear();
}

