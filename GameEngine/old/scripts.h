#ifndef SCRIPTS_H
#define SCRIPTS_H
#include "component.h"
#include "parse.h"
#include "overlay.h"
class MeshInstance;

class Script : public MechanicalComponent {
public:
	Script();
	virtual void Update(double t);
};

class InitConfigFile {
public:
	const static string path;
	const static string name;

	string	meshpath;
	string	fontpath;
	string	texturepath;
	string	collpath;
	string* models;
	string* fonts;
	string* textures;
	string* colls;
	int		nmeshes;
	int		nfonts;
	int		ntextures;
	int		ncolls;

	InitConfigFile	();
	~InitConfigFile	();
};

class InitScript : public Script {
public:
	InitConfigFile file;
					InitScript		();
	void			Init_OpenGL		();
	void			Init_Meshes		();
	void			Init_Fonts		();
	void			Init_Textures	();
	void			Init_Misc		();
	virtual void	Update			(double t);
};

class IntroScript : public Script {
public:
	GLuint vao;
	IntroScript();
	virtual void Update(double t);
};

class MainMenuScript : public Script {
public:
	static MainMenuScript* active;
	ScreenFade*			fade;
	Rect2D*				background;
	SimpleRectButton*	playbutton;
	bool				isdone;

					MainMenuScript	();
					~MainMenuScript	();
	virtual void	Update			(double t);
	static void		Play			(void* arg);
};

class LevelSelectScript : public Script {
public:
	static LevelSelectScript* active;
	ScreenFade*			fade;
	Rect2D*				background;
	SimpleRectButton**	levelbuttons;
	int					nlevelbuttons;
	Text2D*				titletext;
	Text2D*				levelinfo;
	int					destination;
	bool				isdone;

					LevelSelectScript	();
					~LevelSelectScript	();
	virtual void	Update				(double t);
	static void		Play_Level			(void* arg);
};

class LevelRewardScript : public Script {
public:
	LevelRewardScript();
	virtual void Update(double t);
};

class CustomizeScript : public Script {
public:
	static CustomizeScript* active;
	ScreenFade*			fade;
	Rect2D*				background;
	SimpleRectButton*	continuebutton;
	int					level;
	bool				isdone;

					CustomizeScript	(int level);
					~CustomizeScript();
	virtual void	Update			(double t);
	static void		Continue		(void* arg);
};

class Level1Script : public Script {
public:
	PlayerShip*		playership;
	MeshInstance*	enforcer;

	Level1Script();
	virtual void Update(double t);
};

#endif
