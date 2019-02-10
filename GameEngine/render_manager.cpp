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

void RenderManager::Add(RenderableObject* in_renderable) {
	RenderableWithZ chunk = { in_renderable, in_renderable->Z() };
	auto position = std::lower_bound(mRenderables.begin(), mRenderables.end(), chunk, Z_Sorter);	
	mRenderables.insert(position, chunk);
}

void RenderManager::Remove(RenderableObject* in_renderable) {
	for (uint i = 0; i < mRenderables.size(); i++) {
		if (mRenderables[i].mObject == in_renderable) {
			mRenderables.erase(mRenderables.begin() + i);
			return;
		}
	}
}

void RenderManager::Render_Pass(RenderPass* in_pass) {
	in_pass->Begin(mRenderPassArg);

	RenderableObject* object = nullptr;

	for (uint i = 0; i < mRenderables.size(); i++) {

		object = (in_pass->Get_Sort_Order() == RenderPass::front_to_back) ?
			mRenderables[mRenderables.size() - i - 1].mObject :
			mRenderables[i].mObject;

		if (object->Is_Enabled() && in_pass->Get_Query().Evaluate(*object) && !object->Should_Cull()) {
			object->Render();
		}

	}

	mRenderPassArg = in_pass->End();
}

bool RenderManager::Z_Sorter(const RenderableWithZ& in_chunk1, const RenderableWithZ& in_chunk2) {
	return in_chunk1.mObject->Z() < in_chunk2.mObject->Z();
}

void RenderManager::Render_Frame() {
	static uint c = 0;
	//Log::main(std::string("frame ") + std::to_string(c++));
	mRenderPassArg = nullptr;

	for (uint i = 0; i < mPasses.size(); i++) {
		if (mPasses[i] == nullptr) {
			throw InvalidArgumentException("render pass was null");
		}
		Render_Pass(mPasses[i]);
	}
}