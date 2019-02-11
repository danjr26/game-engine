#ifndef RENDERER_H
#define RENDERER_H

#include "renderable_object.h"
#include "render_pass.h"
#include <vector>

class Renderer {
private:
	std::vector<RenderableObject*> objects;

public:
	Renderer();
	void Render(const RenderPass& in_pass);
	void Add(RenderableObject* in_object);
	void Remove(RenderableObject* in_object);
};

#endif 
