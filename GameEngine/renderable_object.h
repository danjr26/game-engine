#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "vector.h"
#include "shader_program.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "axis_aligned_box.h"

class Renderer;

class RenderableObject : public FilteredObject, public DisableableObject {
private:
	AxisAlignedRectangled mCullingRectangle;

public:
	RenderableObject();
	virtual ~RenderableObject();
	virtual double z() const = 0;
	virtual bool shouldCull() const = 0;
	virtual void render() = 0;
};

#endif