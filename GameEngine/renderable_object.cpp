#include "renderable_object.h"
#include "renderer.h"
#include "log.h"
#include "matrix.h"
#include "Texture2.h"
#include "Texture2_instance.h"

RenderableObject::RenderableObject()
{}

RenderableObject::~RenderableObject() 
{}

/*
TestRenderableObject::TestRenderableObject(Renderer* in_renderer) :
RenderableObject(in_renderer, 0)
{}

double TestRenderableObject::Z() {
	return 0;
}

bool TestRenderableObject::Should_Cull() {
	return false;
}

void TestRenderableObject::Render() {
	/*glDisable(GL_CULL_FACE);
	
	float vertices[3][3] = {
		{-0.5, 0.5, 0.5},
		{-0.5, -0.5, 0.5},
		{0.5, -0.5, 0.5}
	};

	float normals[3][3] = {
		{0.707106781, 0, 0.707106781},
		{0, 0.707106781, 0.707106781},
		{0, 0, 1}
	};
	
	float uvs[3][2] = {
		{0.75, 0.75},
		{0.25, 0.75},
		{1.0, 0.0}
	};

	GLuint vertexArrayID, vertexBuffer, normalBuffer, uvBuffer;
	
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 3, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 3, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * 3, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


	Shader vertexShader(Shader::Type::vertex, "shaders/", "deferred_1_v.glsl");
	Shader fragmentShader(Shader::Type::fragment, "shaders/", "deferred_1_f.glsl");
	std::vector<Shader*> shaders;
	shaders.push_back(&vertexShader);
	shaders.push_back(&fragmentShader);
	ShaderProgram shaderProgram("deferred_1", shaders);

	ColorRGBc textureColors[] = {
		ColorRGBc(255, 255, 255),
		ColorRGBc(255, 0, 255),
		ColorRGBc(255, 255, 0),
		ColorRGBc(0, 0, 255)
	};

	Texture2 tex = Texture2("", Vector2i(2, 2), 3, 8, (uchar*)textureColors);
	Texture2Instance texInstance = Texture2Instance(&tex);
	texInstance.Use();

	Matrix<float, 4, 4> iden = Matrix<float, 4, 4>::Identity();
	Matrix<float, 4, 4> move = {
		1.0f, 0.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Matrix<float, 4, 4> rot = {
		0.707106781f, -0.707106781f, 0.0f, 0.0f,
		0.707106781f, 0.707106781f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Matrix<float, 4, 4> scale = Matrix<float, 4, 4>::Identity() * 0.5f;
	scale.Element(3, 3, 1.0f);

	GLint locations[3] = { 
		shaderProgram.Get_Uniform_Location("modelMatrix"),
		shaderProgram.Get_Uniform_Location("viewMatrix"),
		shaderProgram.Get_Uniform_Location("projectionMatrix")
	};

	shaderProgram.Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, (move * rot * scale).Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, iden.Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, iden.Pointer());

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
*/
