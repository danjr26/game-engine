#ifndef VECTOR2_H
#define VECTOR2_H

#include "vector.h"

template<class T>
class Vector2 : public Vector<T, 2> {
public:
	using Vector<T, 2>::Vector;

	T X() const {
		return members[0];
	}

	T Y() const {
		return members[1];
	}

	void Rotate(T angle) {
		angle = angle * DEG_TO_RAD;
		T sine = sin(angle);
		T cosine = cos(angle);
		T tempX = members[0] * cosine - members[1] * sine;
		members[1] = members[0] * sine + members[1] * cosine;
		members[0] = tempX;
	}

	Vector<T, 2> Rotated(T angle) const {
		Vector<T, 2> vOut = members;
		vOut.Rotate(angle);
		return vOut;
	}

	Vector<T, 2> Orthogonal() const {
		return { members[1], -members[0] };
	}
};

using Vector2ui = Vector2<uint>;
using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;



#endif