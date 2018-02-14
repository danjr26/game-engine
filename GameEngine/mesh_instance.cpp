#include "mesh_instance.h"
#include "framework.h"

MeshInstance::MeshInstance(Mesh* mesh) :
RenderComponent	(RenderType::opaque),
mesh			(mesh),
transform		()
{}

MeshInstance::~MeshInstance() 
{}

void MeshInstance::Render(Flags callflags) {
	if (callflags == RenderType::opaque) {
		transform.Apply_Object();
		mesh->Render(callflags);
		transform.Unapply_Object();
	}
}
