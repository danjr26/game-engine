#ifndef N_PLANE_H
#define N_PLANE_H

#include "vector.h"
#include "line.h"

template<class T, uint n>
struct NPlane {
public:
	enum class Side {
		front,
		on,
		back
	};
private:
	// for equation ax + by + ... = k:
	Vector<T, n> axis;	// {a, b, ... }
	T pointDot;			// k

public:
	NPlane(Vector<T, n> in_point, Vector<T, n> in_axis) :
		axis(in_axis),
		pointDot(in_axis.Dot(in_point))
	{}

	NPlane(const Line<T, n>& in_line) :
		axis(in_line.direction.Orthogonal()),
		pointDot(axis.Dot(in_line.point)) {

		static_assert(n == 2);
	}

	NPlane(const LineSegment<T, n>& in_segment) :
		axis((in_segment.point2 - in_segment.point1).Orthogonal()),
		pointDot(axis.Dot(in_segment.point1)) {
		
		static_assert(n == 2);
	}

	Side Which_Side(Vector<T, n> in_point) {
		T thatDot = axis.Dot(in_point);

		if (thatDot > pointDot) {
			return Side::front;
		}
		if (thatDot < pointDot) {
			return Side::back;
		} 
		return Side::on;
	}

	T Signed_Distance_To(Vector<T, n> in_point) {
		return axis.Dot(in_point) - pointDot;
	}

	T Unsigned_Distance_To(Vector<T, n> in_point) {
		return abs(axis.Dot(in_point) - pointDot);
	}
/*
	template<class T>
	Vector<T, 3> Coincidence(Plane<T> in_plane) {
		return axis.Cross(in_plane.axis);
	}
	*/
};

#endif