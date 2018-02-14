#include "mesh.h"
#include "framework.h"

Material::Material() :
name			("default_material"),
ambient			(Color4f(0.1f, 0.1f, 0.1f, 0.0f)),
diffuse			(Color4f(0.6f, 0.6f, 0.6f, 1.0f)),
specular		(Color4f(0.8f, 0.8f, 0.8f, 50.0f)),
emission		(Color4f(0.0f, 0.0f, 0.0f, 0.0f)),
textures		(8),
blendSettings	()
{}

Material::Material(string in_name, Color4f in_ambient, Color4f in_diffuse, Color4f in_specular,
	Color4f in_emission, BlendSettings in_blendSettings) :
name			(in_name),
ambient			(in_ambient),
diffuse			(in_diffuse),
specular		(in_specular),
emission		(in_emission),
textures		(8),
blendSettings	(in_blendSettings) 
{}
/*
void Material::Apply() {
	RenderContext& context = GEngine::Get().Render().Context();
	context.Set_Blend_Settings(blendSettings);
	if (blendSettings.useBlending) {
		GEngine::G
	}

	if (texture != nullptr)
		texture->Activate();
	
	ShaderProgram* prog = ShaderProgram::active;
	if (prog->name != "default_shader")
		Die("Wrong shader!");

	glUniform4fv(glGetUniformLocation(prog->vertexArrayID, "mat.amb"), 1, &ambient.r);
	glUniform4fv(glGetUniformLocation(prog->vertexArrayID, "mat.dif"), 1, &diffuse.r);
	glUniform4fv(glGetUniformLocation(prog->vertexArrayID, "mat.spe"), 1, &specular.r);
	glUniform1f(glGetUniformLocation(prog->vertexArrayID, "mat.shi"), shininess);
	glUniform1i(glGetUniformLocation(prog->vertexArrayID, "mat.lig"), lighting);
}

void Material::Apply(bool front, bool back) {
	int fb;
	if (front) {
		if (back)
			fb = GL_FRONT_AND_BACK;
		else
			fb = GL_FRONT;
	}
	else {
		if (back)
			fb = GL_BACK;
		else
			return;
	}
	if (!lighting) {
		glDisable(GL_LIGHTING);
		glColor4fv(&diffuse.r);
	}
	if (blend.blend) {
		glEnable(GL_BLEND);
		glBlendFunc(blend.src, blend.dst);
		glBlendEquation(blend.eqn);
		glDepthMask(0);
	}
	if (texture != nullptr)
		texture->Activate();

	glMaterialfv(fb, GL_AMBIENT,	&ambient.r);
	glMaterialfv(fb, GL_DIFFUSE,	&diffuse.r);
	glMaterialfv(fb, GL_SPECULAR,	&specular.r);
	glMaterialf (fb, GL_SHININESS,	shininess);
}

void Material::Unapply() {
	if (!lighting)
		glEnable(GL_LIGHTING);
	if (texture)
		texture->Deactivate();
	if (blend.blend) {
		glDisable(GL_BLEND);
		glDepthMask(1);
	}
}

bool Material::Is_Transparent() {
	return blend.blend;
}
*/
/*

SphereTree::SphereTree(Vector3f* tris, int ntris) {
	Vector3f*	triscpy =	new Vector3f	[ntris * 3];
	SphereNode**	sbuffer =	new SphereNode*		[ntris * 2];
	short*		posbuffer = new short		[ntris * 2];
	short*		lenbuffer = new short		[ntris * 2];

	memcpy(triscpy, tris, ntris * 3 * sizeof(Vector3f));

	trunk = new SphereNode;

	sbuffer		[0] = trunk;
	posbuffer	[0] = 0;
	lenbuffer	[0] = ntris * 3;
	
	int			nsphs =		1;
	Vector3f	farthestv;
	Vector3f	c;
	float		farthest;
	float		newfarthest;
				nspheres =	0;

	while (nsphs) {
		nspheres++;

		//c is the mean point
		for (int i = posbuffer[0]; i < posbuffer[0] + lenbuffer[0]; i++) {
			c += triscpy[i];
		}
		c /= lenbuffer[0];

		//find the farthest point from c to get the outer radius
		farthest = 0;
		for (int i = posbuffer[0]; i < posbuffer[0] + lenbuffer[0]; i++) {
			newfarthest = (triscpy[i] - c).Magnitude();
			if (newfarthest > farthest) {
				farthestv = triscpy[i];
				farthest = newfarthest;
			}
		}

		//create outer sphere
		sbuffer[0]->localCenter = c;
		sbuffer[0]->radius = (farthestv - c).Magnitude();
		sbuffer[0]->tri = nullptr;

		sbuffer		[0]->child1 = nullptr;
		sbuffer		[0]->child2 = nullptr;
		sbuffer		[0] = sbuffer	[nsphs - 1];
		lenbuffer	[0] = lenbuffer	[nsphs - 1];
		posbuffer	[0] = posbuffer	[nsphs - 1];
		nsphs--;
		continue;

		switch (lenbuffer[0]) {
		case 0:
			exit(-2);
		case 3:
			sbuffer[0]->child1 = nullptr;
			sbuffer[0]->child2 = nullptr;
			sbuffer[0]->tri = new Vector3f[3];
			sbuffer[0]->objtri = new Vector3f[3];
			sbuffer[0]->tri[0] = triscpy[posbuffer[0] + 0];
			sbuffer[0]->tri[1] = triscpy[posbuffer[0] + 1];
			sbuffer[0]->tri[2] = triscpy[posbuffer[0] + 2];
			sbuffer[0] =	sbuffer		[nsphs - 1];
			lenbuffer[0] =	lenbuffer	[nsphs - 1];
			posbuffer[0] =	posbuffer	[nsphs - 1];
			nsphs--;
			break;
		case 6:
			sbuffer[0]->child1 = new SphereNode;
			sbuffer[0]->child2 = new SphereNode;
			sbuffer[nsphs] = sbuffer[0]->child2;
			posbuffer[nsphs] = posbuffer[0] + 3;
			lenbuffer[nsphs] = 3;
			sbuffer[0] = sbuffer[0]->child1;
			lenbuffer[0] = 3;
			nsphs++;
			break;
		default:
			if (lenbuffer[0] % 3 != 0)
				exit(-3);
			int nleft = 0;
			Vector3f rad = farthestv - sbuffer[0]->localCenter;
			float mid = 0;
			for (int i = posbuffer[0]; i < posbuffer[0] + lenbuffer[0]; i++) {
				mid += rad.Dot(triscpy[i]);
			}
			mid /= lenbuffer[0];
			float proj;
			Vector3f m;
			for (int i = posbuffer[0]; i < posbuffer[0] + lenbuffer[0]; i += 3) {
				m = Vector3f(0, 0, 0);
				for (int j = 0; j < 3; j++)
					m += triscpy[i + j];
				m *= 0.333333333333f;
				proj = rad.Dot(m);
				if (proj >= mid) {
					for (int j = 0; j < 3; j++) {
						std::swap(triscpy[i + j], triscpy[posbuffer[0] + nleft + j]);
					}
					nleft += 3;
				}
			}
			if (nleft == lenbuffer[0] || nleft == 0)
				exit(-4);

			sbuffer[0]->child1 = new SphereNode;
			sbuffer[0]->child2 = new SphereNode;
			sbuffer[nsphs] = sbuffer[0]->child2;
			posbuffer[nsphs] = posbuffer[0] + nleft;
			lenbuffer[nsphs] = lenbuffer[0] - nleft;
			sbuffer[0] = sbuffer[0]->child1;
			lenbuffer[0] = nleft;
			nsphs++;
		}
	}

	delete[] triscpy;
	delete[] sbuffer;
	delete[] posbuffer;
	delete[] lenbuffer;
}

SphereTree::~SphereTree() {
	SphereNode** sbuffer = new SphereNode*[nspheres];
	int nsphs = 1;
	sbuffer[0] = trunk;
	while (nsphs) {
		SphereNode* s1 = sbuffer[0]->child1;
		SphereNode* s2 = sbuffer[0]->child2;

		if (sbuffer[0]->tri != nullptr)
			delete[] sbuffer[0]->tri;

		delete sbuffer[0];

		if (s1 != nullptr) {
			sbuffer[0] = s1;
			sbuffer[nsphs] = s2;
			nsphs++;
		}
		else {
			sbuffer[0] = sbuffer[nsphs - 1];
			nsphs--;
		}
	}
	delete[] sbuffer;
}

SubMesh::SubMesh(Vector3f* vertices, Vector3f* norms, Vector2f* texcs, int nVertices, Material material) :
vertexArrayID		(0),
vertbuffer	(0),
normbuffer	(0),
texcbuffer	(0),
indxbuffer	(0),
material	(material),
nVertices		(nVertices) {
	Vector3f* vertbufferdata =	new Vector3f[nVertices];
	Vector3f* normbufferdata =	new Vector3f[nVertices];
	Vector2f* texcbufferdata =	new Vector2f[nVertices];
	ushort*	  indxbufferdata =	new ushort  [nVertices];

	ushort indx = 0;
	bool iscopy;

	for (int i = 0; i < nVertices; i++) {
		iscopy = false;
		for (int j = 0; j < indx; j++) {
			if (
							vertbufferdata[j] == vertices[i] &&
				(!norms ||	normbufferdata[j] == norms[i]) &&
				(!texcs ||	texcbufferdata[j] == texcs[i])
				) {
				indxbufferdata[i] = j;
				iscopy = true;
				break;
			}
		}
		if (!iscopy) {
				vertbufferdata[indx] = vertices[i];
			if (norms)
				normbufferdata[indx] = norms[i];
			if (texcs)
				texcbufferdata[indx] = texcs[i];
			indxbufferdata[i] = indx;
			indx++;
		}
	}

	if (!norms) {
		for (int i = 0; i < nVertices; i++)
			normbufferdata[i] = Vector3f();
	}

	if (!texcs) {
		for (int i = 0; i < nVertices; i++)
			texcbufferdata[i] = Vector2f();
	}

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	
	glGenBuffers(1, &vertbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * indx, vertbufferdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &normbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * indx, normbufferdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &texcbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * indx, texcbufferdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &indxbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * nVertices, indxbufferdata, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

SubMesh::~SubMesh() {
	glDeleteBuffers(1, &vertbuffer);
	if (normbuffer)
		glDeleteBuffers(1, &normbuffer);
	if (texcbuffer)
		glDeleteBuffers(1, &texcbuffer);
	glDeleteBuffers(1, &indxbuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void SubMesh::Render(Flags callflags) {
	Matrix4f viewmat, projmat, modelmat;
	switch(callflags) {
	case RenderType::shadow:
		modelmat = modelStack.Top();
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "modmatrix"), 1, GL_FALSE, modelmat.data);
		glBindVertexArray(vertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxbuffer);

		glDisable(GL_CULL_FACE);

		glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
		break;
	case RenderType::opaque:
		viewmat = viewStack.Top();
		projmat = projectionStack.Top();
		modelmat = modelStack.Top();

		ShaderProgram::Activate("default_shader");

		if (material.texture != nullptr)
			material.texture->Activate();
;
		glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "viematrix"), 1, GL_FALSE, viewmat.data);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "promatrix"), 1, GL_TRUE, projmat.data);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "modmatrix"), 1, GL_FALSE, modelmat.data);
		glUniform1i(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex"), 0);
		glUniform1i(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "texbnd"), (material.texture != nullptr));

		glBindVertexArray(vertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxbuffer);

		glEnable(GL_CULL_FACE);

		material.Apply();

		glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_SHORT, 0);

		material.Unapply();

		if (material.texture != nullptr)
			material.texture->Deactivate();

		ShaderProgram::Deactivate();

		glBindVertexArray(0);
		break;
	}
}

Mesh::Mesh(string path, string name) :
Resource	(path, name), 
tree		(nullptr) 
{}

void Mesh::Load_Raw(Vector3f* vertices, Vector3f* norms, Vector2f* texcs, int ntris, Material matl) {
	int nVertices = ntris * 3;
	nsubmeshes = 1;
	submeshes = new SubMesh*[1];
	submeshes[0] = new SubMesh(vertices, norms, texcs, nVertices, matl);
	rawvertices =	vertices;
	rawnormals =	norms;
	rawtexcoords =	texcs;
	ntriangles =	ntris;
	tree = new SphereTree(vertices, ntris, 0.5);
}

void Mesh::Load_OBJ() {
	ifstream file;
	file.open(path + name);

	if (!file.is_open())
		Die(std::string("Mesh.Load_OBJ: *.obj file '") + path + name + std::string("' not found."));

	string s;
	string mtlfilename = "";

	int nVertices = 0;
	int nnorms = 0;
	int ntexcs = 0;
	int nfaces = 0;
	int nsubms = 0;

	while (getline(file, s)) {
		if (s.substr(0, 2) == "v ") {
			nVertices++;
			continue;
		}
		if (s.substr(0, 3) == "vn ") {
			nnorms++;
			continue;
		}
		if (s.substr(0, 3) == "vt ") {
			ntexcs++;
			continue;
		}
		if (s.substr(0, 2) == "f ") {
			nfaces++;
			continue;
		}
		if (s.substr(0, 7) == "usemtl ") {
			nsubms++;
			continue;
		}
	}

	file.clear();
	file.seekg(0, ifstream::beg);

	Vector3f*	vertices = new Vector3f	[nVertices];
	Vector3f*	norms = (nnorms) ? new Vector3f	[nnorms] : nullptr;
	Vector2f*	texcs = (ntexcs) ? new Vector2f	[ntexcs] : nullptr;
	int*		faces = new int		[nfaces * 9];
	int*		subms = new int		[nsubms];
	Material*	matls = new Material[nsubms];

	int verti = 0;
	int normi = 0;
	int texci = 0;
	int facei = 0;
	int submi = 0;

	while (getline(file, s)) {
		int length = s.length();
		if (length > 2 && s.substr(0, 2) == "v ") {
			s = s.substr(2, length - 2);
			stringstream ss;
			ss << s;
			ss >>  vertices[verti].x
				>> vertices[verti].y
				>> vertices[verti].z;
			verti++;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "vn ") {
			s = s.substr(3, length - 3);
			stringstream ss;
			ss << s;
			ss >>  norms[normi].x
				>> norms[normi].y
				>> norms[normi].z;
			normi++;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "vt ") {
			s = s.substr(3, length - 3);
			stringstream ss;
			ss << s;
			ss >>  texcs[texci].x
				>> texcs[texci].y;
			texci++;
			continue;
		}
		if (length > 2 && s.substr(0, 2) == "f ") {
			s = s.substr(2, length - 2);

			for (unsigned int i = 0; i<s.length(); i++) {
				if (s[i] == '/')
					s[i] = ' ';
			}

			stringstream ss;
			ss << s;

			for (int i = 0; i<3; i++) {
				while (s[0] == ' ')
					s = s.substr(1, s.length() - 1);

				ss >> faces[facei * 9 + i * 3 + 0];
				ss.ignore();
				if (ntexcs)
					ss >> faces[facei * 9 + i * 3 + 1];
				ss.ignore();
				if (nnorms)
					ss >> faces[facei * 9 + i * 3 + 2];
				ss.ignore();
			}
			facei++;
			continue;
		}
		if (length > 7 && s.substr(0, 7) == "usemtl ") {
			s = s.substr(7, length - 7);
			subms[submi] = facei * 3;
			matls[submi].name = s;
			submi++;
			continue;
		}
		if (length > 7 && s.substr(0, 7) == "mtllib ") {
			mtlfilename = s.substr(7, length - 7);
			continue;
		}
	}
	file.close();
	if (verti != nVertices || normi != nnorms || texci != ntexcs || submi != nsubms || facei != nfaces)
		Die("Mesh.Load_OBJ: parse count mismatch.");

	if (mtlfilename == "")
		Die("Mesh.Load_OBJ: no *.mtl file specified.");

	ifstream file2;
	file2.open(path + mtlfilename);

	if (!file2.is_open())
		Die(std::string("Mesh.Load_OBJ: *.mtl file '") + path + mtlfilename + std::string("' not found."));

	Material* currentmatl = nullptr;

	while (getline(file2, s)) {
		int length = s.length();
		if (length && s[0] == '#')
			continue;
		if (length > 7 && s.substr(0, 7) == "newmtl ") {
			s = s.substr(7, s.length() - 7);
			for (int i = 0; i<submi; i++) {
				if (matls[i].name == s) {
					currentmatl = &matls[i];
					break;
				}
			}
			currentmatl->shininess = 0.5;
			currentmatl->texture = nullptr;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "Ka ") {
			s = s.substr(3, length - 3);

			stringstream ss;
			ss << s;
			ss >> currentmatl->ambient.r;
			ss >> currentmatl->ambient.g;
			ss >> currentmatl->ambient.b;
			currentmatl->ambient.a = 1.0;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "Kd ") {
			s = s.substr(3, length - 3);

			stringstream ss;
			ss << s;
			ss >> currentmatl->diffuse.r;
			ss >> currentmatl->diffuse.g;
			ss >> currentmatl->diffuse.b;
			currentmatl->diffuse.a = 1.0;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "Ks ") {
			s = s.substr(3, length - 3);

			stringstream ss;
			ss << s;
			ss >> currentmatl->specular.r;
			ss >> currentmatl->specular.g;
			ss >> currentmatl->specular.b;
			currentmatl->specular.a = 1.0;
			continue;
		}
		if (length > 2 && s.substr(0, 2) == "d ") {
			s = s.substr(2, length - 2);

			stringstream ss;
			ss << s;
			ss >> currentmatl->diffuse.a;
			continue;
		}
		if (length > 3 && s.substr(0, 3) == "Ns ") {
			s = s.substr(3, length - 3);

			stringstream ss;
			ss << s;
			ss >> currentmatl->shininess;
			continue;
		}
		if (length > 7 && s.substr(0, 7) == "map_Kd ") {
			s = s.substr(7, length - 7);
			if (GEngine::Get().Resource().Get_Resource(s) != nullptr)
				currentmatl->texture = (Texture2D*)(GEngine::Get().Resource().Get_Resource(s));
			else {
				currentmatl->texture = new Texture2D("img/", s, GL_RGBA8, 0);
				currentmatl->texture->Load_TGA();
			}
			continue;
		}
		if (length > 6 && s.substr(0, 6) == "illum ") {
			s = s.substr(6, length - 6);

			stringstream ss;
			ss << s;
			ss >> currentmatl->lighting;
			continue;
		}
	}
	file2.close();

	for (int i = 0; i < nsubms; i++) {
		for (int j = i + 1; j < nsubms; j++) {
			if (matls[i].name == matls[j].name)
				matls[j] = matls[i];
		}
	}

	Vector3f* vertbuffer =				new Vector3f[nfaces * 3];
	Vector3f* normbuffer = (nnorms) ?	new Vector3f[nfaces * 3] : nullptr;
	Vector2f* texcbuffer = (ntexcs) ?	new Vector2f[nfaces * 3] : nullptr;

	for (int i = 0; i < nfaces; i++) {
		for (int j = 0; j < 3; j++) {
				vertbuffer[i * 3 + j] = vertices[faces[i * 9 + j * 3 + 0] - 1];
			if (nnorms)
				normbuffer[i * 3 + j] = norms[faces[i * 9 + j * 3 + 2] - 1];
			if (ntexcs)
				texcbuffer[i * 3 + j] = texcs[faces[i * 9 + j * 3 + 1] - 1];
		}
	}
	
	//tree = new SphereTree(vertbuffer, nfaces, 0.25);

	ntriangles = nfaces;

	rawvertices = new Vector3f[nfaces * 3];
	for (int i = 0; i < nfaces * 3; i++)
		rawvertices[i] = vertbuffer[i];

	if (nnorms) {
		rawnormals = new Vector3f[nfaces * 3];
		for (int i = 0; i < nfaces * 3; i++)
			rawnormals[i] = normbuffer[i];
	}
	else
		rawnormals = nullptr;
	
	if (ntexcs) {
		rawtexcoords = new Vector2f[nfaces * 3];
		for (int i = 0; i < nfaces * 3; i++)
			rawtexcoords[i] = texcbuffer[i];
	}
	else
		rawtexcoords = nullptr;

	nsubmeshes = nsubms;
	submeshes = new SubMesh*[nsubms];

	int pos = 0;
	for (int i = 0; i < nsubms; i++) {
		int length = (i == nsubms - 1) ? nfaces * 3 - subms[i] : subms[i + 1] - subms[i];
		submeshes[i] = new SubMesh(
			(vertbuffer == nullptr) ? nullptr : &vertbuffer[pos], 
			(normbuffer == nullptr) ? nullptr : &normbuffer[pos], 
			(texcbuffer == nullptr) ? nullptr : &texcbuffer[pos], 
			length, matls[i]);
		pos += length;
	}

	delete[] vertices;
	delete[] vertbuffer;
	if (nnorms) {
		delete[] norms;
		delete[] normbuffer;
	}
	if (ntexcs) {
		delete[] texcs;
		delete[] texcbuffer;
	}
	delete[] faces;
	delete[] subms;
	delete[] matls;
}

Mesh::~Mesh() {
	if (submeshes != nullptr) {
		for (int i = 0; i < nsubmeshes; i++)
			delete submeshes[i];
		delete[] submeshes;
	}
	if(tree != nullptr)
		delete tree;
	delete[] rawvertices;
	if(rawnormals)
		delete[] rawnormals;
	if(rawtexcoords)
		delete[] rawtexcoords;
}

void Mesh::Render(Flags callflags) {
	glUseProgram(((ShaderProgram*)GEngine::Get().Resource().Get_Resource("default_shader"))->vertexArrayID);
	for (int i = 0; i < nsubmeshes; i++)
		submeshes[i]->Render(callflags);
	glUseProgram(0);
}

CustomArray<Mesh*> Load_Mesh_File(string in_filePath, string in_fileName) {
	return CustomArray<Mesh*>();
}

RawMesh Ico_Sphere(int nsubdiv, float radius, bool calcnorms, bool calcuvs) {
	int ntris = 4;
	int cntris = 4;
	Vector3f origpts[4] = {
		Vector3f(1, 1, 1),
		Vector3f(-1, -1, 1),
		Vector3f(1, -1, -1),
		Vector3f(-1, 1, -1)
	};
	for (int i = 0; i < 4; i++)
		origpts[i].Normalize();

	Vector3f* vertices = new Vector3f[pow(4, nsubdiv + 1) * 3];
	vertices[0] = origpts[0]; vertices[1] = origpts[1]; vertices[2] = origpts[2];
	vertices[3] = origpts[0]; vertices[5] = origpts[1]; vertices[4] = origpts[3];
	vertices[6] = origpts[0]; vertices[7] = origpts[2]; vertices[8] = origpts[3];
	vertices[9] = origpts[1]; vertices[11] = origpts[2]; vertices[10] = origpts[3];

	for (int i = 0; i < nsubdiv; i++) {
		for (int j = 0; j < cntris * 3; j += 3) {
			Vector3f norm = (vertices[j + 1] - vertices[j]).Cross(vertices[j + 2] - vertices[j]);
			Vector3f oldtri[3] = { vertices[j], vertices[j + 1], vertices[j + 2] };
			Vector3f newtri[3] = {
				(vertices[j] + vertices[j + 1]) * 0.5f,
				(vertices[j + 1] + vertices[j + 2]) * 0.5f,
				(vertices[j + 2] + vertices[j]) * 0.5f
			};
			vertices[j + 0] = oldtri[0];
			vertices[j + 1] = newtri[0];
			vertices[j + 2] = newtri[2];
			vertices[ntris * 3 + 1] = oldtri[1];
			vertices[ntris * 3 + 2] = newtri[1];
			vertices[ntris * 3 + 0] = newtri[0];
			ntris++;
			vertices[ntris * 3 + 1] = oldtri[2];
			vertices[ntris * 3 + 2] = newtri[2];
			vertices[ntris * 3 + 0] = newtri[1];
			ntris++;
			vertices[ntris * 3 + 1] = newtri[0];
			vertices[ntris * 3 + 2] = newtri[1];
			vertices[ntris * 3 + 0] = newtri[2];
			ntris++;
		}
		cntris = ntris;
	}

	for (int i = 0; i < ntris * 3; i++) {
		vertices[i].Normalize();
		vertices[i] *= radius;
	}

	if (ntris != pow(4, nsubdiv + 1))
		Die("");

	Vector3f* norms = nullptr;
	if (calcnorms) {
		norms = new Vector3f[ntris * 3];
		for (int i = 0; i < ntris * 3; i++)
			norms[i] = vertices[i];
	}

	Vector2f* uvs = nullptr;
	if (calcuvs) {
		uvs = new Vector2f[ntris * 3];
		for (int i = 0; i < ntris * 3; i++) {
			Vector3f antinorm = norms[i] * -1;
			uvs[i].x = 0.5f + atan2f(antinorm.z, antinorm.x) / 2.0f / (float)PI;
			uvs[i].y = 0.5f - asinf((antinorm.y > 1) ? 1 : ((antinorm.y < -1) ? -1 : antinorm.y)) / (float)PI;
		}
	}

	RawMesh mesh = {
		vertices,
		norms,
		uvs,
		ntris
	};
		
	return mesh;
}
*/

