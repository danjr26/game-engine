#include "renderable_object.h"
#include "log.h"
#include "matrix.h"
#include "Texture2.h"
#include "Texture2_instance.h"

RenderableObject::RenderableObject() :
	mCullingRectangle(AxisAlignedRectangled::fromExtrema(Vector2d(), Vector2d()))
{}

RenderableObject::~RenderableObject() 
{}

