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

class Sprite : public RenderableObject, public DeepTransformableObject2d {
protected:
	Transform2d mInnerTransform;
	DepthTransform2d mDepthTransform;
	MeshVertexData mMeshVertexData;
	MeshVertexGPUPusher mGPUPusher;
	TextureInstance mTextureInstance;
	ColorRGBAf mColor;
public:
	Sprite(
		const AxisAlignedRectangled& in_rectangle = AxisAlignedRectangled(), 
		Texture* in_texture = nullptr, 
		const ColorRGBAf& in_color = ColorRGBAf(1, 1, 1, 1)
	);
	~Sprite();

	void setRectangle(const AxisAlignedRectangled& in_rectangle);
	void setTexture(Texture* in_texture);
	void setUVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight);
	void setColor(const ColorRGBAf& in_color);
	TextureInstance& getTextureInstance();
	double z() const override final;
	bool shouldCull() const override final;
	virtual void render() override;
};

#endif