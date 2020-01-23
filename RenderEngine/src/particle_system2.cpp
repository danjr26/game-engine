#include "particle_system2.h"
#include "game_engine.h"

ParticleSystem2::ParticleSystem2(Texture* i_texture, Specifier* i_specifier, uint i_stateFlags) :
	mVertexData(MeshVertexData::DataType::_uint, MeshVertexData::FaceMode::points),
	mGPUPusher(),
	mTextureInstance(i_texture),
	mSpecifier(i_specifier),
	mStateFlags(i_stateFlags) {

	if (i_texture == nullptr) throw InvalidArgumentException();
	
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

	if ((mSpecifier != nullptr) && !(mStateFlags & StateFlags::suppress_init)) {
		Accessor accessor;
		access(0, accessor);
		mSpecifier->init(*this, accessor);
	}

	mTextureInstance.getSettings().setMagnifyFilter(TextureSettings::FilterMode::trilinear);
	mTextureInstance.getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);
}

ParticleSystem2::~ParticleSystem2() {
	if (mSpecifier != nullptr) delete mSpecifier;

	if (mStateFlags & StateFlags::delete_when_empty) {
		getTransform().deleteChainParents();
		getDepthTransform().deleteChainParents();
	}

	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.render().remove(this);
	}
}

uint ParticleSystem2::getCount() const {
	return mVertexData.getNumberVertices();
}

void ParticleSystem2::planDeletion() {
	mStateFlags |= StateFlags::planned_deletion;
	subTransform(getTransform().cloneChain());
	subDepthTransform(getDepthTransform().cloneChain());
}

ParticleSystem2::Specifier* ParticleSystem2::getSpecifier() {
	return mSpecifier;
}

BlendSettings ParticleSystem2::getBlendSettings() const {
	return mBlendSettings;
}

void ParticleSystem2::setBlendSettings(const BlendSettings& i_blendSettings) {
	mBlendSettings = i_blendSettings;
}

uint ParticleSystem2::add(uint i_nParticles) {
	uint out = mVertexData.getNumberVertices();
	if (i_nParticles > 0) {
		mVertexData.addVertices(i_nParticles, {});
	}
	return out;
}

void ParticleSystem2::remove(uint i_index) {
	mVertexData.swapVertices(i_index, mVertexData.getNumberVertices() - 1);
	mVertexData.removeVertex(mVertexData.getNumberVertices() - 1);
}

void ParticleSystem2::access(uint i_index, Accessor& o_accessor) {
	o_accessor.mPosition = ((position_t*)mVertexData.getMemberPointer(position)) + i_index;
	o_accessor.mColor = ((color_t*)mVertexData.getMemberPointer(color)) + i_index;
	o_accessor.mUV1 = ((uv1_t*)mVertexData.getMemberPointer(uv1)) + i_index;
	o_accessor.mUV2 = ((uv2_t*)mVertexData.getMemberPointer(uv2)) + i_index;
	o_accessor.mDimensions = ((dimensions_t*)mVertexData.getMemberPointer(dimensions)) + i_index;
	o_accessor.mAngle = ((angle_t*)mVertexData.getMemberPointer(angle)) + i_index;
	o_accessor.mAge = ((age_t*)mVertexData.getMemberPointer(age)) + i_index;
	o_accessor.mLinearVelocity = ((linear_velocity_t*)mVertexData.getMemberPointer(linear_velocity)) + i_index;
	o_accessor.mAngularVelocity = ((angular_velocity_t*)mVertexData.getMemberPointer(angular_velocity)) + i_index;
}

void ParticleSystem2::reserve(uint i_nParticles) {
	mGPUPusher.reserveTotalVertices(i_nParticles);
}

void ParticleSystem2::update(double i_dt) {
	if ((mStateFlags & StateFlags::delete_when_empty) && getCount() == 0) {
		GE.destruction().add(this);
		return;
	}

	if (mSpecifier != nullptr) {
		Accessor accessor;
		access(0, accessor);

		if (!(mStateFlags & StateFlags::suppress_update)) mSpecifier->update(*this, accessor, i_dt);
		if (!(mStateFlags & StateFlags::suppress_destroy)) mSpecifier->destroy(*this, accessor, i_dt);
		if (!(mStateFlags & StateFlags::suppress_generate)) mSpecifier->generate(*this, accessor, i_dt);
	}
}

double ParticleSystem2::z() const {
	return getDepthTransform().getWorldDepth();
}

bool ParticleSystem2::shouldCull() const {
	return false;
}

void ParticleSystem2::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("Particle2Shader");

	Matrix4f modelMatrix = Matrix4f::identity(); //getDeepWorldMatrix();
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
	mBlendSettings.use();
	glDepthMask(0);

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
	BlendSettings::standard().use();
	glDepthMask(1);

}

void ParticleSystem2::transformParticles(Accessor& i_accessor, uint i_nParticles, Transform2d& i_transform, DepthTransform2d& i_depthTransform) {
	for (uint i = 0; i < i_nParticles; i++) {
		i_accessor.mPosition[i] = Vector3d(
			i_transform.applyChainToLocalPoint(Vector2f(i_accessor.mPosition[i])),
			i_accessor.mPosition[i].z() + i_depthTransform.getWorldDepth()
		);
		i_accessor.mLinearVelocity[i] = Vector3d(
			i_transform.applyChainToLocalVector(Vector2f(i_accessor.mLinearVelocity[i])), 
			(double)i_accessor.mLinearVelocity[i].z()
		);
		i_accessor.mAngle[i] = i_transform.applyChainToLocalRotation(Rotation2d(i_accessor.mAngle[i])).getAngle();
	}
}

void ParticleSystem2::Accessor::clear(uint i_index) {
	mPosition[i_index] = position_t();
	mColor[i_index] = color_t();
	mUV1[i_index] = uv1_t();
	mUV2[i_index] = uv2_t();
	mDimensions[i_index] = dimensions_t();
	mAngle[i_index] = angle_t(0);
	mAge[i_index] = age_t(0);
	mLinearVelocity[i_index] = linear_velocity_t();
	mAngularVelocity[i_index] = angular_velocity_t(0);
}
