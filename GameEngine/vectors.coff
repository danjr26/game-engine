#include "vectors.h"
#include "framework.h"

/*
template<class T>
Vector2<T>::Vector2() : 
x(0), 
y(0) 
{}

template<class T>
Vector2<T>::Vector2(T x, T y) : 
x(x),
y(y) 
{}

template<class T>
template<class T2>
Vector2<T>::Vector2(Vector2<T2> v) :
x(v.x),
y(v.y)
{}

template<class T>
template<class T2>
Vector2<T>::Vector2(Vector3<T2> v) :
x(v.x),
y(v.y)
{}

template<class T>
Vector2<T> Vector2<T>::operator-() const {
	return Vector2<T>(-x, -y);
}

template<class T>
template<class T2>
bool Vector2<T>::operator==(Vector2<T2> v) const {
	return x == v.x && y == v.y;
}

template<class T>
template<class T2>
bool Vector2<T>::operator!=(Vector2<T2> v) const {
	return x != v.x || y != v.y;
}

template<class T>
template<class T2>
Vector2<T> Vector2<T>::operator+(Vector2<T2> v) const {
	return Vector2<T>(x + v.x, y + v.y);
}

template<class T>
template<class T2>
Vector2<T> Vector2<T>::operator-(Vector2<T2> v) const {
	return Vector2<T>(x - v.x, y - v.y);
}

template<class T>
template<class T2>
Vector2<T> Vector2<T>::operator*(T2 s) const {
	return Vector2<T>(x * s, y * s);
}

template<class T>
template<class T2>
Vector2<T> Vector2<T>::operator/(T2 s) const {
	return Vector2<T>(x / s, y / s);
}

template<class T>
template<class T2>
void Vector2<T>::operator+=(Vector2<T2> v) {
	x += v.x;
	y += v.y;
}

template<class T>
template<class T2>
void Vector2<T>::operator-=(Vector2<T2> v) {
	x -= v.x;
	y -= v.y;
}

template<class T>
template<class T2>
void Vector2<T>::operator*=(T2 s) {
	x *= s;
	y *= s;
}

template<class T>
template<class T2>
void Vector2<T>::operator/=(T2 s) {
	x /= s;
	y /= s;
}

template <class T>
bool Vector2<T>::Is_Zero() const {
	return x == 0.0 && y == 0.0;
}

template<class T>
T Vector2<T>::Theta() const {
	return (acos(x) + (y < 0) ? PI : 0.0) * RAD_TO_DEG;
}

template<class T>
template<class T2>
T Vector2<T>::Theta(Vector2<T2> v) const {
	double t = acos(Normalized().Dot(v.Normalized())) * RAD_TO_DEG;
	return (t == t) ? t : 180.0;
}

template<class T>
template<class T2>
void Vector2<T>::Rotate(T2 angle) {
	angle = angle * DEG_TO_RAD;
	double sine = sin(angle);
	double cosine = cos(angle);
	double tempx = x * cosine - y * sine;
	y = x * sine + y * cosine;
	x = tempx;
}

template<class T>
template<class T2>
void Vector2<T>::Rotate(T2 angle, Vector2<T2> point) {
	*this -= point;
	Rotate(angle);
	*this += point;
}

template<class T>
template<class T2>
Vector2<T>	Vector2<T>::Rotated(T2 angle) const {
	Vector2<T> v = *this;
	v.Rotate(angle);
	return v;
}

template<class T>
template<class T2>
Vector2<T>	Vector2<T>::Rotated(T2 angle, Vector2<T2> point) const {
	Vector2<T> v = *this;
	v.Rotate(angle, point);
	return v;
}

template<class T>
T Vector2<T>::Magnitude() const {
	return sqrt(x * x + y * y);
}

template<class T>
void Vector2<T>::Normalize() {
	if (Is_Zero())
		return;
	T mag = Magnitude();
	x /= mag;
	y /= mag;
}

template<class T>
Vector2<T> Vector2<T>::Normalized() const {
	Vector2<T> v = *this;
	v.Normalize();
	return v;
}

template<class T>
template<class T2>
T Vector2<T>::Dot(Vector2<T2> v) const {
	return x * v.x + y * v.y + z * v.z;
}

template<class T>
template<class T2>
Vector2<T> Vector2<T>::Compwise(Vector2<T2> v) const {
	return Vector2<T>(x * v.x, y * v.y);
}

template<class T>
Vector2<T> Vector2<T>::Random_Normalized() {
	return Vector2<T>(1, 0).Rotated(Rand_Double(0.0, 360.0));
}
*/
/*
template<class T>
Vector3<T>::Vector3() : 
x(0), 
y(0),
z(0)
{}

template<class T>
Vector3<T>::Vector3(T x, T y, T z) : 
x(x),
y(y),
z(z)
{}

template<class T>
template<class T2>
Vector3<T>::Vector3(Vector3<T2> v) :
x(v.x),
y(v.y),
z(v.z)
{}

template<class T>
template<class T2>
Vector3<T>::Vector3(Vector2<T2> v, T2 z) :
x(v.x),
y(v.y),
z(z)
{}

template<class T>
template<class T2>
Vector3<T>::Vector3(Vector4<T2> v) :
x(v.x),
y(v.y),
z(v.z)
{}

template<class T>
Vector3<T> Vector3<T>::operator-() const {
	return Vector3<T>(-x, -y, -z);
}

template<class T>
template<class T2>
bool Vector3<T>::operator==(Vector3<T2> v) const {
	return x == v.x && y == v.y && z == v.z;
}

template<class T>
template<class T2>
bool Vector3<T>::operator!=(Vector3<T2> v) const {
	return x != v.x || y != v.y || z != v.z;
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::operator+(Vector3<T2> v) const {
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::operator-(Vector3<T2> v) const {
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::operator*(T2 s) const {
	return Vector3<T>(x * s, y * s, z * s);
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::operator/(T2 s) const {
	return Vector3<T>(x / s, y / s, z / s);
}

template<class T>
template<class T2>
void Vector3<T>::operator+=(Vector3<T2> v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

template<class T>
template<class T2>
void Vector3<T>::operator-=(Vector3<T2> v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

template<class T>
template<class T2>
void Vector3<T>::operator*=(T2 s) {
	x *= s;
	y *= s;
	z *= s;
}

template<class T>
template<class T2>
void Vector3<T>::operator/=(T2 s) {
	x /= s;
	y /= s;
	z /= s;
}

template <class T>
bool Vector3<T>::Is_Zero() const {
	return x == 0.0 && y == 0.0 && z == 0.0;
}

template<class T>
T Vector3<T>::Theta() const {
	return acos(-z) * RAD_TO_DEG;;
}

template<class T>
template<class T2>
T Vector3<T>::Theta(Vector3<T2> v) const {
	double t = acos(Normalized().Dot(v.Normalized())) * RAD_TO_DEG;
	return (t == t) ? t : 180.0;
}

template<class T>
template<class T2>
void Vector3<T>::Rotate(Vector3<T2> v) {
	Rotate(v.Theta(), v.Normalized());
}

template<class T>
template<class T2>
void Vector3<T>::Rotate(T2 angle, Vector3<T2> axis) {
	double theta = angle * DEG_TO_RAD;
	Vector3d cross = axis.Cross(*this);
	double c = cos(theta);
	double s = sin(theta);
	double dot = axis.Dot(*this);
	*this = *this * c + cross * s + axis * dot * (1.0 - c);
}

template<class T>
template<class T2>
void Vector3<T>::Rotate(Vector3<T2> v, Vector3<T2> point) {
	Rotate(v.Theta(), v.Normalized(), point);
}

template<class T>
template<class T2>
void Vector3<T>::Rotate(T2 angle, Vector3<T2> axis, Vector3<T2> point) {
	*this -= point;
	Rotate(angle);
	*this += point;
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::Rotated(Vector3<T2> v) const {
	return Rotated(v.Theta(), v.Normalized());
}

template<class T>
template<class T2>
Vector3<T>	Vector3<T>::Rotated(T2 angle, Vector3<T2> axis) const {
	Vector3<T> v = *this;
	v.Rotate(angle);
	return v;
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::Rotated(Vector3<T2> v, Vector3<T2> point) const {
	return Rotated(v.Theta(), v.Normalized(), point);
}

template<class T>
template<class T2>
Vector3<T>	Vector3<T>::Rotated(T2 angle, Vector3<T2> axis, Vector3<T2> point) const {
	Vector3<T> v = *this;
	v.Rotate(angle, point);
	return v;
}

template<class T>
T Vector3<T>::Magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

template<class T>
void Vector3<T>::Normalize() {
	if (Is_Zero())
		return;
	T mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

template<class T>
Vector3<T> Vector3<T>::Normalized() const {
	Vector3<T> v = *this;
	v.Normalize();
	return v;
}

template<class T>
template<class T2>
T Vector3<T>::Dot(Vector3<T2> v) const {
	return x * v.x + y * v.y + z * v.z;
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::Cross(Vector3<T2> v) const {
	return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template<class T>
Vector3<T> Vector3<T>::Random_Normalized() {
	double a = Rand_Double(-1.0, 1.0);
	double b = Rand_Double(0.0, 2.0 * PI);
	double c = sqrt(1 - a * a);
	return Vector3<T>(c * cos(b), c * sin(b), a);
}

template<class T>
template<class T2>
Vector3<T> Vector3<T>::Compwise(Vector3<T2> v) const {
	return Vector3<T>(x * v.x, y * v.y, z * v.z);
}
*/
/*
template<class T>
Vector4<T>::Vector4() :
x(0),
y(0),
z(0),
w(0)
{}

template<class T>
Vector4<T>::Vector4(T x, T y, T z, T w) :
x(x),
y(y),
z(z),
w(w)
{}

template<class T>
template<class T2>
Vector4<T>::Vector4(Vector4<T2> v)  :
x(v.x),
y(v.y),
z(v.z),
w(v.w)
{}

template<class T>
template<class T2>
Vector4<T>::Vector4(Vector3<T2> v, T2 w) :
x(v.x),
y(v.y),
z(v.y),
w(w)
{}

template<class T>
template<class T2>
bool Vector4<T>::operator==(Vector4<T2> v) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<class T>
template<class T2>
bool Vector4<T>::operator!=(Vector4<T2> v) const {
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

template<class T>
template<class T2>
Vector4<T> Vector4<T>::operator+(Vector4<T2> v) const {
	return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<class T>
template<class T2>
Vector4<T> Vector4<T>::operator-(Vector4<T2> v) const {
	return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<class T>
template<class T2>
Vector4<T> Vector4<T>::operator*(T2 s) const {
	return Vector4<T>(x * s, y * s, z * s, w * s);
}

template<class T>
template<class T2>
Vector4<T> Vector4<T>::operator/(T2 s) const {
	return Vector4<T>(x / s, y / s, z / s, w / s);
}

template<class T>
template<class T2>
void Vector4<T>::operator+=(Vector4<T2> v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

template<class T>
template<class T2>
void Vector4<T>::operator-=(Vector4<T2> v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

template<class T>
template<class T2>
void Vector4<T>::operator*=(T2 s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

template<class T>
template<class T2>
void Vector4<T>::operator/=(T2 s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

template<class T>
Vector4<T> Vector4<T>::operator-() const {
	return Vector4<T>(-x, -y, -z, -w);
}

template<class T>
bool Vector4<T>::Is_Zero() const {
	return x == 0 && y == 0 && z == 0 && w == 0;
}

template<class T>
T Vector4<T>::Magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

template<class T>
void Vector4<T>::Normalize() {
	if (Is_Zero())
		return;
	T mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

template<class T>
Vector4<T> Vector4<T>::Normalized() const {
	Vector4<T> v = *this;
	v.Normalize();
	return v;
}

template<class T>
template<class T2>
T Vector4<T>::Dot(Vector4<T2> v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<class T>
template<class T2>
Vector4<T> Vector4<T>::Compwise(Vector4<T2> v) const {
	return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
}
*/