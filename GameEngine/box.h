#ifndef BOX_H
#define BOX_H

#include "transformable_object.h"
#include "matrix.h"
#include "definitions.h"
#include <initializer_list>

template<class T, uint n>
class Box {
private:
	Vector<T, n> origin;
	Vector<T, n> axes[n];

private:
	Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes);

public:
	Vector<T, n> Get_Origin();
	Vector<T, n> Get_Center();
	void Get_Axes(Vector<T, n>* out_axes);
	void Get_Corners(Vector<T, n>* out_corners);
	void Apply_Transform(const Transform<T, n>& transform);

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Area() const;
	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Perimeter() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Volume() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Surface_Area() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Total_Edge_Length() const;

	Vector<T, n> Random_Point_Boundary() const;
	Vector<T, n> Random_Point_Inside() const;

public:
	static Box<T, n> From_Center_Axes(const Vector<T, n>& in_center, const Vector<T, n>* in_axes);
	static Box<T, n> From_Center_Axes(const Vector<T, n>& in_center, std::initializer_list<Vector<T, n>> in_axes);
	static Box<T, n> From_Origin_Axes(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes);
	static Box<T, n> From_Origin_Axes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes);
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif



