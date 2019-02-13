#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "asset_manager.h"
#include "camera.h"
#include <unordered_map>

class CameraManager {
public:
	enum ID {
		main,
		main_overlay,
		user_defined
	};

private:
	Camera* mActive;
	std::unordered_map<uint, Camera*> mCameras;

public:
	CameraManager();

	void setActive(uint in_id);
	void setActive(Camera* in_camera);
	Camera* getActive();
	
	void add(uint in_id, Camera* in_camera);
	void remove(uint in_id);
	Camera* get(uint in_id);
};

#endif