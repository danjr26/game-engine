#include "text2.h"
#include "display_units.h"
#include "game_engine.h"

Text2::Text2(std::string in_text, FontFaceRasterSet* in_rasterSet, Transform in_transform, ColorRGBAc in_color) :
	TransformableObject(in_transform),
	text(in_text),
	rasterSet(in_rasterSet),
	bitmapArray(in_rasterSet->texture) {
	
	if (in_rasterSet->texture == nullptr || in_rasterSet->texture->Get_Type() != Texture::Type::_2d_array) {
		Log::main("error: invalid texture passed to text object");
		exit(-1);
	}

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

	for (uint i = 0; i < text.size(); i++) {
		if (FontFace::Is_Printable(text[i])) {
			uint charIndex = text[i] - ' ';

			Vector2i targetDimensions = GE.Render().mainTarget->Get_Dimensions();

			Vector2f minCoords = pen + rasterSet->offsets[charIndex];
			Vector2f dimensions = rasterSet->dimensions[charIndex];
			Vector2f maxCoords = minCoords + dimensions;

			Vector2f minUVs = Vector2f(0.0f, 0.0f);
			Vector2f maxUVs = dimensions.Compwise((Vector2f(rasterSet->texture->Get_Dimensions())).Component_Inverted());

			Vector2f minScreenCoords = DisplayUnits2::Pixels(minCoords, targetDimensions).OpenGL_Position();
			Vector2f maxScreenCoords = DisplayUnits2::Pixels(maxCoords, targetDimensions).OpenGL_Position();

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

			pen[0] += rasterSet->advances[charIndex];

			glyphIndex++;
		}
		else if (text[i] == '\n') {
			pen[0] = 0;
			pen[1] += rasterSet->size;
		}
	}

	/*
	Vector3f vertices[] = {
		Vector3f(minScreenCoords.X(), minScreenCoords.Y(), 0.0f),
		Vector3f(minScreenCoords.X(), maxScreenCoords.Y(), 0.0f),
		Vector3f(maxScreenCoords.X(), maxScreenCoords.Y(), 0.0f),
		Vector3f(maxScreenCoords.X(), minScreenCoords.Y(), 0.0f)
	};

	Vector3f uvs[] = {
		Vector3f(0.0f, 0.0f, charIndex),
		Vector3f(0.0f, 1.0f, charIndex),
		Vector3f(1.0f, 1.0f, charIndex),
		Vector3f(1.0f, 0.0f, charIndex)
	};

	ushort indices[] = {
		0, 3, 1,
		2, 1, 3
	};
	*/

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

Text2::~Text2() 
{}

double Text2::Z() const {
	return transform.Get_Position().Z();
}

bool Text2::Should_Cull() const {
	return false;
}

void Text2::Render() {
	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("TextShader");

	glDisable(GL_CULL_FACE);

	GLint locations[1] = {
		shaderProgram->Get_Uniform_Location("matrix")
	};

	bitmapArray.Use();

	glBindVertexArray(vertexArrayID);

	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, (GE.Cameras().active->Get_Matrix() * (Matrix4f)transform.Get_Matrix()).Pointer());

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
