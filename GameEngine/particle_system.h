#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "renderable_object.h"
#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"

class ParticleSystem2 : public RenderableObject, public PerFrameUpdateableObject {
public:
	enum MemberID : ubyte {
		position = MeshVertexData::MemberID::position,
		color = MeshVertexData::MemberID::color,
		uv1 = MeshVertexData::MemberID::uv,
		uv2 = MeshVertexData::MemberID::extra,
		angle,
		age
	};

protected:
	MeshVertexData vertexData;
	MeshVertexGPUPusher gpuPusher;

public:
	ParticleSystem2();
	~ParticleSystem2();



	void Update(double in_dt) override;
	double Z() const override;
	bool Should_Cull() const override;
	void Render() override;

protected:

};

/*

*/

#endif