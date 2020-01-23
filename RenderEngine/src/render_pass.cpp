#include "../include/internal/render_pass.h"

RenderPass::RenderPass(id_t i_id, RenderTarget* i_RenderTarget, Camera* i_camera) :
	mId(i_id),
	mRenderTarget(i_RenderTarget),
	mCamera(i_camera),
	mClearBits(0),
	mClearColor(),
	mClearDepth(0),
	mClearStencil(0),
	mSortOrder(dont_care) 
{}

void RenderPass::begin(void* i_params) {
	OnBegin(i_params);
	mTarget->drawToThis();
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

RenderPass& RenderPass::clearColor(const Color4f& i_value) {
	setBit<GLuint>(mClearBits, GL_COLOR_BUFFER_BIT, true);
	mClearColor = i_value;
	return (*this);
}

RenderPass& RenderPass::clearDepth(GLfloat i_value) {
	setBit<GLuint>(mClearBits, GL_DEPTH_BUFFER_BIT, true);
	mClearDepth = i_value;
	return (*this);
}

RenderPass& RenderPass::clearStencil(GLint i_value) {
	setBit<GLuint>(mClearBits, GL_STENCIL_BUFFER_BIT, true);
	mClearStencil = i_value;
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
