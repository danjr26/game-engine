#ifndef RENDER_REQUEST_H
#define RENDER_REQUEST_H

#include "shader.h"
#include "texture.h"
#include "uniform_pusher.h"
#include "virtual_vertex_pusher.h"
#include "camera.h"
#include "shader_data_pusher.h"
#include <unordered_set>
#include <unordered_map>

struct RenderRequest {
	std::vector<Shader*> mShaders;
	VirtualVertexPusher* mVertices;
	VirtualVertexPusher::RenderCommand* mCommand;
	std::unordered_map<shader_data_slot_t, ShaderData*> mShaderData;

	uint64_t mPassFlags;
	std::function<double(const Camera*)> mZ;

	RenderRequest() :
	mPassFlags(0ull),
	mVertices(nullptr),
	mCommand(nullptr),
	mZ(defaultZ)
	{}

	static double defaultZ(const Camera* c) {
		return 0.0; 
	}
};

struct RenderRequestBatch {
	ShaderProgram* mShader;
	std::unordered_map<shader_data_slot_t, ShaderDataPusher*> mShaderData;
	VertexPusher* mVertices;
	std::vector<VertexPusher::RenderCommand> mCommands;
};

#endif