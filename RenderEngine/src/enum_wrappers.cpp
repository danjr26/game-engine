#include "../include/internal/enum_wrappers.h"
#include <windows.h>
#undef min
#undef max

PushableType::PushableType() :
	mValue(Value::undefined) {}

PushableType::PushableType(Value i_value) :
	mValue(i_value) {}

PushableType::operator bool() const {
	return mValue != Value::undefined;
}

ubyte PushableType::getSize() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::i8:
	case Value::ui8: return 1;
	case Value::i16:
	case Value::ui16:
	case Value::f16: return 2;
	case Value::ui24_ui8:
	case Value::i32:
	case Value::ui32:
	case Value::f32: return 4;
	case Value::f32_ui8:
	case Value::f64: return 8;
	default: fail();
	}
}

GLenum PushableType::toGL() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::i8: return GL_BYTE;
	case Value::ui8: return GL_UNSIGNED_BYTE;
	case Value::i16: return GL_SHORT;
	case Value::ui16: return GL_UNSIGNED_SHORT;
	case Value::i32: return GL_INT;
	case Value::ui32: return GL_UNSIGNED_INT;
	case Value::f16: return GL_HALF_FLOAT;
	case Value::f32: return GL_FLOAT;
	case Value::f64: return GL_DOUBLE;
	case Value::ui24_ui8: return GL_UNSIGNED_INT_24_8;
	case Value::f32_ui8: return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
	default: fail();
	}
}

PushableType PushableType::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_BYTE: return Value::i8;
	case GL_UNSIGNED_BYTE: return Value::ui8;
	case GL_SHORT: return Value::i16;
	case GL_UNSIGNED_SHORT: return Value::ui16;
	case GL_INT: return Value::i32;
	case GL_UNSIGNED_INT: return Value::ui32;
	case GL_HALF_FLOAT: return Value::f16;
	case GL_FLOAT: return Value::f32;
	case GL_DOUBLE: return Value::f64;
	case GL_UNSIGNED_INT_24_8: return Value::ui24_ui8;
	case GL_FLOAT_32_UNSIGNED_INT_24_8_REV: return Value::f32_ui8;
	default: return Value::undefined;
	}
}

BufferUsage::BufferUsage() :
	mReadFreq(Frequency::undefined),
	mReadLocale(DataLocale::undefined),
	mWriteFreq(Frequency::undefined),
	mWriteLocale(DataLocale::undefined) {}

BufferUsage::BufferUsage(Frequency i_readFreq, DataLocale i_readLocale, Frequency i_writeFreq, DataLocale i_writeLocale) :
	mReadFreq(i_readFreq),
	mReadLocale(i_readLocale),
	mWriteFreq(i_writeFreq),
	mWriteLocale(i_writeLocale) {}

BufferUsage::operator bool() const {
	return
		mReadFreq != Frequency::undefined &&
		mReadLocale != DataLocale::undefined &&
		mWriteFreq != Frequency::undefined &&
		mWriteLocale != DataLocale::undefined;
}

GLenum BufferUsage::toGL() const {
	if (mReadFreq == Frequency::seldom)
		if (mWriteFreq == Frequency::seldom)
			if (mReadLocale == DataLocale::user)
				if (mWriteLocale == DataLocale::user) return GL_STREAM_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_STREAM_DRAW;
				else fail();
			else if (mReadLocale == DataLocale::renderer)
				if (mWriteLocale == DataLocale::user) return GL_STREAM_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_STREAM_COPY;
				else fail();
			else fail();
		else if (mWriteFreq == Frequency::often)
			if (mReadLocale == DataLocale::user)
				if (mWriteLocale == DataLocale::user) return GL_DYNAMIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_DYNAMIC_DRAW;
				else fail();
			else if (mReadLocale == DataLocale::renderer)
				if (mWriteLocale == DataLocale::user) return GL_DYNAMIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_DYNAMIC_COPY;
				else fail();
			else fail();
		else fail();
	else if (mReadFreq == Frequency::often)
		if (mWriteFreq == Frequency::seldom)
			if (mReadLocale == DataLocale::user)
				if (mWriteLocale == DataLocale::user) return GL_STATIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_STATIC_DRAW;
				else fail();
			else if (mReadLocale == DataLocale::renderer)
				if (mWriteLocale == DataLocale::user) return GL_STATIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_STATIC_COPY;
				else fail();
			else fail();
		else if (mWriteFreq == Frequency::often)
			if (mReadLocale == DataLocale::user)
				if (mWriteLocale == DataLocale::user) return GL_DYNAMIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_DYNAMIC_DRAW;
				else fail();
			else if (mReadLocale == DataLocale::renderer)
				if (mWriteLocale == DataLocale::user) return GL_DYNAMIC_READ;
				else if (mWriteLocale == DataLocale::renderer) return GL_DYNAMIC_COPY;
				else fail();
			else fail();
		else fail();
	else fail();
}

