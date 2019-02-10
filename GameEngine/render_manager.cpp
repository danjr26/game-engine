#include "render_manager.h"
#include "clock.h"
#include "log.h"
#include "game_engine.h"

RenderManager::RenderManager() :
	mPasses(),
	mActivePass(nullptr),
	mActiveTarget(nullptr),
	mMainTarget(nullptr),
	mMainWindow(nullptr)
{}

void RenderManager::add(RenderableObject* in_renderable) {
	RenderableWithZ chunk = { in_renderable, in_renderable->z() };
	auto position = std::lower_bound(mRenderables.begin(), mRenderables.end(), chunk, zSorter);	
	mRenderables.insert(position, chunk);
}

void RenderManager::remove(RenderableObject* in_renderable) {
	for (uint i = 0; i < mRenderables.size(); i++) {
		if (mRenderables[i].mObject == in_renderable) {
			mRenderables.erase(mRenderables.begin() + i);
			return;
		}
	}
}

void RenderManager::renderPass(RenderPass* in_pass) {
	in_pass->begin(mRenderPassArg);

	RenderableObject* object = nullptr;

	for (uint i = 0; i < mRenderables.size(); i++) {

		object = (in_pass->getSortOrder() == RenderPass::front_to_back) ?
			mRenderables[mRenderables.size() - i - 1].mObject :
			mRenderables[i].mObject;

		if (object->isEnabled() && in_pass->getQuery().evaluate(*object) && !object->shouldCull()) {
			object->render();
		}

	}

	mRenderPassArg = in_pass->end();
}

bool RenderManager::zSorter(const RenderableWithZ& in_chunk1, const RenderableWithZ& in_chunk2) {
	return in_chunk1.mObject->z() < in_chunk2.mObject->z();
}

void RenderManager::renderFrame() {
	static uint c = 0;
	//Log::main(std::string("frame ") + std::to_string(c++));
	mRenderPassArg = nullptr;

	for (uint i = 0; i < mPasses.size(); i++) {
		if (mPasses[i] == nullptr) {
			throw InvalidArgumentException("render pass was null");
		}
		renderPass(mPasses[i]);
	}
}