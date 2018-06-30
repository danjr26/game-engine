#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "vector.h"
#include "shader_program.h"
#include "filtered_object.h"
#include "disableable_object.h"

class Renderer;

class RenderableObject : public FilteredObject, public DisableableObject {
public:
	RenderableObject();
	virtual ~RenderableObject();
	virtual double Z() const = 0;
	virtual bool Should_Cull() const = 0;
	virtual void Render() = 0;
};

/*
class TestRenderableObject : public RenderableObject {
public:
	TestRenderableObject(Renderer* in_renderer);
	double Z() const override final;
	bool Should_Cull() const override final;
	void Render() override final;
};
*/
#endif