void Test_Function(void* arg) {
	OutputDebugStringA("\nThis function was called.\n");
}

SphereNode::SphereNode() :
localCenter		(),
worldCenter		(),
radius			(0.0f),
lastCenterUpdate(0),
isLeaf			(false),
end				({ {} }) {
	end.branch.child1 = nullptr;
	end.branch.child2 = nullptr;
}

SphereNode::SphereNode(Spheref in_sphere, SphereNode * in_child1, SphereNode * in_child2) :
localCenter		(in_sphere.center),
worldCenter		(in_sphere.center),
radius			(in_sphere.radius),
lastCenterUpdate(0),
isLeaf			(false),
end				({ {} }) {
	end.branch.child1 = in_child1;
	end.branch.child2 = in_child2;
}

SphereNode::SphereNode(Triangle3f& in_triangle) :
localCenter		(),
worldCenter		(),
radius			(),
lastCenterUpdate(0),
isLeaf			(true),
end				({ {} }) {
	Spheref boundingSphere = in_triangle.Bounding_Sphere();
	localCenter = boundingSphere.center;
	worldCenter = boundingSphere.center;
	radius = boundingSphere.radius;
	end.leaf.lastTriangleUpdate = 0;
	end.leaf.localTriangle = in_triangle;
	end.leaf.worldTriangle = in_triangle;
}

