#ifndef ENUM_WRAPPERS_H
#define ENUM_WRAPPERS_H

#include "../../../Utilities/include/utilities.h"
#include <GL/glew.h>
#include "color.h"

struct PushableType {
	enum class Value {
		undefined,
		i8,
		ui8,
		i16,
		ui16,
		i32,
		ui32,
		f16,
		f32,
		f64,
		ui24_ui8,
		f32_ui8
	};

	Value mValue;

	PushableType();
	PushableType(Value i_value);

	operator bool() const;

	ubyte getSize() const;
	GLenum toGL() const;
	static PushableType fromGL(GLenum i_value);
};

struct BufferUsage {
	enum class Frequency { undefined, seldom, often };
	enum class DataLocale { undefined, user, renderer };

	Frequency mReadFreq;
	DataLocale mReadLocale;
	Frequency mWriteFreq;
	DataLocale mWriteLocale;

	BufferUsage();
	BufferUsage(Frequency i_readFreq, DataLocale i_readLocale, Frequency i_writeFreq, DataLocale i_writeLocale);

	operator bool() const;

	GLenum toGL() const;
	static BufferUsage fromGL(GLenum i_value);
};

struct Topology {
	enum class Value {
		undefined,
		points,
		lines,
		lines_adj,
		line_strip,
		line_strip_adj,
		triangles,
		triangles_adj,
		triangle_strip,
		triangle_strip_adj
	};

	Value mValue;

	Topology();
	Topology(Value i_value);

	operator bool() const;

	GLenum toGL() const;
	static Topology fromGL(GLenum i_value);
};

struct TextureFilter {
	enum class Value {
		undefined,
		nearest,
		linear
	};

	Value mDirect;
	Value mMipmap;

	TextureFilter();
	TextureFilter(Value i_direct, Value i_mipmap);

	operator bool() const;

	GLenum toGL() const;
	static TextureFilter fromGL(GLenum i_value);
};

struct EdgeBehavior {
	enum class Value {
		undefined,
		clamp,
		repeat,
		mirror,
		mirror_once
	};

	Value mValue;

	EdgeBehavior();
	EdgeBehavior(Value i_value);

	operator bool() const;

	GLenum toGL() const;
	static EdgeBehavior fromGL(GLenum i_value);
};

struct Swizzle {
	enum class Value {
		undefined,
		channel_1,
		channel_2,
		channel_3,
		channel_4,
		zero,
		one
	};

	Value mValue;

	Swizzle();
	Swizzle(Value i_value);

	operator bool() const;

	GLenum toGL() const;
	static Swizzle fromGL(GLenum i_value);
};

struct TextureType {
	enum class Value {
		undefined,
		single_1,
		array_1,
		single_2,
		array_2,
		single_msaa_2,
		array_msaa_2,
		single_cube_2,
		array_cube_2,
		single_3,
	};

	Value mValue;
	ubyte mMSAA;

	TextureType();
	TextureType(Value i_value, ubyte i_msaa = 0);

	operator bool() const;

	bool isArray() const;
	bool hasMSAA() const;
	bool isCube() const;
	ubyte getBasicDimen() const;
	ubyte getTotalDimen() const;

	GLenum toGL() const;
	static TextureType fromGL(GLenum i_value);
};

struct CubeMapFace {
	enum class Value {
		undefined,
		x_positive,
		x_negative,
		y_positive,
		y_negative,
		z_positive,
		z_negative
	};

	Value mValue;

	CubeMapFace();
	CubeMapFace(Value i_value);

	operator bool() const;

	ubyte toIndex() const;
	GLenum toGL() const;
	static CubeMapFace fromIndex(ubyte i_index);
	static CubeMapFace fromGL(GLenum i_value);
};

struct PixelFormat {
	enum class Function {
		undefined,
		color,
		depth,
		stencil,
		depth_stencil
	};

	enum class Type {
		undefined,
		si,
		ui,
		f,
		f_ui
	};

	Type mType;
	Color4b mBits;
	Function mFunction;

	PixelFormat();
	PixelFormat(Type i_type, Color4b i_bits, Function i_function);
	PixelFormat(PushableType i_type, ubyte i_n, Function i_function);

	operator bool() const;
	ubyte getDepth() const;
	ubyte getSize() const;
	PushableType getPushableType() const;

	GLenum toGL() const;
	GLenum toGLSimple() const;
	static PixelFormat fromGL(GLenum i_value);
};

struct BufferType {
	enum class Value {
		undefined,
		vertex_attributes,
		copy_read,
		copy_write,
		draw_command,
		compute_command,
		vertex_indices,
		shader_storage,
		uniform_data
	};

	Value mValue;

	BufferType();
	BufferType(Value i_value);

	operator bool() const;

	GLenum toGL() const;
	static BufferType fromGL(GLenum i_value);
};

struct DisplayMagnifyMode {
	enum class Value {
		undefined,
		sys_default,
		stretch,
		center
	};

	Value mValue;

	DisplayMagnifyMode();
	DisplayMagnifyMode(Value i_value);

	operator bool() const;

	uint toWin() const;
	static DisplayMagnifyMode fromWin(uint i_value);
};

struct ShaderType {
	enum class Value {
		undefined,
		vertex,
		fragment,
		geometry,
		tess_control,
		tess_eval,
		compute
	};

	Value mValue;

	ShaderType();
	ShaderType(Value i_value);

	operator bool() const;

	GLenum toGL() const;
	static ShaderType fromGL(GLenum i_value);
};

struct ClearBits {
	enum {
		color = 1 << 0,
		depth = 1 << 1,
		stencil = 1 << 2
	};

	static GLbitfield toGL(uint i_value);
	static uint fromGL(GLuint i_value);
};

#endif