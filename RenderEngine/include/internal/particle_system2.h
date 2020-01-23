#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "renderable_object.h"
#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "texture_instance.h"
#include "color.h"
#include "destructable_object.h"
#include "deep_transformable_object.h"
#include "blend_settings.h"

class ParticleSystem2 : 
	public Renderable, 
	public PerFrameUpdateableObject, 
	public DestructableObject, 
	public DeepTransformableObject2d {

public:
	using position_t = Vector3f;
	using color_t = Color4f;
	using uv1_t = Vector2f;
	using uv2_t = Vector2f;
	using dimensions_t = Vector2f;
	using angle_t = float;
	using age_t = float;
	using linear_velocity_t = Vector3f;
	using angular_velocity_t = float;

	struct Accessor {
		position_t* mPosition;
		color_t* mColor;
		uv1_t* mUV1;
		uv2_t* mUV2;
		dimensions_t* mDimensions;
		angle_t* mAngle;
		age_t* mAge;
		linear_velocity_t* mLinearVelocity;
		angular_velocity_t* mAngularVelocity;
		std::unordered_map<ubyte, void*> mExtra;

		void clear(uint i_index);
	};

	class Specifier {
	public:
		virtual ~Specifier() {};
		virtual void init(ParticleSystem2& i_system, Accessor& i_accessor) = 0;
		virtual void update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) = 0;
		virtual void destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) = 0;
		virtual void generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) = 0;
	};

	enum SortOrder {
		lowest_first,
		highest_first

	};

	enum StateFlags : uint {
		suppress_init = 1,
		suppress_update = suppress_init << 1,
		suppress_generate = suppress_update << 1,
		suppress_destroy = suppress_generate << 1,
		delete_when_empty = suppress_destroy << 1,
		planned_deletion = suppress_generate | delete_when_empty
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
	MeshVertexData mVertexData;
	MeshVertexGPUPusher mGPUPusher;
	TextureInstance mTextureInstance;
	Specifier* mSpecifier;
	BlendSettings mBlendSettings;
	uint mStateFlags;

public:
	ParticleSystem2(Texture* i_texture, Specifier* i_specifier, uint i_stateFlags = 0);
	~ParticleSystem2();

	uint getCount() const;

	void planDeletion();
	Specifier* getSpecifier();

	BlendSettings getBlendSettings() const;
	void setBlendSettings(const BlendSettings& i_blendSettings);

	uint add(uint i_nParticles);
	void remove(uint i_index);
	void access(uint i_index, Accessor& o_accessor);

	void reserve(uint i_nParticles);

	void sort(SortOrder i_order);

	void update(double i_dt) override;
	double z() const override;
	bool shouldCull() const override;
	void render() override;

	static void transformParticles(Accessor& i_accessor, uint i_nParticles,
		Transform2d& i_transform, DepthTransform2d& i_depthTransform);
};

#endif