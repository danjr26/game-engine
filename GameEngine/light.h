#ifndef LIGHT_H
#define LIGHT_H

#include "component.h"
#include "texture.h"

namespace LightFlags {
	enum: Flags {
		point =				0x00,
		directional =		0x01,
		on =				0x00,
		off =				0x02,
		ambientChange =		0x04,
		diffuseChange =		0x08,
		specularChange =	0x10,
		positionChange =	0x20,
		toggleChange =		0x40,
		changes = ambientChange | diffuseChange | specularChange | positionChange | toggleChange,
		settings = point | directional | on | off,
		sourceBit = point | directional,
		onOffBit = on | off
	};
};

class Light {
	friend class LightManager;

private:
	Color4f		ambient;
	Color4f		diffuse;
	Color4f		specular;
	Vector3f	position;
	uint		lightManagerArrayIndex;
	Flags		flags;

public:
			Light		(Color4f in_ambient, Color4f in_diffuse, Color4f in_specular, Vector3f in_position, Flags in_flags);
			~Light		();
	void	Set_Ambient	(Color4f in_ambient);
	void	Set_Diffuse	(Color4f in_diffuse);
	void	Set_Specular(Color4f in_specular);
	void	Set_Position(Vector3f in_position);
	void	Set_Source	(Flags in_source);
	void	Set_Toggle	(Flags in_toggle);

private:
	void Render	();
};

template class IndexedArray<Light>;
class LightManager : public RenderComponent {
public:
	static const uint CAPACITY = 8;
private:
	IndexedArray<Light> lights;

public:
			LightManager	();
			~LightManager	();
	void	Add				(Light* in_light);
	void	Remove			(Light* in_light);
	void	Render			(Flags in_callflags) override final;
};

class DirectionalShadowMap {
public:

};

#endif