bool SphereNode::Is_Leaf() const {
	return isLeaf;
}

Vector3f const & SphereNode::Local_Center() const {
	return localCenter;
}

Vector3f const & SphereNode::World_Center() const {
	return worldCenter;
}

float SphereNode::Radius() const {
	return radius;
}

SphereNode * SphereNode::Child_1() const {
	if (isLeaf) {
		throw MismatchException("SphereNode:Child_1:isLeaf is true");
	}
	return end.branch.child1;
}

SphereNode * SphereNode::Child_2() const {
	if (isLeaf) {
		throw MismatchException("SphereNode:Child_2:isLeaf is true");
	}
	return end.branch.child2;
}

Triangle3f const & SphereNode::Local_Triangle() const {
	if (!isLeaf) {
		throw MismatchException("SphereNode:Local_Triangle:isLeaf is false");
	}
	return end.leaf.localTriangle;
}

Triangle3f const & SphereNode::World_Triangle() const {
	if (!isLeaf) {
		throw MismatchException("SphereNode:World_Triangle:isLeaf is false");
	}
	return end.leaf.worldTriangle;
}

void SphereNode::Update_Center(Transform3d const & in_transform, uint in_uniqueCallID) {
	if (lastCenterUpdate != in_uniqueCallID) {
		worldCenter = in_transform.S_To_O(localCenter);
	}
}

