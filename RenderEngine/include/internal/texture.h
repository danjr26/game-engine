#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../Geometry/include/geometry.h"
#include "enum_wrappers.h"

using texture_slot_t = uint;

class Texture {
friend class RenderEngine;
public:
	struct Settings {
		struct {
			Swizzle r;
			Swizzle g;
			Swizzle b;
			Swizzle a;
		} mSwizzle;

		struct {
			EdgeBehavior s;
			EdgeBehavior t;
			EdgeBehavior r;
		} mEdgeMode;

		struct {
			TextureFilter mMinify;
			TextureFilter mMagnify;
		} mFilter;

		Settings();
	};

private:
	GLuint mId;
	TextureType mType;
	PixelFormat mLocalFormat;
	PixelFormat mGPUFormat;
	Vector4ui mDimen; // (x, y, array, face)
	ubyte* mData;
	Settings mSettings;

public:
	Texture(TextureType i_type, PixelFormat i_localFormat, PixelFormat i_gpuFormat, const Vector4ui& i_dimen);
	~Texture();

	void* getData(uint i_arrayIndex = 0, CubeMapFace i_cubeFace = CubeMapFace());
	size_t getDataSize() const;

	void pushData();
	void pullData();

	Settings& getSettings();

	void pushSettings();

	void clearLocal();
	void clearGPU();

	void loadFile(const std::string& i_filename, uint i_arrayIndex = 0, CubeMapFace i_cubeFace = CubeMapFace());
	void loadData(void* i_data, uint i_arrayIndex = 0, CubeMapFace i_cubeFace = CubeMapFace());
	void loadBlank(void* i_color, uint i_arrayIndex = 0, CubeMapFace i_cubeFace = CubeMapFace());

private:
	template<class T>
	void _loadFile(const std::string& i_filename, uint i_arrayIndex, CubeMapFace i_cubeFace);

	void _bind(texture_slot_t i_slot);
};

#endif

