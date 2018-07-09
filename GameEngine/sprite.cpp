#include "sprite.h"
#include "display_units.h"
#include "game_engine.h"

Sprite::Sprite(const Vector3d& in_topLeft, const Vector2d& in_dimensions, Texture* in_texture) :
	meshVertexData(MeshVertexData::DataType::_ubyte), 
	//gpuPusher(&meshVertexData),
	tint(255, 255, 255, 255),
	textureInstance(in_texture) {

	transform.Set_Local_Position(in_topLeft);
	transform.Scale_Local(Vector3d(in_dimensions, 1.0));

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

	//meshVertexData.Add_Vertices(4, (float*)positions, nullptr, nullptr, (float*)uvs);
	//meshVertexData.Add_Faces(2, indices);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 4, positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * 4, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * 6, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

Sprite::~Sprite() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &uvBufferID);
	glDeleteBuffers(1, &indexBufferID);
}

void Sprite::Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight) {
	Vector2f uvs[] = {
		in_topLeft,
		Vector2f(in_topLeft.X(), in_bottomRight.Y()),
		in_bottomRight,
		Vector2f(in_bottomRight.X(), in_topLeft.Y())
	};
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * 4, uvs);
	glBindVertexArray(0);
}

void Sprite::Set_Tint(const ColorRGBAc& in_tint) {
	tint = in_tint;
}

TextureInstance& Sprite::Texture_Instance() {
	return textureInstance;
}

double Sprite::Z() const {
	return transform.Get_Local_Position().Z();
}

bool Sprite::Should_Cull() const {
	return false;
}

void Sprite::Render() {
	Matrix<float, 4, 4> iden = Matrix<float, 4, 4>::Identity();

	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("DeferredShader");

	GLint locations[4] = { 
		shaderProgram->Get_Uniform_Location("modelMatrix"),
		shaderProgram->Get_Uniform_Location("viewMatrix"),
		shaderProgram->Get_Uniform_Location("projectionMatrix"),
		shaderProgram->Get_Uniform_Location("colorTint")
	};

	if (textureInstance.Get_Texture() != nullptr) {
		textureInstance.Use();
	}
	else {
		textureInstance.Get_Texture()->Use_None();
	}

	ColorRGBAf tintFloat = tint;

	glBindVertexArray(vertexArrayID);

	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, ((Matrix4f)transform.Get_World_Matrix()).Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, GE.Cameras().active->Get_View_Matrix().Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, GE.Cameras().active->Get_Projection_Matrix().Pointer());
	glUniform4fv(locations[3], 1, tintFloat.Pointer());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}
