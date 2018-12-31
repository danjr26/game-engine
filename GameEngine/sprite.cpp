#include "sprite.h"
#include "display_units.h"
#include "game_engine.h"

Sprite::Sprite(const AxisAlignedRectangled& in_rectangle, Texture* in_texture, const ColorRGBAf& in_color) :
	meshVertexData(MeshVertexData::DataType::_ushort), 
	gpuPusher(),
	color(in_color),
	textureInstance(in_texture) {

	innerTransform.Set_Local_Position(in_rectangle.Get_Minima());
	innerTransform.Scale_Local(in_rectangle.Get_Dimensions());
	innerTransform.Set_Parent(&transform);

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

	meshVertexData.Add_Vertices(4, {});
	meshVertexData.Add_Member(MeshVertexData::MemberID::position, MeshVertexData::DataType::_float, 3, positions);
	meshVertexData.Add_Member(MeshVertexData::MemberID::uv, MeshVertexData::DataType::_float, 2, uvs);
	meshVertexData.Add_Faces(2, indices);

	gpuPusher.Initialize(&meshVertexData);
}

Sprite::~Sprite() 
{}

void Sprite::Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight) {
	Vector2f uvs[] = {
		in_topLeft,
		Vector2f(in_topLeft.X(), in_bottomRight.Y()),
		in_bottomRight,
		Vector2f(in_bottomRight.X(), in_topLeft.Y())
	};

	meshVertexData.Set_Member_Values(MeshVertexData::MemberID::uv, 0, 4, uvs);
	gpuPusher.Push_Member(MeshVertexData::MemberID::uv);
}

void Sprite::Set_Color(const ColorRGBAf& in_color) {
	color = in_color;
}

TextureInstance& Sprite::Texture_Instance() {
	return textureInstance;
}

double Sprite::Z() const {
	return depthTransform.Get_World_Depth();
}

bool Sprite::Should_Cull() const {
	return false;
}

void Sprite::Render() {
	Matrix<float, 4, 4> iden = Matrix<float, 4, 4>::Identity();

	ShaderProgram* shaderProgram;

	Matrix4f modelMatrix = innerTransform.Get_World_Matrix();
	Matrix4f viewMatrix = GE.Cameras().active->Get_View_Matrix();
	Matrix4f projectionMatrix = GE.Cameras().active->Get_Projection_Matrix();

	if (textureInstance.Get_Texture() == nullptr) {
		shaderProgram = GE.Assets().Get<ShaderProgram>("MonoShader");

		GLint locations[2] = {
			shaderProgram->Get_Uniform_Location("mvpMatrix"),
			shaderProgram->Get_Uniform_Location("color")
		};

		ColorRGBAf tintFloat = color;

		Matrix4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		shaderProgram->Use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.Pointer());
		glUniform4fv(locations[1], 1, tintFloat.Pointer());
	}
	else {
		shaderProgram = GE.Assets().Get<ShaderProgram>("DeferredShader");

		GLint locations[4] = {
			shaderProgram->Get_Uniform_Location("modelMatrix"),
			shaderProgram->Get_Uniform_Location("viewMatrix"),
			shaderProgram->Get_Uniform_Location("projectionMatrix"),
			shaderProgram->Get_Uniform_Location("color")
		};

		textureInstance.Use();

		ColorRGBAf tintFloat = color;

		shaderProgram->Use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.Pointer());
		glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.Pointer());
		glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.Pointer());
		glUniform4fv(locations[3], 1, tintFloat.Pointer());
	}

	gpuPusher.Draw();
}
