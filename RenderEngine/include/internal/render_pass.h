#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "../../../Geometry/include/geometry.h"
#include "render_target.h"
#include "camera.h"
#include "color.h"
#include "clear_command.h"
#include "render_request.h"
#include <functional>

enum class RenderSortOrder : ubyte {
	undefined,
	front_first,
	back_first
};

struct RenderPass {
	std::function<bool(const RenderRequest&)> mFilter;
	RenderSortOrder mSortOrder;

	Camera* mCamera;
	RenderTarget* mRenderTarget;
	ClearCommand mClearCommand;

	RenderPass() :
	mFilter(defaultFilter),
	mSortOrder(RenderSortOrder::undefined),
	mCamera(nullptr),
	mRenderTarget(nullptr),
	mClearCommand()
	{}

	static bool defaultFilter(const RenderRequest& r) {
		return true;
	}
};

#endif