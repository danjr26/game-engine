#include "text2.h"
#include "RenderTarget_units.h"
#include "game_engine.h"

Text2::Text2(const std::string& i_text, FontFaceRasterSet* i_rasterSet, Color4c i_color) :
	mText(i_text),
	mRasterSet(i_rasterSet),
	mBitmapArray(i_rasterSet->mTexture),
	mContainerDimensions(300, 300) {
	
	if (i_rasterSet->mTexture == nullptr || i_rasterSet->mTexture->getType() != Texture::Type::_2d_array) {
		throw InvalidArgumentException("invalid raster texture passed to text");
	}

	mScrollTransform.setParent(&getTransform());

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
		dimensions = Vector2f(geutil::max(dimensions.x(), pen.x()), geutil::max(dimensions.y(), pen.y()));
	}

	return dimensions;
}

Vector2f Text2::getLocalCharPosition(uint i_index) const {
	Vector2f pen = Vector2f(0, 0);
	for (uint i = 0; i < i_index; i++) {
		incrementPen(pen, i);
	}
	return pen;
}

Vector2f Text2::getCharDimensions(uint i_index) const {
	if (FontFace::isPrintable(mText[i_index])) {
		uint charIndex = mText[i_index] - ' ';
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

uint Text2::getLineIndex(uint i_charIndex) const {
	uint out = 0;
	uint lineWrapI = 0;
	for (uint i = 0; i < i_charIndex && i < mText.size(); i++) {
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

Rangeui Text2::getLineRange(uint i_lineIndex) const {
	uint low = 0;
	uint high = (uint)mText.size();
	uint lineIndex = 0;
	uint lineWrapI = 0;
	uint i;
	for (i = 0; i < mText.size() && lineIndex < i_lineIndex; i++) {
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

uint Text2::getClosestCharIndex(Vector2f i_point) const {
	int lineIndex = (int)(i_point.y() / getNewlineHeight());
	if (lineIndex < 0) {
		return 0;
	}
	if (lineIndex >= (int)getNumberLines()) {
		return (uint)mText.size();
	}
	Rangeui lineRange = getLineRange(lineIndex);

	uint charIndex = lineRange.getLow();
	if (i_point.x() < 0.0f) {
		return charIndex;
	}
	Vector2f lastPen = Vector2f(0.0f, i_point.y());
	for (Vector2f pen = Vector2f(0.0f, i_point.y()); charIndex < lineRange.getHigh(); incrementPen(pen, charIndex++)) {
		if (pen.x() >= i_point.x()) {
			if (lastPen.x() == pen.x() || geutil::invLerp(lastPen.x(), pen.x(), i_point.x()) > 0.5) {
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

void Text2::setScrollPosition(Vector2f i_position) {
	mScrollTransform.setLocalPosition(i_position);
}

Vector2f Text2::getScrollToIncludeChar(uint i_charIndex) const {
	Vector2d minima = getLocalCharPosition(i_charIndex);
	Vector2d maxima = minima + getCharDimensions(i_charIndex);
	Vector2d currentMinima = -Vector2d(mScrollTransform.getLocalPosition());
	Vector2d currentMaxima = currentMinima + getContainerDimensions();

	Vector2d out = Vector2d(mScrollTransform.getLocalPosition());
	if (std::find(mLineWrap.begin(), mLineWrap.end(), i_charIndex) == mLineWrap.end()) {
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

void Text2::insert(char i_char, uint i_index) {
	std::string s;
	s += i_char;
	insert(s, i_index);
}

void Text2::insert(const std::string& i_text, uint i_index) {
	destroyOpenGL();
	mText.insert(mText.begin() + i_index, i_text.begin(), i_text.end());
	createOpenGL();
}

void Text2::remove(uint i_start, uint i_number) {
	destroyOpenGL();
	mText.erase(i_start, i_number);
	createOpenGL();
}

void Text2::replace(const std::string& i_text) {
	destroyOpenGL();
	mText = i_text;
	createOpenGL();
}

void Text2::replace(const std::string& i_text, uint i_start, uint i_number) {
	destroyOpenGL();
	mText.replace(i_start, i_number, i_text);
	createOpenGL();
}

double Text2::z() const {
	return getDepthTransform().getWorldDepth();
}

bool Text2::shouldCull() const {
	return false;
}

void Text2::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("TextShader");

	// glDisable(GL_CULL_FACE);

	Matrix4f modelMatrix = mScrollTransform.getWorldMatrix();
	Matrix4f viewMatrix = GE.cameras().getActive()->getMatrix();
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

void Text2::incrementPen(Vector2f& io_pen, uint i_index) const {
	if (std::find(mLineWrap.begin(), mLineWrap.end(), i_index) != mLineWrap.end()) {
		io_pen[0] = 0;
		io_pen[1] += getNewlineHeight();
	}

	if (FontFace::isPrintable(mText[i_index])) {
		uint charIndex = mText[i_index] - ' ';
		io_pen[0] += mRasterSet->mAdvances[charIndex];
	}
	else if (mText[i_index] == '\n') {
		io_pen[0] = 0;
		io_pen[1] += getNewlineHeight();
	}
}

void Text2::createOpenGL() {
	mBitmapArray.getSettings().setMagnifyFilter(TextureSettings::FilterMode::none);
	mBitmapArray.getSettings().setMinifyFilter(TextureSettings::FilterMode::none);
	mBitmapArray.getSettings().setSwizzle(
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

			Vector2f minScreenCoords = RenderTargetUnits2f::pixels(minCoords, targetDimensions).openGLPosition();
			Vector2f maxScreenCoords = RenderTargetUnits2f::pixels(maxCoords, targetDimensions).openGLPosition();

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
