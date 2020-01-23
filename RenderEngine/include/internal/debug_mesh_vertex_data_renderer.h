#ifndef DEBUG_MESH_VERTEX_DATA_RENDERER_H
#define DEBUG_MESH_VERTEX_DATA_RENDERER_H

#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "renderable_object.h"
#include "transformable_object.h"


class DebugMeshVertexDataRenderer : public Renderable, public TransformableObject2d {
private:
	MeshVertexData* mMeshVertexData;
	MeshVertexGPUPusher mGPUPusher;
	Color4f mFillColor;
	Color4f mWireframeColor;
public:
	DebugMeshVertexDataRenderer(MeshVertexData* i_meshVertexData, const Color4f& i_fillColor = Color4f(0.6, 0.6, 0.6, 1.0), 
		const Color4f& i_wireframeColor = Color4f(1.0, 1.0, 1.0, 1.0));
	virtual double z() const override;
	virtual bool shouldCull() const override;
	virtual void render() override;
};

#endif