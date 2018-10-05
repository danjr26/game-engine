#ifndef BOX_H
#define BOX_H

#include "transformable_object.h"
#include "definitions.h"
#include <initializer_list>

template<class T, uint n>
struct Box {
private:
	Vector<T, n> origin;
	Vector<T, n> axes[n];

private:
	Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) :
		origin(in_origin) {

		for (uint i = 0; i < n; i++) axes[i] = in_axes[i];
	}

public:
	Vector<T, n> Get_Origin() {
		return origin;
	}

	void Get_Axes(Vector<T, n>* out_axes) {
		for (uint i = 0; i < n; i++) out_axes[i] = axes[i];
	}

	void Apply_Transform(Transform& transform) {
		origin = Vector<T, n>(transform.Apply_To_Local_Point(Vector<T, 3>(origin)));
		for (uint i = 0; i < n; i++) {
			axes[i] = Vector<T, n>(transform.Apply_To_Local_Vector(Vector<T, 3>(axes[i])));
		}
	}

	static Box From_Origin_Axes(const Vector<T, n>& in_origin, std::initializer_list<const Vector<T, n>&> in_axes) {
		return Box(in_origin, in_axes.begin);
	}
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif