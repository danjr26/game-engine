#ifndef TEXT2_H
#define TEXT2_H

#include "font_face.h"
#include "renderable_object.h"
#include "transformable_object.h"
#include "texture_instance.h"
#include "renderer.h"
#include "range.h"

class Text2 : public RenderableObject, public TransformableObject {
public:
	struct Params {
		ColorRGBAc color;
	};
private:
	std::string text;
	std::vector<uint> lineWrap;

	FontFaceRasterSet* rasterSet;
	TextureInstance bitmapArray;

	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint indexBufferID;

public:
	Text2(const std::string& in_text, FontFaceRasterSet* in_rasterSet, Transform in_transform, ColorRGBAc in_color = ColorRGBAc(255, 255, 255, 255));
	~Text2();
	Vector2f Get_Local_Dimensions() const;
	Vector2f Get_Local_Char_Position(uint in_index) const;
	Vector2f Get_Char_Dimensions(uint in_index) const;
	uint Get_Number_Lines() const;
	uint Get_Line_Index(uint in_charIndex) const;
	Rangeui Get_Line_Range(uint in_lineIndex) const;
	float Get_Char_Height() const;
	uint Get_Closest_Char_Index(Vector2f in_point) const;

	std::string Get_Text() const;

	void Insert(const std::string& in_text, uint in_index);
	void Delete(uint in_start, uint in_number);
	void Replace(const std::string& in_text);
	void Replace(const std::string& in_text, uint in_start, uint in_number);

	virtual double Z() const override;
	virtual bool Should_Cull() const override;
	virtual void Render() override;

private:
	uint Get_Number_Printable();
	void Increment_Pen(Vector2f& pen, uint in_index) const;

	void Create_OpenGL();
	void Destroy_OpenGL();
};

#endif