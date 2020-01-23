#include "../include/internal/perlin.h"

template<class T, uint n>
Perlin<T, n>::Perlin(Vector<uint, n>& i_dimensions) :
	mDimensions(i_dimensions),
	mGrid(nullptr),
	mWrap{ 0 } {
	for (uint i = 0; i < n; i++) {
		if (i_dimensions[i] == 0) fail();
		mWrap[i] = false;
	}

	mGrid = new Vector<T, n>[i_dimensions.compProduct()];
	for (uint i = 0; i < mDimensions.compProduct(); i++) {
		for (uint j = 0; j < n; j++) {
			mGrid[i][j] = gauss<T>();
		}
		mGrid[i].normalize();
	}
}

template<class T, uint n>
Perlin<T, n>::Perlin(Perlin<T, n>&& i_other) :
	mDimensions(i_other.mDimensions),
	mGrid(nullptr),
	mWrap{ 0 } {
	
	std::swap(mGrid, i_other.mGrid);
	for (uint i = 0; i < n; i++) {
		mWrap[i] = i_other.mWrap[i];
	}
}

template<class T, uint n>
Perlin<T, n>::~Perlin() {
	if (mGrid) delete[] mGrid;
}

template<class T, uint n>
Vector<uint, n> Perlin<T, n>::getDimensions() const {
	return mDimensions;
}

template<class T, uint n>
bool Perlin<T, n>::getWrap(uint i_index) const {
	if (i_index >= n) fail();
	return mWrap[i_index];
}

template<class T, uint n>
void Perlin<T, n>::setWrap(uint i_index, bool i_value) {
	if (i_index >= n) fail();
	mWrap[i_index] = i_value;
}

template<class T, uint n>
T Perlin<T, n>::evaluate(const Vector<T, n>& i_coords) const {
	T out = 0;
	
	Vector<T, n> clampedCoords = i_coords;
	Vector<T, n> innerCoords;
	Vector<uint, n> minima, maxima;
	Vector<uint, n> widths = mDimensions;
	for (uint i = 0; i < n; i++) {
		clampedCoords[i] = clamp<T>(clampedCoords[i], 0, 1);
		widths[i] += (uint)mWrap[i];
		widths[i]--;
		innerCoords[i] = clampedCoords[i] * widths[i];
		minima[i] = (uint)innerCoords[i];
		innerCoords[i] -= minima[i];
		maxima[i] = minima[i] + 1;
	}

	for (uint i = 0; i < (1 << n); i++) {
		Vector<uint, n> relativeIndices;
		Vector<uint, n> vectorIndices;
		for (uint j = 0; j < n; j++) {
			relativeIndices[j] = ((i >> j & 1));
			vectorIndices[j] = ((i >> j) & 1) ? maxima[j] : minima[j];
			if (vectorIndices[j] == mDimensions[j]) 
				vectorIndices[j] = 0;
		}
		uint gridIndex = vectorIndices.getAsCoords(mDimensions);
		Vector<T, n>& vector = mGrid[gridIndex];
		T dot = (innerCoords - relativeIndices).dot(vector);
		for (uint j = 0; j < n; j++) {
			T factor = ((i >> j) & 1) ?
				qerp<T>(0, 1, innerCoords[j]) :
				qerp<T>(0, 1, 1 - innerCoords[j]);
			dot *= factor;
		}
		out += dot;
	}

	return (out + (T)1.0) / (T)2.0;
}

//template class Perlin<float, 1>;
//template class Perlin<double, 1>;
template class Perlin<float, 2>;
template class Perlin<double, 2>;
//template class Perlin<float, 3>;
//template class Perlin<double, 3>;
//template class Perlin<float, 4>;
//template class Perlin<double, 4>;
