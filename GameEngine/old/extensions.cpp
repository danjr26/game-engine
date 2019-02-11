#include "extensions.h"

void Init_Extensions() {
	glAttachShader =			(PFNGLATTACHSHADERPROC)				wglGetProcAddress("glAttachShader");
	glBindBuffer =				(PFNGLBINDBUFFERPROC)				wglGetProcAddress("glBindBuffer");
	glBufferData =				(PFNGLBUFFERDATAPROC)				wglGetProcAddress("glBufferData");
	glCreateProgram =			(PFNGLCREATEPROGRAMPROC)			wglGetProcAddress("glCreateProgram");
	glCreateShader =			(PFNGLCREATESHADERPROC)				wglGetProcAddress("glCreateShader");
	glCompileShader =			(PFNGLCOMPILESHADERPROC)			wglGetProcAddress("glCompileShader");
	glDeleteProgram =			(PFNGLDELETEPROGRAMPROC)			wglGetProcAddress("glDeleteProgram");
	glDeleteShader =			(PFNGLDELETESHADERPROC)				wglGetProcAddress("glDeleteShader");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)	wglGetProcAddress("glEnableVertexAttribArray");
	glGenBuffers =				(PFNGLGENBUFFERSPROC)				wglGetProcAddress("glGenBuffers");
	glGetProgramiv =			(PFNGLGETPROGRAMIVPROC)				wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog =		(PFNGLGETPROGRAMINFOLOGPROC)		wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderiv =				(PFNGLGETSHADERIVPROC)				wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog =		(PFNGLGETSHADERINFOLOGPROC)			wglGetProcAddress("glGetShaderInfoLog");
	glLinkProgram =				(PFNGLLINKPROGRAMPROC)				wglGetProcAddress("glLinkProgram");
	glShaderSource =			(PFNGLSHADERSOURCEPROC)				wglGetProcAddress("glShaderSource");
	glUseProgram =				(PFNGLUSEPROGRAMPROC)				wglGetProcAddress("glUseProgram");
	glVertexAttribPointer =		(PFNGLVERTEXATTRIBPOINTERPROC)		wglGetProcAddress("glVertexAttribPointer");
}