void SphereNode::Update_Triangle(Transform3d const & in_transform, uint in_uniqueCallID) {
	if (!isLeaf) {
		throw MismatchException("SphereNode:Update_Triangle:isLeaf is false");
	}
	if (lastCenterUpdate != in_uniqueCallID) {
		for (int i = 0; i < 3; i++) {
			end.leaf.worldTriangle[i] = in_transform.S_To_O(end.leaf.localTriangle[i]);
		}
	}
}

CustomArray<SphereNode> SphereNode::Create_Tree(CompressedTriangleArray3f& in_triangles) {
	CompressedTriangleArray3f triangles = in_triangles;
	uint nTriangles = triangles.Size_Triples();
	uint sphereIndex = 0;
	CustomArray<SphereNode>		tree	(2 * nTriangles - 1);
	CustomArray<ArraySegment>	segments(nTriangles); // length, offset by triple

	// trivial cases
	switch (nTriangles) {
	case 0:
		return tree;
	case 1:
		tree.Add(SphereNode((Triangle3f)in_triangles.Triple_At(0)));
		return tree;
	}

	segments.Add({0, nTriangles});
	tree.Add(SphereNode());

	while (segments.Size() > 0) {
		// processing and production of both segments and spheres parallel with each other, 
		// so segments[0] always corresponds to tree[sphereIndex]
		ArraySegment& segment = segments[0]; 

		// get bounding sphere of all points
		CustomArray<Vector3f> points = triangles.Sub_Value_Array(segment, true);
		CustomArray<Vector3f> boundaryPoints(3);
		Spheref boundingSphere = SphereNode::Welzl(points, boundaryPoints);
		if (boundaryPoints.Size() != 3) {
			throw MismatchException("SphereNode:CreateTree:boundaryPoints.size != 3; degenerate triangle");
		}

		// get current sphere to be developed
		SphereNode* node = &tree[sphereIndex];
		sphereIndex++;

		// if is leaf, finish
		if (segment.length == 1) {
			*node = SphereNode(Triangle3f(points.Pointer()));
			segments.Remove(0, true);
			continue;
		}

		// otherwise, create two empty spheres to be assigned as children
		tree.Add(SphereNode());
		tree.Add(SphereNode());
		*node = SphereNode(boundingSphere, &tree[tree.Size() - 2], &tree[tree.Size() - 1]);

		// divide triangles into two child "front" and "back" groups via a simple plane test,
		// sort original data set to match
		Planef dividingPlane = Planef(boundingSphere.center, (boundaryPoints[1] + boundaryPoints[2]) * 0.5 - boundaryPoints[0]);
		uint frontIndex = segment.offset;
		uint backIndex = segment.offset + segment.length - 1;
		for (uint i = 0; i < segment.length; i++) {
			Triangle3f triangle = triangles.Triple_At(i);
			Vector3f centroid = triangle.Centroid();
			if (dividingPlane.Which_Side(centroid) == Planef::Side::front) {
				triangles.Set(frontIndex, triangle);
				frontIndex++;
			}
			else {
				triangles.Set(backIndex, triangle);
				backIndex--;
			}
		}

		if (frontIndex == segment.offset || backIndex == segment.offset + segment.length - 1) {
			throw MismatchException("SphereNode:Create_Tree:dividingPlane not actually dividing");
		}

		// add two child segments to match child spheres created earlier
		ArraySegment frontSegment = { segment.offset, frontIndex };
		ArraySegment backSegment = { segment.offset + frontIndex, segment.length - frontIndex };

		segments.Remove(0, true);
		segments.Add(frontSegment);
		segments.Add(backSegment);
	}

	if (sphereIndex != tree.Size()) {
		throw MismatchException("SphereNode:CreateTree:tree container too big");
	}

	return tree;
}

