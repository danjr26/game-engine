#include "../../RenderEngine/include/render.h"
#include "../../Geometry/include/geometry.h"
#include "../../Utilities/include/utilities.h"

#include <iostream>
#include <windows.h>
#undef min
#undef max

void printAllDisplayInfo();
void setupConsole();

int WINAPI WinMain(HINSTANCE i_instance, HINSTANCE i_prevInstance, LPSTR i_commandLine, int i_showCommand) {

	setupConsole();

	RenderEngine renderEngine;
	renderEngine.mWindow.init();
	renderEngine.reportErrors();
	renderEngine.mWindow.show();

	RenderPass pass;
	pass.mRenderTarget = &renderEngine.mWindow;
	pass.mClearCommand.mBits = ClearBits::color | ClearBits::depth;
	pass.mClearCommand.mColor = Color4f(0.0, 0.0, 0.8, 1.0);
	pass.mClearCommand.mDepth = 0.0;
	auto& passNode = renderEngine.mPasses.addNode(&pass);

	VertexPusherLayout layout;
	layout.mTopology = Topology::Value::triangles;
	layout.mIndex.mType = PushableType::Value::ui16;
	layout.mIndex.mFreq = BufferUsage::Frequency::seldom;
	VertexLayout sublayout;
	sublayout.mDepth = 3;
	sublayout.mFreq = BufferUsage::Frequency::seldom;
	sublayout.mType = PushableType::Value::f32;
	layout.mVertex.insert(decltype(layout.mVertex)::value_type(1, sublayout));

	RenderRequest request;
	
	renderEngine.requestVertexPusher(request, layout, 3, 3);

	uint16_t* indicesEdit = (uint16_t*)request.mVertices->editIndices();
	indicesEdit[0] = 0;
	indicesEdit[1] = 1;
	indicesEdit[2] = 2;
	request.mVertices->commitIndices(indicesEdit);

	Vector3f* verticesEdit = (Vector3f*)request.mVertices->editVertices(1);
	verticesEdit[0] = Vector3f(0, 0, 0);
	verticesEdit[1] = Vector3f(1, 0, 0);
	verticesEdit[2] = Vector3f(0, 1, 0);
	request.mVertices->commitVertices(1, verticesEdit);

	renderEngine.mRequests.push_back(&request);
	
	renderEngine.render();

	renderEngine.mWindow.flipBuffers();

	std::this_thread::sleep_for(std::chrono::seconds(5));

	return 0;
}

void printAllDisplayInfo() {
	DISPLAY_DEVICEA mDevice;
	DEVMODEA mMode;
	mDevice.cb = sizeof(mDevice);
	mMode.dmSize = sizeof(mMode);
	mMode.dmDriverExtra = 0;
	for (uint i = 0; EnumDisplayDevicesA(NULL, i, &mDevice, 0); i++) {
		std::cout
			<< "name: " << mDevice.DeviceName << std::endl
			<< "desc: " << mDevice.DeviceString << std::endl
			<< "attr: "
				<< ((mDevice.StateFlags & DISPLAY_DEVICE_ACTIVE) ? "active " : "")
				<< ((mDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) ? "mirror " : "")
				<< ((mDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) ? "primary " : "")
				<< ((mDevice.StateFlags & DISPLAY_DEVICE_REMOVABLE) ? "removable " : "")
				<< std::endl;
		for (uint j = 0; EnumDisplaySettingsA(mDevice.DeviceName, j, &mMode); j++) {
			std::cout 
			<< "  " << mMode.dmBitsPerPel << "bit " 
			<< mMode.dmPelsWidth << "x" << mMode.dmPelsHeight << " "
			<< "@" << mMode.dmDisplayFrequency << "Hz ";
			if ((mMode.dmFields & DM_DISPLAYFIXEDOUTPUT)) {
				switch (mMode.dmDisplayFixedOutput) {
				case DMDFO_DEFAULT: std::cout << "fixed-default "; break;
				case DMDFO_CENTER: std::cout << "fixed-center "; break;
				case DMDFO_STRETCH: std::cout << "fixed-stretch "; break;
				default: std::cout << "fixed-unknown "; break;
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		ZeroMemory(&mDevice, sizeof(mDevice));
	}
}

void setupConsole() {
	//AllocConsole();
	FILE* fin, *fout;
	//freopen_s(&fin, "CONIN$", "r", stdin);
	freopen_s(&fout, "dbgout.txt", "w", stdout);
}
