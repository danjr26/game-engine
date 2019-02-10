#ifndef DEBUG_MESH_VERTEX_DATA_RENDERER_H
#define DEBUG_MESH_VERTEX_DATA_RENDERER_H

#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "renderable_object.h"
#include "transformable_object.h"


class DebugMeshVertexDataRenderer : public RenderableObject, public TransformableObject2d {
private:
	MeshVertexData* mMeshVertexData;
	MeshVertexGPUPusher mGPUPusher;
	ColorRGBAf mFillColor;
	ColorRGBAf mWireframeColor;
public:
	DebugMeshVertexDataRenderer(MeshVertexData* in_meshVertexData, const ColorRGBAf& in_fillColor = ColorRGBAf(0.6, 0.6, 0.6, 1.0), 
		const ColorRGBAf& in_wireframeColor = ColorRGBAf(1.0, 1.0, 1.0, 1.0));
	virtual double Z() const override;
	virtual bool Should_Cull() const override;
	virtual void Render() override;
};

#endif