Spheref SphereNode::Welzl(CustomArray<Vector3f>& in_points, CustomArray<Vector3f>& in_borderPoints) {
	// Welzl's magical algorithm; recursive, linear time
	if (in_points.Size() == 0 || in_borderPoints.Size() >= 3) {
		Vector3f v;
		Spheref  s;
		switch (in_borderPoints.Size()) {
		case 0:
			return { Vector3f(), 0.0 };
		case 1:
			return { in_borderPoints[0], 0.0 };
		case 2:
			v = (in_borderPoints[1] - in_borderPoints[0]) * 0.5;
			return { in_borderPoints[0] + v, v.Magnitude() };
		case 3:
			return Triangle3f(in_borderPoints.Pointer()).Bounding_Sphere();
		default:
			s = Triangle3f(in_borderPoints.Pointer()).Bounding_Sphere();
			for (int i = 0; i < in_borderPoints.Size() - 3; i++) {
				if ((in_borderPoints[i] - s.center).Dot_Self() != (s.radius * s.radius)) {
					throw InvalidParameterException("SphereNode:Welzl:in_borderPoints not co-spherical");
				}
			}
			return s;
		}
	}
	Vector3f point = in_points[in_points.Size() - 1];
	in_points.Remove(in_points.Size() - 1, false);
	Spheref lastSphere = Welzl(in_points, in_borderPoints);
	if ((point - lastSphere.center).Dot_Self() <= (lastSphere.radius * lastSphere.radius)) {
		return lastSphere;
	}
	in_borderPoints.Add(point);
	Spheref newSphere = Welzl(in_points, in_borderPoints);
	in_borderPoints.Remove(in_borderPoints.Size() - 1, false);
	in_points.Add(point);
	return newSphere;
}