BufferUsage BufferUsage::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_STREAM_READ: return BufferUsage(Frequency::seldom, DataLocale::user, Frequency::seldom, DataLocale::renderer);
	case GL_STREAM_COPY: return BufferUsage(Frequency::seldom, DataLocale::renderer, Frequency::seldom, DataLocale::renderer);
	case GL_STREAM_DRAW: return BufferUsage(Frequency::seldom, DataLocale::renderer, Frequency::seldom, DataLocale::user);
	case GL_STATIC_READ: return BufferUsage(Frequency::often, DataLocale::user, Frequency::seldom, DataLocale::renderer);
	case GL_STATIC_COPY: return BufferUsage(Frequency::often, DataLocale::renderer, Frequency::seldom, DataLocale::renderer);
	case GL_STATIC_DRAW: return BufferUsage(Frequency::often, DataLocale::renderer, Frequency::seldom, DataLocale::user);
	case GL_DYNAMIC_READ: return BufferUsage(Frequency::often, DataLocale::user, Frequency::often, DataLocale::renderer);
	case GL_DYNAMIC_COPY:	return BufferUsage(Frequency::often, DataLocale::renderer, Frequency::often, DataLocale::renderer);
	case GL_DYNAMIC_DRAW:	return BufferUsage(Frequency::often, DataLocale::renderer, Frequency::often, DataLocale::user);
	default: return BufferUsage();
	}
}

Topology::Topology() :
	mValue(Value::undefined) {}

Topology::Topology(Value i_value) :
	mValue(i_value) {}

Topology::operator bool() const {
	return mValue != Value::undefined;
}

GLenum Topology::toGL() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::points: return GL_POINTS;
	case Value::lines: return GL_LINES;
	case Value::lines_adj: return GL_LINES_ADJACENCY;
	case Value::line_strip: return GL_LINE_STRIP;
	case Value::line_strip_adj: return GL_LINE_STRIP_ADJACENCY;
	case Value::triangles: return GL_TRIANGLES;
	case Value::triangles_adj: return GL_TRIANGLES_ADJACENCY;
	case Value::triangle_strip: return GL_TRIANGLE_STRIP;
	case Value::triangle_strip_adj: return GL_TRIANGLE_STRIP_ADJACENCY;
	default: fail();
	}
}

Topology Topology::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_POINTS: return Value::points;
	case GL_LINES: return Value::lines;
	case GL_LINES_ADJACENCY: return Value::lines_adj;
	case GL_LINE_STRIP: return Value::line_strip;
	case GL_LINE_STRIP_ADJACENCY: return Value::line_strip_adj;
	case GL_TRIANGLES: return Value::triangles;
	case GL_TRIANGLES_ADJACENCY: return Value::triangles_adj;
	case GL_TRIANGLE_STRIP: return Value::triangle_strip;
	case GL_TRIANGLE_STRIP_ADJACENCY: return Value::triangle_strip_adj;
	default: fail();
	}
}

TextureFilter::TextureFilter() :
mDirect(Value::undefined),
mMipmap(Value::undefined)
{}

TextureFilter::TextureFilter(Value i_direct, Value i_mipmap) :
mDirect(i_direct),
mMipmap(i_mipmap)
{}

TextureFilter::operator bool() const {
	return mDirect != Value::undefined;
}

GLenum TextureFilter::toGL() const {
	if (mDirect == Value::nearest)
		if (mMipmap == Value::nearest) return GL_NEAREST_MIPMAP_NEAREST;
		else if (mMipmap == Value::linear) return GL_NEAREST_MIPMAP_LINEAR;
		else if (mMipmap == Value::undefined) return GL_NEAREST;
		else fail();
	else if (mDirect == Value::linear)
		if (mMipmap == Value::nearest) return GL_LINEAR_MIPMAP_NEAREST;
		else if (mMipmap == Value::linear) return GL_LINEAR_MIPMAP_LINEAR;
		else if (mMipmap == Value::undefined) return GL_LINEAR;
		else fail();
	else fail();
}

