#include "render_pass.h"
#include "game_engine.h"

void RenderPass::OnBegin(void* in_params) 
{}

void* RenderPass::OnEnd() {
	return nullptr;
}

RenderPass::RenderPass(RenderTarget* in_target, Camera* in_camera) :
	camera(in_camera),
	target(in_target),
	query(FilterQuery::Any()),
	clearBits(0),
	sortOrder(dont_care) 
{}

RenderPass::~RenderPass() 
{}

RenderPass& RenderPass::Filter(const FilterQuery& in_query) {
	query = in_query;
	return (*this);
}

RenderPass& RenderPass::Sort_Order(SortOrder in_order) {
	sortOrder = in_order;
	return (*this);
}

void RenderPass::Begin(void* in_params) {
	OnBegin(in_params);
	target->Draw_To_This();
	camera->Use();

	if (clearBits & GL_COLOR_BUFFER_BIT) {
		glClearColor(
			clearColor.R(), 
			clearColor.G(), 
			clearColor.B(), 
			clearColor.A()
		);
	}
	if (clearBits & GL_DEPTH_BUFFER_BIT) {
		glClearDepth(clearDepth);
	}
	if (clearBits & GL_STENCIL_BUFFER_BIT) {
		glClearStencil(clearStencil);
	}
	glFinish();
	//Log::main(std::string("before glClear(): ") + std::to_string(GE.Time().Now()));
	glClear(clearBits);
	//Log::main(std::string("after glClear(): ") + std::to_string(GE.Time().Now()));
}

void* RenderPass::End() {
	return OnEnd();
}

RenderTarget* RenderPass::Get_Render_Target() {
	return target;
}

const FilterQuery& RenderPass::Get_Query() const {
	return query;
}

RenderPass::SortOrder RenderPass::Get_Sort_Order() const {
	return sortOrder;
}

RenderPass& RenderPass::Clear_Color(const ColorRGBAf& in_value) {
	Set_Bit<GLuint>(clearBits, GL_COLOR_BUFFER_BIT, true);
	clearColor = in_value;
	return (*this);
}

RenderPass& RenderPass::Clear_Depth(GLfloat in_value) {
	Set_Bit<GLuint>(clearBits, GL_DEPTH_BUFFER_BIT, true);
	clearDepth = in_value;
	return (*this);
}

RenderPass& RenderPass::Clear_Stencil(GLint in_value) {
	Set_Bit<GLuint>(clearBits, GL_STENCIL_BUFFER_BIT, true);
	clearStencil = in_value;
	return (*this);
}

RenderPass& RenderPass::Cancel_Clear_Color() {
	Set_Bit<GLuint>(clearBits, GL_COLOR_BUFFER_BIT, false);
	return (*this);
}

RenderPass& RenderPass::Cancel_Clear_Depth() {
	Set_Bit<GLuint>(clearBits, GL_DEPTH_BUFFER_BIT, false);
	return (*this);
}

RenderPass & RenderPass::Cancel_Clear_Stencil() {
	Set_Bit<GLuint>(clearBits, GL_STENCIL_BUFFER_BIT, false);
	return (*this);
}
