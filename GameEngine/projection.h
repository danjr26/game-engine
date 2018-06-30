#ifndef PROJECTION_H
#define PROJECTION_H

#include "matrix.h"
#include "box.h"

class Projection {
public:
	Vector3f minima;
	Vector3f maxima;
	enum Type : bool {
		orthographic = false,
		perspective = true
	} type;

public:
	Projection();
	Projection(float in_near, float in_far, float in_horizontalAngle, float in_xToYRatio, Type in_type = Type::perspective);
	Projection(Vector3f in_minima, Vector3f in_maxima, Type in_type = Type::orthographic);

	float Get_View_Angle();
	void Set_View_Angle(float in_value);

	Matrix4f Get_Matrix();
};

#endif