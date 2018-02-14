#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <Windows.h>
#include <gl/GL.h>
#include <glext.h>

extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLBINDBUFFERPROC					glBindBuffer;
extern PFNGLBUFFERDATAPROC					glBufferData;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC					glGenBuffers;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;


void Init_Extensions();

#endif