TextureFilter TextureFilter::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_NEAREST: return TextureFilter(Value::nearest, Value::undefined);
	case GL_LINEAR: return TextureFilter(Value::linear, Value::undefined);
	case GL_NEAREST_MIPMAP_NEAREST: return TextureFilter(Value::nearest, Value::nearest);
	case GL_LINEAR_MIPMAP_LINEAR: return TextureFilter(Value::linear, Value::linear);
	case GL_NEAREST_MIPMAP_LINEAR: return TextureFilter(Value::nearest, Value::linear);
	case GL_LINEAR_MIPMAP_NEAREST: return TextureFilter(Value::linear, Value::nearest);
	default: fail();
	}
}

EdgeBehavior::EdgeBehavior() :
mValue(Value::undefined)
{}

EdgeBehavior::EdgeBehavior(Value i_value) :
mValue(i_value)
{}

EdgeBehavior::operator bool() const {
	return mValue != Value::undefined;
}

GLenum EdgeBehavior::toGL() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::clamp: return GL_CLAMP_TO_EDGE;
	case Value::repeat: return GL_REPEAT;
	case Value::mirror: return GL_MIRRORED_REPEAT;
	case Value::mirror_once: return GL_MIRROR_CLAMP_TO_EDGE;
	default: fail();
	}
}

EdgeBehavior EdgeBehavior::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_CLAMP_TO_EDGE: return Value::clamp;
	case GL_REPEAT: return Value::repeat;
	case GL_MIRRORED_REPEAT: return Value::repeat;
	case GL_MIRROR_CLAMP_TO_EDGE: return Value::mirror_once;
	default: return Value::undefined;
	}
}

Swizzle::Swizzle() :
mValue(Value::undefined)
{}

Swizzle::Swizzle(Value i_value) :
mValue(i_value)
{}

Swizzle::operator bool() const {
	return mValue != Value::undefined;
}

GLenum Swizzle::toGL() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::channel_1: return GL_RED;
	case Value::channel_2: return GL_GREEN;
	case Value::channel_3: return GL_BLUE;
	case Value::channel_4: return GL_ALPHA;
	case Value::zero: return GL_ZERO;
	case Value::one: return GL_ONE;
	default: fail();
	}
}

Swizzle Swizzle::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_RED: return Value::channel_1;
	case GL_GREEN: return Value::channel_2;
	case GL_BLUE: return Value::channel_3;
	case GL_ALPHA: return Value::channel_4;
	case GL_ZERO: return Value::zero;
	case GL_ONE: return Value::one;
	default: return Value::undefined;
	}
}

TextureType::TextureType() :
mValue(Value::undefined),
mMSAA(0)
{}

TextureType::TextureType(Value i_value, ubyte i_msaa) :
mValue(i_value),
mMSAA(i_msaa)
{}

TextureType::operator bool() const {
	return mValue != Value::undefined;
}

bool TextureType::isArray() const {
	switch (mValue) {
	case Value::array_1:
	case Value::array_2:
	case Value::array_cube_2:
	case Value::array_msaa_2: return true;
	default: return false;
	}
}

bool TextureType::hasMSAA() const {
	switch (mValue) {
	case Value::single_msaa_2:
	case Value::array_msaa_2: return true;
	default: return false;
	}
}

bool TextureType::isCube() const {
	switch (mValue) {
	case Value::single_cube_2:
	case Value::array_cube_2: return true;
	default: return false;
	}
}

ubyte TextureType::getBasicDimen() const {
	switch(mValue) {
	case Value::single_1: 
	case Value::array_1: return 1;
	case Value::single_2:
	case Value::array_2:
	case Value::single_msaa_2:
	case Value::array_msaa_2:
	case Value::single_cube_2:
	case Value::array_cube_2: return 2;
	case Value::single_3: return 3;
	default: return 0;
	}
}

ubyte TextureType::getTotalDimen() const {
	switch (mValue) {
	case Value::single_1: return 1;
	case Value::array_1: 
	case Value::single_2:
	case Value::single_msaa_2: return 2;
	case Value::array_2:
	case Value::array_msaa_2:
	case Value::single_cube_2: 
	case Value::single_3: return 3;
	case Value::array_cube_2: return 4;
	default: return 0;
	}
}

