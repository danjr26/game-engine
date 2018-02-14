#include "transform.h"
#include "framework.h"

Matrix4fStack modelStack =		Matrix4fStack();
Matrix4fStack viewStack =		Matrix4fStack();
Matrix4fStack projectionStack = Matrix4fStack();

Orientation2d::Orientation2d() :
right(Vector2d(1, 0))
{}

Orientation2d::Orientation2d(Vector2d right) :
right(right)
{}

Orientation2d::~Orientation2d()
{}

void Orientation2d::Calculate_Up() {
	up = Vector2d(-right.y, right.x);
}

void Orientation2d::Rotate(double angle) {
	right.Rotate(angle);
}

Vector2d Orientation2d::S_To_O(Vector2d v) {
	Calculate_Up();
	Vector2d v2 = Vector2d();
	v2 += right *	v.x;
	v2 += up *		v.y;
	return v2;
}

Transform2d::Transform2d() :
position		(),
orientation	(),
scale	(1.0, 1.0)
{}

Transform2d::Transform2d(Vector2d position) :
position		(position),
orientation	(),
scale	(1.0, 1.0)
{}

Transform2d::Transform2d(Vector2d position, Orientation2d orientation) :
position		(position),
orientation	(orientation),
scale	(1.0, 1.0)
{}

Transform2d::Transform2d(Vector2d position, Orientation2d orientation, Vector2d scale) :
position		(position),
orientation	(orientation),
scale	(scale)
{}

Transform2d::~Transform2d()
{}

void Transform2d::Rotate(double angle, Vector2d pt) {
	orientation.right.Rotate(angle);
	position -= pt;
	position.Rotate(angle);
	position += pt;
}

Matrix4f Transform2d::Object_Matrix() {
	orientation.Calculate_Up();

	float m[16] = {
		orientation.right.x * scale.x, 	orientation.right.y * scale.y, 	0,	0,
		orientation.up.x * scale.x, 		orientation.up.y * scale.y, 		0, 	0,
		0, 							0, 							1,	0,
		position.x, 						position.y, 						0, 	1
	};

	return Matrix4f(m);
}

Matrix4f Transform2d::Camera_Matrix() {
	orientation.Calculate_Up();

	float m[16] = {
		orientation.right.x * scale.x,	orientation.up.x * scale.x,	0, 0,
		orientation.right.y * scale.y,	orientation.up.y * scale.y,	0, 0,
		0,							0,						1, 0,
		-position.Dot(orientation.right),		-position.Dot(orientation.up),	0, 1
	};

	return Matrix4f(m);
}

void Transform2d::Apply_Object() {
	modelStack.Push();
	modelStack.Top() *= Matrix4f(Object_Matrix());
}

void Transform2d::Apply_Camera() {
	viewStack.Push();
	viewStack.Top() *= Matrix4f(Object_Matrix());
}

void Transform2d::Unapply_Object() {
	modelStack.Pop();
}

void Transform2d::Unapply_Camera() {
	viewStack.Pop();
}

Vector2d Transform2d::S_To_O(Vector2d v) {
	orientation.Calculate_Up();
	Vector2d v2 = position;
	v2 += orientation.right *	v.x * scale.x;
	v2 += orientation.up *		v.y * scale.y;

	return v2;
}

Orientation2d Transform2d::S_To_O(Orientation2d o) {
	return Orientation2d(
		orientation.S_To_O(o.right)
		);
}

Transform2d Transform2d::S_To_O(Transform2d t) {
	return Transform2d(
		S_To_O(t.position),
		S_To_O(t.orientation),
		Vector2d(t.scale.x * scale.x, t.scale.y * scale.y)
		);
}

Orientation3d::Orientation3d() :
forward		(Vector3d(0, 0, -1)),
up		(Vector3d(0, 1, 0)),
right	(Vector3d(1, 0, 0))
{}

Orientation3d::Orientation3d(Vector3d forward, Vector3d up) :
forward		(forward),
up		(up),
right	(forward.Cross(up))
{}

Orientation3d::~Orientation3d()
{}
/*
void Orientation3d::Calculate_Right() {
	right = forward.Cross(up);
}*/

void Orientation3d::Rotate(Vector3d r) {
	double angle = r.Magnitude();
	if (angle == 0.0)
		return;
	r /= angle;
	forward.Rotate(angle, r);
	up.Rotate(angle, r);
	right = forward.Cross(up);
}

void Orientation3d::Rotate(double angle, Vector3d axis) {
	forward.Rotate(angle, axis);
	up.Rotate(angle, axis);
	right = forward.Cross(up);
}

void Orientation3d::Rotate_S(Vector3d r) {
	r = S_To_O(r);
	forward.Rotate(r);
	up.Rotate(r);
	right = forward.Cross(up);
}

Transform3d::Transform3d() :
position		(),
orientation	(),
scale	(1.0, 1.0, 1.0)
{}

