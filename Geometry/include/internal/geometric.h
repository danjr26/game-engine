#ifndef GEOMETRIC_H
#define GEOMETRIC_H

#include "../../../Utilities/include/utilities.h"
#include "transform.h"

template<class T, uint n>
struct ClosestRequest {
	using flags_t = uint;

	enum Fields : flags_t {
		point = 1 << 0,
		normal = 1 << 1
	};

	enum NormalContact {
		on_point,
		on_edge,
		on_face
	};

	Vector<T, n> mPoint;
	Vector<T, n> mNormal;
	NormalContact mContactType;
	flags_t mFlags;

	ClosestRequest() :
		mPoint(),
		mNormal(),
		mContactType(on_point),
		mFlags(0) {}

	ClosestRequest(const Vector<T, n>& i_point, flags_t i_flags = Fields::point) :
		mPoint(i_point),
		mNormal(),
		mContactType(on_point),
		mFlags(i_flags) {}

	bool wantsPoint() const {
		return (mFlags & Fields::point);
	}

	bool wantsNormal() const {
		return mFlags & Fields::normal;
	}
};

template<class T, uint n>
class Geometric {
	virtual void applyTransform(const Transform<T, n>& i_transform) = 0;
	virtual void getClosest(ClosestRequest<T, n>& io_request) const = 0;
};

#endif 
