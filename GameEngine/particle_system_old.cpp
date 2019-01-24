#include "particle_system.h"
#include "framework.h"

ParticleSystemManager* ParticleSystemManager::active = nullptr;

ParticleSystemManager::ParticleSystemManager() :
RenderComponent(RenderType::transparent),
systems			(1024, offsetof(ParticleSystem, psmindex)),
pcls			(nullptr),
parents			(nullptr),
dists			(nullptr),
npcls			(0),
maxpcls			(10000) {
	active =	this;
	pcls =		new int				[maxpcls];
	parents =	new ParticleSystem*	[maxpcls];
	dists =		new float			[maxpcls];

	pcllocs =	new Vector3f[maxpcls];
	pclrigs =	new Vector3f[maxpcls];
	pclups =	new Vector3f[maxpcls];
	pclcols =	new Color4f [maxpcls];
	pcltxc1s =	new Vector2f[maxpcls];
	pcltxc2s =	new Vector2f[maxpcls];
}

ParticleSystemManager::~ParticleSystemManager() {
	while (systems.Size())
		delete systems[0];
	delete[] pcls;
	delete[] parents;
	delete[] dists;
	delete[] pcllocs;
	delete[] pclrigs;
	delete[] pclups;
	delete[] pclcols;
	delete[] pcltxc1s;
	delete[] pcltxc2s;

	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(6, &positionBuffer);
}

void ParticleSystemManager::Init_Arrays() {
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glGenBuffers(6, &positionBuffer);

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, rigbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, upbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color4f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, txc1buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, txc2buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * maxpcls, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystemManager::Add(ParticleSystem* system) {
	systems.Add(system);
}

void ParticleSystemManager::Remove(ParticleSystem* system) {
	systems.Remove(system);

	for (int i = 0; i < npcls; i++) {
		if (parents[i] == system) {
			for (int j = i; j < npcls - 1; j++) {
				parents[j] = parents[j + 1];
				pcls[j] = pcls[j + 1];
			}
			npcls--;
			i--;
		}
	}

}

void ParticleSystemManager::Render(Flags callflags) {
	Calc_Dists		();
	Sort			();
	Prepare_Arrays	();

	glEnable	(GL_BLEND);
	glDisable	(GL_CULL_FACE);
	glDepthMask	(0);

	ShaderProgram::Activate("particle_shader");

	Matrix4f viewmat = viewStack.Top();
	Matrix4f projmat = projectionStack.Top();
	Matrix4f modelmat = modelStack.Top();

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * npcls, pcllocs);

	glBindBuffer(GL_ARRAY_BUFFER, rigbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * npcls, pclrigs);

	glBindBuffer(GL_ARRAY_BUFFER, upbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * npcls, pclups);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Color4f) * npcls, pclcols);

	glBindBuffer(GL_ARRAY_BUFFER, txc1buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * npcls, pcltxc1s);

	glBindBuffer(GL_ARRAY_BUFFER, txc2buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * npcls, pcltxc2s);

	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(	glGetUniformLocation(ShaderProgram::active->vertexArrayID, "viematrix"), 1, GL_FALSE, viewmat.data);
	glUniformMatrix4fv(	glGetUniformLocation(ShaderProgram::active->vertexArrayID, "promatrix"), 1, GL_TRUE,  projmat.data);
	glUniformMatrix4fv(	glGetUniformLocation(ShaderProgram::active->vertexArrayID, "modmatrix"), 1, GL_TRUE,  modelmat.data);
	glUniform1i(		glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex"), 0);

	int finish = 0;
	for (int start = 0; finish < npcls; start = finish) {
		for (finish = start + 1; finish < npcls &&
			parents[start]->blend.dst == parents[finish]->blend.dst &&
			parents[start]->blend.eqn ==	parents[finish]->blend.eqn &&
			parents[start]->blend.src ==	parents[finish]->blend.src &&
			parents[start]->texture ==		parents[finish]->texture; finish++) {}

		glBlendFunc(parents[start]->blend.src, parents[start]->blend.dst);
		glBlendEquation(parents[start]->blend.eqn);

		if (parents[start]->texture != nullptr) {
			parents[start]->texture->Activate();
		}

		glDrawArrays(GL_POINTS, start, (finish - start));

		if (parents[start]->texture != nullptr) {
			parents[start]->texture->Deactivate();
		}
	}
	glDisable(GL_BLEND);
	glDepthMask(1);
	ShaderProgram::Deactivate();
}

void ParticleSystemManager::Calc_Dists() {
	Vector3f tocam;
	for (int i = 0; i < npcls; i++) {
		tocam = parents[i]->data.Get<Vector3f>(PS_LOC, pcls[i]) - GEngine::Get().Render().Camera().transform.position;
		dists[i] = tocam.Dot(tocam);
	}
}

