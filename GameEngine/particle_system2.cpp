#include "particle_system2.h"
#include "game_engine.h"

ParticleSystem2::ParticleSystem2(Texture* in_texture) :
	vertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	gpuPusher(),
	textureInstance(in_texture) {
	
	vertexData.Add_Member(MemberID::position, MeshVertexData::DataType::_float, 3, nullptr);
	vertexData.Add_Member(MemberID::color, MeshVertexData::DataType::_float, 4, nullptr);
	vertexData.Add_Member(MemberID::uv1, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::uv2, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::dimensions, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::angle, MeshVertexData::DataType::_float, 1, nullptr);
	vertexData.Add_Member(MemberID::age, MeshVertexData::DataType::_float, 1, nullptr);

	Vector3f position(200, 200, 0);
	Vector2f uv1(0.0, 0.0);
	Vector2f uv2(1.0, 1.0);
	ColorRGBAf color(0.5, 0.5, 0.8, 0.8);
	Vector2f dimensions(100, 100);
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

	vertexData.Reserve_Total_Face_Elements(1);
	vertexData.Add_Face_Elements(1, &index);

	MeshVertexGPUPusher::ExtraParams params;
	params.membersToIgnore = (1ul << MemberID::age);
	gpuPusher.Initialize(&vertexData, params);
}

ParticleSystem2::~ParticleSystem2()
{}


void ParticleSystem2::Update(double in_dt) {}

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

	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.Pointer());
	glUniform1i(locations[3], 0);

	gpuPusher.Draw();

	if (textureInstance.Get_Texture() != nullptr) {
		textureInstance.Use_None();
	}
	shaderProgram->Use_None();
}

