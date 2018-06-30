#include "projection.h"

Projection::Projection() :
	minima(Vector3f(-1, -1, 0)),
	maxima(Vector3f(1, 1, -1)),
	type(Type::orthographic)
{}

Projection::Projection(float in_near, float in_far, float in_horizontalAngle, float in_xToYRatio, Type in_type) :
type(in_type) {
	maxima[0] = tanf(in_horizontalAngle) * in_near;
	maxima[1] = maxima[0] / in_xToYRatio;
	maxima[2] = in_far;
	minima[0] = -maxima[0];
	minima[1] = -maxima[1];
	minima[2] = in_near;
}

Projection::Projection(Vector3f in_minima, Vector3f in_maxima, Type in_type) :
	minima(in_minima),
	maxima(in_maxima),
	type(in_type)
{}

float Projection::Get_View_Angle() {
	return atanf(maxima[0] / minima[2]);
}

void Projection::Set_View_Angle(float in_value) {
	maxima[0] = tanf(in_value) * minima[2];
	minima[0] = -maxima[0];
}

Matrix4f Projection::Get_Matrix() {
	if (type == Type::perspective) {
		return Matrix4f::Perspective(minima, maxima);
	}
	else {
		return Matrix4f::Orthographic(minima, maxima);
	}
}
