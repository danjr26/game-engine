#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "render_pass.h"
#include "render_target.h"
#include "renderable_object.h"
#include "window.h"
#include <algorithm>

class RenderManager {
public:
	enum Filter : ubyte {
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
	void Add(RenderableObject* in_renderable);
	void Remove(RenderableObject* in_renderable);
	void Render_Frame();

private:
	void Render_Pass(RenderPass* in_pass);

	static bool Z_Sorter(const RenderableWithZ& in_chunk1, const RenderableWithZ& in_chunk2);
};

#endif