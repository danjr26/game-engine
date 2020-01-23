#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "render_pass.h"
#include "renderable.h"
#include <unordered_set>

class RenderManager {
private:
	std::unordered_set<Renderable*> mRenderables;
	std::vector<RenderPass*> mPasses;

public:
	RenderManager();
	void add(Renderable* i_renderable);
	void remove(Renderable* i_renderable);
	void renderFrame();

private:
	void renderPass(RenderPass* i_pass);

	static bool zSorter(const RenderableWithZ& i_chunk1, const RenderableWithZ& i_chunk2);
};

#endif