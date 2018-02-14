#ifndef OVERLAY_H
#define OVERLAY_H

#include "component.h"
#include "text.h"
#include "shader.h"

class PlayerShip;

template class IndexedArray<OverlayComponent>;
class OverlayManager : public RenderComponent {
public:
	static OverlayManager* active;
	IndexedArray<OverlayComponent> components;

			OverlayManager	();
			~OverlayManager	();
	void	Add				(OverlayComponent* component);
	void	Remove			(OverlayComponent* component);
	void	Sort			();
	void	Render			(Flags callflags) override final;
	void	Process_Click	(Vector2d coords);
};

namespace Rect2DFlags {
	enum : Flags {
		clickopaque =	0x01,
		locchange =		0x02,
		colchange =		0x04,
		txcchange =		0x08,

		changes = locchange | colchange | txcchange
	};
}

class Rect2D : public OverlayComponent {
public:
	Transform2d		transform;
	double			depth;

	Texture2D*		tex;
	Color4f			col;
	BlendSettings	blend;

	GLuint			vertexArrayID, positionBuffer, uvBuffer, colorBuffer;

	Flags			flags;

public:
						Rect2D			(Transform2d transform, double depth, Texture2D* tex, Color4f col, uint maxchildren);
						~Rect2D			();
	virtual void		Render			();
	virtual bool		Process_Click	(Vector2d coords);
	virtual Vector3d	Get_Loc			();
	void				Init_Arrays		();
};


class ScreenFade : public Rect2D, public MechanicalComponent {
public:
	Color4f color1;
	Color4f color2;
	double	timeAccumulation;
	double	lifet;

					ScreenFade	(Texture2D* tex, Color4f color1, Color4f color2, double lifet);
					~ScreenFade	();
	void			Reset		();
	void			Reverse		();
	virtual void	Update		(double t);
};

struct SimpleRectButtonSettings {
	Texture2D*	tex;
	Color4f		color;
	Color4f		textcolor;
	
	Color4f		hovercolor;
	Color4f		texthovercolor;
	double		hovert;

	Color4f		clickcolor;
	Color4f		textclickcolor;
	double		clickt;
};

class SimpleRectButton : public Rect2D, public MechanicalComponent {
public:
	SimpleRectButtonSettings settings;
	Text2D* text;
	bool hovering;
	void(*On_Click)(void*);
	void* arg;

	Color4f rectcolor1;
	Color4f rectcolor2;

	Color4f textcolor1;
	Color4f textcolor2;

	double colorstartt;
	double colorlifet;


					SimpleRectButton	(Transform2d transform, double depth, SimpleRectButtonSettings settings, Text2D* text, void(*On_Click)(void*), void* arg);
					~SimpleRectButton	();
	virtual void	Update				(double t);
	virtual void	Process_Hover		();
	virtual void	Process_Unhover		();
	virtual bool	Process_Click		(Vector2d coords);
};

class PlayerShipSpeedometer : public OverlayComponent, public MechanicalComponent {
public:
	PlayerShip* parent;
	Text2D*		text;

					PlayerShipSpeedometer	(PlayerShip* parent, double step);
	virtual			~PlayerShipSpeedometer	();
	virtual void	Update					(double t);
};

class PlayerShipSystemsStatus : public OverlayComponent, public MechanicalComponent {
public:
	PlayerShip* parent;
	Rect2D* lasergunicon;
	Rect2D* missilelaunchericon;
	Rect2D* propulsionicon;
	Rect2D* reactoricon;
	Rect2D* repairericon;
	Rect2D* shieldicon;
	
			PlayerShipSystemsStatus	(PlayerShip* parent, double step);
			~PlayerShipSystemsStatus()			override final;
	void	Update					(double t)	override final;
};

class Reticule : public Rect2D, public MechanicalComponent {
public:
	PlayerShip* parent;

	Reticule(PlayerShip* parent);
	~Reticule() override final;
	void Update(double t) override final;
};

class Cursor : public Rect2D, public MechanicalComponent {
public:
	Cursor();
	~Cursor() override final;
	void Update(double t) override final;
};

bool Is_In_Rect(Transform2d transform, Vector2d v);

#endif
