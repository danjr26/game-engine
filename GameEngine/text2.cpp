#include "text2.h"
#include "display_units.h"
#include "game_engine.h"

Text2::Text2(const std::string& in_text, FontFaceRasterSet* in_rasterSet, ColorRGBAc in_color) :
	mText(in_text),
	mRasterSet(in_rasterSet),
	mBitmapArray(in_rasterSet->mTexture),
	mContainerDimensions(300, 300) {
	
	if (in_rasterSet->mTexture == nullptr || in_rasterSet->mTexture->getType() != Texture::Type::_2d_array) {
		throw InvalidArgumentException("invalid raster texture passed to text");
	}

	mScrollTransform.setParent(&mTransform);

	createOpenGL();
}

Text2::~Text2() {
	destroyOpenGL();
}

Vector2f Text2::getLocalDimensions() const {
	Vector2f dimensions;

	Vector2f pen = Vector2f(0, mRasterSet->mTopToBaseline);
	for (uint i = 0; i < mText.size(); i++) {
		incrementPen(pen, i);
		dimensions = Vector2f(GEUtil::max(dimensions.x(), pen.x()), GEUtil::max(dimensions.y(), pen.y()));
	}

	return dimensions;
}

Vector2f Text2::getLocalCharPosition(uint in_index) const {
	Vector2f pen = Vector2f(0, 0);
	for (uint i = 0; i < in_index; i++) {
		incrementPen(pen, i);
	}
	return pen;
}

Vector2f Text2::getCharDimensions(uint in_index) const {
	if (FontFace::isPrintable(mText[in_index])) {
		uint charIndex = mText[in_index] - ' ';
		return Vector2f(mRasterSet->mAdvances[charIndex], getCharHeight());
	}
	return Vector2f(0, getCharHeight());
}

uint Text2::getNumberLines() const {
	uint out = 1u + (uint)mLineWrap.size();
	for (uint i = 0; i < mText.size(); i++) {
		if (mText[i] == '\n') {
			out++;
		}
	}
	return out;
}

uint Text2::getLineIndex(uint in_charIndex) const {
	uint out = 0;
	uint lineWrapI = 0;
	for (uint i = 0; i < in_charIndex && i < mText.size(); i++) {
		if (mText[i] == '\n') {
			out++;
		}
		if (lineWrapI < mLineWrap.size() && mLineWrap[lineWrapI] == i) {
			out++;
			lineWrapI++;
		}
	}
	return out;
}

Rangeui Text2::getLineRange(uint in_lineIndex) const {
	uint low = 0;
	uint high = (uint)mText.size();
	uint lineIndex = 0;
	uint lineWrapI = 0;
	uint i;
	for (i = 0; i < mText.size() && lineIndex < in_lineIndex; i++) {
		if (mText[i] == '\n') {
			lineIndex++;
		}
		if (lineWrapI < mLineWrap.size() && mLineWrap[lineWrapI] == i) {
			lineIndex++;
			lineWrapI++;
		}
	}
	low = i;
	for (; i < mText.size(); i++) {
		if (mText[i] == '\n' || (lineWrapI < mLineWrap.size() && mLineWrap[lineWrapI] == i)) {
			high = i;
			break;
		}
	}
	return Rangeui(low, high);
}

float Text2::getCharHeight() const {
	return mRasterSet->mTopToBaseline;
}

float Text2::getNewlineHeight() const {
	return (float)mRasterSet->mSize;
}

