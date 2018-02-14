#include "light.h"
#include "framework.h"

Light::Light(Color4f in_ambient, Color4f in_diffuse, Color4f in_specular, Vector3f in_position, Flags in_flags) :
ambient	(in_ambient),
diffuse	(in_diffuse),
specular(in_specular),
position(in_position),
flags	(in_flags) {
	flags &= LightFlags::settings;
	flags |= LightFlags::changes;
	GEngine::Get().Render().Light().Add(this);
}

Light::~Light() {
	GEngine::Get().Render().Light().Remove(this);
}

void Light::Set_Ambient(Color4f in_ambient) {
	ambient = in_ambient;
	flags |= LightFlags::ambientChange;
}

void Light::Set_Diffuse(Color4f in_diffuse) {
	diffuse = in_diffuse;
	flags |= LightFlags::diffuseChange;
}

void Light::Set_Specular(Color4f in_specular) {
	specular = in_specular;
	flags |= LightFlags::specularChange;
}

void Light::Set_Position(Vector3f in_position) {
	position = in_position;
	flags |= LightFlags::positionChange;
}

void Light::Set_Source(Flags in_flags) {
	in_flags &= LightFlags::sourceBit;
	flags &= ~LightFlags::sourceBit;
	flags |= in_flags;
}

void Light::Set_Toggle(Flags in_flags) {
	in_flags &= LightFlags::onOffBit;
	flags &= ~LightFlags::onOffBit;
	flags |= in_flags;
}

void Light::Render() {
	if (flags & LightFlags::changes) {
		string lightString = string("ligs[") + to_string(lightManagerArrayIndex) + "]";

		if(flags & LightFlags::ambientChange)
			glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".amb").c_str()), 1, &ambient.r);
		if(flags & LightFlags::diffuseChange)
			glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".dif").c_str()), 1, &diffuse.r);
		if(flags & LightFlags::specularChange)
			glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".spe").c_str()), 1, &specular.r);
		if (flags & LightFlags::positionChange) {
			Vector4f position4 = Vector4f(position, ((flags & LightFlags::sourceBit) == LightFlags::directional) ? 0.0f : 1.0f);
			glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".pos").c_str()), 1, &position4.x);
		}
		if (flags & LightFlags::toggleChange) {
			bool isOn = (flags & LightFlags::onOffBit) == LightFlags::on;
			glUniform1i(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".on").c_str()), isOn);
		}
		flags &= ~LightFlags::changes;
	}
}

LightManager::LightManager() :
RenderComponent(RenderType::first),
lights(CAPACITY, offsetof(Light, lightManagerArrayIndex)) 
{}

LightManager::~LightManager() 
{}

void LightManager::Add(Light* in_light) {
	lights.Add(in_light);
}

void LightManager::Remove(Light* in_light) {
	string lightString = string("ligs[") + to_string(in_light->lightManagerArrayIndex) + "]";
	float zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//this part may not be necessary
	ShaderProgram::Activate("default_shader");
	glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".amb").c_str()), 1, zeros);
	glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".dif").c_str()), 1, zeros);
	glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".spe").c_str()), 1, zeros);
	glUniform4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".pos").c_str()), 1, zeros);
	glUniform1i (glGetUniformLocation(ShaderProgram::active->vertexArrayID, (lightString + ".on" ).c_str()), 0);
	ShaderProgram::Deactivate();
	//

	lights[lights.Size() - 1]->flags |= LightFlags::changes;
	lights.Remove(in_light);
}

void LightManager::Render(Flags in_callflags) {
	ShaderProgram::Activate("default_shader");
	glUniform1i(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "nligs"), lights.Size());
	for (uint i = 0; i < lights.Size(); i++)
		lights[i]->Render();
	ShaderProgram::Deactivate();
}