GLenum TextureType::toGL() const {
	switch (mValue) {
	case Value::undefined: fail();
	case Value::single_1: return GL_TEXTURE_1D;
	case Value::array_1: return GL_TEXTURE_1D_ARRAY;
	case Value::single_2: return GL_TEXTURE_2D;
	case Value::array_2: return GL_TEXTURE_2D_ARRAY;
	case Value::single_msaa_2: return GL_TEXTURE_2D_MULTISAMPLE;
	case Value::array_msaa_2: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	case Value::single_cube_2: return GL_TEXTURE_CUBE_MAP;
	case Value::array_cube_2: return GL_TEXTURE_CUBE_MAP_ARRAY;
	case Value::single_3: return GL_TEXTURE_3D;
	default: fail();
	}
}

TextureType TextureType::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_TEXTURE_1D: return Value::single_1;
	case GL_TEXTURE_1D_ARRAY: return Value::array_1;
	case GL_TEXTURE_2D: return Value::single_2;
	case GL_TEXTURE_2D_ARRAY: return Value::array_2;
	case GL_TEXTURE_2D_MULTISAMPLE: return Value::single_msaa_2;
	case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return Value::array_msaa_2;
	case GL_TEXTURE_CUBE_MAP: return Value::single_cube_2;
	case GL_TEXTURE_CUBE_MAP_ARRAY: return Value::array_cube_2;
	case GL_TEXTURE_3D: return Value::single_3;
	default: return Value::undefined;
	}
}

CubeMapFace::CubeMapFace() :
mValue(Value::undefined)
{}

CubeMapFace::CubeMapFace(Value i_value) :
mValue(i_value)
{}

CubeMapFace::operator bool() const {
	return mValue != Value::undefined;
}

ubyte CubeMapFace::toIndex() const {
	switch (mValue) {
	case Value::x_positive: return 0;
	case Value::x_negative: return 1;
	case Value::y_positive: return 2;
	case Value::y_negative: return 3;
	case Value::z_positive: return 4;
	case Value::z_negative: return 5;
	default: fail();
	}
}

GLenum CubeMapFace::toGL() const {
	switch (mValue) {
	case Value::x_positive: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case Value::x_negative: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case Value::y_positive: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case Value::y_negative: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case Value::z_positive: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case Value::z_negative: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	default: fail();
	}
}

CubeMapFace CubeMapFace::fromIndex(ubyte i_index) {
	switch (i_index) {
	case 0: return Value::x_positive;
	case 1:	return Value::x_negative;
	case 2:	return Value::y_positive;
	case 3:	return Value::y_negative;
	case 4:	return Value::z_positive;
	case 5:	return Value::z_negative;
	default: fail();
	}
}

CubeMapFace CubeMapFace::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_TEXTURE_CUBE_MAP_POSITIVE_X: return Value::x_positive;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_X: return Value::x_negative;
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Y: return Value::y_positive;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y: return Value::y_negative;
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Z: return Value::z_positive;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z: return Value::z_negative;
	default: return Value::undefined;
	}
}

PixelFormat::PixelFormat() :
mType(Type::undefined),
mBits(0, 0, 0, 0),
mFunction(Function::undefined)
{}

PixelFormat::PixelFormat(Type i_type, Color4b i_bits, Function i_function) :
mType(i_type),
mBits(i_bits),
mFunction(i_function)
{}

PixelFormat::PixelFormat(PushableType i_type, ubyte i_n, Function i_function) :
mFunction(i_function)
{
	switch (i_type.mValue) {
	case PushableType::Value::i8:
	case PushableType::Value::i16:
	case PushableType::Value::i32:
		mType = Type::si;
		break;
	case PushableType::Value::ui8:
	case PushableType::Value::ui16:
	case PushableType::Value::ui32:
		mType = Type::ui;
		break;
	case PushableType::Value::f16:
	case PushableType::Value::f32:
	case PushableType::Value::f64:
		mType = Type::f;
		break;
	default: fail();
	}

	for (ubyte i = 0; i < i_n; i++) {
		mBits[i] = i_type.getSize();
	}
}

PixelFormat::operator bool() const {
	return mType != Type::undefined && mBits.compSum() && mFunction != Function::undefined;
}

ubyte PixelFormat::getDepth() const {
	for (int i = 3; i >= 0; i--) {
		if (mBits[i]) return i + 1;
	}
	return 0;
}

ubyte PixelFormat::getSize() const {
	return (mBits.compSum() + 7) / 8;
}

