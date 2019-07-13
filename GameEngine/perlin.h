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
	~Perlin();

	Vector<uint, n> getDimensions() const;

	bool getWrap(uint in_index) const;
	void setWrap(uint in_index, bool in_value);

	T evaluate(const Vector<T, n>& in_coords) const;
};

#endif