#ifndef SKYBOX_H
#define SKYBOX_H
#include "texture.h"

class Skybox : RenderComponent {
public:
	GLuint	texglid;
	GLuint	meshglid;
	GLuint	vertbuffer;
	//Texture2D* texes[6]; //r, l, t, b, f, b

			Skybox	(string path, string name);
			~Skybox	();
	void	Render	(Flags callflags);
};

#endif
