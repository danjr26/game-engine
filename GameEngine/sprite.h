#ifndef SIMPLE_TEXTURED_RECTANGLE_H
#define SIMPLE_TEXTURED_RECTANGLE_H

#include "texture.h"
#include "renderable_object.h"
#include "deep_transformable_object.h"
#include "texture_instance.h"
#include "display_units.h"
#include "texture_instance.h"
#include "mesh_vertex_gpu_pusher.h"
#include "shader_program_instance.h"
#include "geometric_figures.h"

class Sprite : public RenderableObject, public DeepTransformableObject2d {
protected:
	Transform2d mInnerTransform;
	MeshVertexData mMeshVertexData;
	MeshVertexGPUPusher mGPUPusher;
	TextureInstance mTextureInstance;
	ColorRGBAf mColor;
public:
	Sprite(const AxisAlignedRectangled& in_rectangle, Texture* in_texture = nullptr, const ColorRGBAf& in_color = ColorRGBAf(1, 1, 1, 1));
	~Sprite();
	void Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight);
	void Set_Color(const ColorRGBAf& in_color);
	TextureInstance& Texture_Instance();
	double Z() const override final;
	bool Should_Cull() const override final;
	virtual void Render() override;
};

#endif