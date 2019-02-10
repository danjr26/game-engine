#include "render_pass.h"
#include "game_engine.h"

void RenderPass::OnBegin(void* in_params) 
{}

void* RenderPass::OnEnd() {
	return nullptr;
}

RenderPass::RenderPass(RenderTarget* in_target, Camera* in_camera) :
	mCamera(in_camera),
	mTarget(in_target),
	mQuery(FilterQuery::everything()),
	mClearBits(0),
	mSortOrder(dont_care) 
{}

RenderPass::~RenderPass() 
{}

RenderPass& RenderPass::filter(const FilterQuery& in_query) {
	mQuery = in_query;
	return (*this);
}

RenderPass& RenderPass::sortOrder(SortOrder in_order) {
	mSortOrder = in_order;
	return (*this);
}

void RenderPass::begin(void* in_params) {
	OnBegin(in_params);
	mTarget->Draw_To_This();
	mCamera->use();

	if (mClearBits & GL_COLOR_BUFFER_BIT) {
		glClearColor(
			mClearColor.r(), 
			mClearColor.g(), 
			mClearColor.b(), 
			mClearColor.a()
		);
	}
	if (mClearBits & GL_DEPTH_BUFFER_BIT) {
		glClearDepth(mClearDepth);
	}
	if (mClearBits & GL_STENCIL_BUFFER_BIT) {
		glClearStencil(mClearStencil);
	}
	//glFinish();
	//Log::main(std::string("before glClear(): ") + std::to_string(GE.clock().now()));
	glClear(mClearBits);
	//Log::main(std::string("after glClear(): ") + std::to_string(GE.clock().now()));
}

void* RenderPass::end() {
	return OnEnd();
}

RenderTarget* RenderPass::getRenderTarget() {
	return mTarget;
}

const FilterQuery& RenderPass::getQuery() const {
	return mQuery;
}

RenderPass::SortOrder RenderPass::getSortOrder() const {
	return mSortOrder;
}

RenderPass& RenderPass::clearColor(const ColorRGBAf& in_value) {
	setBit<GLuint>(mClearBits, GL_COLOR_BUFFER_BIT, true);
	mClearColor = in_value;
	return (*this);
}

RenderPass& RenderPass::clearDepth(GLfloat in_value) {
	setBit<GLuint>(mClearBits, GL_DEPTH_BUFFER_BIT, true);
	mClearDepth = in_value;
	return (*this);
}

RenderPass& RenderPass::clearStencil(GLint in_value) {
	setBit<GLuint>(mClearBits, GL_STENCIL_BUFFER_BIT, true);
	mClearStencil = in_value;
	return (*this);
}

RenderPass& RenderPass::cancelClearColor() {
	setBit<GLuint>(mClearBits, GL_COLOR_BUFFER_BIT, false);
	return (*this);
}

RenderPass& RenderPass::cancelClearDepth() {
	setBit<GLuint>(mClearBits, GL_DEPTH_BUFFER_BIT, false);
	return (*this);
}

RenderPass & RenderPass::cancelClearStencil() {
	setBit<GLuint>(mClearBits, GL_STENCIL_BUFFER_BIT, false);
	return (*this);
}
