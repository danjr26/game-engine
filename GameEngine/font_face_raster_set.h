#ifndef FONT_FACE_RASTER_SET_H
#define FONT_FACE_RASTER_SET_H

#include "vector.h"
#include <vector>
#include "asset.h"

class FontFace;
class Texture;

struct FontFaceRasterSet : public Asset {
	FontFace* face;
	Texture* texture;
	std::vector<Vector2i> offsets;
	std::vector<Vector2i> dimensions;
	std::vector<float> advances;
	float topToBaseline;
	float baselineToBottom;
	uint size;
};

#endif