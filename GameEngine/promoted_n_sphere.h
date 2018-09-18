#ifndef PROMOTED_N_SPHERE_H
#define PROMOTED_N_SPHERE_H

#include "n_sphere.h"

template<class T, uint n>
class PromotedNSphere : private Sphere<T, n - 1> {

};

#endif