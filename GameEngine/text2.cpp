#include "text2.h"
#include "display_units.h"
#include "game_engine.h"

Text2::Text2(const std::string& in_text, FontFaceRasterSet* in_rasterSet, ColorRGBAc in_color) :
	text(in_text),
	rasterSet(in_rasterSet),
	bitmapArray(in_rasterSet->texture),
	containerDimensions(300, 300) {
	
	if (in_rasterSet->texture == nullptr || in_rasterSet->texture->Get_Type() != Texture::Type::_2d_array) {
		throw InvalidArgumentException("invalid raster texture passed to text");
	}

	scrollTransform.Set_Parent(&transform);

	Create_OpenGL();
}

Text2::~Text2() {
	Destroy_OpenGL();
}

Vector2f Text2::Get_Local_Dimensions() const {
	Vector2f dimensions;

	Vector2f pen = Vector2f(0, rasterSet->topToBaseline);
	for (uint i = 0; i < text.size(); i++) {
		Increment_Pen(pen, i);
		dimensions = Vector2f(Max(dimensions.X(), pen.X()), Max(dimensions.Y(), pen.Y()));
	}

	return dimensions;
}

Vector2f Text2::Get_Local_Char_Position(uint in_index) const {
	Vector2f pen = Vector2f(0, 0);
	for (uint i = 0; i < in_index; i++) {
		Increment_Pen(pen, i);
	}
	return pen;
}

Vector2f Text2::Get_Char_Dimensions(uint in_index) const {
	if (FontFace::Is_Printable(text[in_index])) {
		uint charIndex = text[in_index] - ' ';
		return Vector2f(rasterSet->advances[charIndex], Get_Char_Height());
	}
	return Vector2f(0, Get_Char_Height());
}

uint Text2::Get_Number_Lines() const {
	uint out = 1u + (uint)lineWrap.size();
	for (uint i = 0; i < text.size(); i++) {
		if (text[i] == '\n') {
			out++;
		}
	}
	return out;
}

uint Text2::Get_Line_Index(uint in_charIndex) const {
	uint out = 0;
	uint lineWrapI = 0;
	for (uint i = 0; i < in_charIndex && i < text.size(); i++) {
		if (text[i] == '\n') {
			out++;
		}
		if (lineWrapI < lineWrap.size() && lineWrap[lineWrapI] == i) {
			out++;
			lineWrapI++;
		}
	}
	return out;
}

Rangeui Text2::Get_Line_Range(uint in_lineIndex) const {
	uint low = 0;
	uint high = (uint)text.size();
	uint lineIndex = 0;
	uint lineWrapI = 0;
	uint i;
	for (i = 0; i < text.size() && lineIndex < in_lineIndex; i++) {
		if (text[i] == '\n') {
			lineIndex++;
		}
		if (lineWrapI < lineWrap.size() && lineWrap[lineWrapI] == i) {
			lineIndex++;
			lineWrapI++;
		}
	}
	low = i;
	for (; i < text.size(); i++) {
		if (text[i] == '\n' || (lineWrapI < lineWrap.size() && lineWrap[lineWrapI] == i)) {
			high = i;
			break;
		}
	}
	return Rangeui(low, high);
}

float Text2::Get_Char_Height() const {
	return rasterSet->topToBaseline;
}

float Text2::Get_Newline_Height() const {
	return (float)rasterSet->size;
}

uint Text2::Get_Closest_Char_Index(Vector2f in_point) const {
	int lineIndex = (int)(in_point.Y() / Get_Newline_Height());
	if (lineIndex < 0) {
		return 0;
	}
	if (lineIndex >= (int)Get_Number_Lines()) {
		return (uint)text.size();
	}
	Rangeui lineRange = Get_Line_Range(lineIndex);

	uint charIndex = lineRange.Get_Low();
	if (in_point.X() < 0.0f) {
		return charIndex;
	}
	Vector2f lastPen = Vector2f(0.0f, in_point.Y());
	for (Vector2f pen = Vector2f(0.0f, in_point.Y()); charIndex < lineRange.Get_High(); Increment_Pen(pen, charIndex++)) {
		if (pen.X() >= in_point.X()) {
			if (lastPen.X() == pen.X() || Inv_Lerp(lastPen.X(), pen.X(), in_point.X()) > 0.5) {
				return charIndex;
			}
			else {
				return charIndex - 1;
			}
		}
		lastPen = pen;
	}
	return charIndex;
}

