#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "bit_manipulation.h"
#include "definitions.h"
#include "vector.h"
#include "color.h"
#include "render_target.h"
#include "renderable_object.h"
#include "filter_query.h"
#include "camera.h"
#include <vector>

class ShaderProgram;

class RenderPass {
public:
	enum SortOrder : ubyte {
		dont_care,
		front_to_back,
		back_to_front
	};

private:
	RenderTarget* target;
	Camera* camera;

	GLuint clearBits;
	ColorRGBAf clearColor;
	GLfloat clearDepth;
	GLint clearStencil;
	SortOrder sortOrder;

	FilterQuery query;

protected:
	virtual void OnBegin(void* in_params);
	virtual void* OnEnd();

public:
	RenderPass(RenderTarget* in_target, Camera* in_camera);
	virtual ~RenderPass();
	RenderPass& Filter(const FilterQuery& in_query);
	RenderPass& Sort_Order(SortOrder in_order);
	RenderPass& Clear_Color(const ColorRGBAf& in_value);
	RenderPass& Clear_Depth(GLfloat in_value);
	RenderPass& Clear_Stencil(GLint in_value);
	RenderPass& Cancel_Clear_Color();
	RenderPass& Cancel_Clear_Depth();
	RenderPass& Cancel_Clear_Stencil();

	void Begin(void* in_params);
	void* End();

	RenderTarget* Get_Render_Target();
	const FilterQuery& Get_Query() const;
	SortOrder Get_Sort_Order() const;
};

#endif