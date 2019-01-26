#include "particle_system2.h"
#include "game_engine.h"

ParticleSystem2::ParticleSystem2(Texture* in_texture, Specifier* in_specifier) :
	vertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	gpuPusher(),
	textureInstance(in_texture),
	specifier(in_specifier) {
	
	MeshVertexData::DataType dataType = MeshVertexData::DataType::_float;

	vertexData.Add_Member(position, dataType, 3, nullptr);
	vertexData.Add_Member(color, dataType, 4, nullptr);
	vertexData.Add_Member(uv1, dataType, 2, nullptr);
	vertexData.Add_Member(uv2, dataType, 2, nullptr);
	vertexData.Add_Member(dimensions, dataType, 2, nullptr);
	vertexData.Add_Member(angle, dataType, 1, nullptr);
	vertexData.Add_Member(age, dataType, 1, nullptr);
	vertexData.Add_Member(linear_velocity, dataType, 3, nullptr);
	vertexData.Add_Member(angular_velocity, dataType, 1, nullptr);

	Vector3f position(200, 200, 0);
	Vector2f uv1(0.0, 0.0);
	Vector2f uv2(1.0, 1.0);
	ColorRGBAf color(0.2, 0.5, 0.8, 1.0);
	Vector2f dimensions(20, 20);
	float angle = PI / 4.0f;
	uint index = 0;

	std::unordered_map<ubyte, const void*> vertices;
	vertices[MemberID::position] = &position;
	vertices[MemberID::color] = &color;
	vertices[MemberID::uv1] = &uv1;
	vertices[MemberID::uv2] = &uv2;
	vertices[MemberID::dimensions] = &dimensions;
	vertices[MemberID::angle] = &angle;

	vertexData.Reserve_Total_Vertices(1);
	vertexData.Add_Vertices(1, vertices);

	MeshVertexGPUPusher::ExtraParams params;
	params.membersToIgnore = (1ul << age) | (1ul << linear_velocity) | (1ul << angular_velocity);
	gpuPusher.Initialize(&vertexData, params);

	textureInstance.Settings().Set_Magnify_Filter(TextureSettings::FilterMode::trilinear);
	textureInstance.Settings().Set_Minify_Filter(TextureSettings::FilterMode::trilinear);
}

ParticleSystem2::~ParticleSystem2()
{}

uint ParticleSystem2::Add(uint in_nParticles) {
	uint out = vertexData.Get_Number_Vertices();
	if (in_nParticles > 0) {
		vertexData.Add_Vertices(in_nParticles, {});
	}
	return out;
}

void ParticleSystem2::Remove(uint in_index) {
	vertexData.Swap_Vertices(in_index, vertexData.Get_Number_Vertices() - 1);
	vertexData.Remove_Vertex(vertexData.Get_Number_Vertices() - 1);
}

void ParticleSystem2::Access(uint in_index, Accessor& out_accessor) {
	out_accessor.position = ((position_t*)vertexData.Get_Member_Pointer(position)) + in_index;
	out_accessor.color = ((color_t*)vertexData.Get_Member_Pointer(color)) + in_index;
	out_accessor.uv1 = ((uv1_t*)vertexData.Get_Member_Pointer(uv1)) + in_index;
	out_accessor.uv2 = ((uv2_t*)vertexData.Get_Member_Pointer(uv2)) + in_index;
	out_accessor.dimensions = ((dimensions_t*)vertexData.Get_Member_Pointer(dimensions)) + in_index;
	out_accessor.angle = ((angle_t*)vertexData.Get_Member_Pointer(angle)) + in_index;
	out_accessor.age = ((age_t*)vertexData.Get_Member_Pointer(age)) + in_index;
	out_accessor.linearVelocity = ((linear_velocity_t*)vertexData.Get_Member_Pointer(linear_velocity)) + in_index;
	out_accessor.angularVelocity = ((angular_velocity_t*)vertexData.Get_Member_Pointer(angular_velocity)) + in_index;
}

void ParticleSystem2::Update(double in_dt) {
	if (specifier != nullptr) {
		Accessor accessor;
		Access(0, accessor);

		specifier->Update(*this, accessor, in_dt);
		specifier->Destroy(*this, accessor, in_dt);
		specifier->Generate(*this, accessor, in_dt);
	}
}

double ParticleSystem2::Z() const {
	return 0.0;
}

bool ParticleSystem2::Should_Cull() const {
	return false;
}

void ParticleSystem2::Render() {
	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("Particle2Shader");

	Matrix4f modelMatrix = Matrix4f::Identity();
	Matrix4f viewMatrix = GE.Cameras().active->Get_View_Matrix();
	Matrix4f projectionMatrix = GE.Cameras().active->Get_Projection_Matrix();

	GLint locations[4] = {
		shaderProgram->Get_Uniform_Location("modelMatrix"),
		shaderProgram->Get_Uniform_Location("viewMatrix"),
		shaderProgram->Get_Uniform_Location("projectionMatrix"),
		shaderProgram->Get_Uniform_Location("colorTexture")
	};

	if (textureInstance.Get_Texture() != nullptr) {
		textureInstance.Use();
	}
	shaderProgram->Use();
	glDepthMask	(0);

	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.Pointer());
	glUniform1i(locations[3], 0);

	gpuPusher.Push_Face_Count();
	gpuPusher.Push_Faces();
	gpuPusher.Push_Vertex_Count();
	gpuPusher.Push_Vertices();
	gpuPusher.Draw_Raw();

	if (textureInstance.Get_Texture() != nullptr) {
		textureInstance.Use_None();
	}
	shaderProgram->Use_None();
	glDepthMask	(1);
}

