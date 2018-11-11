#include "circle_renderer.h"
#include "game_engine.h"

CircleRenderer::CircleRenderer(const DeepCircled& in_circle, const ColorRGBAf& in_color, 
	double in_outlineWidth, const ColorRGBAf& in_outlineColor) :

	Sprite(DeepAxisAlignedRectangled(AxisAlignedRectangled::From_Center(in_circle.Get_Center(), Vector2d(in_circle.Get_Radius(), in_circle.Get_Radius()) * 2.0), 0.0), nullptr, in_color),
	outlineWidth(in_outlineWidth),
	outlineColor(in_outlineColor)
{}

void CircleRenderer::Render() {
	ShaderProgram* shaderProgram = GE.Assets().Get<ShaderProgram>("EllipseShader");

	GLint locations[] = {
		shaderProgram->Get_Uniform_Location("mvpMatrix"),
		shaderProgram->Get_Uniform_Location("fillColor"),
		shaderProgram->Get_Uniform_Location("outlineColor"),
		shaderProgram->Get_Uniform_Location("outlineWidth"),
		shaderProgram->Get_Uniform_Location("pixelDimensions")
	};

	Matrix4f modelMatrix = (Matrix4f)transform.Get_World_Matrix();
	Matrix4f viewMatrix = (Matrix4f)GE.Cameras().active->Get_View_Matrix();
	Matrix4f projectionMatrix = (Matrix4f)GE.Cameras().active->Get_Projection_Matrix();
	Matrix4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.Pointer());
	glUniform4fv(locations[1], 1, color.Pointer());
	glUniform4fv(locations[2], 1, outlineColor.Pointer());
	glUniform1f(locations[3], (float)outlineWidth);
	glUniform2fv(locations[4], 1, Vector2f(Vector3f(Get_Transform().Get_Local_Scale())).Pointer());

	/*
	shaderProgram->Use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, ((Matrix4f)transform.Get_World_Matrix()).Pointer());
	glUniformMatrix4fv(locations[1], 1, GL_TRUE, GE.Cameras().active->Get_View_Matrix().Pointer());
	glUniformMatrix4fv(locations[2], 1, GL_TRUE, GE.Cameras().active->Get_Projection_Matrix().Pointer());
	glUniform4fv(locations[3], 1, tintFloat.Pointer());*/

	gpuPusher.Draw();
}