PushableType PixelFormat::getPushableType() const {
	ubyte depth = getDepth();
	switch (mType) {
	case PixelFormat::Type::si:
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 8) break;
			if (i == depth - 1) return PushableType::Value::i8;
		}
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 16) break;
			if (i == depth - 1) return PushableType::Value::i16;
		}
		break;
	case PixelFormat::Type::ui:
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 8) break;
			if (i == depth - 1) return PushableType::Value::ui8;
		}
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 16) break;
			if (i == depth - 1) return PushableType::Value::ui16;
		}
		if (mBits == Color4b(24, 8, 0, 0)) return PushableType::Value::ui24_ui8;
		break;
	case PixelFormat::Type::f:
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 16) break;
			if (i == depth - 1) return PushableType::Value::f16;
		}
		for (ubyte i = 0; i < depth; i++) {
			if (mBits[i] != 32) break;
			if (i == depth - 1) return PushableType::Value::f32;
		}
		break;
	case PixelFormat::Type::f_ui: return PushableType::Value::f32_ui8;
	default: return PushableType();
	}
}

GLenum PixelFormat::toGL() const {
	switch (mFunction) {
	case PixelFormat::Function::color:
		switch (mType) {
		case Type::si:
			if (mBits == Color4b(8, 0, 0, 0)) return GL_R8_SNORM;
			if (mBits == Color4b(16, 0, 0, 0)) return GL_R16_SNORM;
			if (mBits == Color4b(8, 8, 0, 0)) return GL_RG8_SNORM;
			if (mBits == Color4b(16, 16, 0, 0)) return GL_RG16_SNORM;
			if (mBits == Color4b(8, 8, 8, 0)) return GL_RGB8_SNORM;
			if (mBits == Color4b(16, 16, 16, 0)) return GL_RGB16_SNORM;
			if (mBits == Color4b(8, 8, 8, 8)) return GL_RGBA8_SNORM;
			if (mBits == Color4b(16, 16, 16, 16)) return GL_RGBA16_SNORM;
			if (mBits == Color4b(0, 0, 0, 8)) return GL_ALPHA8_SNORM;
			if (mBits == Color4b(0, 0, 0, 16)) return GL_ALPHA16_SNORM;
			fail();
		case Type::ui:
			if (mBits == Color4b(8, 0, 0, 0)) return GL_R8;
			if (mBits == Color4b(16, 0, 0, 0)) return GL_R16;
			if (mBits == Color4b(8, 8, 0, 0)) return GL_RG8;
			if (mBits == Color4b(16, 16, 0, 0)) return GL_RG16;
			if (mBits == Color4b(8, 8, 8, 0)) return GL_RGB8;
			if (mBits == Color4b(16, 16, 16, 0)) return GL_RGB16;
			if (mBits == Color4b(8, 8, 8, 8)) return GL_RGBA8;
			if (mBits == Color4b(16, 16, 16, 16)) return GL_RGBA16;
			if (mBits == Color4b(3, 3, 2, 0)) return GL_R3_G3_B2;
			if (mBits == Color4b(4, 4, 4, 0)) return GL_RGB4;
			if (mBits == Color4b(5, 5, 5, 0)) return GL_RGB5;
			if (mBits == Color4b(2, 2, 2, 2)) return GL_RGBA2;
			if (mBits == Color4b(4, 4, 4, 4)) return GL_RGBA4;
			if (mBits == Color4b(5, 5, 5, 1)) return GL_RGB5_A1;
			if (mBits == Color4b(10, 10, 10, 2)) return GL_RGB10_A2;
			if (mBits == Color4b(12, 12, 12, 12)) return GL_RGBA12;
			if (mBits == Color4b(0, 0, 0, 4)) return GL_ALPHA4;
			if (mBits == Color4b(0, 0, 0, 8)) return GL_ALPHA8;
			if (mBits == Color4b(0, 0, 0, 12)) return GL_ALPHA12;
			if (mBits == Color4b(0, 0, 0, 16)) return GL_ALPHA16;
			fail();
		case Type::f:
			if (mBits == Color4b(16, 0, 0, 0)) return GL_R16F;
			if (mBits == Color4b(16, 16, 0, 0)) return GL_RG16F;
			if (mBits == Color4b(16, 16, 16, 0)) return GL_RGB16F;
			if (mBits == Color4b(16, 16, 16, 16)) return GL_RGBA16F;
			if (mBits == Color4b(32, 0, 0, 0)) return GL_R32F;
			if (mBits == Color4b(32, 32, 0, 0)) return GL_RG32F;
			if (mBits == Color4b(32, 32, 32, 0)) return GL_RGB32F;
			if (mBits == Color4b(32, 32, 32, 32)) return GL_RGBA32F;
			if (mBits == Color4b(11, 11, 10, 0)) return GL_R11F_G11F_B10F;
			fail();
		default: fail();
		}
		break;
	case PixelFormat::Function::depth:
		switch (mType) {
		case Type::ui:
			if (mBits == Color4b(16, 0, 0, 0)) return GL_DEPTH_COMPONENT16;
			if (mBits == Color4b(24, 0, 0, 0)) return GL_DEPTH_COMPONENT24;
			if (mBits == Color4b(32, 0, 0, 0)) return GL_DEPTH_COMPONENT32;
			fail();
		case Type::f:
			if (mBits == Color4b(32, 0, 0, 0)) return GL_DEPTH_COMPONENT32F;
			fail();
		default: fail();
		}
	case PixelFormat::Function::stencil:
		switch (mType) {
		case Type::ui:
			if (mBits == Color4b(0, 1, 0, 0)) return GL_STENCIL_INDEX1;
			if (mBits == Color4b(0, 4, 0, 0)) return GL_STENCIL_INDEX4;
			if (mBits == Color4b(0, 8, 0, 0)) return GL_STENCIL_INDEX8;
			if (mBits == Color4b(0, 16, 0, 0)) return GL_STENCIL_INDEX16;
			default: fail();
		}
	case PixelFormat::Function::depth_stencil:
		switch (mType) {
		case Type::ui:
			if (mBits == Color4b(24, 8, 0, 0)) return GL_DEPTH24_STENCIL8;
			fail();
		case Type::f_ui:
			if (mBits == Color4b(32, 8, 0, 0)) return GL_DEPTH32F_STENCIL8;
			fail();
		default: fail();
		}
	default: fail();
	}
	
}

