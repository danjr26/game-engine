#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H
#include "mesh.h"
#include "transform.h"
#include "component.h"

class MeshInstance : public RenderComponent {
public:
	Mesh*		mesh;
	Transform3d	transform;

			MeshInstance	(Mesh* mesh);
			~MeshInstance	();
	void	Render			(Flags callflags);
};

#endif
