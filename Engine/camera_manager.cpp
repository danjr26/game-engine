#include "camera_manager.h"
#include "log.h"

CameraManager::CameraManager() :
mActive(nullptr)
{}

void CameraManager::setActive(uint i_id) {
	mActive = mCameras[i_id];
}

void CameraManager::setActive(Camera* i_camera) {
	mActive = i_camera;
}

Camera* CameraManager::getActive() {
	return mActive;
}

void CameraManager::add(uint i_id, Camera* i_camera) {
	mCameras[i_id] = i_camera;
}

void CameraManager::remove(uint i_id) {
	mCameras.erase(i_id);
}

Camera* CameraManager::get(uint i_id) {
	return mCameras[i_id];
}