GLenum PixelFormat::toGLSimple() const {
	switch (mFunction) {
	case PixelFormat::Function::color:
		switch (getDepth()) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: fail();
		}
	case PixelFormat::Function::depth: return GL_DEPTH_COMPONENT;
	case PixelFormat::Function::stencil: return GL_STENCIL_INDEX;
	case PixelFormat::Function::depth_stencil: return GL_DEPTH_STENCIL;
	default: fail();
	}
}

PixelFormat PixelFormat::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_R8: return PixelFormat(Type::ui, { 8, 0, 0, 0 }, Function::color);
	case GL_R8_SNORM: return PixelFormat(Type::si, { 8, 0, 0, 0 }, Function::color);
	case GL_R16: return PixelFormat(Type::ui, { 16, 0, 0, 0 }, Function::color);
	case GL_R16_SNORM: return PixelFormat(Type::si, { 16, 0, 0, 0 }, Function::color);
	case GL_RG8: return PixelFormat(Type::ui, { 8, 8, 0, 0 }, Function::color);
	case GL_RG8_SNORM: return PixelFormat(Type::si, { 8, 8, 0, 0 }, Function::color);
	case GL_RG16: return PixelFormat(Type::ui, { 16, 16, 0, 0 }, Function::color);
	case GL_RG16_SNORM: return PixelFormat(Type::si, { 16, 16, 0, 0 }, Function::color);
	case GL_R3_G3_B2: return PixelFormat(Type::ui, { 3, 3, 2, 0 }, Function::color);
	case GL_RGB4: return PixelFormat(Type::ui, { 4, 4, 4, 0 }, Function::color);
	case GL_RGB5: return PixelFormat(Type::ui, { 5, 5, 5, 0 }, Function::color);
	case GL_RGB8: return PixelFormat(Type::ui, { 8, 8, 8, 0 }, Function::color);
	case GL_RGB8_SNORM: return PixelFormat(Type::si, { 8, 8, 8, 0 }, Function::color);
	case GL_RGB10: return PixelFormat(Type::ui, { 10, 10, 10, 0 }, Function::color);
	case GL_RGB12: return PixelFormat(Type::ui, { 12, 12, 12, 0 }, Function::color);
	case GL_RGB16_SNORM: return PixelFormat(Type::si, { 16, 16, 16, 0 }, Function::color);
	case GL_RGBA2: return PixelFormat(Type::ui, { 2, 2, 2, 2 }, Function::color);
	case GL_RGBA4: return PixelFormat(Type::ui, { 4, 4, 4, 4 }, Function::color);
	case GL_RGB5_A1: return PixelFormat(Type::ui, { 5, 5, 5, 1 }, Function::color);
	case GL_RGBA8: return PixelFormat(Type::ui, { 8, 8, 8, 8 }, Function::color);
	case GL_RGBA8_SNORM: return PixelFormat(Type::si, { 8, 8, 8, 8 }, Function::color);
	case GL_RGB10_A2: return PixelFormat(Type::ui, { 10, 10, 10, 2 }, Function::color);
	case GL_RGBA12: return PixelFormat(Type::ui, { 12, 12, 12, 12 }, Function::color);
	case GL_RGBA16: return PixelFormat(Type::ui, { 16, 16, 16, 16 }, Function::color);
	case GL_RGBA16_SNORM: return PixelFormat(Type::si, { 16, 16, 16, 16 }, Function::color);
	case GL_R16F: return PixelFormat(Type::f, { 16, 0, 0, 0 }, Function::color);
	case GL_RG16F: return PixelFormat(Type::f, { 16, 16, 0, 0 }, Function::color);
	case GL_RGB16F: return PixelFormat(Type::f, { 16, 16, 16, 0 }, Function::color);
	case GL_RGBA16F: return PixelFormat(Type::f, { 16, 16, 16, 16 }, Function::color);
	case GL_R32F: return PixelFormat(Type::f, { 32, 0, 0, 0 }, Function::color);
	case GL_RG32F: return PixelFormat(Type::f, { 32, 32, 0, 0 }, Function::color);
	case GL_RGB32F: return PixelFormat(Type::f, { 32, 32, 32, 0 }, Function::color);
	case GL_RGBA32F: return PixelFormat(Type::f, { 32, 32, 32, 32 }, Function::color);
	case GL_R11F_G11F_B10F: return PixelFormat(Type::f, { 11, 11, 10, 0 }, Function::color);
	case GL_ALPHA4: return PixelFormat(Type::ui, { 0, 0, 0, 4 }, Function::color);
	case GL_ALPHA8: return PixelFormat(Type::ui, { 0, 0, 0, 8 }, Function::color);
	case GL_ALPHA12: return PixelFormat(Type::ui, { 0, 0, 0, 12 }, Function::color);
	case GL_ALPHA16: return PixelFormat(Type::ui, { 0, 0, 0, 16 }, Function::color);
	case GL_ALPHA8_SNORM: return PixelFormat(Type::si, { 0, 0, 0, 8 }, Function::color);
	case GL_ALPHA16_SNORM: return PixelFormat(Type::si, { 0, 0, 0, 16 }, Function::color);
	case GL_DEPTH_COMPONENT16: return PixelFormat(Type::ui, { 16, 0, 0, 0 }, Function::depth);
	case GL_DEPTH_COMPONENT24: return PixelFormat(Type::ui, { 24, 0, 0, 0 }, Function::depth);
	case GL_DEPTH_COMPONENT32: return PixelFormat(Type::ui, { 32, 0, 0, 0 }, Function::depth);
	case GL_DEPTH_COMPONENT32F: return PixelFormat(Type::f, {32, 0, 0, 0}, Function::depth);
	case GL_STENCIL_INDEX1: return PixelFormat(Type::ui, { 0, 1, 0, 0 }, Function::stencil);
	case GL_STENCIL_INDEX4: return PixelFormat(Type::ui, { 0, 4, 0, 0 }, Function::stencil);
	case GL_STENCIL_INDEX8: return PixelFormat(Type::ui, { 0, 8, 0, 0 }, Function::stencil);
	case GL_STENCIL_INDEX16: return PixelFormat(Type::ui, { 0, 16, 0, 0 }, Function::stencil);
	case GL_DEPTH24_STENCIL8: return PixelFormat(Type::ui, { 24, 8, 0, 0 }, Function::depth_stencil);
	case GL_DEPTH32F_STENCIL8: return PixelFormat(Type::f_ui, { 32, 8, 0, 0 }, Function::depth_stencil);
	default: fail();
	}
}