void ParticleSystemManager::Sort() {
	//Insertion sort
	int				pcl;
	ParticleSystem* pclparent;
	float			dist;
	for (int i = 0; i < npcls; i++) {
		pcl = pcls[i];
		pclparent = parents[i];
		dist = dists[i];
		int j;
		for (j = i - 1; j >= 0 && dists[i] > dists[j]; j--) {}
		for (int k = i; k > j + 1; k--) {
			pcls	[k] = pcls		[k - 1];
			parents	[k] = parents	[k - 1];
			dists	[k] = dists		[k - 1];
		}
		pcls	[j + 1] = pcl;
		parents	[j + 1] = pclparent;
		dists	[j + 1] = dist;
	}
}

void ParticleSystemManager::Prepare_Arrays() {
	/*
	ParticleData*	pcldata;
	for (int i = 0; i < npcls; i++) {
		pcldata = &parents[i]->data;
		pcllocs[i] = parents[i]->transform.S_To_O(pcldata->Get<Vector3f>(PS_LOC, pcls[i]));
		pcldims[i] = pcldata->Get<Vector2f>(PS_DIM, pcls[i]);
		switch (parents[i]->face) {
		case FACE_CAM:
			pcldirs[i] = (GEngine::Get().Render().Camera().transform.position - pcllocs[i]).Normalized();
			pclups [i] = GEngine::Get().Render().Camera().transform.orientation.up;
			pclups [i].Rotate(pcldata->Get<float>(PS_ORI, pcls[i]), pcldirs[i]);
			break;
		case FACE_CAM_RES:
			pcldirs[i] = Vector3f();
			pclups[i] = pcldata->Get<Vector3f>(PS_ORI, pcls[i]);
			break;
		case FACE_IND:
			pcldirs[i] = pcldata->Get<Orientation3d>(PS_ORI, pcls[i]).forward;
			pclups [i] = pcldata->Get<Orientation3d>(PS_ORI, pcls[i]).up;
			break;
		}
		pclcols[i] = pcldata->Get<Color4f>(PS_COL, pcls[i]);
		pclfres[i] = parents[i]->face;
		pcltxc1s[i] = pcldata->Get<Vector2f>(PS_TEXCOO1, pcls[i]);
		pcltxc2s[i] = pcldata->Get<Vector2f>(PS_TEXCOO2, pcls[i]);
	}
	*/
	Orientation3d	pclori;
	Vector2f		pcldim;
	Vector3d		pcldir;
	ParticleSystem* pclparent;
	ParticleData*	pcldata;

	for (int i = 0; i < npcls; i++) {
		pclparent = parents[i];
		pcldata = &pclparent->data;

		pcllocs[i] = pcldata->Get<Vector3f>(PS_LOC, pcls[i]);
		pcllocs[i] = pclparent->transform.S_To_O(pcllocs[i]);

		switch (pclparent->face) {
		case FACE_CAM:
			pcldir =		(GEngine::Get().Render().Camera().transform.position - pcllocs[i]).Normalized();
			pclups[i] =		GEngine::Get().Render().Camera().transform.orientation.up;
			pclrigs[i] =	pcldir.Cross(pclups[i]).Normalized();
			break;

		case FACE_CAM_RES:
			pcldir =		(GEngine::Get().Render().Camera().transform.position - pcllocs[i]).Normalized();
			pclups[i] =		pcldata->Get<Vector3f>(PS_ORI, pcls[i]);
			pclrigs[i] =	pcldir.Cross(pclups[i]).Normalized();
			break;

		case FACE_IND:
			pcldir =		Vector3f(0, 0, -1);
			pclups[i] =		Vector3f(0, 1, 0);
			pclrigs[i] =	Vector3f(1, 0, 0);
			break;
		}

		switch (pclparent->face) {
		case FACE_CAM:
			pclori = Orientation3d();
			//pclori.Rotate(pcldir * pcldata->Get<float>(PS_ORI, pcls[i]));
			break;
		case FACE_CAM_RES:
			pclori = Orientation3d();
			break;
		case FACE_IND:
			pclori = pcldata->Get<Orientation3d>(PS_ORI, pcls[i]);
			pcldir = pclori.S_To_O(pcldir);
			pclups[i] = pclori.S_To_O(pclups[i]);
			pclrigs[i] = pclori.S_To_O(pclrigs[i]);
			break;
		}
		pcldim = pcldata->Get<Vector2f>(PS_DIM, pcls[i]);
		pclrigs [i] *= pcldim.x;
		pclups  [i] *= pcldim.y;
		pclcols [i] = pcldata->Get<Color4f>(PS_COL, pcls[i]);
		pcltxc1s[i] = pcldata->Get<Vector2f>(PS_TEXCOO1, pcls[i]);
		pcltxc2s[i] = pcldata->Get<Vector2f>(PS_TEXCOO2, pcls[i]);
	}
}

