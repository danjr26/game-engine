#ifndef PERLIN_LIGHTNING_BOLT_H
#define PERLIN_LIGHTNING_BOLT_H

#include "fractal_perlin.h"
#include "ribbon2.h"

class PerlinLightningBolt {
private:
	Ribbon2 mRibbon;

public:
	PerlinLightningBolt(const Vector2d& in_point1, const Vector2d& in_point2);
	~PerlinLightningBolt();

	void update();
};

#endif