#ifndef TEXT2_H
#define TEXT2_H

#include "font_face.h"
#include "renderable_object.h"
#include "deep_transformable_object.h"
#include "texture_instance.h"
#include "renderer.h"
#include "range.h"

class Text2 : public RenderableObject, public DeepTransformableObject2d {
public:
	enum OverflowPolicy {
		overflow,
		cull,
		wrap,
		word_wrap
	};
private:
	std::string mText;

	OverflowPolicy mOverflowPolicy;
	std::vector<uint> mLineWrap;
	Vector2f mContainerDimensions;
	Transform2d mScrollTransform;

	FontFaceRasterSet* mRasterSet;
	TextureInstance mBitmapArray;

	GLuint mVertexArrayID;
	GLuint mVertexBufferID;
	GLuint mUVBufferID;
	GLuint mIndexBufferID;

public:
	Text2(const std::string& in_text, FontFaceRasterSet* in_rasterSet, ColorRGBAc in_color = ColorRGBAc(255, 255, 255, 255));
	~Text2();

	std::string getText() const;

	Vector2f getLocalDimensions() const;
	Vector2f getLocalCharPosition(uint in_index) const;
	Vector2f getCharDimensions(uint in_index) const;
	uint getNumberLines() const;
	uint getLineIndex(uint in_charIndex) const;
	Rangeui getLineRange(uint in_lineIndex) const;
	float getCharHeight() const;
	float getNewlineHeight() const;
	uint getClosestCharIndex(Vector2f in_point) const;
	Vector2f getContainerDimensions() const;

	Transform2d& getScrollTransform();
	void setScrollPosition(Vector2f in_position);
	Vector2f getScrollToIncludeChar(uint in_charIndex) const;

	void insert(char in_char, uint in_index);
	void insert(const std::string& in_text, uint in_index);
	void remove(uint in_start, uint in_number);
	void replace(const std::string& in_text);
	void replace(const std::string& in_text, uint in_start, uint in_number);

	virtual double z() const override;
	virtual bool shouldCull() const override;
	virtual void render() override;

private:
	uint getNumberPrintable();
	void incrementPen(Vector2f& inout_pen, uint in_index) const;

	void createOpenGL();
	void destroyOpenGL();
};

#endif