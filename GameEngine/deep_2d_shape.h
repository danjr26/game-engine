#ifndef DEEP_2D_SHAPE_H
#define DEEP_2D_SHAPE_H

#include "axis_aligned_box.h"
#include "box.h"
#include "sphere.h"

template<class T, class ShapeT>
class Deep2DShape : public ShapeT {
protected:
	T depth;

public:
	Deep2DShape(const ShapeT& in_shape, T in_depth) : 
		ShapeT(in_shape),
		depth(in_depth)
	{}

	T Get_Depth() const {
		return depth;
	}

	void Set_Depth(T in_depth) {
		depth = in_depth;
	}
};

template<class T>
using DeepCircle = Deep2DShape<T, Circle<T>>;
using DeepCirclef = DeepCircle<float>;
using DeepCircled = DeepCircle<double>;

template<class T>
using DeepAxisAlignedRectangle = Deep2DShape<T, AxisAlignedRectangle<T>>;
using DeepAxisAlignedRectanglef = DeepAxisAlignedRectangle<float>;
using DeepAxisAlignedRectangled = DeepAxisAlignedRectangle<double>;

#endif 
