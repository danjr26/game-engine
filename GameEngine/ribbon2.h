#ifndef RIBBON2_H
#define RIBBON2_H

#include "deep_transformable_object.h"
#include "graph.h"
#include "renderable_object.h"
#include "vector.h"
#include "mesh_vertex_data.h"
#include "mesh_vertex_gpu_pusher.h"
#include "color.h"
#include "texture_instance.h"

class Ribbon2 : public RenderableObject, public DeepTransformableObject2d {
public:
	struct NodeData {
		Vector2d mPosition;
		double mWidth;
		ColorRGBAf mColor;
		Vector2f mUV1;
		Vector2f mUV2;
	};

	struct EdgeData {
	};

	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;
	using graph_t = Graph<NodeData, EdgeData>;

private:
	Transform2d mTransform;
	DepthTransform2d mDepthTransform;
	MeshVertexData mMeshVertexData;
	MeshVertexGPUPusher mGPUPusher;
	TextureInstance mTextureInstance;
	graph_t mGraph;

public:
	Ribbon2(Texture* in_texture = nullptr);
	graph_t& getGraph();
	TextureInstance& getTextureInstance();
	void updateMesh();
	double z() const override final;
	bool shouldCull() const override final;
	virtual void render() override;
};

#endif