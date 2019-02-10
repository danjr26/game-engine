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
	mQuery(FilterQuery::Any()),
	mClearBits(0),
	mSortOrder(dont_care) 
{}

RenderPass::~RenderPass() 
{}

RenderPass& RenderPass::Filter(const FilterQuery& in_query) {
	mQuery = in_query;
	return (*this);
}

RenderPass& RenderPass::Sort_Order(SortOrder in_order) {
	mSortOrder = in_order;
	return (*this);
}

void RenderPass::Begin(void* in_params) {
	OnBegin(in_params);
	mTarget->Draw_To_This();
	mCamera->Use();

	if (mClearBits & GL_COLOR_BUFFER_BIT) {
		glClearColor(
			mClearColor.R(), 
			mClearColor.G(), 
			mClearColor.B(), 
			mClearColor.A()
		);
	}
	if (mClearBits & GL_DEPTH_BUFFER_BIT) {
		glClearDepth(mClearDepth);
	}
	if (mClearBits & GL_STENCIL_BUFFER_BIT) {
		glClearStencil(mClearStencil);
	}
	//glFinish();
	//Log::main(std::string("before glClear(): ") + std::to_string(GE.Time().Now()));
	glClear(mClearBits);
	//Log::main(std::string("after glClear(): ") + std::to_string(GE.Time().Now()));
}

void* RenderPass::End() {
	return OnEnd();
}

RenderTarget* RenderPass::Get_Render_Target() {
	return mTarget;
}

const FilterQuery& RenderPass::Get_Query() const {
	return mQuery;
}

RenderPass::SortOrder RenderPass::Get_Sort_Order() const {
	return mSortOrder;
}

RenderPass& RenderPass::Clear_Color(const ColorRGBAf& in_value) {
	Set_Bit<GLuint>(mClearBits, GL_COLOR_BUFFER_BIT, true);
	mClearColor = in_value;
	return (*this);
}

RenderPass& RenderPass::Clear_Depth(GLfloat in_value) {
	Set_Bit<GLuint>(mClearBits, GL_DEPTH_BUFFER_BIT, true);
	mClearDepth = in_value;
	return (*this);
}

RenderPass& RenderPass::Clear_Stencil(GLint in_value) {
	Set_Bit<GLuint>(mClearBits, GL_STENCIL_BUFFER_BIT, true);
	mClearStencil = in_value;
	return (*this);
}

RenderPass& RenderPass::Cancel_Clear_Color() {
	Set_Bit<GLuint>(mClearBits, GL_COLOR_BUFFER_BIT, false);
	return (*this);
}

RenderPass& RenderPass::Cancel_Clear_Depth() {
	Set_Bit<GLuint>(mClearBits, GL_DEPTH_BUFFER_BIT, false);
	return (*this);
}

RenderPass & RenderPass::Cancel_Clear_Stencil() {
	Set_Bit<GLuint>(mClearBits, GL_STENCIL_BUFFER_BIT, false);
	return (*this);
}
