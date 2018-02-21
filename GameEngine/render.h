#ifndef RENDER_H
#define RENDER_H
//#define GLEW_STATIC
#include <Windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include "component.h"
#include "texture.h"
#include "camera.h"
#include "vectors.h"
#include "light.h"
#include "fx.h"
#include "particle_system.h"
#include "text.h"
#include "shader.h"
#include "overlay.h"
#include <string>
#include "trq.h"

class RenderTR;

class Window : MutexProtected {
private:
	HWND		hwnd;
	WNDCLASSEX	winclass;
	HDC			hdc;
	HGLRC		hglrc;

public:
				Window		(HINSTANCE hinst, Vector2i dim, std::wstring name);
				~Window		();
	void		Flip_Buffers();
	Vector2i	Get_Pos		();
	Vector2i	Get_Dim		();	
	Vector2i	Get_Full_Pos();
	Vector2i	Get_Full_Dim();
	void		Set_Position(Vector2i pos);
	void		Set_Dim		(Vector2i dim);
	HWND		Get_Handle	();
};


namespace RenderType {
	enum : Flags {
		beforecam =		0x01,
		first =			0x02,
		opaque =		0x04,
		transparent =	0x08,
		shadow =		0x10,
		last =			0x20
	};
};

class RenderManager {
private:
	IndexedArray 
		<RenderComponent>	components;

	RenderTR				timeRequest;

	RenderContext			context;
	Camera					camera;
	LightManager			lightmanager;
	OverlayManager			overlaymanager;
	LaserManager			lasermanager;
	ParticleSystemManager	psmanager;
	TextManager				textmanager;

	Framebuffer*			fb1;
	Framebuffer*			fb2;

	Vector2i				resolution;

public:
			RenderManager	(Vector2i resolution, double step);
			~RenderManager	();
	void	Render_Frame	();
	void	Add				(RenderComponent* component);
	void	Remove			(RenderComponent* component);

	Camera&					Camera	();
	LightManager&			Light	();
	OverlayManager&			Overlay	();
	LaserManager&			Laser	();
	ParticleSystemManager&	Particle();
	TextManager&			Text	();

	Vector2i	Get_Resolution			();
	double		Get_Target_Frame_Freq	();
	double		Get_Target_Frame_Rate	();
};

#endif
