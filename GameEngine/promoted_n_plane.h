#ifndef PROMOTED_N_PLANE
#define PROMOTED_N_PLANE

#include "n_plane.h"

template<class T, uint n>
class PromotedNPlane : private NPlane<T, n - 1> {
private:
	Vector<T, n> orientation;
public:
	PromotedNPlane(NPlane<T, n - 1>& in_plane, Vector<T, n>& in_orientation) :
		NPlane(in_plane),
		orientation(in_orientation)
	{}
};

#endif