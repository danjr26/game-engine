#include "renderer.h"

Renderer::Renderer() {
	objects.reserve(256);
}

void Renderer::Render(const RenderPass& in_pass) {
	in_pass.Begin();

	for (uint i = 0; i < objects.size(); i++) {
		if (!objects[i]->Should_Cull()) {
			objects[i]->Render();
		}
	}

	in_pass.End();
}

void Renderer::Add(RenderableObject* in_object) {
	objects.push_back(in_object);
}

void Renderer::Remove(RenderableObject* in_object) {
	for (uint i = 0; i < objects.size(); i++) {
		if (objects[i] == in_object) {
			objects.erase(objects.begin() + i);
			return;
		}
	}
}
