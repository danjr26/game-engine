#ifndef FONT_H
#define FONT_H

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
}
#include <string>
#include "texture.h"
#include "font_face_raster_set.h"
#include "asset.h"

class FontFace : public Asset {
private:
	FT_Face mFace;

public:
	FontFace(std::string i_filename);
	~FontFace();
	FontFaceRasterSet* rasterize(uint i_size);

	static bool isPrintable(uchar i_char);
	static bool isPrintableOrEOL(uchar i_char);

	static void loadXMLList(const std::string& i_filename);
};

#endif