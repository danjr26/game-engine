#ifndef INDUSTRIAL_LASER_BEAM_H
#define INDUSTRIAL_LASER_BEAM_H

#include <unordered_set>
#include <functional>
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "rigid_body.h"
#include "sprite.h"
#include "basic_collider.h"
#include "collision_queue.h"

class IndustrialLaserBeam : 
	public DeepTransformableObject2d, 
	public PerFrameUpdateableObject,
	public DestructableObject {

private:
	std::unordered_set<Collider2d*> mIgnoreMasks;
	Sprite mSprite;
	std::function<bool(Collider2d*)> mRaycastFilter;
	double mRadius;
	bool mIsOn;

public:
	IndustrialLaserBeam();
	~IndustrialLaserBeam();

	void update(double i_dt) override;

	void setFilter(std::function<bool(Collider2d*)> i_filter);
	void setIgnoreMasks(std::unordered_set<Collider2d*>& i_masks);
	std::unordered_set<Collider2d*>& getIgnoreMasks();

	void setToggle(bool i_value);
};

#endif