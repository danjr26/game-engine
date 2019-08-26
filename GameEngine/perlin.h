#ifndef PERLIN_H
#define PERLIN_H

#include "texture_settings.h"
#include "vector.h"
#include "misc.h"

template<class T, uint n>
class Perlin {
public:
	using EdgeCase = TextureSettings::EdgeCase;
private:
	Vector<uint, n> mDimensions;
	Vector<T, n>* mGrid;
	bool mWrap[n];

public:
	Perlin(Vector<uint, n>& in_dimensions);
	Perlin(Perlin<T, n>&& in_other);
	~Perlin();

	Vector<uint, n> getDimensions() const;

	bool getWrap(uint in_index) const;
	void setWrap(uint in_index, bool in_value);

	T evaluate(const Vector<T, n>& in_coords) const;
};

using Perlin1f = Perlin<float, 1>;
using Perlin2f = Perlin<float, 2>;
using Perlin3f = Perlin<float, 3>;
using Perlin4f = Perlin<float, 4>;

using Perlin1d = Perlin<double, 1>;
using Perlin2d = Perlin<double, 2>;
using Perlin3d = Perlin<double, 3>;
using Perlin4d = Perlin<double, 4>;

#endif