Vector2f Text2::Get_Container_Dimensions() const {
	return containerDimensions;
}

Transform2d& Text2::Get_Scroll_Transform() {
	return scrollTransform;
}

void Text2::Set_Scroll_Position(Vector2f in_position) {
	scrollTransform.Set_Local_Position(in_position);
}

Vector2f Text2::Get_Scroll_To_Include_Char(uint in_charIndex) const {
	Vector2d minima = Get_Local_Char_Position(in_charIndex);
	Vector2d maxima = minima + Get_Char_Dimensions(in_charIndex);
	Vector2d currentMinima = -Vector2d(scrollTransform.Get_Local_Position());
	Vector2d currentMaxima = currentMinima + Get_Container_Dimensions();

	Vector2d out = Vector2d(scrollTransform.Get_Local_Position());
	if (std::find(lineWrap.begin(), lineWrap.end(), in_charIndex) == lineWrap.end()) {
		if (minima.X() < currentMinima.X()) {
			out[0] = minima.X();
		}
		else if (maxima.X() > currentMaxima.X()) {
			out[0] -= maxima.X() - currentMaxima.X();
		}
	}

	if (minima.Y() < currentMinima.Y()) {
		out[1] = minima.Y();
	}
	else if (maxima.Y() > currentMaxima.Y()) {
		out[1] -= maxima.Y() - currentMaxima.Y();
	}

	return out;
}

std::string Text2::Get_Text() const {
	return text;
}

void Text2::Insert(char in_char, uint in_index) {
	std::string s;
	s += in_char;
	Insert(s, in_index);
}

void Text2::Insert(const std::string& in_text, uint in_index) {
	Destroy_OpenGL();
	text.insert(text.begin() + in_index, in_text.begin(), in_text.end());
	Create_OpenGL();
}

void Text2::Delete(uint in_start, uint in_number) {
	Destroy_OpenGL();
	text.erase(in_start, in_number);
	Create_OpenGL();
}

void Text2::Replace(const std::string& in_text) {
	Destroy_OpenGL();
	text = in_text;
	Create_OpenGL();
}

void Text2::Replace(const std::string& in_text, uint in_start, uint in_number) {
	Destroy_OpenGL();
	text.replace(in_start, in_number, in_text);
	Create_OpenGL();
}

double Text2::Z() const {
	return depthTransform.Get_World_Depth();
}

bool Text2::Should_Cull() const {
	return false;
}

void Text2::Render() {
	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("TextShader");

	// glDisable(GL_CULL_FACE);

	Matrix4f modelMatrix = scrollTransform.Get_World_Matrix();
	Matrix4f viewMatrix = GE.Cameras().active->Get_Matrix();
	Matrix4f modelviewMatrix = viewMatrix * modelMatrix;

	GLint locations[1] = {
		shaderProgram->Get_Uniform_Location("matrix")
	};
	
	bitmapArray.Use();

	glBindVertexArray(vertexArrayID);

	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelviewMatrix.Pointer());

	glDrawElements(GL_TRIANGLES, 6 * Get_Number_Printable(), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
}

uint Text2::Get_Number_Printable() {
	uint nPrintable = 0;
	for (uint i = 0; i < text.size(); i++) {
		if (FontFace::Is_Printable(text[i])) {
			nPrintable++;
		}
	}
	return nPrintable;
}

