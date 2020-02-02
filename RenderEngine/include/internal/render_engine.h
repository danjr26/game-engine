#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "shader_program.h"
#include "render_pass.h"
#include "render_request.h"
#include "display_manager.h"
#include "window.h"
#include "clear_command.h"

void GLAPIENTRY defaultErrorCallback(
	GLenum i_source,
	GLenum i_type,
	GLuint i_id,
	GLenum i_severity,
	GLsizei i_length,
	const GLchar* i_message,
	const void* i_param
);

class RenderEngine {
public:
	using passes_node_data_t = RenderPass*;
	struct passes_edge_data_t {};
	using passes_graph_t = Graph<passes_node_data_t, passes_edge_data_t>;

private:
	struct CachedShader {
		std::vector<Shader*> mShaderParts;
		ShaderProgram mShader;
	};

private:
	RenderTarget* mActiveRenderTarget;
	ShaderProgram* mActiveShader;
	std::unordered_map<shader_data_slot_t, ShaderDataPusher*> mActiveShaderData;
	std::unordered_map<texture_slot_t, Texture*> mActiveTextures;

	std::list<CachedShader> mCachedShaders;
	std::unordered_set<ShaderDataPusher*> mFreeShaderDataPushers;
	std::vector<VertexPusher*> mVertexPushers;

public:
	DisplayManager mDisplayManager;
	Window mWindow;
	passes_graph_t mPasses;
	std::vector<RenderRequest*> mRequests;

public:
	RenderEngine();

	void render();

	void requestVertexPusher(RenderRequest& io_request, const VertexPusherLayout& i_layout, uint i_nIndices, uint i_nVertices);
	void reportErrors(GLDEBUGPROC i_callback = defaultErrorCallback, const void* i_param = nullptr);

private:
	void activate(RenderPass& i_pass);
	void activate(RenderRequestBatch& i_batch);

	void activate(RenderTarget* i_target);
	ShaderProgram* consolidate(const std::vector<Shader*>& i_shaderParts);
	void release(ShaderProgram* i_shader);
	void activate(ShaderProgram* i_shader);
	ShaderDataPusher* consolidate(const std::vector<ShaderData*>& i_shaderData);
	void release(ShaderDataPusher* i_shaderData);
	void activate(ShaderDataPusher* i_shaderData, shader_data_slot_t i_slot);

	void createBatches(const RenderPass& i_pass, std::list<RenderRequestBatch>& o_batches);
	void destroyBatches(std::list<RenderRequestBatch>& i_batches);

	void execute(const ClearCommand& i_command);
};

#endif