BufferType::BufferType() :
mValue(Value::undefined)
{}

BufferType::BufferType(Value i_value) :
mValue(i_value)
{}

BufferType::operator bool() const {
	return mValue != Value::undefined;
}

GLenum BufferType::toGL() const {
	switch (mValue) {
	case Value::vertex_attributes: return GL_ARRAY_BUFFER;
	case Value::copy_read: return GL_COPY_READ_BUFFER;
	case Value::copy_write: return GL_COPY_WRITE_BUFFER;
	case Value::draw_command: return GL_DRAW_INDIRECT_BUFFER;
	case Value::compute_command: return GL_DISPATCH_INDIRECT_BUFFER;
	case Value::vertex_indices: return GL_ELEMENT_ARRAY_BUFFER;
	case Value::shader_storage: return GL_SHADER_STORAGE_BUFFER;
	case Value::uniform_data: return GL_UNIFORM_BUFFER;
	default: fail();
	}
}

BufferType BufferType::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_ARRAY_BUFFER: return Value::vertex_attributes;
	case GL_COPY_READ_BUFFER: return Value::copy_read;
	case GL_COPY_WRITE_BUFFER: return Value::copy_write;
	case GL_DRAW_INDIRECT_BUFFER: return Value::draw_command;
	case GL_DISPATCH_INDIRECT_BUFFER: return Value::compute_command;
	case GL_ELEMENT_ARRAY_BUFFER: return Value::vertex_indices;
	case GL_SHADER_STORAGE_BUFFER: return Value::shader_storage;
	case GL_UNIFORM_BUFFER: return Value::uniform_data;
	default: return Value::undefined;
	}
}

