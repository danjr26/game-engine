#include "particle_system.h"

]ParticleSystem2::ParticleSystem2() :
	vertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	gpuPusher() {
	
	vertexData.Add_Member(MemberID::position, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::color, MeshVertexData::DataType::_float, 4, nullptr);
	vertexData.Add_Member(MemberID::uv1, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::uv2, MeshVertexData::DataType::_float, 2, nullptr);
	vertexData.Add_Member(MemberID::angle, MeshVertexData::DataType::_float, 1, nullptr);
	vertexData.Add_Member(MemberID::age, MeshVertexData::DataType::_float, 1, nullptr);

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
	
}

