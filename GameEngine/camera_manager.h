#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "asset_manager.h"
#include "camera.h"

class CameraManager {
public:
	Camera* active;

public:
	CameraManager();
};

#endif