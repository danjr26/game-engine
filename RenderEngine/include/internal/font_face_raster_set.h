#ifndef FONT_FACE_RASTER_SET_H
#define FONT_FACE_RASTER_SET_H

#include "vector.h"
#include <vector>
#include "asset.h"

class FontFace;
class Texture;

struct FontFaceRasterSet : public Asset {
	FontFace* mFace;
	Texture* mTexture;
	std::vector<Vector2i> mOffsets;
	std::vector<Vector2i> mDimensions;
	std::vector<float> mAdvances;
	float mTopToBaseline;
	float mBaselineToBottom;
	uint mSize;
};

#endif