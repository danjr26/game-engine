#ifndef PERLi_LIGHTNING_BOLT_H
#define PERLi_LIGHTNING_BOLT_H

#include "fractal_perlin.h"
#include "ribbon2.h"

class PerlinLightningBolt {
private:
	Ribbon2 mRibbon;

public:
	PerlinLightningBolt(const Vector2d& i_point1, const Vector2d& i_point2);
	~PerlinLightningBolt();

	void update();
};

#endif