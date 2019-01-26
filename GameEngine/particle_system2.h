#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "renderable_object.h"
#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "texture_instance.h"
#include "color.h"

class ParticleSystem2 : public RenderableObject, public PerFrameUpdateableObject {
public:
	using position_t = Vector3f;
	using color_t = ColorRGBAf;
	using uv1_t = Vector2f;
	using uv2_t = Vector2f;
	using dimensions_t = Vector2f;
	using angle_t = float;
	using age_t = float;
	using linear_velocity_t = Vector3f;
	using angular_velocity_t = float;

	struct Accessor {
		position_t* position;
		color_t* color;
		uv1_t* uv1;
		uv2_t* uv2;
		dimensions_t* dimensions;
		angle_t* angle;
		age_t* age;
		linear_velocity_t* linearVelocity;
		angular_velocity_t* angularVelocity;
		std::unordered_map<ubyte, void*> extra;
	};

	class Specifier {
	public:
		virtual void Update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) = 0;
		virtual void Destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) = 0;
		virtual void Generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) = 0;
	};

	enum SortOrder {
		lowest_first,
		highest_first

	};
	enum MemberID : ubyte {
		position = MeshVertexData::MemberID::position,
		color = MeshVertexData::MemberID::color,
		uv1 = MeshVertexData::MemberID::uv,
		uv2 = MeshVertexData::MemberID::extra,
		dimensions,
		angle,
		age,
		linear_velocity,
		angular_velocity
	};

protected:
	MeshVertexData vertexData;
	MeshVertexGPUPusher gpuPusher;
	TextureInstance textureInstance;
	Specifier* specifier;

public:
	ParticleSystem2(Texture* in_texture, Specifier* in_specifier);
	~ParticleSystem2();

	uint Add(uint in_nParticles);
	void Remove(uint in_index);
	void Access(uint in_index, Accessor& out_accessor);

	void Sort(SortOrder in_order);

	void Update(double in_dt) override;
	double Z() const override;
	bool Should_Cull() const override;
	void Render() override;

protected:

};

#endif