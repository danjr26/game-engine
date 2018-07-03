#ifndef GEOMETRIC_INTERSECTIONS_H
#define GEOMETRIC_INTERSECTIONS_H

#include "geometric_figures.h"
#include "matrix.h"
#include "exceptions.h"

template<class T, uint n>
class GeometricIntersection {
private:
	bool doesIntersect;

	enum class Type {
		none,
		def,
		point,
		ray,
		lineSegment,
		line,
		plane,
		sphere,
		promotedCircle
	} type;

	union {
		Vector<T, n> point;
		Ray<T, n> ray;
		LineSegment<T, n> lineSegment;
		Line<T, n> line;
		NPlane<T, n> plane;
		NSphere<T, n> sphere;
		PromotedCircle<T, n> promotedCircle;
	} intersection;

public:
	enum class FigureSolidity {
		wireframe,
		solid
	};

public:
	GeometricIntersection(
		const Vector<T, n>& in_point, 
		const Ray<T, n>& in_ray) {

		if ((in_point - in_ray.point).Dot(in_ray.direction) > 0 &&
			GeometricIntersection<T, n>(in_point, Line(in_ray)).Yes()) {

			intersection = in_point;
			doesIntersect = true;
		}
	}

	GeometricIntersection(
		const Vector<T, n>& in_point, 
		const LineSegment<T, n>& in_segment) {

		Vector<T, n> direction = in_segment.point2 - in_segment.point1;
		if ((in_point - in_segment.point1).Dot(direction) > 0 &&
			(in_point - in_segment.point2).Dot(direction) < 0 &&
			Intersection(in_point, Line<T, n>(in_ray.point, in_ray.direction))) {


		}

	}

	template<class T, uint n>
	GeometricIntersection(const Vector<T, n>& in_point, const Line<T, n>& in_line) {
		Matrix<T, n, 2> m;
		m.Column(0, in_line.direction);
		m.Column(1, in_point - in_line.point);
		m.REFify();
		return m.Submatrix_Is_Zero(1, n - 1, n - 1, n - 1);
	}

	template<class T, uint n>
	GeometricIntersection(const Vector<T, n>& in_point, const NPlane<T, n>& in_plane) {
		return in_plane.axis.Dot(in_point) == in_plane.pointDot;
	}

	template<class T, uint n>
	GeometricIntersection(const Vector<T, n>& in_point, const NSphere<T, n>& in_sphere, FigureSolidity in_solidity) {
		switch (in_solidity) {
		case FigureSolidity::wireframe:
			return (in_point - in_sphere.center).Dot_Self() == in_sphere.radius * in_sphere.radius;
		case FigureSolidity::solid:
			return (in_point - in_sphere.center).Dot_Self() <= in_sphere.radius * in_sphere.radius;
		}
	}

	template<class T, uint n>
	GeometricIntersection(const Vector<T, n>& in_point, const Triangle<T, n>& in_triangle, FigureSolidity in_solidity) {
		switch (in_solidity) {
		case FigureSolidity::wireframe:
			for (uint i = 0; i < 3; i++) {
				if (Intersection(in_point, in_ngon.Side())) {
					return true;
				}
			}
			return false;
		case FigureSolidity::solid:
			switch (n) {
			case 2:
				NPlane::Side sides[3];
				for (uint i = 0; i < 3; i++) {
					sides[i] = NPlane<T, n>(in_triangle.Side(i)).Which_Side(in_point);
				}
				if (sides[0] == NPlane::Side::front) {
					return sides[1] == NPlane::Side::back || sides[2] == NPlane::Side::back;
				}
				if (sides[0] == NPlane::Side::back) {
					return sides[1] == NPlane::Side::front || sides[2] == NPlane::Side::front;
				}
				if (sides[1] == NPlane::Side::front) {
					return sides[2] == NPlane::Side::back;
				}
				if (sides[1] == NPlane::Side::back) {
					return sides[2] == NPlane::Side::front;
				}
				return false;

			case 3:
				throw NotImplementedException();
				break;
			default:
				throw NotImplementedException();
			}
		}
	}

	template<class T, uint n, uint np>
	GeometricIntersection(const Vector<T, n>& in_point, const NGon<T, n, np>& in_ngon, FigureSolidity in_solidity) {
		switch(in_solidity) {
		case FigureSolidity::wireframe:
			for (uint i = 0; i < np; i++) {
				if (Intersection(in_point, in_ngon.Side())) {
					return true;
				}
			}
			return false;
		case FigureSolidity::solid:
			throw NotImplementedException();
		}
	}

	template<class T, uint n>
	inline GeometricIntersection(const Ray<T, n>& in_ray1, const Ray<T, n>& in_ray2, Ray<T, n>& out_ray) {

	}

	template<class T, uint n>
	GeometricIntersection(
		const Ray<T, n>& in_ray, 
		const LineSegment<T, n>& in_segment, 
		LineSegment<T, n> out_segment
	);

	template<class T, uint n>
	GeometricIntersection(
		const Ray<T, n>& in_ray, 
		const Line<T, n>& in_line, 
		Ray<T, n>& out_ray
	);

	template<class T, uint n>
	GeometricIntersection(
		const Ray<T, n>& in_ray, 
		const NPlane<T, n>& in_plane, 
		Ray<T, n>& out_ray
	);

	template<class T, uint n>
	GeometricIntersection(
		const Ray<T, n>& in_ray, 
		const NSphere<T, n>& in_sphere, 
		LineSegment<T, n>& out_segment, 
		FigureSolidity in_solidity = FigureSolidity::wireframe
	);

	template<class T, uint n, uint np>
	GeometricIntersection(
		const Ray<T, n>& in_ray, 
		const NGon<T, n, np>& in_polygon, 
		LineSegment<T, n>& out_segment, 
		FigureSolidity in_solidity = FigureSolidity::wireframe
	);

	template<class T>
	GeometricIntersection(const Line<T, 2>& in_line1, const Line<T, 2>& in_line2, Vector<T, 2>& out_point);

	template<class T>
	GeometricIntersection(const Line<T, 2>& in_line, const NSphere<T, 2>& in_circle, Vector<T, 2>& out_point1, Vector<T, 2>& out_point2);

	template<class T>
	GeometricIntersection(const Line<T, 3>& in_line1, const Line<T, 3>& in_line2, Vector<T, 3>& out_point);

	bool Yes() {
		return doesIntersect;
	}


};

#endif 