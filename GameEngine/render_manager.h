#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "render_pass.h"
#include "render_target.h"
#include "renderable_object.h"
#include "window.h"
#include <algorithm>

class RenderManager {
public:
	enum filter : ubyte {
		default_opaque,
		default_transparent
	};

private:
	struct RenderableWithZ {
		RenderableObject* mObject;
		double mZ;
	};
	std::vector<RenderableWithZ> mRenderables;

	void* mRenderPassArg;

public:
	std::vector<RenderPass*> mPasses;
	RenderPass* mActivePass;
	RenderTarget* mActiveTarget;
	RenderTarget* mMainTarget;
	Window* mMainWindow;

public:
	RenderManager();
	void add(RenderableObject* in_renderable);
	void remove(RenderableObject* in_renderable);
	void renderFrame();

private:
	void renderPass(RenderPass* in_pass);

	static bool zSorter(const RenderableWithZ& in_chunk1, const RenderableWithZ& in_chunk2);
};

#endif