void Text2::Increment_Pen(Vector2f& inout_pen, uint in_index) const {
	if (std::find(lineWrap.begin(), lineWrap.end(), in_index) != lineWrap.end()) {
		inout_pen[0] = 0;
		inout_pen[1] += Get_Newline_Height();
	}

	if (FontFace::Is_Printable(text[in_index])) {
		uint charIndex = text[in_index] - ' ';
		inout_pen[0] += rasterSet->advances[charIndex];
	}
	else if (text[in_index] == '\n') {
		inout_pen[0] = 0;
		inout_pen[1] += Get_Newline_Height();
	}
}

void Text2::Create_OpenGL() {
	bitmapArray.Settings().Set_Magnify_Filter(TextureSettings::FilterMode::none);
	bitmapArray.Settings().Set_Minify_Filter(TextureSettings::FilterMode::none);
	bitmapArray.Settings().Set_Swizzle(
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::channel_1
	);

	uint nGlyphs = Get_Number_Printable();

	uint nVertices = 4 * nGlyphs;

	std::vector<Vector2f> vertices;
	std::vector<Vector3f> uvs;
	std::vector<ushort> indices;

	vertices.reserve(nVertices);
	uvs.reserve(nVertices);
	indices.reserve(nVertices / 2 * 3);

	uint glyphIndex = 0;
	Vector2f pen = Vector2f(0, rasterSet->topToBaseline);

	lineWrap.clear();

	for (uint i = 0; i < text.size(); i++) {
		uint charIndex = text[i] - ' ';
		Vector2f testPen = pen;
		Increment_Pen(testPen, i);
		bool isPrintable = FontFace::Is_Printable(text[i]);
		bool didWrap = isPrintable ? testPen.X() > containerDimensions.X() : false;
		if (didWrap) {
			pen[0] = 0;
			pen[1] += Get_Newline_Height();
		}

		if (isPrintable) {
			Vector2i targetDimensions = GE.Render().mainTarget->Get_Dimensions();

			Vector2f minCoords = pen + rasterSet->offsets[charIndex];
			Vector2f dimensions = rasterSet->dimensions[charIndex];
			Vector2f maxCoords = minCoords + dimensions;

			Vector2f minUVs = Vector2f(0.0f, 0.0f);
			Vector2f maxUVs = dimensions.Compwise((Vector2f(Vector3f(rasterSet->texture->Get_Dimensions()))).Component_Inverted());

			Vector2f minScreenCoords = DisplayUnits2f::Pixels(minCoords, targetDimensions).OpenGL_Position();
			Vector2f maxScreenCoords = DisplayUnits2f::Pixels(maxCoords, targetDimensions).OpenGL_Position();

			vertices.push_back(Vector2f(minCoords.X(), minCoords.Y()));
			vertices.push_back(Vector2f(minCoords.X(), maxCoords.Y()));
			vertices.push_back(Vector2f(maxCoords.X(), maxCoords.Y()));
			vertices.push_back(Vector2f(maxCoords.X(), minCoords.Y()));

			uvs.push_back(Vector3f(minUVs.X(), minUVs.Y(), charIndex));
			uvs.push_back(Vector3f(minUVs.X(), maxUVs.Y(), charIndex));
			uvs.push_back(Vector3f(maxUVs.X(), maxUVs.Y(), charIndex));
			uvs.push_back(Vector3f(maxUVs.X(), minUVs.Y(), charIndex));

			indices.push_back(glyphIndex * 4 + 0);
			indices.push_back(glyphIndex * 4 + 3);
			indices.push_back(glyphIndex * 4 + 1);
			indices.push_back(glyphIndex * 4 + 2);
			indices.push_back(glyphIndex * 4 + 1);
			indices.push_back(glyphIndex * 4 + 3);

			glyphIndex++;
		}

		Increment_Pen(pen, i);

		if (didWrap) {
			lineWrap.push_back(i);
		}
	}

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * nVertices, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * nVertices, uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * nVertices / 2 * 3, indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Text2::Destroy_OpenGL() {
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &uvBufferID);
	glDeleteBuffers(1, &indexBufferID);
	glDeleteVertexArrays(1, &vertexArrayID);
}
