#ifndef AXIS_ALIGNED_HALF_SPACE_H
#define AXIS_ALIGNED_HALF_SPACE_H

#include "geometric.h"

template<class T, uint n>
class AAHalfSpace : public Geometric<T, n> {
private:
	int mDimension;
	T mValue;

	AAHalfSpace(int i_dimension, T i_value);

public:
	AAHalfSpace();

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	uint getDimension() const;
	T getValue() const;
	bool isPositive() const;
	bool containsValue(T i_value) const;
	bool containsPoint(const Vector<T, n>& in_point) const;
	Vector<T, n> getDirection() const;

public:
	static AAHalfSpace<T, n> fromDimensionValue(uint i_dimension, T i_value, bool i_isPositive);
	static AAHalfSpace<T, n> fromInverse(const AAHalfSpace& i_inverse);
};

using AAHalfSpace2f = AAHalfSpace<float, 2>;
using AAHalfSpace2d = AAHalfSpace<double, 2>;
using AAHalfSpace3f = AAHalfSpace<float, 3>;
using AAHalfSpace3d = AAHalfSpace<double, 3>;


#endif


