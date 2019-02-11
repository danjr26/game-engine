#ifndef POLYGON_H
#define POLYGON_H

#include "vector.h"
#include <vector>

template<template T, uint n>
class Polygon {
private:
	std::vector<Vector<T, n>> points;

private:
	Polygon(Vector<T, n> const* in_point, uint in_nPoints);

public:
	uint Get_Number_Points() const;
	Vector<T, n> Get_Point(uint in_index) const;
	void Set_Point(uint in_index, const Vector<T, n>& in_value);
	void
};

using Polygon2f = Polygon<float, 2>;
using Polygon2d = Polygon<double, 2>;
using Polygon3f = Polygon<float, 3>;
using Polygon3d = Polygon<double, 3>;

#endif