void Parse_MML_String(MMLElement & inout_element, string in_string) {
	stringstream stream(in_string);
	enum MMLElementPart {
		header,
		body,
		footer
	} elementPart = MMLElementPart::header;
	string whitespace = " \t\n\r\v\f";

	Ignore_To_Not_Char(stream, whitespace);

	if (stream.get() != '<') {
		throw InvalidObjectException("Parse_MML_String:in_string not properly formatted");
	}

	Ignore_To_Not_Char(stream, whitespace);

	Read_To_Char(stream, inout_element.title, whitespace);

	string name;
	string value;
	while (elementPart == MMLElementPart::header) {
		Ignore_To_Not_Char(stream, whitespace);
		switch (stream.peek()) {
		case '/':
			Assert_Get_Char(stream, string(">"));
			break;
		case '>':
			stream.ignore();
			elementPart = MMLElementPart::body;
			break;
		default: {
			// get attribute name
			Read_To_Char(stream, name, whitespace + "=");

			Ignore_To_Not_Char(stream, whitespace + "=");

			Assert_Get_Char(stream, string("\'"));

			// get attribute value
			Read_To_Char(stream, name, string("\'"));
			stream.ignore();

			inout_element.attributes.insert(std::map<string, string>::value_type(name, value));
		}
			break;
		}
	}


	while (elementPart == MMLElementPart::body) {
		Ignore_To_Not_Char(stream, whitespace);
		if (stream.peek() == '<') {
			string subElementString;
			uint start = stream.tellg();
			uint end;
			uint nOpen = 0;
			char c;
			char* charBuffer;
			do {
				c = stream.get();
				if (c == '<') {
					nOpen++;
				}
				else {
					if (c == '/') {
						if (stream.peek() == '>') {
							nOpen--;
						}
						else {
							nOpen -= 2;
						}
					}
				}
			} while (nOpen != 0);
			Ignore_To_Char(stream, string(">"));
			stream.ignore();
			end = stream.tellg();
			stream.seekg(start);
			charBuffer = new char[end - start + 1];
			stream.get(charBuffer, end - start);
			subElementString = charBuffer;
			delete[] charBuffer;

			MMLElement subElement;
			Parse_MML_String(subElement, subElementString);

			inout_element.children.insert(std::map<uint, MMLElement>::value_type(inout_element.children.size(), subElement))
		}
		else {
			string body;
			Read_To_Char(stream, body, string("<"));
			Ignore_To_Not_Char(stream, whitespace);
			Assert_Get_String(stream, string("</") + inout_element.title + string(">"));
			inout_element.body = body;
			elementPart = MMLElementPart::footer;
		}
	}
}