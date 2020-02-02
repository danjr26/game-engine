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
		activate(pass);

		std::list<RenderRequestBatch> batches;
		createBatches(pass, batches);

		for (RenderRequestBatch& batch : batches) {
			activate(batch);

			uint instanceIndex = 0;
			VertexPusher::RenderCommand* commandsOut = batch.mVertices->newCommands((uint)batch.mCommands.size());
			for (size_t i = 0; i < batch.mCommands.size(); i++) {
				commandsOut[i] = batch.mCommands[i];
				commandsOut[i].mInstanceBegin += instanceIndex;
				instanceIndex += commandsOut[i].mNInstances;
			}
			batch.mVertices->commitCommands(commandsOut);
			batch.mVertices->finalize();
			batch.mVertices->render();
			batch.mVertices->mNCommands = 0;
		}
	}
}

void RenderEngine::requestVertexPusher(RenderRequest& io_request, const VertexPusherLayout& i_layout, uint i_nIndices, uint i_nVertices) {
	mVertexPushers.push_back(new VertexPusher(i_layout, i_nIndices, i_nVertices, i_nIndices));
	VertexPusher& vertexPusher = *mVertexPushers.back();
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

void RenderEngine::activate(RenderPass& i_pass) {
	activate(i_pass.mRenderTarget);
	execute(i_pass.mClearCommand);
}

void RenderEngine::activate(RenderRequestBatch& i_batch) {
	activate(i_batch.mShader);
	for (auto it = i_batch.mShaderData.begin(); it != i_batch.mShaderData.end(); it++) {
		activate(it->second, it->first);
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
	mActiveRenderTarget = i_target;
}

ShaderProgram* RenderEngine::consolidate(const std::vector<Shader*>& i_shaderParts) {
	for (CachedShader& cachedShader : mCachedShaders) {
		if (i_shaderParts == cachedShader.mShaderParts) {
			return &cachedShader.mShader;
		}
	}
	mCachedShaders.push_back(CachedShader());
	mCachedShaders.back().mShaderParts = i_shaderParts;
	mCachedShaders.back().mShader.init(i_shaderParts.data(), (uint)i_shaderParts.size());
	return &mCachedShaders.back().mShader;
}

void RenderEngine::release(ShaderProgram* i_shader) {
	
}

void RenderEngine::activate(ShaderProgram* i_shader) {
	if (mActiveShader == i_shader) return;
	if (i_shader == nullptr) {
		glUseProgram(0);
	}
	else {
		i_shader->_activate();
	}
	mActiveShader = i_shader;
}

ShaderDataPusher* RenderEngine::consolidate(const std::vector<ShaderData*>& i_shaderData) {
	if (i_shaderData.empty()) return nullptr;
	uint size = 0;
	ShaderDataLayout layout = i_shaderData[0]->mLayout;
	for (ShaderData* data : i_shaderData) {
		if (layout != data->mLayout) fail();
		size += data->mSize;
	}
	ShaderDataPusher* out = nullptr;
	for (auto it = mFreeShaderDataPushers.begin(); it != mFreeShaderDataPushers.end(); it++) {
		ShaderDataPusher* pusher = *it;
		if (pusher->mSize >= size && pusher->mLayout == layout && (out == nullptr || out->mSize > pusher->mSize)) {
			mFreeShaderDataPushers.erase(it);
			out = pusher;
		}
	}
	if (out == nullptr) {
		out = new ShaderDataPusher(layout, size * 2);
	}

	uint offset = 0;
	void* edit = out->mBuffer.edit(0, size);
	for (ShaderData* data : i_shaderData) {
		memcpy((ubyte*)(edit) + offset, data->mData, data->mSize);
		offset += data->mSize;
	}
	out->mBuffer.commit(edit);

	return out;
}

void RenderEngine::release(ShaderDataPusher* i_shaderData) {
	mFreeShaderDataPushers.insert(i_shaderData);
}

void RenderEngine::activate(ShaderDataPusher* i_shaderData, shader_data_slot_t i_slot) {
	auto it = mActiveShaderData.find(i_slot);
	if (it == mActiveShaderData.end()) {
		mActiveShaderData.insert(std::make_pair(i_slot, i_shaderData));
		i_shaderData->_activate(i_slot);
	}
	else if (i_shaderData == nullptr) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i_slot, 0);
		mActiveShaderData.erase(it);
	}
	else {
		it->second = i_shaderData;
		i_shaderData->_activate(i_slot);
	}
}

void RenderEngine::createBatches(const RenderPass& i_pass, std::list<RenderRequestBatch>& o_batches) {
	for (RenderRequest* request : mRequests) {
		if (i_pass.mFilter(*request)) {
			RenderRequestBatch batch;
			batch.mVertices = request->mVertices->mParent;
			VertexPusher::RenderCommand command;
			request->mVertices->translate(*request->mCommand, command);
			batch.mCommands.push_back(command);
			batch.mShader = consolidate(request->mShaders);
			o_batches.push_back(batch);
		}
	}
}

void RenderEngine::destroyBatches(std::list<RenderRequestBatch>& i_batches) {
	for (RenderRequestBatch& batch : i_batches) {
		release(batch.mShader);
		for (auto shaderDataBinding : batch.mShaderData) {
			release(shaderDataBinding.second);
		}
	}
}


/*
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
*/

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
