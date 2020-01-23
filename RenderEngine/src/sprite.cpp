#include "../include/internal/sprite.h"
#include "RenderTarget_units.h"

Sprite::Sprite(const AABox2d& i_rectangle, Texture* i_texture, const Color4f& i_color) :
	mMeshVertexData(MeshVertexData::DataType::_ushort), 
	mGPUPusher(),
	mColor(i_color),
	mTextureInstance(i_texture) {

	setRectangle(i_rectangle);
	mInnerTransform.setParent(&getTransform());

	Vector3f positions[] = {
		Vector3f(0.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(1.0f, 1.0f, 0.0f),
		Vector3f(1.0f, 0.0f, 0.0f)
	};

	Vector2f uvs[] = {
		Vector2f(0.0f, 0.0f),
		Vector2f(0.0f, 1.0f),
		Vector2f(1.0f, 1.0f),
		Vector2f(1.0f, 0.0f)
	};

	ushort indices[] = {
		0, 3, 1,
		2, 1, 3
	};

	mMeshVertexData.addVertices(4, {});
	mMeshVertexData.addMember(MeshVertexData::MemberID::position, MeshVertexData::DataType::_float, 3, positions);
	mMeshVertexData.addMember(MeshVertexData::MemberID::color, MeshVertexData::DataType::_float, 4, nullptr);
	mMeshVertexData.addMember(MeshVertexData::MemberID::uv, MeshVertexData::DataType::_float, 2, uvs);
	mMeshVertexData.addFaces(2, indices);

	mGPUPusher.initialize(&mMeshVertexData);
	setColor(i_color);
}

Sprite::~Sprite() 
{}

void Sprite::setRectangle(const AABox2d& i_rectangle) {
	mInnerTransform.setLocalPosition(i_rectangle.getMinima());
	mInnerTransform.setLocalScale(i_rectangle.getDimensions());
}

void Sprite::setTexture(Texture* i_texture) {
	mTextureInstance.setTexture(i_texture);
}

void Sprite::setUVs(const Vector2f& i_topLeft, const Vector2f& i_bottomRight) {
	Vector2f uvs[] = {
		i_topLeft,
		Vector2f(i_topLeft.x(), i_bottomRight.y()),
		i_bottomRight,
		Vector2f(i_bottomRight.x(), i_topLeft.y())
	};

	mMeshVertexData.setMemberValues(MeshVertexData::MemberID::uv, 0, 4, uvs);
	mGPUPusher.pushMember(MeshVertexData::MemberID::uv);
}

void Sprite::setColor(const Color4f& i_color) {
	mColor = i_color;
	mGPUPusher.setMemberDefault(MeshVertexData::MemberID::color, i_color.pointer());
}

TextureInstance& Sprite::getTextureInstance() {
	return mTextureInstance;
}

double Sprite::z() const {
	return mDepthTransform.getWorldDepth();
}

bool Sprite::shouldCull() const {
	return false;
}

void Sprite::render() {
	ShaderProgram* shaderProgram;

	Matrix4f modelMatrix = mDepthTransform.getWorldMatrix() * mInnerTransform.getWorldMatrix();
	Matrix4f viewMatrix = GE.cameras().getActive()->getViewMatrix();
	Matrix4f projectionMatrix = GE.cameras().getActive()->getProjectionMatrix();

	if (mTextureInstance.getTexture() == nullptr) {
		shaderProgram = GE.assets().get<ShaderProgram>("MonoShader");

		GLint locations[2] = {
			shaderProgram->getUniformLocation("mvpMatrix"),
			shaderProgram->getUniformLocation("color")
		};

		Color4f tintFloat = mColor;

		Matrix4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		shaderProgram->use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.pointer());
		glUniform4fv(locations[1], 1, tintFloat.pointer());
	}
	else {
		shaderProgram = GE.assets().get<ShaderProgram>("DeferredShader");

		GLint locations[4] = {
			shaderProgram->getUniformLocation("modelMatrix"),
			shaderProgram->getUniformLocation("viewMatrix"),
			shaderProgram->getUniformLocation("projectionMatrix"),
			shaderProgram->getUniformLocation("color")
		};

		mTextureInstance.use();

		Color4f tintFloat = mColor;

		shaderProgram->use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.pointer());
		glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.pointer());
		glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.pointer());
		glUniform4fv(locations[3], 1, tintFloat.pointer());
	}

	mGPUPusher.draw();
}
