#ifndef EXTRA_MESH_VERTEX_DATUM_H
#define EXTRA_MESH_VERTEX_DATUM_H

#include "vector.h"
#include <GL/glew.h>

struct ExtraMeshVertexDatum {
	enum DataType : GLenum {
		_byte = GL_BYTE,
		_ubyte = GL_UNSIGNED_BYTE,
		_short = GL_SHORT,
		_ushort = GL_UNSIGNED_SHORT,
		_int = GL_INT,
		_uint = GL_UNSIGNED_INT,
		_float = GL_FLOAT,
		_double = GL_DOUBLE
	};

	uint attributeIndex;
	DataType dataType;
	ubyte nComponents;
	void* data;

	uint Get_Data_Type_Size() const;
};

#endif