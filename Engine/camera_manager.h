#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "asset_manager.h"
#include "camera.h"
#include <unordered_map>

class CameraManager {
public:
	enum ID {
		main,
		mai_overlay,
		user_defined
	};

private:
	Camera* mActive;
	std::unordered_map<uint, Camera*> mCameras;

public:
	CameraManager();

	void setActive(uint i_id);
	void setActive(Camera* i_camera);
	Camera* getActive();
	
	void add(uint i_id, Camera* i_camera);
	void remove(uint i_id);
	Camera* get(uint i_id);
};

#endif