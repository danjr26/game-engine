#ifndef INDUSTRIAL_LASER_BEAM_H
#define INDUSTRIAL_LASER_BEAM_H

#include <unordered_set>
#include <functional>
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "rigid_body.h"
#include "sprite.h"
#include "basic_collision_mask.h"

class IndustrialLaserBeam : public DeepTransformableObject2d, public PerFrameUpdateableObject,
	public DestructableObject {

private:
	std::unordered_set<CollisionMask2d*> mIgnoreMasks;
	LineSegment2CollisionMask<double> mCollisionMask;
	Sprite mSprite;
	std::function<bool(CollisionMask2d*)> mRaycastFilter;
	double mRadius;

public:
	IndustrialLaserBeam();
	~IndustrialLaserBeam();

	void update(double in_dt) override;

	CollisionMask2d& getCollisionMask();
	void setFilter(std::function<bool(CollisionMask2d*)> in_filter);
	void setIgnoreMasks(std::unordered_set<CollisionMask2d*>& in_masks);
	std::unordered_set<CollisionMask2d*>& getIgnoreMasks();
};

#endif