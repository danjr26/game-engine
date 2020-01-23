#include "debug_mesh_vertex_data_renderer.h"
#include "game_engine.h"

/*
DebugMeshVertexDataRenderer::DebugMeshVertexDataRenderer(MeshVertexData* i_meshVertexData) :
	meshVertexData(i_meshVertexData)
{}

double DebugMeshVertexDataRenderer::z() const {
	return transform.getWorldPosition().z();
}

bool DebugMeshVertexDataRenderer::shouldCull() const {
	return false;
}

void DebugMeshVertexDataRenderer::render() {
	
}
*/

DebugMeshVertexDataRenderer::DebugMeshVertexDataRenderer(MeshVertexData* i_meshVertexData, const Color4f& i_fillColor, const Color4f& i_wireframeColor) :
	mMeshVertexData(i_meshVertexData),
	mGPUPusher(),
	mFillColor(i_fillColor),
	mWireframeColor(i_wireframeColor) {
	
	mGPUPusher.initialize(mMeshVertexData);
}

double DebugMeshVertexDataRenderer::z() const {
	return 0;
}

bool DebugMeshVertexDataRenderer::shouldCull() const {
	return false;
}

void DebugMeshVertexDataRenderer::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("MonoShader");

	GLint locations[] = {
		shaderProgram->getUniformLocation("mvpMatrix"),
		shaderProgram->getUniformLocation("color")
	};

	Matrix4f mvpMatrix = 
		GE.cameras().getActive()->getProjectionMatrix() * 
		GE.cameras().getActive()->getViewMatrix() * 
		(Matrix4f)getTransform().getWorldMatrix();

	shaderProgram->use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.pointer());
	glUniform4fv(locations[1], 1, mFillColor.pointer());

	mGPUPusher.draw();

	glUniform4fv(locations[1], 1, mWireframeColor.pointer());
	glLineWidth(1.5f);
//	glEnable(GL_LINE_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mGPUPusher.draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
