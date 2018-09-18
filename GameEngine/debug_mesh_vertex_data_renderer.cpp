#include "debug_mesh_vertex_data_renderer.h"
#include "game_engine.h"

/*
DebugMeshVertexDataRenderer::DebugMeshVertexDataRenderer(MeshVertexData* in_meshVertexData) :
	meshVertexData(in_meshVertexData)
{}

double DebugMeshVertexDataRenderer::Z() const {
	return transform.Get_World_Position().Z();
}

bool DebugMeshVertexDataRenderer::Should_Cull() const {
	return false;
}

void DebugMeshVertexDataRenderer::Render() {
	
}
*/

DebugMeshVertexDataRenderer::DebugMeshVertexDataRenderer(MeshVertexData* in_meshVertexData, const ColorRGBAf& in_fillColor, const ColorRGBAf& in_wireframeColor) :
	meshVertexData(in_meshVertexData),
	gpuPusher(),
	fillColor(in_fillColor),
	wireframeColor(in_wireframeColor) {
	
	gpuPusher.Initialize(meshVertexData);
}

double DebugMeshVertexDataRenderer::Z() const {
	return transform.Get_World_Position().Z();
}

bool DebugMeshVertexDataRenderer::Should_Cull() const {
	return false;
}

void DebugMeshVertexDataRenderer::Render() {
	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("MonoShader");

	GLint locations[] = {
		shaderProgram->Get_Uniform_Location("mvpMatrix"),
		shaderProgram->Get_Uniform_Location("color")
	};

	Matrix4f mvpMatrix = 
		GE.Cameras().active->Get_Projection_Matrix() * 
		GE.Cameras().active->Get_View_Matrix() * 
		(Matrix4f)transform.Get_World_Matrix();

	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.Pointer());
	glUniform4fv(locations[1], 1, fillColor.Pointer());

	gpuPusher.Draw();

	glUniform4fv(locations[1], 1, wireframeColor.Pointer());
	glLineWidth(1.5f);
//	glEnable(GL_LINE_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	gpuPusher.Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
