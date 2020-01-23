#include "projection.h"

template<class T>
Projection<T>::Projection() :
	mMinima(Vector<T, 3>(-1, -1, 0)),
	mMaxima(Vector<T, 3>(1, 1, -1)),
	type(Type::orthographic)
{}

template<class T>
Projection<T>::Projection(T i_near, T i_far, T i_horizontalAngle, T i_xToYRatio, Type i_type) :
type(i_type) {
	mMaxima[0] = tan(i_horizontalAngle) * i_near;
	mMaxima[1] = mMaxima[0] / i_xToYRatio;
	mMaxima[2] = i_far;
	mMinima[0] = -mMaxima[0];
	mMinima[1] = -mMaxima[1];
	mMinima[2] = i_near;
}

template<class T>
Projection<T>::Projection(Vector<T, 3> i_minima, Vector<T, 3> i_maxima, Type i_type) :
	mMinima(i_minima),
	mMaxima(i_maxima),
	type(i_type)
{}

template<class T>
T Projection<T>::getViewAngle() {
	return atan(mMaxima[0] / mMinima[2]);
}

template<class T>
void Projection<T>::setViewAngle(T i_value) {
	mMaxima[0] = tan(i_value) * mMinima[2];
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