ParticleData::ParticleData(ParticleSystem* parent, int maxpcls, int maxelements) :
parent		(parent),
maxpcls		(maxpcls),
maxelements	(maxelements),
npcls		(0) {
	elements =	new void*	[maxelements];
	isstatic =	new bool	[maxelements];
	sizes =		new short	[maxelements];
	for (int i = 0; i < maxelements; i++) {
		elements	[i] = nullptr;
		isstatic	[i] = false;
		sizes		[i] = 0;
	}
}

ParticleData::~ParticleData() {
	for (int i = 0; i < maxelements; i++) {
		if (elements[i] != nullptr) {
			if (isstatic[i])
				delete elements[i];
			else
				delete[] elements[i];
		}
	}
	delete[] elements;
	delete[] isstatic;
	delete[] sizes;
}

template <class T> void ParticleData::Add_Element(int index) {
	isstatic	[index] = false;
	elements	[index] = new T[maxpcls];
	sizes		[index] = sizeof(T);
}

template <class T> void ParticleData::Add_Element(int index, T value) {
	isstatic		[index] =	true;
	elements		[index] =	new T;
	*(T*)(elements	[index]) =	value;
	sizes			[index] =	sizeof(T);
}

template <class T> T ParticleData::Get(int arrayindex, int pclindex) {
	if (isstatic[arrayindex])
		return *(((T**)elements)[arrayindex]);
	else
		return ((T**)elements)[arrayindex][pclindex];
}

template <class T> void ParticleData::Set(int arrayindex, int pclindex, T value) {
	if (isstatic[arrayindex])
		*(((T**)elements)[arrayindex]) = value;
	else
		((T**)elements)[arrayindex][pclindex] = value;
}

void ParticleData::Copy(int arrayindex, int pclindex1, int pclindex2) {
	if (!isstatic[arrayindex])
		memcpy(
			(char*)elements[arrayindex] + pclindex2 * sizes[arrayindex], 
			(char*)elements[arrayindex] + pclindex1 * sizes[arrayindex], 
			sizes[arrayindex]
		);
}

void ParticleData::Add_Particle() {
	ParticleSystemManager* psm = ParticleSystemManager::active;
	if (psm->npcls == psm->maxpcls)
		Die("Too many particles!");
	psm->pcls[psm->npcls] = npcls;
	psm->parents[psm->npcls] = parent;
	psm->npcls++;
	npcls++;
}

void ParticleData::Remove_Particle(int index) {
	ParticleSystemManager* psm = ParticleSystemManager::active;
	psm->npcls--;
	npcls--;
	for (int i = 0; i < maxelements; i++) {
		if (elements[i] != nullptr) {
			Copy(i, npcls, index);
		}
	}
	for (int i = 0; i <= psm->npcls; i++) {
		if (psm->parents[i] == parent && psm->pcls[i] == index) {
			for (int j = i; j < psm->npcls; j++) {
				psm->parents[j] = psm->parents[j + 1];
				psm->pcls[j] = psm->pcls[j + 1];
			}
		}
		if (psm->parents[i] == parent && psm->pcls[i] == npcls)
			psm->pcls[i] = index;
	}
	
}

void ParticleData::Remove_Element(int index) {
	if (isstatic[index]) {
		delete elements[index];
	}
	else {
		delete[] elements[index];
	}
	elements[index] = nullptr;
}

template <class T> void ParticleData::Make_Static(int index, T value) {
	if (!isstatic[index]) {
		delete[] elements[index];
		Add_Element<T>(index, value);
	}
	else {
		Set<T>(index, 0, value);
	}
}

void ParticleData::Make_Static(int index) {
	if (!isstatic[index]) {
		delete[] elements[index];
		isstatic[index] = false;
		elements[index] = new char[maxpcls * sizes[index]];
	}
}

void ParticleData::Make_Unstatic(int index) {
	if (isstatic[index]) {
		delete elements[index];
		isstatic[index] = false;
		elements[index] = new char[maxpcls * sizes[index]];
	}
}

ParticleSystem::ParticleSystem(Transform3d transform, Texture2* texture, BlendSettings blend, int maxpcls, int nelem, double step, int face) :
MechanicalComponent(step),
data		(this, maxpcls, nelem),
transform	(transform),
texture		(texture),
blend		(blend),
face		(face) {
	data.		Add_Element<Vector3f>	(PS_LOC,		Vector3f());
	switch (face) {
	case FACE_CAM:
		data.Add_Element<float>			(PS_ORI, 0.0f);	
		break;
	case FACE_CAM_RES:
		data.Add_Element<Vector3f>		(PS_ORI, Vector3f(0, 0, -1));
		break;
	case FACE_IND:
		data.Add_Element<Orientation3d>	(PS_ORI, Orientation3d());
		break;
	}
	data.		Add_Element<Vector2f>	(PS_DIM,		Vector2f());
	data.		Add_Element<Color4f>	(PS_COL,		Color4f(1, 1, 1, 1));
	data.		Add_Element<Vector2f>	(PS_TEXCOO1,	Vector2f(0, 0));
	data.		Add_Element<Vector2f>	(PS_TEXCOO2,	Vector2f(1, 1));
	ParticleSystemManager::active->Add(this);
}

