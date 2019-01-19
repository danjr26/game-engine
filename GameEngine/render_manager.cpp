#include "render_manager.h"
#include "clock.h"
#include "log.h"
#include "game_engine.h"

RenderManager::RenderManager() :
	passes(),
	activePass(nullptr),
	activeTarget(nullptr),
	mainTarget(nullptr),
	mainWindow(nullptr)
{}

void RenderManager::Add(RenderableObject* in_renderable) {
	RenderableWithZ chunk = { in_renderable, in_renderable->Z() };
	auto position = std::lower_bound(renderables.begin(), renderables.end(), chunk, Z_Sorter);	
	renderables.insert(position, chunk);
}

void RenderManager::Remove(RenderableObject* in_renderable) {
	for (uint i = 0; i < renderables.size(); i++) {
		if (renderables[i].object == in_renderable) {
			renderables.erase(renderables.begin() + i);
			return;
		}
	}
}

void RenderManager::Render_Pass(RenderPass* in_pass) {
	in_pass->Begin(renderPassArg);

	RenderableObject* object = nullptr;

	for (uint i = 0; i < renderables.size(); i++) {

		object = (in_pass->Get_Sort_Order() == RenderPass::front_to_back) ?
			renderables[renderables.size() - i - 1].object :
			renderables[i].object;

		if (object->Is_Enabled() && in_pass->Get_Query().Evaluate(*object) && !object->Should_Cull()) {
			object->Render();
		}

	}

	renderPassArg = in_pass->End();
}

bool RenderManager::Z_Sorter(const RenderableWithZ& in_chunk1, const RenderableWithZ& in_chunk2) {
	return in_chunk1.object->Z() < in_chunk2.object->Z();
}

void RenderManager::Render_Frame() {
	static uint c = 0;
	//Log::main(std::string("frame ") + std::to_string(c++));
	renderPassArg = nullptr;

	for (uint i = 0; i < passes.size(); i++) {
		if (passes[i] == nullptr) {
			throw InvalidArgumentException("render pass was null");
		}
		Render_Pass(passes[i]);
	}
}