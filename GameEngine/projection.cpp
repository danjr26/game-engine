#include "projection.h"

template<class T>
Projection<T>::Projection() :
	mMinima(Vector<T, 3>(-1, -1, 0)),
	mMaxima(Vector<T, 3>(1, 1, -1)),
	type(Type::orthographic)
{}

template<class T>
Projection<T>::Projection(T in_near, T in_far, T in_horizontalAngle, T in_xToYRatio, Type in_type) :
type(in_type) {
	mMaxima[0] = tan(in_horizontalAngle) * in_near;
	mMaxima[1] = mMaxima[0] / in_xToYRatio;
	mMaxima[2] = in_far;
	mMinima[0] = -mMaxima[0];
	mMinima[1] = -mMaxima[1];
	mMinima[2] = in_near;
}

template<class T>
Projection<T>::Projection(Vector<T, 3> in_minima, Vector<T, 3> in_maxima, Type in_type) :
	mMinima(in_minima),
	mMaxima(in_maxima),
	type(in_type)
{}

template<class T>
T Projection<T>::getViewAngle() {
	return atan(mMaxima[0] / mMinima[2]);
}

template<class T>
void Projection<T>::setViewAngle(T in_value) {
	mMaxima[0] = tan(in_value) * mMinima[2];
	mMinima[0] = -mMaxima[0];
}

template<class T>
Matrix<T, 4, 4> Projection<T>::getMatrix() {
	if (type == Type::perspective) {
		return Matrix<T, 4, 4>::perspective(mMinima, mMaxima);
	}
	else {
		return Matrix<T, 4, 4>::orthographic(mMinima, mMaxima);
	}
}

template class Projection<float>;
template class Projection<double>;
