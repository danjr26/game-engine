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
	RenderTarget* mTarget;
	Camera* mCamera;

	GLuint mClearBits;
	ColorRGBAf mClearColor;
	GLfloat mClearDepth;
	GLint mClearStencil;
	SortOrder mSortOrder;

	FilterQuery mQuery;

protected:
	virtual void OnBegin(void* in_params);
	virtual void* OnEnd();

public:
	RenderPass(RenderTarget* in_target, Camera* in_camera);
	virtual ~RenderPass();
	RenderPass& filter(const FilterQuery& in_query);
	RenderPass& sortOrder(SortOrder in_order);
	RenderPass& clearColor(const ColorRGBAf& in_value);
	RenderPass& clearDepth(GLfloat in_value);
	RenderPass& clearStencil(GLint in_value);
	RenderPass& cancelClearColor();
	RenderPass& cancelClearDepth();
	RenderPass& cancelClearStencil();

	void begin(void* in_params);
	void* end();

	RenderTarget* getRenderTarget();
	const FilterQuery& getQuery() const;
	SortOrder getSortOrder() const;
};

#endif