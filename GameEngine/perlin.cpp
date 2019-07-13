#include "perlin.h"
#include "exceptions.h"
#include "misc.h"

template<class T, uint n>
Perlin<T, n>::Perlin(Vector<uint, n>& in_dimensions) {
	for (uint i = 0; i < n; i++) {
		if (in_dimensions[i] == 0) throw InvalidArgumentException();
	}

	mGrid = new Vector<T, n>[in_dimensions.componentProduct()];
	for (uint i = 0; i < mDimensions.componentProduct(); i++) {
		for (uint j = 0; j < n; j++) {
			mGrid[i][j] = GEUtil::gauss<T>();
		}
		mGrid[i].normalize();
	}
}

template<class T, uint n>
Perlin<T, n>::~Perlin() {
	delete[] mGrid;
}

template<class T, uint n>
Vector<uint, n> Perlin<T, n>::getDimensions() const {
	return mDimensions;
}

template<class T, uint n>
bool Perlin<T, n>::getWrap(uint in_index) const {
	if (in_index >= n) throw InvalidArgumentException();
	return mWrap[in_index];
}

template<class T, uint n>
void Perlin<T, n>::setWrap(uint in_index, bool in_value) {
	if (in_index >= n) throw InvalidArgumentException();
	mWrap[in_index] = in_value;
}

template<class T, uint n>
T Perlin<T, n>::evaluate(const Vector<T, n>& in_coords) const {
	T out = 0;
	
	Vector<T, n> clampedCoords = in_coords;
	Vector<T, n> innerCoords;
	Vector<uint, n> minima, maxima;
	Vector<uint, n> widths = mDimensions;
	for (uint i = 0; i < n; i++) {
		clampedCoords[i] = GEUtil::clamp<T>(clampedCoords[i], 0, 1);
		widths[i] += (uint)mWrap[i];
		innerCoords[i] = clampedCoords[i] * widths[i];
		minima[i] = (uint)innerCoords[i];
		innerCoords[i] -= minima[i];
		maxima[i] = minima[i] + 1;
	}

	for (uint i = 0; i < (1 << n); i++) {
		Vector<uint, n> vectorIndices;
		for (uint j = 0; j < n; j++) {
			vectorIndices[j] = ((i >> j) & 1) ? minima[j] : maxima[j];
			if (vectorIndices[j] == mDimensions[j]) vectorIndices[j] = 0;
		}
		Vector<T, n>& vector = mGrid[vectorIndices.getAsCoords(mDimensions)];
		T dot = (innerCoords - vectorIndices).normalized().dot(vector);
		for (uint j = 0; j < n; j++) {
			dot *= ((i >> j) & 1) ? (innerCoords[i]) : (1 - innerCoords[i]);
		}
		out += dot;
	}

	return out;
}

template class Perlin<float, 1>;
template class Perlin<double, 1>;
template class Perlin<float, 2>;
template class Perlin<double, 2>;
template class Perlin<float, 3>;
template class Perlin<double, 3>;
template class Perlin<float, 4>;
template class Perlin<double, 4>;
