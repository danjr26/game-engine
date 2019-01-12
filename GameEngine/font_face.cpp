#include "font_face.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include <fstream>
#include <sstream>
#include "game_engine.h"

FontFace::FontFace(std::string in_filename) {
	FT_Library library = GE.Fonts().Get_Library();
	FT_Error error = FT_New_Face(library, in_filename.c_str(), 0, &face);
	if (error) {
		Log::main(std::string("error: could not load font face '") + in_filename + "'");
		GE.Exit();
	}
}

FontFace::~FontFace() {
	FT_Done_Face(face);
}

FontFaceRasterSet* FontFace::Rasterize(uint in_size) {
	FontFaceRasterSet* rasterSet = new FontFaceRasterSet();
	rasterSet->face = this;
	rasterSet->size = in_size;
	rasterSet->offsets.reserve('~' - ' ' + 1);
	rasterSet->dimensions.reserve('~' - ' ' + 1);
	rasterSet->advances.reserve('~' - ' ' + 1);

	FT_Set_Pixel_Sizes(face, in_size, 0);

	rasterSet->topToBaseline = face->size->metrics.ascender / (float)(1 << 6);
	rasterSet->baselineToBottom = face->size->metrics.descender / (float)(1 << 6);

	std::vector<FT_BitmapGlyph> glyphs = std::vector<FT_BitmapGlyph>();
	glyphs.reserve('~' - ' ' + 1);

	uint charIndex;
	FT_BitmapGlyph glyph;
	FT_Glyph tempGlyph;
	Vector3ui maxDimensions = Vector3ui(0, 0, '~' - ' ' + 1);

	for (uchar c = ' '; c <= '~'; c++) {
		charIndex = FT_Get_Char_Index(face, c);

		if (FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT) ||
			FT_Get_Glyph(face->glyph, &tempGlyph) ||
			FT_Glyph_To_Bitmap(&tempGlyph, FT_RENDER_MODE_NORMAL, nullptr, true)) {

			throw ProcessFailureException("could not rasterize text glyph");
		}

		glyph = (FT_BitmapGlyph)tempGlyph;

		glyphs.push_back(glyph);
		
		rasterSet->offsets.push_back(Vector2i(
			(face->glyph->metrics.horiBearingX) / (float)(1 << 6),
			(-face->glyph->metrics.horiBearingY) / (float)(1 << 6)
		));
		rasterSet->dimensions.push_back(Vector2i(glyph->bitmap.width, glyph->bitmap.rows));
		rasterSet->advances.push_back(face->glyph->advance.x / (float)(1 << 6));

		maxDimensions[0] = max((uint)maxDimensions[0], glyph->bitmap.width);
		maxDimensions[1] = max((uint)maxDimensions[1], glyph->bitmap.rows);
	}

	uchar* bitmap = new uchar[maxDimensions.Component_Product()];

	for (uint i = 0; i < maxDimensions.Z(); i++) {
		if (glyphs[i]->bitmap.buffer == nullptr) {
			for (uint x = 0; x < maxDimensions.X(); x++) {	
				for (uint y = 0; y < maxDimensions.Y(); y++) {
					bitmap[i * maxDimensions.X() * maxDimensions.Y() + y * maxDimensions.X() + x] = 0;
				}
			}
		}
		else {
			for (uint x = 0; x < maxDimensions.X(); x++) {
				for (uint y = 0; y < maxDimensions.Y(); y++) {
					bitmap[i * maxDimensions.X() * maxDimensions.Y() + y * maxDimensions.X() + x] =
						(x < glyphs[i]->bitmap.width && y < glyphs[i]->bitmap.rows) ? 
						glyphs[i]->bitmap.buffer[y * glyphs[i]->bitmap.width + x] : 0;
				}
			}
		}
	}

	Texture* texture = new Texture(Texture::Type::_2d_array, maxDimensions, bitmap, 1, 8, Texture::Flags::mipmaps);
	TextureSettings settings = texture->Get_Active_Settings();
	settings.Set_Swizzle(
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::channel_1
	);
	settings.Use(&texture->Get_Active_Settings());

	delete[] bitmap;

	rasterSet->texture = texture;

	return rasterSet;
}

bool FontFace::Is_Printable(uchar in_char) {
	return in_char >= ' ' && in_char <= '~';
}

bool FontFace::Is_Printable_Or_EOL(uchar in_char) {
	return Is_Printable(in_char) || in_char == '\n';
}

void FontFace::Load_XML_List(const std::string& in_filename) {
	std::ifstream file(in_filename);
	if (!file.is_open()) {
		Log::main(std::string("error: cannot open file '") + in_filename + "'");
		GE.Exit();
	}

	file.seekg(0, file.end);
	uint length = (uint)file.tellg();
	file.seekg(0, file.beg);

	char* buffer = new char[length];
	for (uint i = 0; i < length; i++) {
		buffer[i] = '\0';
	}
	file.read(buffer, length);
	file.close();

	rapidxml::xml_document<char> doc;
	doc.parse<0>(buffer);

	auto masterNode = doc.first_node("FontList");
	if (!masterNode) {
		Log::main(std::string("error: invalid font list file '") + in_filename + "'");
		GE.Exit();
	}

	for (auto node = masterNode->first_node("FontFace"); node; node = node->next_sibling("FontFace")) {
		auto nameAttribute = node->first_attribute("name");
		auto fileAttribute = node->first_attribute("file");

		if (!nameAttribute || !fileAttribute) {
			Log::main(std::string("error: invalid shader list file '") + in_filename + "'");
			GE.Exit();
		}

		FontFace* fontFace = new FontFace(fileAttribute->value());
		GE.Assets().Add(nameAttribute->value(), fontFace);
	}

	delete[] buffer;
}
