#include "text.h"
#include "framework.h"

TextManager* TextManager::active = nullptr;

TextManager::TextManager()
: resolution(72) {
	if(FT_Init_FreeType(&library))
		Die("TextManager.TextManager: unable to load FreeType.");
	active = this;
}

TextManager::~TextManager() {
	FT_Done_FreeType(library);
}

FontFace::FontFace(std::string path, std::string name) :
Resource	(path, name),
sizes(16, offsetof(FontSize, findex)) {
	if(FT_New_Face(TextManager::active->library, (path + name).c_str(), 0, &font))
		Die(string("Font.Font: font file '") + path + name + string("' not found."));
}

FontFace::~FontFace() {
	FT_Done_Face(font);
	for (int i = 0; i < sizes.Size(); i++)
		delete sizes[i];
}

void FontFace::Cache_Size(int size) {
	sizes.Add(new FontSize(this, size));
}

void FontFace::Uncache_Size(int size) {
	for (int i = 0; i < sizes.Size(); i++) {
		if (sizes[i]->size == size) {
			sizes.Remove(sizes[i]);
		}
	}
}

FontSize* FontFace::Get_Size(int size) {
	for (int i = 0; i < sizes.Size(); i++) {
		if (sizes[i]->size == size) {
			return sizes[i];
		}
	}
	return nullptr;
}

FontSize::FontSize(FontFace* font, int size) :
font	(font),
size	(size) {
	FT_Set_Char_Size(font->font, size << 6, 0, TextManager::active->resolution, 0);
	int gindex;	
	FT_Glyph tempglyph = nullptr;
	texdim = Vector2f();
	for (char c = ' '; c <= '~'; c++) {
		gindex = FT_Get_Char_Index(font->font, c);

		if (FT_Load_Glyph(font->font, gindex, FT_LOAD_DEFAULT) ||
			FT_Get_Glyph(font->font->glyph, &tempglyph) ||
			FT_Glyph_To_Bitmap(&tempglyph, FT_RENDER_MODE_NORMAL, 0, true))
			Die("Glyph.Glyph: unable to load glyph.");
		
		glyphs[c - ' '] = (FT_BitmapGlyph)tempglyph;
		if (texdim.x < glyphs[c - ' ']->bitmap.width)
			texdim.x = glyphs[c - ' ']->bitmap.width;
		if (texdim.y < glyphs[c - ' ']->bitmap.rows)
			texdim.y = glyphs[c - ' ']->bitmap.rows;
	}
	for (char c = ' '; c <= '~'; c++) {
		if (glyphs[c - ' ']->bitmap.rows == 0 || glyphs[c - ' ']->bitmap.width == 0)
			texcs[c - ' '] = Vector2f();
		else
			texcs[c - ' '] = Vector2f(1.0, 1.0);//Vector2f(tex	dim.x / glyphs[c - ' ']->bitmap.width, texdim.y / glyphs[c - ' ']->bitmap.rows);
	}
	texes = Text2D::Glyphs_To_Texture(glyphs, texdim.x, texdim.y);
}

FontSize::~FontSize() {
	glDeleteTextures(1, &texes);
	/*for (char c = ' '; c <= '~'; c++) {
		if(texes[c - ' '] != nullptr)
			delete texes[c - ' '];
	}*/
}

Text2D::Text2D(Transform2d transform, double depth, string text, TextFormat format, int maxglyphs) :
OverlayComponent(0),
format		(format),
fontsize	(nullptr),
vertexArrayID		(0),
vertbuffer	(0),
texcbuffer	(0),
vertdata	(nullptr),
texcdata	(nullptr),
colrdata	(nullptr),
transform	(transform),
depth		(depth),
nglyphs		(0),
maxglyphs	(maxglyphs) {
	fontsize = format.font->Get_Size(format.size);
	if (fontsize == nullptr) {
		format.font->Cache_Size(format.size);
		fontsize = format.font->Get_Size(format.size);
	}

	vertdata = new Vector3f[maxglyphs * 4];
	texcdata = new Vector3f[maxglyphs * 4];
	colrdata = new Color4c [maxglyphs * 4];

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertbuffer);
	glGenBuffers(1, &texcbuffer);
	glGenBuffers(1, &colrbuffer);

	Set_Text(text);
}

