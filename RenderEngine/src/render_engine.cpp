#include "../include/internal/render_engine.h"
#include <iostream>

RenderEngine::RenderEngine() :
mActiveRenderTarget(nullptr),
mActiveShader(nullptr),
mDisplayManager(),
mWindow()
{}

void RenderEngine::render() {
	std::vector<passes_graph_t::node_t*> order;
	mPasses.getTopologicalOrder(order);

	for (passes_graph_t::node_t* passNode : order) {
		RenderPass& pass = *passNode->mData;
		std::vector<RenderRequest*> requests;
		retrieveRequests(pass, requests);
		activate(pass);
		std::unordered_set<VertexPusher*> vertexPushers;
		uint instanceIndex = 0;
		for (RenderRequest* request : requests) {
			vertexPushers.insert(request->mVertices->getParent());

			VirtualVertexPusher::RenderCommand* command = request->mVertices->newCommands(1);
			*command = *request->mCommand;
			command->mInstanceBegin += instanceIndex;
			request->mVertices->commitCommands(command);

			instanceIndex += request->mCommand->mNInstances;
		}
		for (VertexPusher* vertexPusher : vertexPushers) {
			vertexPusher->render();
		}
	}
}

void RenderEngine::requestVertexPusher(RenderRequest& io_request, const VertexPusherLayout& i_layout, size_t i_nIndices, size_t i_nVertices) {
	mVertexPushers.emplace_back(i_layout, i_nIndices, i_nVertices, i_nIndices);
	VertexPusher& vertexPusher = mVertexPushers.back();
	io_request.mVertices = new VirtualVertexPusher();
	io_request.mVertices->mParent = &vertexPusher;
	io_request.mVertices->mIndexBegin = 0;
	io_request.mVertices->mIndexLength = i_nIndices;
	io_request.mVertices->mVertexBegin = 0;
	io_request.mVertices->mVertexLength = i_nVertices;
}

void RenderEngine::reportErrors(GLDEBUGPROC i_callback, const void* i_param) {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(i_callback, i_param);
};

void RenderEngine::retrieveRequests(const RenderPass& i_pass, std::vector<RenderRequest*>& o_requests) const {
	o_requests.clear();
	for (RenderRequest* request : mRequests) {
		if (i_pass.mFilter(*request)) {
			o_requests.push_back(request);
		}
	}
}

void RenderEngine::activate(RenderTarget* i_target) {
	if (mActiveRenderTarget == i_target) return;
	if (i_target == nullptr) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	else {
		i_target->_activate();
	}
}

ShaderProgram* RenderEngine::retrieveShader(const std::vector<Shader*>& i_shaderParts) {
	for (CachedShader& cachedShader : mCachedShaders) {
		if (i_shaderParts == cachedShader.mShaderParts) {
			return &cachedShader.mShader;
		}
	}
	return nullptr;
}

void RenderEngine::activate(ShaderProgram* i_shader) {
	if (mActiveShader == i_shader) return;
	if (i_shader == nullptr) {
		glUseProgram(0);
	}
	else {
		i_shader->_activate();
	}
}

void RenderEngine::activate(RenderPass& i_pass) {
	activate(i_pass.mRenderTarget);
	execute(i_pass.mClearCommand);
}

void RenderEngine::bind(texture_slot_t i_slot, Texture* i_texture) {
	auto result = mActiveTextures.find(i_slot);
	if (result == mActiveTextures.end()) {
		if (i_texture == nullptr) return;
		mActiveTextures.insert(std::make_pair(i_slot, i_texture));
		i_texture->_bind(i_slot);
	}
	else if (result->second == i_texture) {
		return;
	}
	else if (i_texture == nullptr) {
		mActiveTextures.erase(result);
	}
	else {
		result->second = i_texture;
		i_texture->_bind(i_slot);
	}
}

void RenderEngine::execute(const ClearCommand& i_command) {
	glClearColor(i_command.mColor.r(), i_command.mColor.g(), i_command.mColor.b(), i_command.mColor.a());
	glClearDepth(i_command.mDepth);
	glClearStencil(i_command.mStencil);
	glClear(ClearBits::toGL(i_command.mBits));
}

void GLAPIENTRY defaultErrorCallback(GLenum i_source, GLenum i_type, GLuint i_id, GLenum i_severity, GLsizei i_length, const GLchar* i_message, const void* i_param) {
	std::cout
		<< "\n!GL ERROR!"
		<< " source:" << i_source
		<< " type:" << i_type
		<< " id:" << i_id
		<< " severity:" << i_severity
		<< " message:'" << std::string(i_message, i_length) << "'"
		<< std::endl;
	exit(-1);
}