Transform3d::Transform3d(Vector3d position) :
position		(position),
orientation	(),
scale	(1.0, 1.0, 1.0)
{}

Transform3d::Transform3d(Vector3d position, Orientation3d orientation) :
position		(position),
orientation	(orientation),
scale	(1.0, 1.0, 1.0)
{}

Transform3d::Transform3d(Vector3d position, Orientation3d orientation, Vector3d scale) :
position		(position),
orientation	(orientation),
scale	(scale)
{}

Transform3d::~Transform3d()
{}

void Transform3d::Rotate(Vector3d r, Vector3d pt) {
	orientation.forward.Rotate(r);
	orientation.up.Rotate(r);
	position -= pt;
	position.Rotate(r);
	position += pt;
}

Matrix4f Transform3d::Object_Matrix() {
	//orientation.Calculate_Right();

	float m[16] = {
		orientation.right.x * scale.x, 	orientation.right.y * scale.y, 	orientation.right.z * scale.z,	0,
		orientation.up.x * scale.x, 		orientation.up.y * scale.y, 		orientation.up.z * scale.z, 		0,
		-orientation.forward.x * scale.x, 	-orientation.forward.y * scale.y, 	-orientation.forward.z * scale.z,	0,
		position.x, 						position.y, 						position.z, 						1
	};

	return Matrix4f(m);
}

Matrix4f Transform3d::Camera_Matrix() {
	//orientation.Calculate_Right();

	float m[16] = {
		orientation.right.x * scale.x,	orientation.up.x * scale.x,	-orientation.forward.x * scale.x,	0,
		orientation.right.y * scale.y,	orientation.up.y * scale.y,	-orientation.forward.y * scale.y,	0,
		orientation.right.z * scale.z,	orientation.up.z * scale.z,	-orientation.forward.z * scale.z,	0,
		-position.Dot(orientation.right),		-position.Dot(orientation.up),	position.Dot(orientation.forward),		1
	};

	return Matrix4f(m);
}

void Transform3d::Apply_Object() {
	modelStack.Push();
	modelStack.Top() *= Matrix4f(Object_Matrix());
}

void Transform3d::Apply_Camera() {
	viewStack.Push();
	viewStack.Top() *= Matrix4f(Camera_Matrix());
}

void Transform3d::Unapply_Object() {
	modelStack.Pop();
}

void Transform3d::Unapply_Camera() {
	viewStack.Pop();
}

Orientation3d Transform3d::S_To_O(Orientation3d o) {
	return Orientation3d(
		orientation.S_To_O(o.forward), 
		orientation.S_To_O(o.up)
		);
}

Transform3d Transform3d::S_To_O(Transform3d t) {
	return Transform3d(
		S_To_O(t.position), 
		S_To_O(t.orientation), 
		Vector3d(t.scale.x * scale.x, t.scale.y * scale.y, t.scale.z * scale.z)
		);
}

Matrix4f::Matrix4f() {
	for (int i = 0; i < 16; i++)
		data[i] = 0;
	data[0] = 1;
	data[5] = 1;
	data[10] = 1;
	data[15] = 1;
}

Matrix4f::Matrix4f(float data[16]) {
	for (int i = 0; i < 16; i++)
		this->data[i] = data[i];
}

Matrix4f::~Matrix4f() 
{}

Matrix4f Matrix4f::operator+(Matrix4f& m) {
	Matrix4f m1 = Matrix4f();
	for (int i = 0; i < 16; i++)
		m1.data[i] = data[i] + m.data[i];
	return m1;
}

Matrix4f Matrix4f::operator-(Matrix4f& m) {
	Matrix4f m1 = Matrix4f();
	for (int i = 0; i < 16; i++)
		m1.data[i] = data[i] - m.data[i];
	return m1;
}

Matrix4f Matrix4f::operator*(Matrix4f& m) {
	Matrix4f m1 = Matrix4f();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float tot = 0;
			for (int k = 0; k < 4; k++)
				tot += data[i * 4 + k] * m.data[k * 4 + j];
			m1.data[i * 4 + j] = tot;
		}
	}
	return m1;
}

void Matrix4f::operator+=(Matrix4f& m) {
	*this = *this + m;
}

void Matrix4f::operator-=(Matrix4f& m) {
	*this = *this - m;
}

void Matrix4f::operator*=(Matrix4f& m) {
	*this = *this * m;
}

Matrix4fStack::Matrix4fStack() :
n(1) 
{}

Matrix4fStack::~Matrix4fStack()
{}

void Matrix4fStack::Push() {
	if (n >= 16)
		Die("matrix stack overflow");
	mats[n] = mats[n - 1];
	n++;
}

void Matrix4fStack::Pop() {
	if (n <= 1)
		Die("matrix stack underflow");
	n--;
}

void Matrix4fStack::Clear() {
	n = 1;
	mats[0] = Matrix4f();
}

Matrix4f& Matrix4fStack::Top() {
	return mats[n - 1];
}

