#include "..\include\internal\shader_data_pusher.h"

ShaderDataPusher::ShaderDataPusher() :
mBuffer(),
mLayout(),
mSize(0)
{}

ShaderDataPusher::ShaderDataPusher(const ShaderDataLayout& i_layout, uint i_size) :
mBuffer(),
mLayout(i_layout),
mSize(i_size) {
	_init();
}

ShaderDataPusher::~ShaderDataPusher() {
	destroy();
}

void ShaderDataPusher::init(const ShaderDataLayout& i_layout, uint i_size) {
	if (isValid()) fail();
	mLayout = i_layout;
	mSize = i_size;
	_init();
}

void ShaderDataPusher::_init() {
	BufferUsage usage(
		(mLayout.mReadWrite.mRead) ? BufferUsage::Frequency::often : BufferUsage::Frequency::seldom, 
		(mLayout.mReadWrite.mRead) ? BufferUsage::DataLocale::user : BufferUsage::DataLocale::renderer,
		(mLayout.mReadWrite.mWrite) ? BufferUsage::Frequency::often : BufferUsage::Frequency::seldom,
		BufferUsage::DataLocale::user
	);
	mBuffer.init(mSize, BufferType::Value::shader_storage, usage, mLayout.mReadWrite);
}

void ShaderDataPusher::destroy() {
	if (!isValid()) return;

	mBuffer.destroy();

	*this = ShaderDataPusher();
}

bool ShaderDataPusher::isValid() const {
	return mBuffer.isValid();
}

void ShaderDataPusher::finalize() {
	mBuffer.finalize();
}

void ShaderDataPusher::_activate(shader_data_slot_t i_slot) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i_slot, mBuffer.getInternalId());
}
