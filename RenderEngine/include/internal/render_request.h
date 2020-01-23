#ifndef RENDER_REQUEST_H
#define RENDER_REQUEST_H

#include "shader.h"
#include "texture.h"
#include "uniform_pusher.h"
#include "virtual_vertex_pusher.h"
#include "camera.h"
#include <unordered_set>
#include <unordered_map>

struct RenderRequest {
	uint64_t mPassFlags;
	std::unordered_set<Shader*> mShaders;
	std::unordered_map<texture_slot_t, Texture*> mTextures; 
	std::unordered_map<uniform_slot_t, UniformPusher*> mUniforms;
	VirtualVertexPusher* mVertices;
	VirtualVertexPusher::RenderCommand* mCommand;
	std::function<double(const Camera*)> mZ;

	RenderRequest() :
	mPassFlags(0ull),
	mShaders(),
	mTextures(),
	mUniforms(),
	mVertices(nullptr),
	mCommand(nullptr),
	mZ(defaultZ)
	{}

	static double defaultZ(const Camera* c) {
		return 0.0; 
	}
};

#endif