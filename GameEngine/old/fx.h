#ifndef FX_H
#define FX_H

#include "mesh_instance.h"
#include "clock.h"

class AlienEntrance : public MechanicalComponent {
private:
	Mesh*				meshData;
	MeshInstance**		models;
	TransitionTracker	tracker;
	double				timeAccumulation;

public:
	AlienEntrance	(Vector3d in_position, float in_radius);
	~AlienEntrance	();
	void Update		(double in_timePassed) override;
};

class LightningBolt : public RenderComponent, public MechanicalComponent {
private:
	Vector3d*	vertices;
	uint		nVertices;
	double		thickness;

public:
				LightningBolt	(Vector3d in_position1, Vector3d in_position2);
				~LightningBolt	();
	void		Render			(Flags in_renderFlags) override;
	void		Update			(double in_timePassed) override;

private:
	void		Calculate_Vertices	();
	Vector3d*	Get_Quads			();
};

class GlowLaser {
friend class LaserManager;
private:
	uint		laserManagerArrayIndex;
	Transform3d	transform;
	double		length;
	double		width;
	Color4f		color;

public:
			GlowLaser	(Transform3d in_transform, double in_length, double in_width, Color4f in_color);
			~GlowLaser	();

private:
	void	Calculate_Vertices();
};

class LaserManager : public RenderComponent {
friend class GlowLaser;
friend class RenderManager;
public:
	static const uint CAPACITY;
private:
	IndexedArray<GlowLaser> components;

	GLuint 
		vertexArrayID,
		positionBuffer,
		colorBuffer, 
		uvBuffer, 
		widthBuffer;

	Vector3f*	positions;
	Color4f*	colors;
	Vector2f*	uvs;
	float*		widths;

public:
			LaserManager	();
			~LaserManager	();
	void	Add				(GlowLaser* in_component);
	void	Remove			(GlowLaser* in_component);
	void	Render			(Flags in_callFlags) override;
	void	Init_Arrays		();
};

/*class CameraShake : public RenderComponent, public MechanicalComponent {
public:
	ProgressTracker	progressTracker;
	double			mag;
	double			period;
	double			decrate; 
	Vector3d		offset;

			CameraShake	(double mag, double period, double decrate);
			~CameraShake();
	void	Render		(Flags callflags) override;
	void	Update		(double t) override;
};*/



#endif