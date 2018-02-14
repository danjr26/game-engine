#ifndef CAMERA_H
#define CAMERA_H
#include "component.h"

class Projection {
public:
	float left;
	float right;
	float top;
	float bottom;
	float close;
	float distant;

	enum Type {
		orthographic,
		perspective
	};

public:
			Projection	(float in_close, float in_distant, float in_horizontalAngle);
			Projection	(float in_left, float in_right, float in_bottom, float in_top, float in_close, float in_distant);
	void	Apply		(Type in_type);
	void	Unapply		();
	double	View_Angle	();

private:
	Matrix4f Orthographic_Matrix();
	Matrix4f Perspective_Matrix	();
};

class Camera {
	public:
		Transform3d	transform;
		Projection	projection;

				Camera	(Transform3d in_transform, Projection in_projection);
		void 	View	();
		void	Clear	();
};

#endif