void Text2D::Set_Text(string text) {
	for (unsigned int i = 0; i < text.length(); i++) {
		if ((text[i] < ' ' || text[i] > '~') && text[i] != '\n')
			text = text.substr(0, i) + text.substr(i + 1, text.length() - i);
	}

	nglyphs = 0;
	for (unsigned int i = 0; i < text.length(); i++) {
		if (text[i] >= ' ' && text[i] <= '~') {
			nglyphs++;
		}
	}

	int glyphi = 0;
	for (unsigned int i = 0; i < text.length(); i++) {
		if (text[i] >= ' ' && text[i] <= '~') {
			texcdata[glyphi * 4 + 2] = Vector3f(fontsize->texcs[text[i] - ' '].x, fontsize->texcs[text[i] - ' '].y, text[i] - ' ');
			texcdata[glyphi * 4 + 0] = Vector3f(0, 0, text[i] - ' ');
			texcdata[glyphi * 4 + 1] = Vector3f(0, texcdata[glyphi * 4 + 2].y, text[i] - ' ');
			texcdata[glyphi * 4 + 3] = Vector3f(texcdata[glyphi * 4 + 2].x, 0, text[i] - ' ');
			glyphi++;
		}
	}

	Make_Lines(fontsize, text);

	Vector3f pen = Vector3f();
	glyphi = 0;
	FT_Set_Char_Size(format.font->font, format.size << 6, 0, TextManager::active->resolution, 0);
	pen.y += format.font->font->size->metrics.ascender >> 6;
	for (int i = 0; i < nlines; i++) {
		switch (format.justify) {
		case TJ_LEFT:
			break;
		case TJ_CENTER:
			pen.x += (format.dim.x - Get_Line_Width(fontsize, lines[i])) * 0.5;
			break;
		case TJ_RIGHT:
			pen.x += (format.dim.x - Get_Line_Width(fontsize, lines[i]));
			break;
		}
		for (unsigned int j = 0; j < lines[i].length(); j++) {
			pen.y -= fontsize->glyphs[lines[i][j] - ' ']->top;

			if (lines[i][j] >= ' ' && lines[i][j] <= '~') {
				for (int k = 0; k < 4; k++)
					vertdata[glyphi * 4 + k] = pen;

				vertdata[glyphi * 4 + 1].y += fontsize->texdim.y;
				vertdata[glyphi * 4 + 2].y += fontsize->texdim.y;
				vertdata[glyphi * 4 + 2].x += fontsize->texdim.x;
				vertdata[glyphi * 4 + 3].x += fontsize->texdim.x;

				glyphi++;
			}
			 
			char c1 = lines[i][j];
			char c2 = (j == lines[i].length() - 1) ? 0 : lines[i][j + 1];

			pen.y += fontsize->glyphs[lines[i][j] - ' ']->top;
			pen.x += Get_Advance(fontsize, c1, c2);
		}
		pen.x = 0.0f;
		pen.y += (format.font->font->size->metrics.ascender - format.font->font->size->metrics.descender) / 64.0f;
	}
	
	for (int i = 0; i < nglyphs * 4; i++)
		colrdata[i] = format.color;

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 4 * nglyphs, vertdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, texcbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 4 * nglyphs, texcdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, colrbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color4c) * 4 * nglyphs, colrdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

Text2D::~Text2D() {
	glDeleteBuffers			(1, &vertbuffer);
	glDeleteBuffers			(1, &texcbuffer);
	glDeleteBuffers			(1, &colrbuffer);
	glDeleteVertexArrays	(1, &vertexArrayID);
	delete[] vertdata;
	delete[] texcdata;
	delete[] colrdata;
	delete[] lines;
}

