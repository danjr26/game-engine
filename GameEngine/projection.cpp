#include "projection.h"

template<class T>
Projection<T>::Projection() :
	minima(Vector<T, 3>(-1, -1, 0)),
	maxima(Vector<T, 3>(1, 1, -1)),
	type(Type::orthographic)
{}

template<class T>
Projection<T>::Projection(T in_near, T in_far, T in_horizontalAngle, T in_xToYRatio, Type in_type) :
type(in_type) {
	maxima[0] = tanf(in_horizontalAngle) * in_near;
	maxima[1] = maxima[0] / in_xToYRatio;
	maxima[2] = in_far;
	minima[0] = -maxima[0];
	minima[1] = -maxima[1];
	minima[2] = in_near;
}

template<class T>
Projection<T>::Projection(Vector<T, 3> in_minima, Vector<T, 3> in_maxima, Type in_type) :
	minima(in_minima),
	maxima(in_maxima),
	type(in_type)
{}

template<class T>
T Projection<T>::Get_View_Angle() {
	return atanf(maxima[0] / minima[2]);
}

template<class T>
void Projection<T>::Set_View_Angle(T in_value) {
	maxima[0] = tanf(in_value) * minima[2];
	minima[0] = -maxima[0];
}

template<class T>
Matrix<T, 4, 4> Projection<T>::Get_Matrix() {
	if (type == Type::perspective) {
		return Matrix<T, 4, 4>::Perspective(minima, maxima);
	}
	else {
		return Matrix<T, 4, 4>::Orthographic(minima, maxima);
	}
}

template class Projection<float>;
template class Projection<double>;