ParticleSystem::~ParticleSystem() {
	ParticleSystemManager::active->Remove(this);
}

void ParticleSystem::Update(double t) 
{}

SpaceDust::SpaceDust() :
ParticleSystem(
	Transform3d(), 
	(Texture2*)GEngine::Get().Resource().Get("glow.tga"), 
	{true, GL_ONE, GL_SRC_ALPHA, GL_ADD}, 
	250, PS__END, 1.0 / 30.0, FACE_CAM) {
	for (int i = 0; i < PS__END; i++)
		data.Make_Unstatic(i);

	Vector3f camloc = GEngine::Get().Render().Camera().transform.position;

	for (int i = 0; i < data.maxpcls; i++) {
		data.Add_Particle();
		data.Set<Vector3f>	(PS_LOC, i, camloc + Vector3f::Random_Normalized() * sqrt(Rand_Float(0.0f, 100.0f)));
		data.Set<float>		(PS_ORI, i, Rand_Float(0, 360));
		data.Set<Vector2f>	(PS_DIM, i, Vector2f(1, 1) * 0.02);
		data.Set<Color4f>	(PS_COL, i, Color4f(0.75, 1.0, 0.75, 0.75));
		data.Set<Vector2f>	(PS_TEXCOO1, i, Vector2f(0, 0));
		data.Set<Vector2f>	(PS_TEXCOO2, i, Vector2f(1.0, 1.0));
	}
}

SpaceDust::~SpaceDust() 
{}

void SpaceDust::Update(double t) {
	for (int i = 0; i < data.npcls; i++) {
		Vector3f fromcam = data.Get<Vector3f>(PS_LOC, i) - GEngine::Get().Render().Camera().transform.position;
		if (fromcam.Dot(fromcam) > 100.0 || fromcam.Dot(GEngine::Get().Render().Camera().transform.orientation.forward) < 0) {
			Vector3f r = Vector3f::Random_Normalized() * 9.0;
			if (r.Dot(GEngine::Get().Render().Camera().transform.orientation.forward) < 0)
				r = r * -1;
			data.Set<Vector3f>(PS_LOC, i, GEngine::Get().Render().Camera().transform.position + r);
		}
	}
}

EmitterGenerator::EmitterGenerator() :
parent(parent) 
{}

void EmitterGenerator::Generate(int n) 
{}

EmitterPopulator::EmitterPopulator() :
parent(parent)
{}

int EmitterPopulator::New_Count(double t) {
	return 0;
}

Emitter::Emitter(EmitterSettings settings, double step) :
ParticleSystem(settings.transform, settings.texture, settings.blend, settings.maxpcls, EM__END, step, settings.face),
settings(settings) {
	settings.generator->parent = this;
	if (settings.populator != nullptr)
		settings.populator->parent = this;
	data.		Add_Element<Vector3f>	(EM_LINVEL, Vector3f());
	if (face == FACE_CAM)
		data.	Add_Element<float>		(EM_ROTVEL, 0.0f);
	else
		data.	Add_Element<Vector3f>	(EM_ROTVEL, Vector3f());
	data.		Add_Element<Vector2f>	(EM_DIMVEL, Vector2f());
	data.		Add_Element<Vector3f>	(EM_LINACC, Vector3f());
	if (face == FACE_CAM)
		data.	Add_Element<float>		(EM_ROTACC, 0.0f);
	else
		data.	Add_Element<Vector3f>	(EM_ROTACC, Vector3f());
	data.		Add_Element<Vector2f>	(EM_DIMACC, Vector2f());
	data.		Add_Element<float>		(EM_STATIM,	GEngine::Get().Clock().Now());
	data.		Add_Element<float>		(EM_LIFTIM,		1.0f);
	data.		Add_Element<ColorPath4f>(EM_COLPAT,	ColorPath4f{ 1, {{0.0, Color4f(1, 1, 1, 1)}, {}, {}, {}} });

	for (int i = 0; i < EM__END; i++) {
		if (!settings.statics[i])
			data.Make_Unstatic(i);
	}

	settings.generator->Generate(settings.npcls);
}

Emitter::~Emitter() {
	delete settings.generator;
	if (settings.populator != nullptr)
		delete settings.populator;
}

