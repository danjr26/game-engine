#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "mesh_instance.h"
#include "shader.h"

class ParticleSystem;
class MeshBlanket;
class Emitter;
class PlayerShip;

template class IndexedArray<ParticleSystem>;
class ParticleSystemManager : public RenderComponent {
public:
	static ParticleSystemManager* active;

	IndexedArray<ParticleSystem> systems;

	int*				pcls;
	ParticleSystem**	parents;
	float*				dists;

	Vector3f*			pcllocs;
	Vector3f*			pclrigs;
	Vector3f*			pclups;
	Color4f*			pclcols;
	Vector2f*			pcltxc1s;
	Vector2f*			pcltxc2s;

	GLuint vertexArrayID, positionBuffer, rigbuffer, upbuffer, colorBuffer, txc1buffer, txc2buffer;

	int					npcls;
	int					maxpcls;

	ParticleSystemManager();
	~ParticleSystemManager();
	void			Init_Arrays		();
	virtual void	Render			(Flags callflags);
	void			Add				(ParticleSystem* system);
	void			Remove			(ParticleSystem* system);
	void			Calc_Dists		();
	void			Sort			();
	void			Prepare_Arrays	();
};

class ParticleData {
public:
	ParticleSystem* parent;

	void**	elements;
	bool*	isstatic;
	short*	sizes;
	int		maxelements;

	int		npcls;
	int		maxpcls;

							ParticleData	(ParticleSystem* parent, int maxpcls, int maxelements);
							~ParticleData	();
	template <class T> void Add_Element		(int index);
	template <class T> void	Add_Element		(int index, T value);
	template <class T> T	Get				(int arrayindex, int pclindex);
	template <class T> void Set				(int arrayindex, int pclindex, T value);
	void					Copy			(int arrayindex, int pclindex1, int pclindex2);
	void					Add_Particle	();
	void					Remove_Particle	(int index);
	void					Remove_Element	(int index);
	template <class T> void Make_Static		(int index, T value);
	void Make_Static	(int index);
	void Make_Unstatic	(int index);
};

enum {
	PS_LOC,
	PS_ORI,
	PS_DIM,
	PS_COL,
	PS_TEXCOO1,
	PS_TEXCOO2, 
	PS__END
};

enum {
	FACE_CAM,
	FACE_CAM_RES,
	FACE_IND
};

class ParticleSystem : public MechanicalComponent {
public:
	ParticleData	data;
	Transform3d		transform;
	Texture2*		texture;
	BlendSettings	blend;
	int				psmindex;
	int				face;

					ParticleSystem	(Transform3d transform, Texture2* texture, BlendSettings blend, 
						int maxpcls, int nelem, double step, int face);
	virtual			~ParticleSystem	();
	virtual void	Update			(double t) override;
};

//Instance classes

class SpaceDust : public ParticleSystem {
public:
			SpaceDust	();
			~SpaceDust	();
	void	Update		(double t) override;
};

enum {
	EM_LINVEL = PS__END,
	EM_ROTVEL,
	EM_DIMVEL,
	EM_LINACC,
	EM_ROTACC,
	EM_DIMACC,
	EM_STATIM,
	EM_LIFTIM,
	EM_COLPAT,
	EM__END
};
 
struct ColorKeyframe4f {
	float	time;
	Color4f color;
};

struct ColorPath4f {
	int				nframes;
	ColorKeyframe4f frames[4];
};

class EmitterGenerator {
public:
	Emitter* parent;
	EmitterGenerator();
	virtual void Generate(int n);
};

class EmitterPopulator {
public:
	Emitter* parent;
	EmitterPopulator();
	virtual int New_Count(double t);
};

struct EmitterSettings {
	EmitterGenerator*	generator;
	EmitterPopulator*	populator;
	Transform3d			transform;
	Texture2*			texture;
	BlendSettings		blend;
	int					maxpcls;
	int					npcls;
	bool				statics[EM__END];
	int					face;
	bool				dieonempty;
};

class Emitter : public ParticleSystem {
public:
	EmitterSettings settings;
					Emitter	(EmitterSettings settings, double step);
	virtual			~Emitter();
	virtual void	Update			(double t);
};

class ExplosionSmokeGen : public EmitterGenerator {
public:
	float size;
	ExplosionSmokeGen(float size);
	virtual void Generate(int n);
};

class ExplosionFlameGen : public EmitterGenerator {
public:
	float size;
	ExplosionFlameGen(float size);
	virtual void Generate(int n);
};

class ExplosionFlashGen : public EmitterGenerator {
public:
	float size;
	ExplosionFlashGen(float size);
	virtual void Generate(int n);
};

class ExplosionSparksGen : public EmitterGenerator {
public:
	float size;
	ExplosionSparksGen(float size);
	virtual void Generate(int n);
};

class ExplosionSparkTrailsGen : public EmitterGenerator {
public:
	float size;
	ExplosionSparkTrailsGen(float size);
	virtual void Generate(int n);
};

class ExplosionSmokeTrailsGen : public EmitterGenerator {
public:
	float size;
	ExplosionSmokeTrailsGen(float size);
	virtual void Generate(int n);
};

class LaserRecoilGen : public EmitterGenerator {
public:
	LaserRecoilGen();
	virtual void Generate(int n);
};

class PulseRecoilGen : public EmitterGenerator {
public:
	PulseRecoilGen();
	virtual void Generate(int n);
};

class LaserFlashGen : public EmitterGenerator {
public:
	LaserFlashGen();
	virtual void Generate(int n);
};

class PulseFlashGen : public EmitterGenerator {
public:
	PulseFlashGen();
	virtual void Generate(int n);
};

class PlayerShipTrailGen : public EmitterGenerator {
public:
	PlayerShip* parentship;
	PlayerShipTrailGen(PlayerShip* parentship);
	virtual void Generate(int n);
};

class PlayerShipTrailPop : public EmitterPopulator {
public:
	Stepper stepper;
	PlayerShipTrailPop();
	virtual int New_Count(double t);
};

enum {
	MB_AMP = PS__END,
	MB_PER,
	MB_WAVCOO,
	MB_NOR,
	MB_NOD,
	MB__END
};

class MeshBlanketGenerator {
public:
	MeshBlanket* parent;
	MeshBlanketGenerator();
	virtual void Generate(int n);
};

struct MeshBlanketSettings {
	MeshBlanketGenerator*	generator;
	MeshInstance*			parent;
	Texture2*				texture;
	BlendSettings			blend;
	int						npcls;
	bool					statics[MB__END];
};

class MeshBlanket : public ParticleSystem {
public:
	MeshBlanketSettings settings;

					MeshBlanket		(MeshBlanketSettings settings, double step);
					~MeshBlanket	();
	virtual void	Update			(double t);
};

class PlayerShipShieldMesBlaGen : public MeshBlanketGenerator {
public:
	PlayerShipShieldMesBlaGen();
	virtual void Generate(int n);
};

struct PixelSwarmSettings {

};

class PixelSwarm : public ParticleSystem {
public:

};


#endif
