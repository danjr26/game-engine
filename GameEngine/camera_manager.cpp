#include "camera_manager.h"
#include "log.h"

CameraManager::CameraManager() :
mActive(nullptr)
{}

void CameraManager::setActive(uint in_id) {
	mActive = mCameras[in_id];
}

void CameraManager::setActive(Camera* in_camera) {
	mActive = in_camera;
}

Camera* CameraManager::getActive() {
	return mActive;
}

void CameraManager::add(uint in_id, Camera* in_camera) {
	mCameras[in_id] = in_camera;
}

void CameraManager::remove(uint in_id) {
	mCameras.erase(in_id);
}

Camera* CameraManager::get(uint in_id) {
	return mCameras[in_id];
}

