#ifndef VECTOR3_H
#define VECTOR3_H

#include "vector.h"

template<class T>
class Vector3 : public Vector<T, 3> {
public:
	using Vector<T, 3>::Vector;

	T X() const {
		return members[0];
	}

	T Y() const {
		return members[1];
	}

	T Z() const {
		return members[2];
	}

	void Rotate(const Vector<T, 3>& v) {
		T angle = v.Magnitude();
		if (angle == 0) {
			return;
		}
		Vector<T, 3> axis = v / angle;

		T sine = sin(angle);
		T cosine = cos(angle);
		T oneMinusCosine = (T)1 - cos(angle);

		T x = v.members[0];
		T y = v.members[1];
		T z = v.members[2];

		Vector<T, 3> old = members;

		members[0] =
			(cosine + x * x * oneMinusCosine) * old.X() +
			(x * y * oneMinusCosine - z * sine) * old.Y() +
			(x * z * oneMinusCosine + y * sine) * old.Z();
		members[1] =
			(y * x * oneMinusCosine + z * sine) * old.X() +
			(cosine + y * y * oneMinusCosine) * old.Y() +
			(y * z * oneMinusCosine - x * sine) * old.Z();
		members[2] =
			(z * x * oneMinusCosine - y * sine) * old.X() +
			(z * y * oneMinusCosine + x * sine) * old.Y() +
			(cosine + z * z * oneMinusCosine) * old.Z();
	}

	Vector<T, 3> Rotated(const Vector<T, 3>& v) const {
		Vector<T, 3> vOut = members;
		vOut.Rotate(v);
		return vOut;
	}

	Vector<T, 3> Cross(const Vector<T, 3>& v) const {
		return Vector<T, 3>(
			members[1] * v.members[2] - members[2] * v.members[1],
			members[2] * v.members[0] - members[0] * v.members[2],
			members[0] * v.members[1] - members[1] * v.members[0]
			);
	}
};

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

#endif