void Text2D::Make_Lines(FontSize* fontsize, string text) {
	stringstream ss;
	static string linebuffer[256];
	ss << text;
	nlines = 0;
	Stepper lstepper(format.dim.x);
	while (getline(ss, linebuffer[nlines], '\n')) {
		lstepper.accumulation = 0.0f;
		for (unsigned int i = 0; i < linebuffer[nlines].length(); i++) {
			if (' ' <= linebuffer[nlines][i] && '~' >= linebuffer[nlines][i]) {
				char c1 = linebuffer[nlines][i];
				char c2 = (i == linebuffer[nlines].length() - 1) ? 0 : linebuffer[nlines][i + 1];

				if (lstepper.Step_Number(Get_Advance(fontsize, c1, c2))) {
					int lbreak;
					for (lbreak = i; lbreak > 0 && linebuffer[nlines][lbreak] != ' '; lbreak--);
					linebuffer[nlines + 1] =	linebuffer[nlines].substr(lbreak, linebuffer[nlines].length() - lbreak);
					linebuffer[nlines] =		linebuffer[nlines].substr(0, lbreak);
					nlines++;
					i = 0;
					lstepper.accumulation = 0.0f;
				}
			}
		}
		nlines++;
	}
	//nlines++;
	lines = new string[nlines];
	for (int i = 0; i < nlines; i++) {
		if (linebuffer[i][0] == ' ')
			linebuffer[i] = linebuffer[i].substr(1, linebuffer[i].length() - 1);
		lines[i] = linebuffer[i];
	}
}

float Text2D::Get_Advance(FontSize* fontsize, char c1, char c2) {
	FT_Vector kerning = { 0, 0 };
	if(c2)
		FT_Get_Kerning(
			format.font->font,
			FT_Get_Char_Index(format.font->font, c1),
			FT_Get_Char_Index(format.font->font, c2),
			FT_KERNING_DEFAULT,
			&kerning
		);

	return (fontsize->glyphs[c1 - ' ']->root.advance.x >> 16) + (kerning.x >> 6);
}

float Text2D::Get_Line_Width(FontSize* fontsize, string line) {
	float length = 0.0f;
	for (unsigned int i = 0; i < line.length(); i++) {
		if (' ' <= line[i] && '~' >= line[i]) {
			char c1 = line[i];
			char c2 = (i == line.length() - 1) ? 0 : line[i + 1];

			length += Get_Advance(fontsize, c1, c2);
		}
	}
	return length;
}

void Text2D::Render(bool trans) {
	if (!trans)
		return;

	Vector2d absloc = Rel_To_Abs_Coords(transform.position);

	Transform3d transform = Transform3d(Vector3d(absloc.x, absloc.y, depth));
	transform.Apply_Object();

	Matrix4f modelmat = modelStack.Top();

	((ShaderProgram*)GEngine::Get().Resource().Get("text_shader"))->Activate();

	glBindVertexArray(vertexArrayID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, fontsize->texes);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDepthMask(0);
	glDisable(GL_CULL_FACE);
	
	int matrixloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "matrix");
	int texesloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "texes");

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glUniformMatrix4fv(matrixloc, 1, GL_TRUE, modelmat.data);
	glUniform1i(texesloc, 0);

	glDisable(GL_CULL_FACE);

	glDrawArrays(GL_QUADS, 0, nglyphs * 4);

	glBindVertexArray(0);

	((ShaderProgram*)GEngine::Get().Resource().Get("default_shader"))->Activate();

	transform.Unapply_Object();

	glDisable(GL_BLEND);
	glDepthMask(1);
}

GLuint Text2D::Glyphs_To_Texture(FT_BitmapGlyph* glyphs, int width, int height) {
	Color4c* bitmap = new Color4c[width * height * ('~' - ' ' + 1)];
	float intensity;
	for (int c = ' '; c <= '~'; c++) {
		for (int i = 0; i < glyphs[c - ' ']->bitmap.rows; i++) {
			for (int j = 0; j < glyphs[c - ' ']->bitmap.width; j++) {
				intensity = ((unsigned char*)glyphs[c - ' ']->bitmap.buffer)[i * glyphs[c - ' ']->bitmap.width + j];
				bitmap[(c - ' ') * width * height + i * width + j] = Color4c(255, 255, 255, intensity);
			}
		}
	}
	GLuint vertexArrayID;
	glGenTextures(1, &vertexArrayID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, vertexArrayID);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, '~' - ' ' + 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	return vertexArrayID;
}