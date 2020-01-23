#ifndef TEXT2_H
#define TEXT2_H

#include "font_face.h"
#include "renderable_object.h"
#include "deep_transformable_object.h"
#include "texture_instance.h"
#include "range.h"

class Text2 : public Renderable, public DeepTransformableObject2d {
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
	Text2(const std::string& i_text, FontFaceRasterSet* i_rasterSet, Color4c i_color = Color4c(255, 255, 255, 255));
	~Text2();

	std::string getText() const;

	Vector2f getLocalDimensions() const;
	Vector2f getLocalCharPosition(uint i_index) const;
	Vector2f getCharDimensions(uint i_index) const;
	uint getNumberLines() const;
	uint getLineIndex(uint i_charIndex) const;
	Rangeui getLineRange(uint i_lineIndex) const;
	float getCharHeight() const;
	float getNewlineHeight() const;
	uint getClosestCharIndex(Vector2f i_point) const;
	Vector2f getContainerDimensions() const;

	Transform2d& getScrollTransform();
	void setScrollPosition(Vector2f i_position);
	Vector2f getScrollToIncludeChar(uint i_charIndex) const;

	void insert(char i_char, uint i_index);
	void insert(const std::string& i_text, uint i_index);
	void remove(uint i_start, uint i_number);
	void replace(const std::string& i_text);
	void replace(const std::string& i_text, uint i_start, uint i_number);

	virtual double z() const override;
	virtual bool shouldCull() const override;
	virtual void render() override;

private:
	uint getNumberPrintable();
	void incrementPen(Vector2f& io_pen, uint i_index) const;

	void createOpenGL();
	void destroyOpenGL();
};

#endif