uint Text2::getClosestCharIndex(Vector2f in_point) const {
	int lineIndex = (int)(in_point.y() / getNewlineHeight());
	if (lineIndex < 0) {
		return 0;
	}
	if (lineIndex >= (int)getNumberLines()) {
		return (uint)mText.size();
	}
	Rangeui lineRange = getLineRange(lineIndex);

	uint charIndex = lineRange.getLow();
	if (in_point.x() < 0.0f) {
		return charIndex;
	}
	Vector2f lastPen = Vector2f(0.0f, in_point.y());
	for (Vector2f pen = Vector2f(0.0f, in_point.y()); charIndex < lineRange.getHigh(); incrementPen(pen, charIndex++)) {
		if (pen.x() >= in_point.x()) {
			if (lastPen.x() == pen.x() || GEUtil::invLerp(lastPen.x(), pen.x(), in_point.x()) > 0.5) {
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

Vector2f Text2::getContainerDimensions() const {
	return mContainerDimensions;
}

Transform2d& Text2::getScrollTransform() {
	return mScrollTransform;
}

void Text2::setScrollPosition(Vector2f in_position) {
	mScrollTransform.setLocalPosition(in_position);
}

Vector2f Text2::getScrollToIncludeChar(uint in_charIndex) const {
	Vector2d minima = getLocalCharPosition(in_charIndex);
	Vector2d maxima = minima + getCharDimensions(in_charIndex);
	Vector2d currentMinima = -Vector2d(mScrollTransform.getLocalPosition());
	Vector2d currentMaxima = currentMinima + getContainerDimensions();

	Vector2d out = Vector2d(mScrollTransform.getLocalPosition());
	if (std::find(mLineWrap.begin(), mLineWrap.end(), in_charIndex) == mLineWrap.end()) {
		if (minima.x() < currentMinima.x()) {
			out[0] = minima.x();
		}
		else if (maxima.x() > currentMaxima.x()) {
			out[0] -= maxima.x() - currentMaxima.x();
		}
	}

	if (minima.y() < currentMinima.y()) {
		out[1] = minima.y();
	}
	else if (maxima.y() > currentMaxima.y()) {
		out[1] -= maxima.y() - currentMaxima.y();
	}

	return out;
}

std::string Text2::getText() const {
	return mText;
}

void Text2::insert(char in_char, uint in_index) {
	std::string s;
	s += in_char;
	insert(s, in_index);
}

void Text2::insert(const std::string& in_text, uint in_index) {
	destroyOpenGL();
	mText.insert(mText.begin() + in_index, in_text.begin(), in_text.end());
	createOpenGL();
}

void Text2::remove(uint in_start, uint in_number) {
	destroyOpenGL();
	mText.erase(in_start, in_number);
	createOpenGL();
}

void Text2::replace(const std::string& in_text) {
	destroyOpenGL();
	mText = in_text;
	createOpenGL();
}

void Text2::replace(const std::string& in_text, uint in_start, uint in_number) {
	destroyOpenGL();
	mText.replace(in_start, in_number, in_text);
	createOpenGL();
}

double Text2::z() const {
	return mDepthTransform.getWorldDepth();
}

bool Text2::shouldCull() const {
	return false;
}

void Text2::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("TextShader");

	// glDisable(GL_CULL_FACE);

	Matrix4f modelMatrix = mScrollTransform.getWorldMatrix();
	Matrix4f viewMatrix = GE.cameras().mActive->getMatrix();
	Matrix4f modelviewMatrix = viewMatrix * modelMatrix;

	GLint locations[1] = {
		shaderProgram->getUniformLocation("matrix")
	};
	
	mBitmapArray.use();

	glBindVertexArray(mVertexArrayID);

	shaderProgram->use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelviewMatrix.pointer());

	glDrawElements(GL_TRIANGLES, 6 * getNumberPrintable(), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
}

uint Text2::getNumberPrintable() {
	uint nPrintable = 0;
	for (uint i = 0; i < mText.size(); i++) {
		if (FontFace::isPrintable(mText[i])) {
			nPrintable++;
		}
	}
	return nPrintable;
}

void Text2::incrementPen(Vector2f& inout_pen, uint in_index) const {
	if (std::find(mLineWrap.begin(), mLineWrap.end(), in_index) != mLineWrap.end()) {
		inout_pen[0] = 0;
		inout_pen[1] += getNewlineHeight();
	}

	if (FontFace::isPrintable(mText[in_index])) {
		uint charIndex = mText[in_index] - ' ';
		inout_pen[0] += mRasterSet->mAdvances[charIndex];
	}
	else if (mText[in_index] == '\n') {
		inout_pen[0] = 0;
		inout_pen[1] += getNewlineHeight();
	}
}

void Text2::createOpenGL() {
	mBitmapArray.settings().setMagnifyFilter(TextureSettings::FilterMode::none);
	mBitmapArray.settings().setMinifyFilter(TextureSettings::FilterMode::none);
	mBitmapArray.settings().setSwizzle(
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::one,
		TextureSettings::Swizzle::channel_1
	);

	uint nGlyphs = getNumberPrintable();

	uint nVertices = 4 * nGlyphs;

	std::vector<Vector2f> vertices;
	std::vector<Vector3f> uvs;
	std::vector<ushort> indices;

	vertices.reserve(nVertices);
	uvs.reserve(nVertices);
	indices.reserve(nVertices / 2 * 3);

	uint glyphIndex = 0;
	Vector2f pen = Vector2f(0, mRasterSet->mTopToBaseline);

	mLineWrap.clear();

	for (uint i = 0; i < mText.size(); i++) {
		uint charIndex = mText[i] - ' ';
		Vector2f testPen = pen;
		incrementPen(testPen, i);
		bool isPrintable = FontFace::isPrintable(mText[i]);
		bool didWrap = isPrintable ? testPen.x() > mContainerDimensions.x() : false;
		if (didWrap) {
			pen[0] = 0;
			pen[1] += getNewlineHeight();
		}

		if (isPrintable) {
			Vector2i targetDimensions = GE.render().mMainTarget->getDimensions();

			Vector2f minCoords = pen + mRasterSet->mOffsets[charIndex];
			Vector2f dimensions = mRasterSet->mDimensions[charIndex];
			Vector2f maxCoords = minCoords + dimensions;

			Vector2f minUVs = Vector2f(0.0f, 0.0f);
			Vector2f maxUVs = dimensions.compwise((Vector2f(Vector3f(mRasterSet->mTexture->getDimensions()))).componentInverted());

			Vector2f minScreenCoords = DisplayUnits2f::pixels(minCoords, targetDimensions).openGLPosition();
			Vector2f maxScreenCoords = DisplayUnits2f::pixels(maxCoords, targetDimensions).openGLPosition();

			vertices.push_back(Vector2f(minCoords.x(), minCoords.y()));
			vertices.push_back(Vector2f(minCoords.x(), maxCoords.y()));
			vertices.push_back(Vector2f(maxCoords.x(), maxCoords.y()));
			vertices.push_back(Vector2f(maxCoords.x(), minCoords.y()));

			uvs.push_back(Vector3f(minUVs.x(), minUVs.y(), charIndex));
			uvs.push_back(Vector3f(minUVs.x(), maxUVs.y(), charIndex));
			uvs.push_back(Vector3f(maxUVs.x(), maxUVs.y(), charIndex));
			uvs.push_back(Vector3f(maxUVs.x(), minUVs.y(), charIndex));

			indices.push_back(glyphIndex * 4 + 0);
			indices.push_back(glyphIndex * 4 + 3);
			indices.push_back(glyphIndex * 4 + 1);
			indices.push_back(glyphIndex * 4 + 2);
			indices.push_back(glyphIndex * 4 + 1);
			indices.push_back(glyphIndex * 4 + 3);

			glyphIndex++;
		}

		incrementPen(pen, i);

		if (didWrap) {
			mLineWrap.push_back(i);
		}
	}

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * nVertices, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &mUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * nVertices, uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &mIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * nVertices / 2 * 3, indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Text2::destroyOpenGL() {
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mUVBufferID);
	glDeleteBuffers(1, &mIndexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}