void Emitter::Update(double t) {
	if (settings.populator != nullptr) {
		int n = settings.populator->New_Count(t);
		if (data.npcls + n <= data.maxpcls)
			settings.generator->Generate(n);
		else
			settings.generator->Generate(data.maxpcls - data.npcls);
	}

	if (settings.dieonempty && data.npcls == 0) {
		delete this;
		return;
	}

	double time = GEngine::Get().Clock().Now();

	for (int i = 0; i < data.npcls; i++) {
		{
			data.Set<Vector3f>(EM_LINVEL, i, data.Get<Vector3f>(EM_LINVEL, i) +
				data.Get<Vector3f>(EM_LINACC, i) * t);

			if (face == FACE_CAM)
				data.Set<float>(EM_ROTVEL, i, data.Get<float>(EM_ROTVEL, i) +
					data.Get<float>(EM_ROTACC, i) * t);
			else
				data.Set<Vector3f>(EM_ROTVEL, i, data.Get<Vector3f>(EM_ROTVEL, i) +
					data.Get<Vector3f>(EM_ROTACC, i) * t);

			data.Set<Vector2f>(EM_DIMVEL, i, data.Get<Vector2f>(EM_DIMVEL, i) +
				data.Get<Vector2f>(EM_DIMACC, i) * t);

			data.Set<Vector3f>(PS_LOC, i, data.Get<Vector3f>(PS_LOC, i) +
				data.Get<Vector3f>(EM_LINVEL, i) * t);

			switch(face) {
			case FACE_CAM: {
				data.Set<float>(PS_ORI, i, data.Get<float>(PS_ORI, i) +
					data.Get<float>(EM_ROTVEL, i) * t);
				break;
			}
			case FACE_CAM_RES: {
				Vector3f forward = data.Get<Vector3f>(PS_ORI, i);
				forward.Rotate(data.Get<Vector3f>(EM_ROTVEL, i) * t);
				data.Set<Vector3f>(PS_ORI, i, forward.Normalized());
				break;
			}
			case FACE_IND: {
				Orientation3d ori = data.Get<Orientation3d>(PS_ORI, i);
				ori.Rotate(data.Get<Vector3f>(EM_ROTVEL, i) * t);
				data.Set<Orientation3d>(PS_ORI, i, ori);
				break;
			}
			}

			data.Set<Vector2f>(PS_DIM, i, data.Get<Vector2f>(PS_DIM, i) +
				data.Get<Vector2f>(EM_DIMVEL, i) * t);
		}

		float pctdone = ((time - data.Get<float>(EM_STATIM, i)) / 
			data.Get<float>(EM_LIFTIM, i));

		if (pctdone >= 1.0) {
			data.Remove_Particle(i);
			i--;
			continue;
		}
		
		ColorPath4f colorpath = data.Get<ColorPath4f>(EM_COLPAT, i);

		int colorstep;
		for (colorstep = 0; colorstep < colorpath.nframes && pctdone >= colorpath.frames[colorstep].time; colorstep++) {}
		colorstep--;

		float nexttime = (colorpath.nframes > colorstep + 1) ? colorpath.frames[colorstep + 1].time : 1.0f;
		float progress = (pctdone - colorpath.frames[colorstep].time) / (nexttime - colorpath.frames[colorstep].time);
		Color4f nextcolor = (colorpath.nframes > colorstep + 1) ? colorpath.frames[colorstep + 1].color : colorpath.frames[colorstep].color;
		Color4f color = nextcolor * progress + colorpath.frames[colorstep].color * (1.0 - progress);

		data.Set<Color4f>(PS_COL, i, color);
	}
}

ExplosionSmokeGen::ExplosionSmokeGen(float size) :
size(size)
{}

void ExplosionSmokeGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>		(PS_LOC, i, Vector3f() * size);
		data->Set<float>		(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>		(PS_DIM, i, Vector2f(0, 0) * size);
		data->Set<Color4f>		(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.5));
		data->Set<Vector2f>		(PS_TEXCOO1, i, Vector2f(0, 0));
		data->Set<Vector2f>		(PS_TEXCOO2, i, Vector2f(0.25, 0.5));
		data->Set<Vector3f>		(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 5) * size);
		data->Set<float>		(EM_ROTVEL, i, Rand_Float(-90, 90));
		data->Set<Vector2f>		(EM_DIMVEL, i, Vector2f(5.0, 5.0) * size);
		data->Set<Vector3f>		(EM_LINACC, i, data->Get<Vector3f>(EM_LINVEL, i) / -8.0);
		data->Set<float>		(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>		(EM_DIMACC, i, Vector2f(-0.5, -0.5) * size);
		data->Set<float>		(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>		(EM_LIFTIM, i, Rand_Float(2.0, 3.0));
		float lum = Rand_Float(0.1, 0.5);
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{ { 0.0, Color4f(lum + 0.2, lum, lum, 0.1) },{1.0, Color4f(lum / 2, lum / 2, lum / 2, 0.0)},{},{} } });
	}
}

ExplosionFlameGen::ExplosionFlameGen(float size) :
size(size)
{}

void ExplosionFlameGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() * size);
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.0, 0.0) * size);
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.25, 0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.5, 0.5));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 7) * size);
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(10.0, 10.0) * size);
		data->Set<Vector3f>(EM_LINACC, i, Vector3f() * size);
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(-8.0, -8.0) * size);
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.5, 1.0));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 4,{ 
			{ 0.0f, Color4f(1.0, 0.5, 0.1, 0.8) },
			{ 0.5f, Color4f(0.9, 0.4, 0.1, 0.7) },
			{ 0.7f, Color4f(0.2, 0.2, 0.2, 0.1) },
			{ 1.0f, Color4f(0.1, 0.1, 0.1, 0.0) } } });
	}
}

ExplosionFlashGen::ExplosionFlashGen(float size) :
size(size)
{}

void ExplosionFlashGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() * size);
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(2.5, 2.5) * size);
		data->Set<Color4f>(PS_COL, i, Color4f(1, 1, 1, 1));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.75, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f() * size);
		data->Set<float>(EM_ROTVEL, i, 0);
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(5.0, 5.0) * size);
		data->Set<Vector3f>(EM_LINACC, i, Vector3f() * size);
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0) * size);
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.2, 0.3));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{ { 0.0f, Color4f(1.0, 1.0, 0.8, 0.8) },{ 1.0f, Color4f(0., 0.0, 0.0, 0.0) },{},{} } });
	}
}

ExplosionSparksGen::ExplosionSparksGen(float size) :
size(size)
{}

void ExplosionSparksGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() * size);
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(2.5, 2.5) * size);
		data->Set<Color4f>(PS_COL, i, Color4f(1, 1, 1, 1));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.5, 0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.75, 0.5));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 20) * size);
		data->Set<float>(EM_ROTVEL, i, 0);
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f() * size);
		data->Set<Vector3f>(EM_LINACC, i, Vector3f() * size);
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0) * size);
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.75, 1.5));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{ { 0.0f, Color4f(1.0, 0.2, 0.0, 1.0) },{ 1.0f, Color4f(0.0, 0.0, 0.0, 0.0) },{},{} } });
	}
}

ExplosionSparkTrailsGen::ExplosionSparkTrailsGen(float size) :
size(size)
{}

void ExplosionSparkTrailsGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		Vector3f forward = Vector3f::Random_Normalized();
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() * size);
		data->Set<Vector3f>(PS_ORI, i, forward);
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.25, 2.0) * size);
		data->Set<Color4f>(PS_COL, i, Color4f(1, 1, 1, 1));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.75, 0.5));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.8125, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, forward * Rand_Float(0, 7) * size);
		data->Set<Vector3f>(EM_ROTVEL, i, Vector3f());
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f() * size);
		data->Set<Vector3f>(EM_LINACC, i, Vector3f() * size);
		data->Set<Vector3f>(EM_ROTACC, i, Vector3f());
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0) * size);
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.5, 1.0));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{ { 0.0f, Color4f(1.0, 0.9, 0.7, 0.4) },{ 1.0f, Color4f(0.0, 0.0, 0.0, 0.0) },{},{} } });
	}
}

ExplosionSmokeTrailsGen::ExplosionSmokeTrailsGen(float size) :
size(size)
{}

void ExplosionSmokeTrailsGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		Vector3f forward = Vector3f::Random_Normalized();
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() * size);
		data->Set<Vector3f>(PS_ORI, i, forward);
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.2, 1.5) * size);
		data->Set<Color4f>(PS_COL, i, Color4f(1, 1, 1, 1));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.875, 0.5));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.93625, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, forward * Rand_Float(0, 2) * size);
		data->Set<Vector3f>(EM_ROTVEL, i, Vector3f());
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(0.6, 4.5) * size);
		data->Set<Vector3f>(EM_LINACC, i, Vector3f() * size);
		data->Set<Vector3f>(EM_ROTACC, i, Vector3f());
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0) * size);
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.5, 1.0));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{ { 0.0f, Color4f(1.0, 0.9, 0.7, 0.4) },{ 1.0f, Color4f(0.0, 0.0, 0.0, 0.0) },{},{} } });
	}
}

LaserRecoilGen::LaserRecoilGen() 
{}

void LaserRecoilGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f() + Vector3f::Random_Normalized() * Rand_Float(0, 0.5));
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.0));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.0, 0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(1.0, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f()); 
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(0.0, 0.0));
		data->Set<Vector3f>(EM_LINACC, i, Vector3f());
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(-0.0, -0.0));
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.3, 0.3));
	}
	for (int i = 0; i < n / 2; i++) {
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.5, 0.5));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(1.0, 1.0, 1.0, 0.2) },
			{ 1.0f, Color4f(1.0, 1.0, 1.0, 0.0) },
			{},
			{} } });
	}

	for (int i = n / 2; i < n; i++) {
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.75, 0.75));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(0.0, 0.2, 1.0, 0.08) },
			{ 1.0f, Color4f(0.0, 0.2, 1.0, 0.0) },
			{},
			{} } });
	}
}

