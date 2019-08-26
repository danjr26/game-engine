#include "perlin.h"
#include "exceptions.h"
#include "misc.h"
#include "log.h"

template<class T, uint n>
Perlin<T, n>::Perlin(Vector<uint, n>& in_dimensions) :
	mDimensions(in_dimensions),
	mGrid(nullptr) {
	for (uint i = 0; i < n; i++) {
		if (in_dimensions[i] == 0) throw InvalidArgumentException();
		mWrap[i] = false;
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
Perlin<T, n>::Perlin(Perlin<T, n>&& in_other) :
	mDimensions(in_other.mDimensions),
	mGrid(nullptr) {
	
	std::swap(mGrid, in_other.mGrid);
	for (uint i = 0; i < n; i++) {
		mWrap[i] = in_other.mWrap[i];
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
				GEUtil::qerp<T>(0, 1, innerCoords[j]) :
				GEUtil::qerp<T>(0, 1, 1 - innerCoords[j]);
			dot *= factor;
		}
		out += dot;
	}

	return (out + 1.0) / 2.0;
}

//template class Perlin<float, 1>;
//template class Perlin<double, 1>;
template class Perlin<float, 2>;
template class Perlin<double, 2>;
//template class Perlin<float, 3>;
//template class Perlin<double, 3>;
//template class Perlin<float, 4>;
//template class Perlin<double, 4>;
