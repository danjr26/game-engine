#include "particle_system2.h"
#include "game_engine.h"

ParticleSystem2::ParticleSystem2(Texture* in_texture, Specifier* in_specifier) :
	mVertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	mGPUPusher(),
	mTextureInstance(in_texture),
	mSpecifier(in_specifier) {
	
	MeshVertexData::DataType dataType = MeshVertexData::DataType::_float;

	mVertexData.addMember(position, dataType, 3, nullptr);
	mVertexData.addMember(color, dataType, 4, nullptr);
	mVertexData.addMember(uv1, dataType, 2, nullptr);
	mVertexData.addMember(uv2, dataType, 2, nullptr);
	mVertexData.addMember(dimensions, dataType, 2, nullptr);
	mVertexData.addMember(angle, dataType, 1, nullptr);
	mVertexData.addMember(age, dataType, 1, nullptr);
	mVertexData.addMember(linear_velocity, dataType, 3, nullptr);
	mVertexData.addMember(angular_velocity, dataType, 1, nullptr);

	MeshVertexGPUPusher::ExtraParams params;
	params.mMembersToIgnore = (1ul << age) | (1ul << linear_velocity) | (1ul << angular_velocity);
	params.mUseCase = MeshVertexGPUPusher::UseCase::changes_often;
	mGPUPusher.initialize(&mVertexData, params);

	if (mSpecifier != nullptr) {
		Accessor accessor;
		access(0, accessor);
		mSpecifier->init(*this, accessor);
	}

	mTextureInstance.getSettings().setMagnifyFilter(TextureSettings::FilterMode::trilinear);
	mTextureInstance.getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);
}

ParticleSystem2::~ParticleSystem2() {
	if (mSpecifier != nullptr) delete mSpecifier;
	GE.perFrameUpdate().remove(this);
	GE.render().remove(this);
}

uint ParticleSystem2::getCount() const {
	return mVertexData.getNumberVertices();
}

uint ParticleSystem2::add(uint in_nParticles) {
	uint out = mVertexData.getNumberVertices();
	if (in_nParticles > 0) {
		mVertexData.addVertices(in_nParticles, {});
	}
	return out;
}

void ParticleSystem2::remove(uint in_index) {
	mVertexData.swapVertices(in_index, mVertexData.getNumberVertices() - 1);
	mVertexData.removeVertex(mVertexData.getNumberVertices() - 1);
}

void ParticleSystem2::access(uint in_index, Accessor& out_accessor) {
	out_accessor.mPosition = ((position_t*)mVertexData.getMemberPointer(position)) + in_index;
	out_accessor.mColor = ((color_t*)mVertexData.getMemberPointer(color)) + in_index;
	out_accessor.mUV1 = ((uv1_t*)mVertexData.getMemberPointer(uv1)) + in_index;
	out_accessor.mUV2 = ((uv2_t*)mVertexData.getMemberPointer(uv2)) + in_index;
	out_accessor.mDimensions = ((dimensions_t*)mVertexData.getMemberPointer(dimensions)) + in_index;
	out_accessor.mAngle = ((angle_t*)mVertexData.getMemberPointer(angle)) + in_index;
	out_accessor.mAge = ((age_t*)mVertexData.getMemberPointer(age)) + in_index;
	out_accessor.mLinearVelocity = ((linear_velocity_t*)mVertexData.getMemberPointer(linear_velocity)) + in_index;
	out_accessor.mAngularVelocity = ((angular_velocity_t*)mVertexData.getMemberPointer(angular_velocity)) + in_index;
}

void ParticleSystem2::reserve(uint in_nParticles) {
	mGPUPusher.reserveTotalVertices(in_nParticles);
}

void ParticleSystem2::update(double in_dt) {
	if (mSpecifier != nullptr) {
		Accessor accessor;
		access(0, accessor);

		mSpecifier->update(*this, accessor, in_dt);
		mSpecifier->destroy(*this, accessor, in_dt);
		mSpecifier->generate(*this, accessor, in_dt);
	}
}

double ParticleSystem2::z() const {
	return 0.0;
}

bool ParticleSystem2::shouldCull() const {
	return false;
}

void ParticleSystem2::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("Particle2Shader");

	Matrix4f modelMatrix = mTransform.getWorldMatrix();
	Matrix4f viewMatrix = GE.cameras().getActive()->getViewMatrix();
	Matrix4f projectionMatrix = GE.cameras().getActive()->getProjectionMatrix();

	GLint locations[4] = {
		shaderProgram->getUniformLocation("modelMatrix"),
		shaderProgram->getUniformLocation("viewMatrix"),
		shaderProgram->getUniformLocation("projectionMatrix"),
		shaderProgram->getUniformLocation("colorTexture")
	};

	if (mTextureInstance.getTexture() != nullptr) {
		mTextureInstance.use();
	}
	shaderProgram->use();
	glDepthMask(0);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.pointer());
	glUniform1i(locations[3], 0);

	mGPUPusher.pushFaceCount();
	mGPUPusher.pushFaces();
	mGPUPusher.pushVertexCount();
	mGPUPusher.pushVertices();

	mGPUPusher.drawRaw();

	if (mTextureInstance.getTexture() != nullptr) {
		mTextureInstance.useNone();
	}
	shaderProgram->useNone();
	glDepthMask(1);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

