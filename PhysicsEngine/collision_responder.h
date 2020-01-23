#ifndef COLLISION_RESPONDER_H
#define COLLISION_RESPONDER_H

#include <functional>
#include "collision_packet.h"
#include "collision_context.h"

using CollisionResponder = std::function<void(const CollisionPartner2d&, CollisionPacket&)>;

#endif