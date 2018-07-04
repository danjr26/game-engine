#include "debug_mesh_vertex_data_renderer.h"

DebugMeshVertexDataRenderer::DebugMeshVertexDataRenderer(MeshVertexData* in_meshVertexData) :
	meshVertexData(in_meshVertexData)
{}

double DebugMeshVertexDataRenderer::Z() const {
	return 0.0;
	//return transform.
}

bool DebugMeshVertexDataRenderer::Should_Cull() const {
	return false;
}

void DebugMeshVertexDataRenderer::Render() {}
