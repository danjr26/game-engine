#ifndef SIMPLE_TEXTURED_RECTANGLE_H
#define SIMPLE_TEXTURED_RECTANGLE_H

#include "texture.h"
#include "renderable_object.h"
#include "transformable_object.h"
#include "texture_instance.h"
#include "display_units.h"
#include "texture_instance.h"
#include "mesh_vertex_gpu_pusher.h"

class Sprite : public RenderableObject, public TransformableObject {
private:
	MeshVertexData meshVertexData;
	//MeshVertexGPUPusher gpuPusher;
	TextureInstance textureInstance;
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint indexBufferID;
	ColorRGBAc tint;
public:
	Sprite(const Vector3d& in_topLeft, const Vector2d& in_dimensions, Texture* in_texture);
	~Sprite();
	void Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight);
	void Set_Tint(const ColorRGBAc& in_tint);
	TextureInstance& Texture_Instance();
	double Z() const override final;
	bool Should_Cull() const override final;
	void Render() override final;
};

#endif