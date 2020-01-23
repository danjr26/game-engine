#ifndef HALF_SPACE_H
#define HALF_SPACE_H

#include "geometric.h"

template<class T, uint n>
class HalfSpace : public Geometric<T, n> {
private:
	Vector<T, n> mPoint;
	Vector<T, n> mDirection;

	HalfSpace(const Vector<T, n>& i_point, const Vector<T, n>& i_direction);

public:
	HalfSpace();

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	Vector<T, n> getPoint() const;
	void setPoint(const Vector<T, n>& i_point);
	Vector<T, n> getDirection() const;
	void setDirection(const Vector<T, n>& i_direction);
	bool containsPoint(const Vector<T, n>& i_point);

public:
	static HalfSpace<T, n> fromPointDirection(const Vector<T, n>& i_point, const Vector<T, n>& i_direction);
};

using HalfSpace2f = HalfSpace<float, 2>;
using HalfSpace2d = HalfSpace<double, 2>;
using HalfSpace3f = HalfSpace<float, 3>;
using HalfSpace3d = HalfSpace<double, 3>;

#endif


