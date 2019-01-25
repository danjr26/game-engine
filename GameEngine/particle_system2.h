#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "renderable_object.h"
#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "texture_instance.h"

class ParticleSystem2 : public RenderableObject, public PerFrameUpdateableObject {
public:
	enum MemberID : ubyte {
		position = MeshVertexData::MemberID::position,
		color = MeshVertexData::MemberID::color,
		uv1 = MeshVertexData::MemberID::uv,
		uv2 = MeshVertexData::MemberID::extra,
		dimensions,
		angle,
		age
	};

protected:
	MeshVertexData vertexData;
	MeshVertexGPUPusher gpuPusher;
	TextureInstance textureInstance;

public:
	ParticleSystem2(Texture* in_texture);
	~ParticleSystem2();

	void Update(double in_dt) override;
	double Z() const override;
	bool Should_Cull() const override;
	void Render() override;

protected:

};

#endif