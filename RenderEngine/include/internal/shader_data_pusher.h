#ifndef SHADER_DATA_PUSHER_H
#define SHADER_DATA_PUSHER_H

#include "gpu_buffer.h"

using shader_data_slot_t = uint;

struct ShaderDataLayout {
	BufferUsage::Frequency mFreq;
	ReadWrite mReadWrite;

	bool operator==(const ShaderDataLayout& i_other) {
		return mFreq == i_other.mFreq && mReadWrite == i_other.mReadWrite;
	}
	bool operator!=(const ShaderDataLayout& i_other) {
		return !(*this == i_other);
	}
};

struct ShaderData {
	ShaderDataLayout mLayout;
	const void* mData;
	uint mSize;
};

class ShaderDataPusher {
	friend class RenderEngine;
	friend class VirtualShaderDataPusher;
private:
	GpuBuffer mBuffer;
	ShaderDataLayout mLayout;
	uint mSize;

public:
	~ShaderDataPusher();

private:
	ShaderDataPusher();
	ShaderDataPusher(const ShaderDataLayout& i_layout, uint i_size);

	void init(const ShaderDataLayout& i_layout, uint i_size);
	void _init();
	void destroy();
	bool isValid() const;

	void finalize();

protected:
	void _activate(shader_data_slot_t i_slot);
};

#endif