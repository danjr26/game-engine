#ifndef FRACTAL_PERLIN_H
#define FRACTAL_PERLIN_H

#include "perlin.h"

template<class T, uint n>
class FractalPerlin {
private:
	std::vector<Perlin<T, n>> mLayers;
	Vector<T, n> mLacunarity;
	T mGain;

public:
	FractalPerlin(const Vector<uint, n>& in_initialDimensions, uint in_nLayers, const Vector<T, n>& in_lacunarity, T in_gain) :
		mLayers(),
		mLacunarity(in_lacunarity),
		mGain(in_gain) {

		mLayers.reserve(in_nLayers);
		
		Vector<T, n> dimensions = Vector<T, n>(in_initialDimensions);
		for (uint i = 0; i < in_nLayers; i++) {
			Vector<uint, n> clampedDimensions = Vector<uint, n>(dimensions);
			for (uint j = 0; j < n; j++) {
				if (clampedDimensions[j] != dimensions[j]) {
					clampedDimensions[j]++;
				}
			}
			mLayers.push_back(Perlin<T, n>(clampedDimensions));
			dimensions = dimensions.compwise(mLacunarity);
		}
	}

	T evaluate(const Vector<T, n>& in_coords) const {
		T value = 0;
		Vector<T, n> dimensions = mLayers.front().getDimensions();
		T intensity = 1;
		for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
			Vector<T, n> clampedDimensions = Vector<T, n>(it->getDimensions());
			Vector<T, n> correctedCoords = in_coords.compwise(clampedDimensions.compwise(dimensions.componentInverted()));

			value += it->evaluate(correctedCoords) * intensity;

			dimensions = dimensions.compwise(mLacunarity);
			intensity /= mGain;
		}
		return value * (mGain - 1) / mGain;
	}
};

using FractalPerlin2f = FractalPerlin<float, 2>;
using FractalPerlin2d = FractalPerlin<double, 2>;

#endif