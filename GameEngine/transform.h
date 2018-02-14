#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "vectors.h"

class Matrix4f;

class Orientation2d {
public:
	Vector2d right;
	Vector2d up;

				Orientation2d	();
				Orientation2d	(Vector2d right);
				~Orientation2d	();
	void		Calculate_Up	();
	void		Rotate			(double angle);
	Vector2d	S_To_O			(Vector2d v);
};

class Transform2d {
public:
	Vector2d		position;
	Orientation2d	orientation;
	Vector2d		scale;

					Transform2d		();
					Transform2d		(Vector2d position);
					Transform2d		(Vector2d position, Orientation2d orientation);
					Transform2d		(Vector2d position, Orientation2d orientation, Vector2d scale);
					~Transform2d	();
	void			Rotate			(double angle, Vector2d pt);
	Matrix4f		Object_Matrix	();
	Matrix4f		Camera_Matrix	();
	void			Apply_Object	();
	void			Apply_Camera	();
	void			Unapply_Object	();
	void			Unapply_Camera	();
	Vector2d		S_To_O			(Vector2d v);
	Orientation2d	S_To_O			(Orientation2d o);
	Transform2d		S_To_O			(Transform2d t);
};

class Orientation3d {
public:
	Vector3d forward;
	Vector3d up;
	Vector3d right;

				Orientation3d	();
				Orientation3d	(Vector3d forward, Vector3d up);
				~Orientation3d	();
	//void		Calculate_Right	();
	void		Rotate			(Vector3d r);
	void		Rotate			(double angle, Vector3d axis);
	void		Rotate_S		(Vector3d r);

	inline Vector3d	S_To_O		(Vector3d v) {
		//Calculate_Right();
		return Vector3d(
			right.x * v.x + up.x * v.y - forward.x * v.z,
			right.y * v.x + up.y * v.y - forward.y * v.z,
			right.z * v.x + up.z * v.y - forward.z * v.z
		);
	};
};

class Transform3d {
public:
	Vector3d		position;
	Orientation3d	orientation;
	Vector3d		scale;

					Transform3d		();
					Transform3d		(Vector3d position);
					Transform3d		(Vector3d position, Orientation3d orientation);
					Transform3d		(Vector3d position, Orientation3d orientation, Vector3d scale);
					~Transform3d	();
	void			Rotate			(Vector3d r, Vector3d pt);
	Matrix4f		Object_Matrix	();
	Matrix4f		Camera_Matrix	();
	void			Apply_Object	();
	void			Apply_Camera	();
	void			Unapply_Object	();
	void			Unapply_Camera	();
	inline Vector3d S_To_O(Vector3d v) const {
		//orientation.right = orientation.forward.Cross(orientation.up);
		Vector3d v2 = position;
		v2 += orientation.right *	v.x * scale.x;
		v2 += orientation.up *		v.y * scale.y;
		v2 -= orientation.forward *		v.z * scale.z;

		return v2;
	}
	Orientation3d	S_To_O			(Orientation3d o);
	Transform3d		S_To_O			(Transform3d t);
};

class Matrix4f {
public:
	float data[16];

				Matrix4f	();
				Matrix4f	(float data[16]);
				~Matrix4f	();
	Matrix4f	operator+	(Matrix4f& m);
	Matrix4f	operator-	(Matrix4f& m);
	Matrix4f	operator*	(Matrix4f& m);
	void		operator+=	(Matrix4f& m);
	void		operator-=	(Matrix4f& m);
	void		operator*=	(Matrix4f& m);
};

class Matrix4fStack {
public:
	Matrix4f	mats[16];
	uint		n;

				Matrix4fStack	();
				~Matrix4fStack	();
	void		Push			();
	void		Pop				();
	void		Clear			();
	Matrix4f&	Top				();
};

extern Matrix4fStack modelStack;
extern Matrix4fStack viewStack;
extern Matrix4fStack projectionStack;

#endif