DisplayMagnifyMode::DisplayMagnifyMode() :
mValue(Value::undefined)
{}

DisplayMagnifyMode::DisplayMagnifyMode(Value i_value) :
mValue(i_value)
{}

DisplayMagnifyMode::operator bool() const {
	return mValue != Value::undefined;
}

uint DisplayMagnifyMode::toWin() const {
	switch (mValue) {
	case Value::sys_default: return DMDFO_DEFAULT;
	case Value::stretch: return DMDFO_STRETCH;
	case Value::center: return DMDFO_CENTER;
	default: fail();
	}
}

DisplayMagnifyMode DisplayMagnifyMode::fromWin(uint i_value) {
	switch (i_value) {
	case DMDFO_DEFAULT: return Value::sys_default;
	case DMDFO_CENTER: return Value::center;
	case DMDFO_STRETCH: return Value::stretch;
	default: return Value::undefined;
	}
}

ShaderType::ShaderType() :
mValue(ShaderType::mValue)
{}

ShaderType::ShaderType(Value i_value) :
mValue(i_value)
{}

ShaderType::operator bool() const {
	return mValue != ShaderType::Value::undefined;
}

GLenum ShaderType::toGL() const {
	switch (mValue) {
	case Value::vertex: return GL_VERTEX_SHADER;
	case Value::fragment: return GL_FRAGMENT_SHADER;
	case Value::geometry: return GL_GEOMETRY_SHADER;
	case Value::tess_control: return GL_TESS_CONTROL_SHADER;
	case Value::tess_eval: return GL_TESS_EVALUATION_SHADER;
	case Value::compute: return GL_COMPUTE_SHADER;
	default: fail();
	}
}

ShaderType ShaderType::fromGL(GLenum i_value) {
	switch (i_value) {
	case GL_VERTEX_SHADER: return Value::vertex;
	case GL_FRAGMENT_SHADER: return Value::fragment;
	case GL_GEOMETRY_SHADER: return Value::geometry;
	case GL_TESS_CONTROL_SHADER: return Value::tess_control;
	case GL_TESS_EVALUATION_SHADER: return Value::tess_eval;
	case GL_COMPUTE_SHADER: return Value::compute;
	default: return Value::undefined;
	}
}

GLbitfield ClearBits::toGL(uint i_value) {
	GLbitfield out = 0;
	if (i_value & color) out |= GL_COLOR_BUFFER_BIT;
	if (i_value & depth) out |= GL_DEPTH_BUFFER_BIT;
	if (i_value & stencil) out |= GL_STENCIL_BUFFER_BIT;
	return out;
}

uint ClearBits::fromGL(GLuint i_value) {
	uint out = 0;
	if (i_value & GL_COLOR_BUFFER_BIT) out |= color;
	if (i_value & GL_DEPTH_BUFFER_BIT) out |= depth;
	if (i_value & GL_STENCIL_BUFFER_BIT) out |= stencil;
	return out;
}

ReadWrite::ReadWrite() :
mRead(false),
mWrite(false)
{}

ReadWrite::ReadWrite(bool i_read, bool i_write) :
mRead(i_read),
mWrite(i_write)
{}

bool ReadWrite::operator==(const ReadWrite & i_other) {
	return mRead == i_other.mRead && mWrite == i_other.mWrite;
}

bool ReadWrite::operator!=(const ReadWrite& i_other) {
	return !(*this == i_other);
}

GLbitfield ReadWrite::toMapGL() const {
	GLbitfield out = 0;
	if (mRead) out |= GL_MAP_READ_BIT;
	if (mWrite) out |= GL_MAP_WRITE_BIT;
	return out;
}

ReadWrite ReadWrite::fromMapGL(GLbitfield i_value) {
	return ReadWrite(i_value & GL_MAP_READ_BIT, i_value & GL_MAP_WRITE_BIT);
}
