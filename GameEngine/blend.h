#ifndef BLEND_H
#define BLEND_H

struct BlendSettings {
	GLenum	mDestinationFactor;
	GLenum	mSourceFactor;
	GLenum	mOperation;
	bool	mUseBlending;

	BlendSettings(bool useBlending = false, GLenum in_destinationFactor = GL_ZERO, GLenum in_sourceFactor = GL_ONE, GLenum in_operation = GL_ADD);
};

#endif