PulseRecoilGen::PulseRecoilGen()
{}

void PulseRecoilGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f());
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(0.0, 0.0));
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.25, 0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.5, 0.5));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 2));
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(2.0, 2.0));
		data->Set<Vector3f>(EM_LINACC, i, Vector3f());
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0));
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.3, 0.5));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(0.1, 1.0, 0.1, 0.5) },
			{ 1.0f, Color4f(0.1, 1.0, 0.1, 0.0) },
			{},
			{} } });
	}
}

LaserFlashGen::LaserFlashGen()
{}

void LaserFlashGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f());
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(1.0, 1.0) * 1.0);
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 1.0));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.0, 0.0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(1.0, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 2));
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(5.0, 5.0));
		data->Set<Vector3f>(EM_LINACC, i, Vector3f());
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0));
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.01, 0.02));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(0.1, 0.2, 1.0, 0.5) },
			{ 1.0f, Color4f(0.1, 0.4, 1.0, 0.0) },
			{},
			{} } });
	}
}

PulseFlashGen::PulseFlashGen()
{}

void PulseFlashGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	for (int i = 0; i < n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, Vector3f());
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(1.0, 1.0) * Rand_Float(0.4, 0.8));
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.75, 1.0));
		data->Set<Vector3f>(EM_LINVEL, i, Vector3f::Random_Normalized() * Rand_Float(0, 2));
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(5.0, 5.0));
		data->Set<Vector3f>(EM_LINACC, i, Vector3f());
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(0.0, 0.0));
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.01, 0.02));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(0.1, 1.0, 0.1, 0.5) },
			{ 1.0f, Color4f(0.1, 1.0, 0.1, 0.0) },
			{},
			{} } });
	}
}

PlayerShipTrailGen::PlayerShipTrailGen(PlayerShip* parentship) :
parentship(parentship)
{}

void PlayerShipTrailGen::Generate(int n) {
	EmitterSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	int orignpcls = data->npcls;
	for (int i = orignpcls; i < orignpcls + n; i++) {
		data->Add_Particle();
		data->Set<Vector3f>(PS_LOC, i, parentship->transform.S_To_O(Vector3d(Rand_Float(-0.4, 0.4), 0.2, 3.2)));
		data->Set<float>(PS_ORI, i, Rand_Float(0, 360));
		data->Set<Vector2f>(PS_DIM, i, Vector2f(1.0, 1.0) * Rand_Float(0.6, 0.6));
		data->Set<Color4f>(PS_COL, i, Color4f(0.5, 0.5, 0.5, 0.5));
		data->Set<Vector2f>(PS_TEXCOO1, i, Vector2f(0.0, 0.0));
		data->Set<Vector2f>(PS_TEXCOO2, i, Vector2f(0.25, 0.5));
		data->Set<Vector3f>(EM_LINVEL, i, ((GetKeyState('W') & 0x8000) && parentship->propulsion.fbvel != 20.0f) ? 
			parentship->transform.orientation.forward * -10.0 + Vector3f::Random_Normalized() * 2.0 : 
			Vector3f());
		data->Set<float>(EM_ROTVEL, i, Rand_Float(-180, 180));
		data->Set<Vector2f>(EM_DIMVEL, i, Vector2f(0.0, 0.0));
		data->Set<Vector3f>(EM_LINACC, i, Vector3f());
		data->Set<float>(EM_ROTACC, i, 0.0);
		data->Set<Vector2f>(EM_DIMACC, i, Vector2f(-0.5, -0.5));
		data->Set<float>(EM_STATIM, i, GEngine::Get().Clock().Now());
		data->Set<float>(EM_LIFTIM, i, Rand_Float(0.1, 0.1));
		data->Set<ColorPath4f>(EM_COLPAT, i, ColorPath4f{ 2,{
			{ 0.0f, Color4f(0.5, 0.5, 1.0, 0.5 * parentship->propulsion.efficiency) },
			{ 1.0f, Color4f(0.1, 0.1, 1.0, 0.0) },
			{},
			{} } });
	}
}

PlayerShipTrailPop::PlayerShipTrailPop() :
stepper(0.001)
{}

int PlayerShipTrailPop::New_Count(double t) {
	return stepper.Step_Number(t);
}

MeshBlanketGenerator::MeshBlanketGenerator()
{}

void MeshBlanketGenerator::Generate(int n)
{}

MeshBlanket::MeshBlanket(MeshBlanketSettings settings, double step) :
ParticleSystem	(settings.parent->transform, settings.texture, settings.blend, settings.npcls, MB__END, step, FACE_IND),
settings		(settings) {
	settings.generator->parent = this;
	data.Add_Element<float>		(MB_AMP,	0.0f);
	data.Add_Element<float>		(MB_PER,	1.0f);
	data.Add_Element<float>		(MB_WAVCOO, 0.0f);
	data.Add_Element<Vector3f>	(MB_NOR,	Vector3f());
	data.Add_Element<Vector3f>	(MB_NOD,	Vector3f());

	for (int i = 0; i < MB__END; i++) {
		if (!settings.statics[i])
			data.Make_Unstatic(i);
	}

	settings.generator->Generate(settings.npcls);
}

