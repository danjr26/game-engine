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

	std::string Get_Text() const;

	Vector2f Get_Local_Dimensions() const;
	Vector2f Get_Local_Char_Position(uint in_index) const;
	Vector2f Get_Char_Dimensions(uint in_index) const;
	uint Get_Number_Lines() const;
	uint Get_Line_Index(uint in_charIndex) const;
	Rangeui Get_Line_Range(uint in_lineIndex) const;
	float Get_Char_Height() const;
	float Get_Newline_Height() const;
	uint Get_Closest_Char_Index(Vector2f in_point) const;
	Vector2f Get_Container_Dimensions() const;

	Transform2d& Get_Scroll_Transform();
	void Set_Scroll_Position(Vector2f in_position);
	Vector2f Get_Scroll_To_Include_Char(uint in_charIndex) const;

	void Insert(char in_char, uint in_index);
	void Insert(const std::string& in_text, uint in_index);
	void Delete(uint in_start, uint in_number);
	void Replace(const std::string& in_text);
	void Replace(const std::string& in_text, uint in_start, uint in_number);

	virtual double Z() const override;
	virtual bool Should_Cull() const override;
	virtual void Render() override;

private:
	uint Get_Number_Printable();
	void Increment_Pen(Vector2f& inout_pen, uint in_index) const;

	void Create_OpenGL();
	void Destroy_OpenGL();
};

#endif