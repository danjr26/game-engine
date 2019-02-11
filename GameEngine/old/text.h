#ifndef TEXT_H
#define TEXT_H

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
}
#include "resources.h"
#include "texture.h"
#include <sstream>

class FontFace;

class TextManager {
public:
	static TextManager* active;
	FT_Library			library;
	int					resolution;

	TextManager	();
	~TextManager();
};

class FontSize {
public:
	FT_BitmapGlyph	glyphs	['~' - ' ' + 1];
	GLuint			texes;
	Vector2f		texdim;
	Vector2f		texcs	['~' - ' ' + 1];
	FontFace*		font;
	int				size;
	uint			findex;

	FontSize	(FontFace* font, int size);
	~FontSize	();
};

class FontFace : public Resource {
public:
	FT_Face		font;
	IndexedArray<FontSize> sizes;
	
				FontFace		(std::string path, std::string name);
				~FontFace		();
	void		Cache_Size	(int size);
	void		Uncache_Size(int size);
	FontSize*	Get_Size	(int size);
};

enum TextJustify {
	TJ_LEFT,
	TJ_CENTER,
	TJ_RIGHT
};

struct TextFormat {
	FontFace*	font;
	TextJustify	justify;
	Color4f		color;
	int			size;
	Vector2d	dim;
};

class Text2D : public OverlayComponent {
public:
	GLuint		vertexArrayID;
	GLuint		vertbuffer;
	GLuint		texcbuffer;
	GLuint		colrbuffer;

	Vector3f*	vertdata;
	Vector3f*	texcdata;
	Color4c*	colrdata;
	int			nglyphs;
	int			maxglyphs;

	string*		lines;
	int			nlines;
	FontSize*	fontsize;
	TextFormat	format;

	Transform2d	transform;
	double		depth;

						Text2D				(Transform2d transform, double depth, string text, TextFormat format, int maxglyphs);
						~Text2D				();
	void				Set_Text			(string text);
	void				Make_Lines			(FontSize* fontsize, string text);
	float				Get_Advance			(FontSize* fontsize, char c1, char c2);
	float				Get_Line_Width		(FontSize* fontsize, string line);
	virtual void		Render				(bool trans);
	static GLuint		Glyphs_To_Texture	(FT_BitmapGlyph* glyphs, int width, int height);
};

#endif