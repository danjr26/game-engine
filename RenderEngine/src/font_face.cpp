#include "font_face.h"
#include "rapidxml.hpp"
#include <fstream>
#include <sstream>
#include "game_engine.h"

FontFace::FontFace(std::string i_filename) {
	FT_Library library = GE.fonts().getLibrary();
	FT_Error error = FT_New_Face(library, i_filename.c_str(), 0, &mFace);
	if (error) {
		Log::main(std::string("error: could not load font face '") + i_filename + "'");
		GE.quit();
	}
}

FontFace::~FontFace() {
	if (GameEngine::exists()) {
		FT_Done_Face(mFace);
	}
}

FontFaceRasterSet* FontFace::rasterize(uint i_size) {
	FontFaceRasterSet* rasterSet = new FontFaceRasterSet();
	rasterSet->mFace = this;
	rasterSet->mSize = i_size;
	rasterSet->mOffsets.reserve('~' - ' ' + 1);
	rasterSet->mDimensions.reserve('~' - ' ' + 1);
	rasterSet->mAdvances.reserve('~' - ' ' + 1);

	FT_Set_Pixel_Sizes(mFace, i_size, 0);

	rasterSet->mTopToBaseline = mFace->size->metrics.ascender / (float)(1 << 6);
	rasterSet->mBaselineToBottom = mFace->size->metrics.descender / (float)(1 << 6);

	std::vector<FT_BitmapGlyph> glyphs = std::vector<FT_BitmapGlyph>();
	glyphs.reserve('~' - ' ' + 1);

	uint charIndex;
	FT_BitmapGlyph glyph;
	FT_Glyph tempGlyph;
	Vector3ui maxDimensions = Vector3ui(0, 0, '~' - ' ' + 1);

	for (uchar c = ' '; c <= '~'; c++) {
		charIndex = FT_Get_Char_Index(mFace, c);

		if (FT_Load_Glyph(mFace, charIndex, FT_LOAD_DEFAULT) ||
			FT_Get_Glyph(mFace->glyph, &tempGlyph) ||
			FT_Glyph_To_Bitmap(&tempGlyph, FT_RENDER_MODE_NORMAL, nullptr, true)) {

			throw ProcessFailureException("could not rasterize text glyph");
		}

		glyph = (FT_BitmapGlyph)tempGlyph;

		glyphs.push_back(glyph);
		
		rasterSet->mOffsets.push_back(Vector2i(
			(mFace->glyph->metrics.horiBearingX) / (float)(1 << 6),
			(-mFace->glyph->metrics.horiBearingY) / (float)(1 << 6)
		));
		rasterSet->mDimensions.push_back(Vector2i(glyph->bitmap.width, glyph->bitmap.rows));
		rasterSet->mAdvances.push_back(mFace->glyph->advance.x / (float)(1 << 6));

		maxDimensions[0] = geutil::max((uint)maxDimensions[0], glyph->bitmap.width);
		maxDimensions[1] = geutil::max((uint)maxDimensions[1], glyph->bitmap.rows);
	}

	uchar* bitmap = new uchar[maxDimensions.componentProduct()];

	for (uint i = 0; i < maxDimensions.z(); i++) {
		if (glyphs[i]->bitmap.buffer == nullptr) {
			for (uint x = 0; x < maxDimensions.x(); x++) {	
				for (uint y = 0; y < maxDimensions.y(); y++) {
					bitmap[i * maxDimensions.x() * maxDimensions.y() + y * maxDimensions.x() + x] = 0;
				}
			}
		}
		else {
			for (uint x = 0; x < maxDimensions.x(); x++) {
				for (uint y = 0; y < maxDimensions.y(); y++) {
					bitmap[i * maxDimensions.x() * maxDimensions.y() + y * maxDimensions.x() + x] =
						(x < glyphs[i]->bitmap.width && y < glyphs[i]->bitmap.rows) ? 
						glyphs[i]->bitmap.buffer[y * glyphs[i]->bitmap.width + x] : 0;
				}
			}
		}
	}

	Texture* texture = new Texture(Texture::Type::_2d_array, maxDimensions, bitmap, 1, 8, Texture::Flags::mipmaps);
	TextureSettings settings = texture->getActiveSettings();
	settings.setSwizzle(
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::one, 
		TextureSettings::Swizzle::channel_1
	);
	settings.use(&texture->getActiveSettings());

	delete[] bitmap;

	rasterSet->mTexture = texture;

	return rasterSet;
}

bool FontFace::isPrintable(uchar i_char) {
	return i_char >= ' ' && i_char <= '~';
}

bool FontFace::isPrintableOrEOL(uchar i_char) {
	return isPrintable(i_char) || i_char == '\n';
}

void FontFace::loadXMLList(const std::string& i_filename) {
	std::ifstream file(i_filename);
	if (!file.is_open()) {
		Log::main(std::string("error: cannot open file '") + i_filename + "'");
		GE.quit();
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
		Log::main(std::string("error: invalid font list file '") + i_filename + "'");
		GE.quit();
	}

	for (auto node = masterNode->first_node("FontFace"); node; node = node->next_sibling("FontFace")) {
		auto nameAttribute = node->first_attribute("name");
		auto fileAttribute = node->first_attribute("file");

		if (!nameAttribute || !fileAttribute) {
			Log::main(std::string("error: invalid shader list file '") + i_filename + "'");
			GE.quit();
		}

		FontFace* fontFace = new FontFace(fileAttribute->value());
		GE.assets().add(nameAttribute->value(), fontFace);
	}

	delete[] buffer;
}