MeshBlanket::~MeshBlanket() {
	delete settings.generator;
}

void MeshBlanket::Update(double t) {
	transform = settings.parent->transform;

	for (int i = 0; i < data.npcls; i++) {
		data.Set<float>(MB_WAVCOO, i,
			(float)(data.Get<float>(MB_WAVCOO, i) +
				t / data.Get<float>(MB_PER, i)));

		data.Set<Vector3f>(PS_LOC, i,
			data.Get<Vector3f>(MB_NOD, i) +
			data.Get<Vector3f>(MB_NOR, i) *
			data.Get<float>(MB_AMP, i) *
			sinf(data.Get<float>(MB_WAVCOO, i) * 2.0f * (float)PI));

		data.Set<float>(MB_WAVCOO, i,
			data.Get<float>(MB_WAVCOO, i) - (int)data.Get<float>(MB_WAVCOO, i));

		Orientation3d ori = transform.orientation;
		ori.Rotate(	ori.forward.Theta(transform.orientation.S_To_O(data.Get<Vector3f>(MB_NOR, i))),
					ori.forward.Cross(transform.orientation.S_To_O(data.Get<Vector3f>(MB_NOR, i))).Normalized());

		data.Set<Orientation3d>(PS_ORI, i, ori);
	}
}

PlayerShipShieldMesBlaGen::PlayerShipShieldMesBlaGen()
{}

void PlayerShipShieldMesBlaGen::Generate(int n) {
	MeshBlanketSettings* settings = &parent->settings;
	ParticleData* data = &parent->data;
	int			ntris = settings->parent->mesh->ntriangles;
	Vector3f*	vertices = settings->parent->mesh->rawvertices;
	Vector3f*	norms = new Vector3f[ntris];
	float*		areas = new float[ntris];
	int*		triindices = new int[settings->npcls];
	float		s1, s2, s3, s;
	float		totalarea = 0.0f;
	for (int i = 0; i < ntris * 3; i += 3) {
		norms[i / 3] = settings->parent->mesh->rawnormals[i] +
			settings->parent->mesh->rawnormals[i + 1] +
			settings->parent->mesh->rawnormals[i + 2];
		norms[i / 3].Normalize();
		s1 = (vertices[i + 1] - vertices[i]).Magnitude();
		s2 = (vertices[i + 2] - vertices[i]).Magnitude();
		s3 = (vertices[i + 2] - vertices[i + 1]).Magnitude();
		s = (s1 + s2 + s3) * 0.5f;
		areas[i / 3] = sqrt(s * (s - s1) * (s - s2) * (s - s3));
		totalarea += areas[i / 3];
	}

	Stepper stepper = Stepper(totalarea / settings->npcls);

	int steps;
	Vector3f pclloc;

	for (int i = 0; i < ntris; i++) {
		steps = stepper.Step_Number(areas[i]);
		for (int j = 0; j < steps; j++) {
			float rand1 = sqrt(Rand_Float(0.0f, 1.0f));
			float rand2 = Rand_Float(0.0f, 1.0f);
			pclloc = vertices[i * 3] * (1.0f - rand1) +
				vertices[i * 3 + 1] * (rand1 * (1 - rand2)) +
				vertices[i * 3 + 2] * (rand1 * rand2);
			data->Set<Vector3f>(MB_NOD, data->npcls,
				pclloc + norms[i] * 0.1);
			triindices[data->npcls] = i;
			data->Add_Particle();
		}
	}

	for (int i = 0; i < data->npcls; i++) {
		data->Set<float>(MB_AMP, i, 0.1);
		data->Set<float>(MB_PER, i, Rand_Float(1, 4));
		data->Set<float>(MB_WAVCOO, i, (false) ? 0.0f : Rand_Float(0.0f, 1.0f));
		data->Set<Vector3f>(MB_NOR, i, norms[triindices[i]]);
		//data->Set<Vector3f>(PS_ORI, i, norms[triindices[i]].Cross(Vector3f(0, 0, -1)));
		data->Set<Vector3f>(PS_LOC, i,
			data->Get<Vector3f>(MB_NOD, i) +
			data->Get<Vector3f>(MB_NOR, i) *
			sinf(data->Get<float>(MB_WAVCOO, i) * 2.0f * (float)PI));
		data->Set<Color4f>(PS_COL, i, Color4f(0.8, 0.8, 1, 0.02));
	}

	data->Set<Vector2f>(PS_DIM, 0, Vector2f(0.75, 0.75));
	
	delete[] norms;
	delete[] areas;
	delete[] triindices;
}

