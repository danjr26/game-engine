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
	FontFace(std::string in_filename);
	~FontFace();
	FontFaceRasterSet* Rasterize(uint in_size);

	static bool Is_Printable(uchar in_char);
	static bool Is_Printable_Or_EOL(uchar in_char);

	static void Load_XML_List(const std::string& in_filename);
};

#endif