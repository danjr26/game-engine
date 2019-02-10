#include "particle_system2.h"
#include "game_engine.h"

ParticleSystem2::ParticleSystem2(Texture* in_texture, Specifier* in_specifier) :
	mVertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	mGPUPusher(),
	mTextureInstance(in_texture),
	mSpecifier(in_specifier) {
	
	MeshVertexData::DataType dataType = MeshVertexData::DataType::_float;

	mVertexData.Add_Member(position, dataType, 3, nullptr);
	mVertexData.Add_Member(color, dataType, 4, nullptr);
	mVertexData.Add_Member(uv1, dataType, 2, nullptr);
	mVertexData.Add_Member(uv2, dataType, 2, nullptr);
	mVertexData.Add_Member(dimensions, dataType, 2, nullptr);
	mVertexData.Add_Member(angle, dataType, 1, nullptr);
	mVertexData.Add_Member(age, dataType, 1, nullptr);
	mVertexData.Add_Member(linear_velocity, dataType, 3, nullptr);
	mVertexData.Add_Member(angular_velocity, dataType, 1, nullptr);

	MeshVertexGPUPusher::ExtraParams params;
	params.mMembersToIgnore = (1ul << age) | (1ul << linear_velocity) | (1ul << angular_velocity);
	params.mUseCase = MeshVertexGPUPusher::UseCase::changes_often;
	mGPUPusher.Initialize(&mVertexData, params);

	mTextureInstance.Settings().Set_Magnify_Filter(TextureSettings::FilterMode::trilinear);
	mTextureInstance.Settings().Set_Minify_Filter(TextureSettings::FilterMode::trilinear);
}

ParticleSystem2::~ParticleSystem2()
{}

uint ParticleSystem2::Count() const {
	return mVertexData.Get_Number_Vertices();
}

uint ParticleSystem2::Add(uint in_nParticles) {
	uint out = mVertexData.Get_Number_Vertices();
	if (in_nParticles > 0) {
		mVertexData.Add_Vertices(in_nParticles, {});
	}
	return out;
}

void ParticleSystem2::Remove(uint in_index) {
	mVertexData.Swap_Vertices(in_index, mVertexData.Get_Number_Vertices() - 1);
	mVertexData.Remove_Vertex(mVertexData.Get_Number_Vertices() - 1);
}

void ParticleSystem2::Access(uint in_index, Accessor& out_accessor) {
	out_accessor.mPosition = ((position_t*)mVertexData.Get_Member_Pointer(position)) + in_index;
	out_accessor.mColor = ((color_t*)mVertexData.Get_Member_Pointer(color)) + in_index;
	out_accessor.mUV1 = ((uv1_t*)mVertexData.Get_Member_Pointer(uv1)) + in_index;
	out_accessor.mUV2 = ((uv2_t*)mVertexData.Get_Member_Pointer(uv2)) + in_index;
	out_accessor.mDimensions = ((dimensions_t*)mVertexData.Get_Member_Pointer(dimensions)) + in_index;
	out_accessor.mAngle = ((angle_t*)mVertexData.Get_Member_Pointer(angle)) + in_index;
	out_accessor.mAge = ((age_t*)mVertexData.Get_Member_Pointer(age)) + in_index;
	out_accessor.mLinearVelocity = ((linear_velocity_t*)mVertexData.Get_Member_Pointer(linear_velocity)) + in_index;
	out_accessor.mAngularVelocity = ((angular_velocity_t*)mVertexData.Get_Member_Pointer(angular_velocity)) + in_index;
}

void ParticleSystem2::Reserve(uint in_nParticles) {
	mGPUPusher.Reserve_Total_Vertices(in_nParticles);
}

void ParticleSystem2::Update(double in_dt) {
	if (mSpecifier != nullptr) {
		Accessor accessor;
		Access(0, accessor);

		mSpecifier->Update(*this, accessor, in_dt);
		mSpecifier->Destroy(*this, accessor, in_dt);
		mSpecifier->Generate(*this, accessor, in_dt);
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
	Matrix4f viewMatrix = GE.Cameras().mActive->Get_View_Matrix();
	Matrix4f projectionMatrix = GE.Cameras().mActive->Get_Projection_Matrix();

	GLint locations[4] = {
		shaderProgram->Get_Uniform_Location("modelMatrix"),
		shaderProgram->Get_Uniform_Location("viewMatrix"),
		shaderProgram->Get_Uniform_Location("projectionMatrix"),
		shaderProgram->Get_Uniform_Location("colorTexture")
	};

	if (mTextureInstance.Get_Texture() != nullptr) {
		mTextureInstance.Use();
	}
	shaderProgram->Use();
	glDepthMask(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.Pointer());
	glUniform1i(locations[3], 0);

	mGPUPusher.Push_Face_Count();
	mGPUPusher.Push_Faces();
	mGPUPusher.Push_Vertex_Count();
	mGPUPusher.Push_Vertices();
	mGPUPusher.Draw_Raw();

	if (mTextureInstance.Get_Texture() != nullptr) {
		mTextureInstance.Use_None();
	}
	shaderProgram->Use_None();
	glDepthMask(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

