#include "force_field.h"

template<uint n>
void ForceField<n>::Set_Collision_Context(CollisionContext<double, n>* in_context) 
{}

template class ForceField<2>;