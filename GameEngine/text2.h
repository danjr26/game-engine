#ifndef TEXT2_H
#define TEXT2_H

#include "font_face.h"
#include "renderable_object.h"
#include "transformable_object.h"
#include "texture_instance.h"
#include "renderer.h"

class Text2 : public RenderableObject, public TransformableObject {
public:
	class Params {
	private:
		ColorRGBAc color;
	};
private:
	std::string text;
	FontFaceRasterSet* rasterSet;
	TextureInstance bitmapArray;

	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint indexBufferID;
	ColorRGBAc tint;

public:
	Text2(std::string in_text, FontFaceRasterSet* in_rasterSet, Transform in_transform, ColorRGBAc in_color = ColorRGBAc(255, 255, 255, 255));
	~Text2();
	virtual double Z() const override;
	virtual bool Should_Cull() const override;
	virtual void Render() override;

private:
	uint Get_Number_Printable();
};

#endif