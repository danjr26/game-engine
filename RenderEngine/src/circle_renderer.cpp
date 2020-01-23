#include "../include/internal/circle_renderer.h"

CircleRenderer::CircleRenderer(const Circled& i_circle, const Color4f& i_color, float i_outlineWidth, const Color4f& i_outlineColor) :
	Sprite(AABox2d::fromCenter(i_circle.getCenter(), Vector2d(i_circle.getRadius(), i_circle.getRadius()) * 2.0), nullptr, i_color),
	mOutlineWidth(i_outlineWidth),
	mOutlineColor(i_outlineColor)
{}

void CircleRenderer::render() {
	ShaderProgram* shaderProgram = GE.assets().get<ShaderProgram>("EllipseShader");

	GLint locations[] = {
		shaderProgram->getUniformLocation("mvpMatrix"),
		shaderProgram->getUniformLocation("fillColor"),
		shaderProgram->getUniformLocation("outlineColor"),
		shaderProgram->getUniformLocation("outlineWidth"),
		shaderProgram->getUniformLocation("pixelDimensions")
	};

	Camera& cam = *GE.cameras().getActive();

	Matrix4f modelMatrix = (Matrix4f)(mDepthTransform.getWorldMatrix() * mInnerTransform.getWorldMatrix());
	Matrix4f viewMatrix = (Matrix4f)cam.getViewMatrix();
	Matrix4f projectionMatrix = (Matrix4f)cam.getProjectionMatrix();
	Matrix4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	Vector2d pixelDimensions = mInnerTransform.getLocalScale();
	Vector2d winDim = GE.render().mMainTarget->getDimensions();
	pixelDimensions = pixelDimensions.compwise((Vector2d)(cam.getProjection().mMaxima - cam.getProjection().mMinima).componentInverted()).compwise(winDim);

	shaderProgram->use();
	glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.pointer());
	glUniform4fv(locations[1], 1, mColor.pointer());
	glUniform4fv(locations[2], 1, mOutlineColor.pointer());
	glUniform1f(locations[3], mOutlineWidth);
	glUniform2fv(locations[4], 1, ((Vector2f)pixelDimensions).pointer());

	mGPUPusher.draw();
}

void CircleRenderer::setCircle(const Circled& i_circle) {
	setRectangle(AABox2d::fromCenter(i_circle.getCenter(), Vector2d(i_circle.getRadius(), i_circle.getRadius()) * 2.0));
}
