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
	std::unordered_map<texture_slot_t, Texture*> mActiveTextures;
	std::list<CachedShader> mCachedShaders;
	std::list<VertexPusher> mVertexPushers;

public:
	DisplayManager mDisplayManager;
	Window mWindow;
	passes_graph_t mPasses;
	std::vector<RenderRequest*> mRequests;

public:
	RenderEngine();

	void render();

	void requestVertexPusher(RenderRequest& io_request, const VertexPusherLayout& i_layout, size_t i_nIndices, size_t i_nVertices);
	void reportErrors(GLDEBUGPROC i_callback = defaultErrorCallback, const void* i_param = nullptr);

private:
	void retrieveRequests(const RenderPass& i_pass, std::vector<RenderRequest*>& o_requests) const;
	void activate(RenderTarget* i_target);
	ShaderProgram* retrieveShader(const std::vector<Shader*>& i_shaderParts);
	void activate(ShaderProgram* i_shader);
	void activate(RenderPass& i_pass);
	void bind(texture_slot_t i_slot, Texture* i_texture);
	void execute(const ClearCommand& i_command);
};

#endif